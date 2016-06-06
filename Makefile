EXTENSION = frkn_avg              # the extension name
DATA      = frkn_avg--0.0.1.sql   # script files to install
MODULES   = frkn_avg              # our c module file to build

# Postgres build stuff
PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
