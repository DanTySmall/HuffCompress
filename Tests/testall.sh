#!/usr/bin/env sh

make -C ../ clean
make -C ../ all
echo
total=0
passed=0
for file in test*.txt; do
    nonascii=0
    ((total++))

    echo "Processing: $file"
    [[ -f "$file" ]] || continue

    # Compress and Extract Test Cases
    ../compress $file "compressed$file" > /dev/null 2>&1
    exit_code=$?
    if [ $exit_code -eq 0 ]; then
        # This block runs if exit code is 0 (success)
        echo "Success Compression"
    else
        # This block runs if exit code is non-zero (like exit 1)
        if [ $exit_code -eq 2 ]; then
            echo "Non-Ascii File Detected"
            nonascii=1
        else
            echo "Failed Compression Execution"
        fi
    fi
    if ../decompress "compressed$file" "extracted$file" > /dev/null 2>&1; then
        echo "Success Extraction"
    else
        echo "Failed Extraction Execution"
    fi

    # Generate Diffs of Input and Output
    if diff "$file" "extracted$file"; then
        echo "Test Passed!"
        ((passed++))
    else
        if [ $nonascii -eq 1 ]; then
            echo "File Not Generated"
            echo "Test Passed!"
            ((passed++))
        else
            echo "Test Failed"
        fi
    fi
    echo
done

echo "The program passed $passed / $total tests"
