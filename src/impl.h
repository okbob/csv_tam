#ifndef METHODS_IMPL_H
#define METHODS_IMPL_H

#include "stub.h"

const TupleTableSlotOps *csv_slot_callbacks(Relation relation);
TableScanDesc csv_beginscan(Relation relation, Snapshot snapshot, int nkeys,
							struct ScanKeyData *key, ParallelTableScanDesc parallel_scan, uint32 flags);
void		csv_endscan(TableScanDesc sscan);
bool		csv_getnextslot(TableScanDesc sscan, ScanDirection direction, TupleTableSlot *slot);
void		csv_relation_set_new_filelocator(Relation rel, const RelFileLocator *newrlocator, char persistence,
											 TransactionId *freezeXid, MultiXactId *minmulti);
bool		csv_relation_needs_toast_table(Relation rel);
void		csv_estimate_rel_size(Relation rel, int32 *attr_widths, BlockNumber *pages,
								  double *tuples, double *allvisfrac);

#if PG_VERSION_NUM >= 190000

void		csv_tuple_insert(Relation relation, TupleTableSlot *slot, CommandId cid,
							 uint32 options, BulkInsertState bistate);
void		csv_multi_insert(Relation relation, TupleTableSlot **slots, int ntuples,
							 CommandId cid, uint32 options, BulkInsertState bistate);

#else

void		csv_tuple_insert(Relation relation, TupleTableSlot *slot, CommandId cid,
							 int options, BulkInsertState bistate);
void		csv_multi_insert(Relation relation, TupleTableSlot **slots, int ntuples,
							 CommandId cid, int options, BulkInsertState bistate);

#endif

#endif
