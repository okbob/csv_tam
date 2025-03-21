create or replace function csv_fmt_handler(internal)
returns table_am_handler as 'csv_tam', 'csv_fmt_handler'
language c strict;

create access method csv_tam type table handler csv_fmt_handler;
