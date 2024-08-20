import sys
import re
import os

if len(sys.argv) != 2:
    print("Usage: python3 ManageTXT.py <filename>")
    sys.exit(1)

filename = sys.argv[1]

base, ext = os.path.splitext(filename)
output_filename = f"processed_{os.path.basename(base)}{ext}"
output_dir = f"{os.path.dirname(filename)}/tmp/"

if not os.path.exists(output_dir) and output_dir:
    os.makedirs(output_dir, exist_ok=True)

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
        if re.search('return', line):
            line = line.replace("return", "R")
        if re.search(r'H\d', line):
            line = line.replace("H", "HV")
        if re.search(r'HR\d', line):
            line = line.replace("HR", "HVR")
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

final_lines = param_lines + filtered_lines
filtered_text = '\n'.join(final_lines)
print(output_dir)
print(output_filename)
# Write the processed data to the new file
with open(output_dir + output_filename, 'w', encoding='utf-8') as file:
    file.write(filtered_text)

print(f"\033[32mInput text file processed and saved as: '{output_filename}'.\033[0m")
