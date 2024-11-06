#!/bin/bash

# to be passed as arguments of python script 
INPUT_TEST="${PWD}/input/FULL_TEST_su_cavo_ps_pp1_V3/"
PROCESSED_TEST="${PWD}/docs/ProcessedTest.txt"
# MODE = 0 ---> continuity
# MODE = 1 ---> isolation
# MODE = 2 ---> continuity & isolation
MODE=2
SLEEP_TIME=2

echo "****************************************************************************************************"
echo "*                                  OT CABLES CMS PhaseII Test                                      *"
echo "****************************************************************************************************"
echo "this script is triggered every ${SLEEP_TIME} seconds if new tests not present in the DB are uploaded."

previous_output=""

while true; do
    cd "${PWD}/py/"
    output=$(python3 AutoRun.py)
    cd ".."
   
    if [[ "$output" == "No new tests" ]]; then
        TEXT="No new tests found, $(date)"
        echo "${TEXT}"
    
    else 
        echo "=== SERIAL NUMBER(S) ==="
        echo "$output"
        IFS=$'\n' read -r -d '' -a NewTestsArray <<< "$output"

        if [[ "$output" == "prova" ]]; then 
            for test in "${NewTestsArray[@]}"; do
                echo "$test already tracked. Maybe the output was not produced"
            done
            output="none"
        else
            for test in "${NewTestsArray[@]}"; do
                echo "New Test Found, running script for $test"
                ./CMScables --input "${test}" --mode "${MODE}" >> OTCablesPhaseIItests.log 2>&1
                echo "End for $test"
            done
            echo "ALL NEW TESTS HAVE BEEN PROCESSED." 
        fi
    fi

    previous_output="$output"
    sleep ${SLEEP_TIME}
done

