#/bin/bash

SRC_FILE_PATH="src/inc.c"
INCLUDE_PATH="-Ilib"
OUTPUT="run"
CFLAGS="-Werror -Wall -Wextra -pedantic -ggdb -lm"

cc $SRC_FILE_PATH -o $OUTPUT $CFLAGS $INCLUDE_PATH

# Check if the compilation succeeded
if [ $? -eq 0 ]; then
    echo "Compilation successful!"

    # Check if the data directory exists and contains .txt files
    if [ -d "data" ] && [ "$(ls -1 data/*.txt 2>/dev/null | wc -l)" -gt 0 ]; then
        echo "Available .txt files in the data directory:"
        
        # List all .txt files
        for file in data/*.txt; do
            echo " - $file"
        done
        
        echo
        echo "To run the program, use: ./$OUTPUT <file_path>"
    else
        echo "Error: No .txt files found in the data directory."
    fi
else
    echo "Compilation failed. Please fix the errors."
fi
