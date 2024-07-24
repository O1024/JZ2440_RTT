#!/bin/bash

GDBINIT=$BSP_ROOT/.gdbinit
GDBCMD=$RTT_EXEC_PATH/${RTT_CC_PREFIX}gdb

if [ "debug" = $1 ]; then
    $GDBCMD -f $GDBINIT -tui -ex "layout split" -ex "focus cmd" -ex "gdb_debug_flash_bin"
else
    $GDBCMD -f $GDBINIT -ex "gdb_flash_bin"
fi
