#/bin/bash

SRC_FILE_PATH="src/inc.c"
INCLUDE_PATH="-Ilib"
OUTPUT="run"
CFLAGS="-Werror -Wall -Wextra -pedantic -ggdb -lm -Wno-unused-function"

cc $SRC_FILE_PATH -o $OUTPUT $CFLAGS $INCLUDE_PATH
