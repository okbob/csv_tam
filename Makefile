MODULE_big = csv_tam
EXTENSION = csv_tam
DATA = csv_tam--0.0.1.sql

OBJS = csv_tam.o src/impl.o

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)

include $(PGXS)
