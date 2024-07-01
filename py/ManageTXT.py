import sys
import re

if len(sys.argv) != 2:
    print("Usage: python3 filter_passed_failed.py <filename>")
    sys.exit(1)

filename = sys.argv[1]

modified_line = []

try:
    with open(filename, 'r', encoding='utf-8') as file:
        first_line = file.readline().strip()
        if "Ambient Temperature" not in first_line:
            print("Text file already in the right format")
            sys.exit(1)
except UnicodeDecodeError:
    with open(filename, 'r', encoding='iso-8859-1') as file:
        first_line = file.readline().strip()
        if "Ambient Temperature" not in first_line:
            print("Text file already in the right format")
            sys.exit(1)

try:
    with open(filename, 'r', encoding='utf-8') as file:
        lines = file.readlines()
except UnicodeDecodeError:
    with open(filename, 'r', encoding='iso-8859-1') as file:
        lines = file.readlines()

filtered_lines = []
param_lines = []
tsensors_lines = []
Insulation_test_index = None

for index, line in enumerate(lines):
    line = line.strip()

    if re.search(r'\b(Passed|Failed)\b', line):
        line = line.replace(";", "")
        line = line.replace("Ohm", "")
        line = line.replace("MOhm", "")
        line = line.replace("GOhm", "")
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

phr_index = None
for i, line in enumerate(filtered_lines):
    if "PHR" in line:
        print("ok")
        phr_index = i

if phr_index is not None and phr_index > Insulation_test_index:
    filtered_lines = filtered_lines[:phr_index + 1] + tsensors_lines + filtered_lines[phr_index + 1:]

final_lines = param_lines + filtered_lines
filtered_text = '\n'.join(final_lines)

with open(filename, 'w', encoding='utf-8') as file:
    file.write(filtered_text)

print(f"Filtered lines containing 'Passed' or 'Failed' from '{filename}'.")
