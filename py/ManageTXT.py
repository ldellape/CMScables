import sys
import re

if len(sys.argv) != 2:
    print("Usage: python3 filter_passed_failed.py <filename>")
    sys.exit(1)

filename = sys.argv[1]

try:
    with open(filename, 'r', encoding='utf-8') as file:
        first_line = file.readline().strip()
        if "Ambient Temperature" not in first_line:
            print("Text file is already in the right format")
            sys.exit(1)
except UnicodeDecodeError:
    with open(filename, 'r', encoding='iso-8859-1') as file:
        first_line = file.readline().strip()
        if "Ambient Temperature" not in first_line:
            print("Text file is already in the right format")
            sys.exit(1)

try:
    with open(filename, 'r', encoding='utf-8') as file:
        lines = file.readlines()
except UnicodeDecodeError:
    with open(filename, 'r', encoding='iso-8859-1') as file:
        lines = file.readlines()


def read_file(file_name, encoding):
    with open(file_name, 'r', encoding=encoding) as file:
        lines = file.readlines()
    return lines

try:
    lines = read_file(filename, 'utf-8')
except UnicodeDecodeError:
    lines = read_file(filename, 'iso-8859-1')

filtered_lines = []
param_lines = []
tsensors_lines = []
Insulation_test_index = None

continuity_labels = []
insulation_labels = []

for index, line in enumerate(lines):
    line = line.strip()

    if re.search(r'\b(Passed|Failed)\b', line):
        line = line.replace(";", "")
        line = line.replace("Ohm", "")
        line = line.replace("MOhm", "")
        line = line.replace("GOhm", "")
 #       if re.search(r'LVreturn\d', line):
  #          line = line.replace("LVreturn","LVR")
        if re.search('return', line):
            line = line.replace("return", "R")
        if re.search(r'H\d', line):
            line = line.replace("H", "HV")
        if re.search(r'HR\d', line):
            line = line.replace("HR", "HVR")
#        if re.search(r)
        filtered_lines.append(line)
    elif "CONTINUITY AND RESISTANCE MEASUREMENTS" in line:
        filtered_lines.append("ContinuityTest")
    elif "INSULATION TEST" in line:
        filtered_lines.append("InsulationTest")
        Insulation_test_index = len(filtered_lines) - 1
    elif "Tmeas=" in line:
        line = line.replace("Current", "")
        line = line.replace("Threshold", "")
        line = line.replace(";", "")
        line = line.replace("Twait", "")
        line = line.replace("Tmeas", "")
        line = line.replace("Auto ranging", "")
        line = line.replace("Trise", "")
        line = line.replace("m", "")
        line = line.replace("A", "")
        line = line.replace("s", "")
        line = line.replace("Ohm", "")
        line = line.replace("=", "")
        line = line.replace("MOh", "")
        line = line.replace("GOh", "")
        line = line.replace("Voltage", "")
        line = line.replace("k", "")
        line = line.replace(",", ".")
        line = line.replace("Oh", "")
        line = line.replace("V", "")
        param_lines.append(line)
    if re.search(r'Tsensor\d', line) and Insulation_test_index is not None and index > Insulation_test_index:
        tsensors_lines.append(line)

# Collect labels from ContinuityTest and InsulationTest
for i, line in enumerate(filtered_lines):
    if "ContinuityTest" in line:
        j = i + 1
        while j < len(filtered_lines) and "Test" not in filtered_lines[j]:
            label = filtered_lines[j].split()[1]
            continuity_labels.append(label)
            j += 1
    if "InsulationTest" in line:
        j = i + 1
        while j < len(filtered_lines) and "Test" not in filtered_lines[j]:
            label = filtered_lines[j].split()[1]
            insulation_labels.append(label)
            j += 1

# Ensure the labels are added in the same order
for label in continuity_labels:
    if label not in insulation_labels:
        index = Insulation_test_index + 1
        while index < len(filtered_lines) and "Test" not in filtered_lines[index]:
            current_label = filtered_lines[index].split()[1]
            if continuity_labels.index(label) < continuity_labels.index(current_label):
                break
            index += 1
        filtered_lines.insert(index, f"Added {label} 0")

phr_index = None
for i, line in enumerate(filtered_lines):
    if "PHR" in line:
        phr_index = i

if phr_index is not None and phr_index > Insulation_test_index:
    filtered_lines = filtered_lines[:phr_index + 1] + tsensors_lines + filtered_lines[phr_index + 1:]


final_lines = param_lines + filtered_lines
filtered_text = '\n'.join(final_lines)

with open(filename, 'w', encoding='utf-8') as file:
    file.write(filtered_text)

print(f"\033[32mInput text file changed: '{filename}'.\033[0m")
