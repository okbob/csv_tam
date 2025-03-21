#ifndef METHODS_STUB_H
#define METHODS_STUB_H

#include <postgres.h>
#include <access/tableam.h>
#include <access/heapam.h>
#include <commands/vacuum.h>
#include <catalog/index.h>

/* dump for not implemented methods */

static inline void
csv_rescan(TableScanDesc sscan, struct ScanKeyData *key, bool set_params,
		   bool allow_strat, bool allow_sync, bool allow_pagemode)
{
	elog(ERROR, "csv_rescan is not implemented");
}

static inline Size
csv_parallelscan_estimate(Relation rel)
{
	elog(ERROR, "csv_parallelscan_estimate is not implemented");
}


static inline Size
csv_parallelscan_initialize(Relation rel, ParallelTableScanDesc pscan)
{
	elog(ERROR, "csv_parallelscan_initialize is not implemented");
}


static inline void
csv_parallelscan_reinitialize(Relation rel, ParallelTableScanDesc pscan)
{
	elog(ERROR, "csv_parallelscan_reinitialize is not implemented");
}

static inline IndexFetchTableData *
csv_index_fetch_begin(Relation rel)
{
	elog(ERROR, "csv_index_fetch_begin is not implemented");
	return NULL;
}

static inline void
csv_index_fetch_reset(IndexFetchTableData *scan)
{
	elog(ERROR, "csv_index_fetch_reset is not implemented");
}

static inline void
csv_index_fetch_end(IndexFetchTableData *scan)
{
	elog(ERROR, "csv_index_fetch_end is not implemented");
}

static inline bool
csv_index_fetch_tuple(struct IndexFetchTableData *scan, ItemPointer tid, Snapshot snapshot,
					  TupleTableSlot *slot, bool *call_again, bool *all_dead)
{
	elog(ERROR, "csv_index_fetch_tuple is not implemented");
	return false;
}

static inline void
csv_tuple_insert_speculative(Relation relation, TupleTableSlot *slot, CommandId cid,
							 int options, BulkInsertState bistate, uint32 specToken)
{
	elog(ERROR, "csv_tuple_insert_speculative is not implemented");
}

static inline void
csv_tuple_complete_speculative(Relation relation, TupleTableSlot *slot, uint32 specToken, bool succeeded)
{
	elog(ERROR, "csv_tuple_complete_speculative is not implemented");
}

static inline TM_Result
csv_tuple_delete(Relation relation, ItemPointer tid, CommandId cid, Snapshot snapshot,
				 Snapshot crosscheck, bool wait, TM_FailureData *tmfd, bool changingPart)
{
	elog(ERROR, "csv_tuple_delete is not implemented");
	return TM_Ok;
}

static inline TM_Result
csv_tuple_update(Relation relation, ItemPointer otid, TupleTableSlot *slot, CommandId cid,
				 Snapshot snapshot, Snapshot crosscheck, bool wait, TM_FailureData *tmfd,
				 LockTupleMode *lockmode, TU_UpdateIndexes *update_indexes)
{
	elog(ERROR, "csv_tuple_update is not implemented");
	return TM_Ok;
}

static inline TM_Result
csv_tuple_lock(Relation relation, ItemPointer tid, Snapshot snapshot, TupleTableSlot *slot,
			   CommandId cid, LockTupleMode mode, LockWaitPolicy wait_policy, uint8 flags, TM_FailureData *tmfd)
{
	elog(ERROR, "csv_tuple_lock is not implemented");
	return TM_Ok;
}

static inline bool
csv_fetch_row_version(Relation relation, ItemPointer tid, Snapshot snapshot, TupleTableSlot *slot)
{
	elog(ERROR, "csv_fetch_row_version is not implemented");
	return false;
}

static inline void
csv_get_latest_tid(TableScanDesc sscan, ItemPointer tid)
{
	elog(ERROR, "csv_get_latest_tid is not implemented");
}

static inline bool
csv_tuple_tid_valid(TableScanDesc scan, ItemPointer tid)
{
	elog(ERROR, "csv_tuple_tid_valid is not implemented");
	return false;
}

static inline bool
csv_tuple_satisfies_snapshot(Relation rel, TupleTableSlot *slot, Snapshot snapshot)
{
	elog(ERROR, "csv_tuple_satisfies_snapshot is not implemented");
	return false;
}

static inline TransactionId
csv_index_delete_tuples(Relation rel, TM_IndexDeleteOp *delstate)
{
	elog(ERROR, "csv_index_delete_tuples is not implemented");
	return InvalidTransactionId;
}

static inline void
csv_relation_nontransactional_truncate(Relation rel)
{
	elog(ERROR, "csv_relation_nontransactional_truncate is not implemented");
}

static inline void
csv_relation_copy_data(Relation rel, const RelFileLocator *newrlocator)
{
	elog(ERROR, "csv_relation_copy_data is not implemented");
}

static inline void
csv_relation_copy_for_cluster(Relation OldHeap, Relation NewHeap, Relation OldIndex, bool use_sort,
							  TransactionId OldestXmin, TransactionId *xid_cutoff, MultiXactId *multi_cutoff,
							  double *num_tuples, double *tups_vacuumed, double *tups_recently_dead
)
{
	elog(ERROR, "csv_relation_copy_for_cluster is not implemented");
}

static inline void
csv_vacuum_rel(Relation rel, VacuumParams *params, BufferAccessStrategy bstrategy)
{
	elog(ERROR, "csv_vacuum_rel is not implemented");
}

static inline bool
csv_scan_analyze_next_block(TableScanDesc scan, BlockNumber blockno, BufferAccessStrategy bstrategy)
{
	elog(ERROR, "csv_scan_analyze_next_block is not implemented");
	return false;
}

static inline bool
csv_scan_analyze_next_tuple(TableScanDesc scan, TransactionId OldestXmin, double *liverows,
							double *deadrows, TupleTableSlot *slot)
{
	elog(ERROR, "csv_scan_analyze_next_tuple is not implemented");
	return false;
}

static inline double
csv_index_build_range_scan(Relation heapRelation, Relation indexRelation, IndexInfo *indexInfo,
						   bool allow_sync, bool anyvisible, bool progress, BlockNumber start_blockno,
						   BlockNumber numblocks, IndexBuildCallback callback, void *callback_state, TableScanDesc scan)
{
	elog(ERROR, "csv_index_build_range_scan is not implemented");
	return 0;
}

static inline void
csv_index_validate_scan(Relation heapRelation, Relation indexRelation, IndexInfo *indexInfo,
						Snapshot snapshot, ValidateIndexState *state)
{
	elog(ERROR, "csv_index_validate_scan is not implemented");
}

static inline uint64
csv_relation_size(Relation rel, ForkNumber forkNumber)
{
	elog(ERROR, "csv_relation_size is not implemented");
	return 0;
}

static inline Oid
csv_relation_toast_am(Relation rel)
{
	elog(ERROR, "csv_relation_toast_am is not implemented");
	return InvalidOid;
}

static inline void
csv_fetch_toast_slice(Relation toastrel, Oid valueid, int32 attrsize, int32 sliceoffset,
					  int32 slicelength, struct varlena *result)
{
	elog(ERROR, "csv_fetch_toast_slice is not implemented");
}

static inline bool
csv_scan_sample_next_block(TableScanDesc scan, SampleScanState *scanstate)
{
	elog(ERROR, "csv_scan_sample_next_block is not implemented");
	return false;
}

static inline bool
csv_scan_sample_next_tuple(TableScanDesc scan, SampleScanState *scanstate, TupleTableSlot *slot)
{
	elog(ERROR, "csv_scan_sample_next_tuple is not implemented");
	return false;
}

static inline void
csv_set_tidrange(TableScanDesc sscan, ItemPointer mintid, ItemPointer maxtid)
{
	elog(ERROR, "csv_set_tidrange is not implemented");
}

static inline bool
csv_getnextslot_tidrange(TableScanDesc sscan, ScanDirection direction,
						 TupleTableSlot *slot)
{
	elog(ERROR, "csv_getnextslot_tidrange is not implemented");
	return false;
}

#endif
