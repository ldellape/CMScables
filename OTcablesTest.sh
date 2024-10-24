#!/bin/bash
# this script will process all new tests (CMScables.cpp will recognize automatically if they are PSPP1/OCTOPUS/PP0/FULL_CHAIN)

INPUT_TEST="${PWD}/input/FULL_TEST_su_cavo_ps_pp1_V3/"
PROCESSED_TEST="${PWD}/docs/ProcessedTest.txt"
echo "Processing New Test"
MODE=2

cd "${PWD}/py/"
output=$(time python3 AutoRun.py)
cd ".."

if [[ "$output" == "No new tests" ]]; then
    TEXT="No new tests found, $(date)"
    echo "${TEXT}" >> CMScables.log
    exit 0
else 
    echo "$output"
    IFS=$'\n' read -r -d '' -a NewTestsArray <<< "$output"

    for test in "${NewTestsArray[@]}"; do
        echo "Processing test: $test"
        echo "${PWD}"
        ./CMScables --input "${test}" --mode "${MODE}" >> CMScables.log
    done
fi
