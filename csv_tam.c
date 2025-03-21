#include "src/impl.h"

PG_MODULE_MAGIC;

static const TableAmRoutine routine = {
	.type = T_TableAmRoutine,

	.slot_callbacks = csv_slot_callbacks,

	.scan_begin = csv_beginscan,
	.scan_end = csv_endscan,
	.scan_rescan = csv_rescan,
	.scan_getnextslot = csv_getnextslot,

	.parallelscan_estimate = csv_parallelscan_estimate,
	.parallelscan_initialize = csv_parallelscan_initialize,
	.parallelscan_reinitialize = csv_parallelscan_reinitialize,

	.index_fetch_begin = csv_index_fetch_begin,
	.index_fetch_reset = csv_index_fetch_reset,
	.index_fetch_end = csv_index_fetch_end,
	.index_fetch_tuple = csv_index_fetch_tuple,

	.tuple_fetch_row_version = csv_fetch_row_version,
	.tuple_get_latest_tid = csv_get_latest_tid,
	.tuple_tid_valid = csv_tuple_tid_valid,
	.tuple_satisfies_snapshot = csv_tuple_satisfies_snapshot,
	.index_delete_tuples = csv_index_delete_tuples,

	.tuple_insert = csv_tuple_insert,
	.tuple_insert_speculative = csv_tuple_insert_speculative,
	.tuple_complete_speculative = csv_tuple_complete_speculative,
	.multi_insert = csv_multi_insert,
	.tuple_delete = csv_tuple_delete,
	.tuple_update = csv_tuple_update,
	.tuple_lock = csv_tuple_lock,

	.relation_set_new_filelocator = csv_relation_set_new_filelocator,
	.relation_nontransactional_truncate = csv_relation_nontransactional_truncate,
	.relation_copy_data = csv_relation_copy_data,
	.relation_copy_for_cluster = csv_relation_copy_for_cluster,
	.relation_vacuum = csv_vacuum_rel,
	.scan_analyze_next_block = csv_scan_analyze_next_block,
	.scan_analyze_next_tuple = csv_scan_analyze_next_tuple,
	.index_build_range_scan = csv_index_build_range_scan,
	.index_validate_scan = csv_index_validate_scan,

	.relation_size = csv_relation_size,
	.relation_needs_toast_table = csv_relation_needs_toast_table,

	.relation_estimate_size = csv_estimate_rel_size,

	.scan_sample_next_block = csv_scan_sample_next_block,
	.scan_sample_next_tuple = csv_scan_sample_next_tuple
};

PG_FUNCTION_INFO_V1(csv_fmt_handler);
Datum
csv_fmt_handler(PG_FUNCTION_ARGS)
{
	PG_RETURN_POINTER(&routine);
}

void
_PG_init(void)
{

}
