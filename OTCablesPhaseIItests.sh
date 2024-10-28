#!/bin/bash


INPUT_TEST="${PWD}/input/FULL_TEST_su_cavo_ps_pp1_V3/"
PROCESSED_TEST="${PWD}/docs/ProcessedTest.txt"
MODE=2
SLEEP_TIME=1

echo "****************************************************************************************************"
echo "*                                  OT CABLES CMS PhaseII Test                                      *"
echo "****************************************************************************************************"
echo "INFN sez.Perugia"
echo "this script is triggered every ${SLEEP_TIME} seconds if new tests not present in the DB are uploaded."

while true; do
    cd "${PWD}/py/"
    output=$(python3 AutoRun.py)
    cd ".."

    if [[ "$output" == "No new tests" ]]; then
        TEXT="No new tests found, $(date)"
        echo "${TEXT}"
        echo "${TEXT}" >> OTCablesPhaseIItests.log
    else 
        echo "$output"
        IFS=$'\n' read -r -d '' -a NewTestsArray <<< "$output"

    for test in "${NewTestsArray[@]}"; do
        echo "New Test Found, running script for $test"
        ./CMScables --input "${test}" --mode "${MODE}" >> OTCablesPhaseIItests.log 2>&1
        echo "End for $test"
    done
    echo "ALL NEW TESTS HAVE BEEN PROCESSED." 
fi
sleep ${SLEEP_TIME}

done
