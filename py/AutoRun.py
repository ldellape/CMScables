import os
import sys

def find_new_tests(InputDir, ListTestsFile, output_file):
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

    if new_tests:
        with open(output_file, 'w') as outfile:
            for full_path, immediate_dir, file_name in current_test:
                if full_path in new_tests:
                    outfile.write(f"{full_path}    {immediate_dir}    {file_name}\n")
        print(f"New tests found and written to {output_file}.")
    else:
        print("No new tests found. END.")
    
    with open(ListTestsFile, 'w') as outfile:
        for full_path, _, _ in current_test:
            outfile.write(f"{full_path}\n")

InputDir = sys.argv[1]
ListTestsFile = sys.argv[2]
output_file = sys.argv[3]

find_new_tests(InputDir, ListTestsFile, output_file)
