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

def changeLine(riga):
        riga = riga.replace("Current", "")
        riga = riga.replace("Threshold", "")
        riga = riga.replace(";", "")
        riga = riga.replace("Twait", "")
        riga = riga.replace("Tmeas", "")
        riga = riga.replace("Auto ranging", "")
        riga = riga.replace("Trise", "")
        riga = riga.replace("m", "")
        riga = riga.replace("A", "")
        riga = riga.replace("s", "")
        riga = riga.replace("Ohm", "")
        riga = riga.replace("=", "")
        riga = riga.replace("MOh", "")
        riga = riga.replace("GOh", "")
        riga = riga.replace("Voltage", "")
        riga = riga.replace("k", "")
        riga = riga.replace(",", ".")
        riga = riga.replace("Oh", "")
        riga = riga.replace("V", "")
        return riga
def cleanParam(riga):
    matches = re.findall(r'\d+.\d+|Off\b|\d+',riga)
    return ' '.join(matches)

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
    elif "Probe Cable Insertion" in line:
            next_line = lines[index +1].strip()
            next_line = next_line.replace(";", "")
            param_lines.append(next_line)
    elif "Tmeas=" in line:
        line = changeLine(line)
        if "Voltage limit" in lines[index +1]:
            next_line = lines[index +1].strip()
            next_line = next_line.replace("Voltage limit=", "")
            next_line = next_line.replace("V", "")
            line+= " " + next_line
        elif "Current limit" in lines[index+1]:
            next_line = lines[index +1].strip()
            next_line = next_line.replace(",",".")
            next_line = cleanParam(next_line)
            line+= " " + next_line
        param_lines.append(line)
    if re.search(r'Tsensor\d', line) and Insulation_test_index is not None and index > Insulation_test_index:
        tsensors_lines.append(line)

final_lines = param_lines + filtered_lines
filtered_text = '\n'.join(final_lines)
# Write the processed data to the new file
with open(output_dir + output_filename, 'w', encoding='utf-8') as file:
    file.write(filtered_text)

print(f"\033[32mInput text file processed and saved as: '{output_dir + output_filename}'.\033[0m")
