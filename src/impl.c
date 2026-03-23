/*
 * This is a minimal implementation of table access method.
 * It allows to write tuples to datafiles in csv format and read them.
 * Implementation is as simple as it may be. Multiple questions are still uncovered.
 * 1) No concurrent writes
 * 2) No transactional writes
 * 3) Limited data types support
 * 4) No crash recovery. Custom resource manager is not implemented.
 * 5) No file closing in case of error
 * 6) Other limitations explained in code.
 * Currently, it looks like a Foreign Data Wrapper, but for TableAM it's just the beginning of the way.
 */

#include "impl.h"

#include <utils/lsyscache.h>
#include <miscadmin.h>
#include <catalog/storage.h>
#include <commands/copy.h>
#include <nodes/makefuncs.h>
#include <optimizer/plancat.h>

#if PG_VERSION_NUM >= 190000

#include "storage/fd.h"

#endif

/* state for read operations */
typedef struct
{
	TableScanDescData base;		/* base state which should be returned by
								 * beginscan */
	CopyFromState cstate;		/* 'copy' state to read csv from file */
}			CsvScanState;

static const char *delimiter = ",";
static const uint32 rows_count = 100;

/* return slot implementation for tuples */
const TupleTableSlotOps *
csv_slot_callbacks(Relation relation)
{
	/* use virtual slot implementation as is */
	return &TTSOpsVirtual;
}

/* do we use toast extension to store csv? */
bool
csv_relation_needs_toast_table(Relation relation)
{
	/* no */
	return false;
}

/* begin scan; init the state to work with later */
TableScanDesc
csv_beginscan(Relation relation, Snapshot snapshot, int nkeys,
			  struct ScanKeyData *key, ParallelTableScanDesc parallel_scan, uint32 flags)
{
	CsvScanState *state;
	List	   *options = NIL;

#if PG_VERSION_NUM >= 180000

	RelPathStr	filename;

#else

	char	   *filename;

#endif

	elog(NOTICE, "Calling %s...", __func__);

	state = palloc0(sizeof(*state));
	/* init base state */
	state->base.rs_rd = relation;
	state->base.rs_snapshot = snapshot;
	state->base.rs_nkeys = nkeys;
	state->base.rs_key = key;
	state->base.rs_flags = flags;
	state->base.rs_parallel = parallel_scan;

	/* init copy state; get target file name and pass options to reader implemented in core */
	filename = relpathperm(relation->rd_locator, MAIN_FORKNUM);
	options = lappend(options, makeDefElem("format", (Node *) makeString("csv"), -1));
	options = lappend(options, makeDefElem("delimiter", (Node *) makeString((char *) delimiter), -1));

#if PG_VERSION_NUM >= 180000

	state->cstate = BeginCopyFrom(NULL, relation, NULL, filename.str, false, NULL, NIL, options);

#else

	state->cstate = BeginCopyFrom(NULL, relation, NULL, filename, false, NULL, NIL, options);

#endif

	return &state->base;
}

/* iterate over scan; read next row and store it in slot */
bool
csv_getnextslot(TableScanDesc sscan, ScanDirection direction, TupleTableSlot *slot)
{
	CsvScanState *state = (CsvScanState *) sscan;
	bool		found;

	elog(NOTICE, "Calling %s...", __func__);

	/* follow the core contract, clear slot from any previous garbage */
	ExecClearTuple(slot);
	/* read next row using core method */
	found = NextCopyFrom(state->cstate, NULL,
						 slot->tts_values, slot->tts_isnull);
	if (found)
		ExecStoreVirtualTuple(slot);

	return found;
}

/* end reading; do deinit for custom objects if any */
void
csv_endscan(TableScanDesc sscan)
{
	CsvScanState *state = (CsvScanState *) sscan;

	/*
	 * Here we can free any object from custom context. No need to free if
	 * custom context is not used. However, 'copy' state should be ended manually.
	 */
	EndCopyFrom(state->cstate);
	elog(NOTICE, "Calling %s...", __func__);
}

/*
 * A helper method.
 * Check if relation has unsupported data types.
 * We leave this example simple, so text types are not used. They need
 * escaping during write.
 * Better to call this in DDL hooks, during CREATE and ALTER, for example.
 */
static void
check_atts_type(Relation relation)
{
	TupleDesc	desc;
	int			natts;

	desc = RelationGetDescr(relation);
	natts = RelationGetNumberOfAttributes(relation);
	for (int i = 0; i < natts; i++)
	{
		Form_pg_attribute attr = TupleDescAttr(desc, i);

		switch (attr->atttypid)
		{
			case INT2OID:
			case INT4OID:
			case INT8OID:
			case FLOAT4OID:
			case FLOAT8OID:
			case DATEOID:
			case TIMESTAMPOID:
			case TIMESTAMPTZOID:
			case NUMERICOID:
				break;
			default:
				elog(ERROR, "Unsupported type for column %s", NameStr(attr->attname));
		}
	}
}

/* initialize storage for new relation; can happen during CREATE or TRUNCATE */
void
csv_relation_set_new_filelocator(Relation relation, const RelFileLocator *newrlocator, char persistence,
								 TransactionId *freezeXid, MultiXactId *minmulti)
{
	SMgrRelation srel;

	elog(NOTICE, "Calling %s...", __func__);
	check_atts_type(relation);

	/* create new datafile for MAIN_FORKNUM */
	srel = RelationCreateStorage(*newrlocator, persistence, true);
	smgrclose(srel);
}

/* return table statistics */
void
csv_estimate_rel_size(Relation relation, int32 *attr_widths, BlockNumber *pages,
					  double *tuples, double *allvisfrac)
{
#if PG_VERSION_NUM >= 180000

	RelPathStr	filename;

#else

	char	   *filename;

#endif

	File		file;
	off_t		filesize;

	elog(NOTICE, "Calling %s...", __func__);

	/* all rows are visible */
	*allvisfrac = 1.0;

	/* get average row width */
	(void) get_rel_data_width(relation, attr_widths);

	/*
	 * Calculate pages count enough to store our data. CSV format is not
	 * block-oriented, so we try to mimic calculation for block format here.
	 * Alternatively, but less accurate:
	 * tuple_width = get_rel_data_width(relation, attr_widths);
	 * pages = rows_count * tuple_width;
	 */
	filename = relpathperm(relation->rd_locator, MAIN_FORKNUM);

#if PG_VERSION_NUM >= 180000

	file = PathNameOpenFile(filename.str, O_RDONLY);

#else

	file = PathNameOpenFile(filename, O_RDONLY);

#endif

	filesize = FileSize(file);
	FileClose(file);
	*pages = filesize / BLCKSZ;

	/*
	 * Get count of rows in the table. Use constant value to keep example
	 * simple. To get exact value, we can use auxiliary heap table, store row
	 * count in it and update during any insert. Or we can use additional
	 * datafile which store metadata only (not csv format).
	 */
	*tuples = rows_count;
}

/* a helper method to write rows to file */
static void
write_rows(Relation relation, TupleTableSlot **slots, int ntuples)
{
	TupleDesc	desc;
	int			natts;
	FmgrInfo   *out_functions;

#if PG_VERSION_NUM >= 180000

	RelPathStr	filename;

#else

	char	   *filename;

#endif

	File		file;
	off_t		filesize;

	check_atts_type(relation);

	desc = RelationGetDescr(relation);
	natts = RelationGetNumberOfAttributes(relation);

	/* find functions which convert value to text */
	out_functions = (FmgrInfo *) palloc(natts * sizeof(FmgrInfo));
	for (int i = 0; i < natts; i++)
	{
		Oid			out_func_oid;
		bool		isvarlena;
		Form_pg_attribute attr = TupleDescAttr(desc, i);

		getTypeOutputInfo(attr->atttypid, &out_func_oid, &isvarlena);
		fmgr_info(out_func_oid, &out_functions[i]);
	}

	/* write all incoming rows to datafile */
	filename = relpathperm(relation->rd_locator, MAIN_FORKNUM);

#if PG_VERSION_NUM >= 180000

	file = PathNameOpenFile(filename.str, O_RDWR);

#else

	file = PathNameOpenFile(filename, O_RDWR);

#endif

	filesize = FileSize(file);
	for (int i = 0; i < ntuples; i++)
	{
		TupleTableSlot *slot = slots[i];
		StringInfoData buf;

		initStringInfo(&buf);

		for (int j = 0; j < natts; j++)
		{
			Datum		value = slot->tts_values[j];
			bool		isnull = slot->tts_isnull[j];

			/*
			 * convert value to text if it's not null and append to line in
			 * text format
			 */
			if (!isnull)
			{
				char	   *string = OutputFunctionCall(&out_functions[j], value);

				appendStringInfoString(&buf, string);
			}
			/* last value doesn't need delimiter, but needs newline symbol */
			if (j < natts - 1)
				appendStringInfoString(&buf, delimiter);
			else
			{

#if PG_VERSION_NUM >= 180000

				elog(NOTICE, "Inserting row '%s' into '%s'", buf.data, filename.str);

#else

				elog(NOTICE, "Inserting row '%s' into '%s'", buf.data, filename);

#endif

				appendStringInfoChar(&buf, '\n');
			}
		}
		/* write row to file */
		FileWrite(file, buf.data, buf.len, filesize, 0);
		filesize += buf.len;
	}
	FileClose(file);
}

/* write multiple rows at once */
void
csv_multi_insert(Relation relation, TupleTableSlot **slots, int ntuples,
				 CommandId cid, int options, BulkInsertState bistate)
{
	elog(NOTICE, "Calling %s...", __func__);
	write_rows(relation, slots, ntuples);
}

/* write single row */
void
csv_tuple_insert(Relation relation, TupleTableSlot *slot,
				 CommandId cid, int options, BulkInsertState bistate)
{
	elog(NOTICE, "Calling %s...", __func__);
	write_rows(relation, &slot, 1);
}
