#!/usr/bin/env sh

make -C ../ clean
make -C ../ all
echo
for file in test*.txt; do
    echo "processing: $file"
    [[ -f "$file" ]] || continue
    # Compress and Extract Test Cases

    if ../compress $file "compressed$file" > /dev/null 2>&1; then
        # This block runs if exit code is 0 (success)
        echo "Success Compression"
    else
        # This block runs if exit code is non-zero (like exit 1)
        echo "Failed Compression Execution"
    fi

    if ../decompress "compressed$file" "extracted$file" > /dev/null 2>&1; then
        # This block runs if exit code is 0 (success)
        echo "Success Extraction"
    else
        # This block runs if exit code is non-zero (like exit 1)
        echo "Failed Extraction Execution"
    fi
    echo
done
