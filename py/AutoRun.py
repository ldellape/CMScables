import os
import sys

def find_new_tests(InputDir, ListTestsFile):
    NewTests = []
    current_test = set()

    InputDir = os.path.abspath(InputDir)

    for dirpath, dirnames, filenames in os.walk(InputDir):
        for filename in filenames:
            if filename.endswith(".txt"): 
                full_path = os.path.join(dirpath, filename)
                immediate_dir = os.path.basename(os.path.dirname(full_path))
                file_name = os.path.basename(full_path)
                current_test.add((full_path, immediate_dir, file_name))

    processed_test = set()
    
    if os.path.exists(ListTestsFile):
        with open(ListTestsFile, 'r') as infile:
            for line in infile:
                processed_test.add(line.strip())
    
    new_tests = set(t[0] for t in current_test) - processed_test
    
    

    for test in current_test:
        if test[0] in new_tests and "tmp" not in f"{test[1]}/{test[2]}":
            NewTests.append(f"{test[1]}/{test[2]}")
            print(f"{test[1]}/{test[2]}")
    
    if not NewTests:
        print("No new tests")

    return NewTests

ListTestsFile = "/home/ldellape/CMScables/stat/processedTest.txt"
InputDir = "/home/ldellape/CMScables/input/FULL_TEST_su_cavo_ps_pp1_V3/"

if __name__ == "__main__":
    new_tests = find_new_tests(InputDir, ListTestsFile)
#    print(f"Final New Tests: {new_tests}")
