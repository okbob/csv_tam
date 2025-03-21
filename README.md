This is a minimal implementation of Postgres Table Access Method. A start point to begin with.
It allows to write tuples to datafiles in csv format and read them.
It has several limitations explained in `impl.c`.

Installation.
Ensure your PG bin dir is in PATH and run the following:
```
make install
```

Usage:
```sql
create extension csv_tam;
create table csv_test(i int, d date) using csv_tam;
insert into csv_test values (1, CURRENT_DATE);
select * from csv_test;
 i |     d      
---+------------
 1 | 2025-03-21
(1 row)

select pg_relation_filepath('csv_test');
 pg_relation_filepath 
----------------------
 base/5/73851
(1 row)

```
```sh
$ cat base/5/73851
1,2025-03-21
```