import sys
import re

if len(sys.argv) != 2:
    print("Usage: python3 filter_passed_failed.py <filename>")
    sys.exit(1)

filename = sys.argv[1]

modified_line = []

with open(filename, 'r') as file:
    lines = file.readlines()

filtered_lines = []
param_lines = []

for line in lines:
    line = line.strip() 

    if re.search(r'\b(Passed|Failed)\b', line):
        line = line.replace(";","")
        line = line.replace("Ohm","")
        line = line.replace("MOhm","")
        line = line.replace("GOhm","")
        filtered_lines.append(line)
    elif "CONTINUITY AND RESISTANCE MEASUREMENTS" in line:
        filtered_lines.append("ContinuityTest")
    elif "INSULATION TEST" in line:
        filtered_lines.append("InsulationTest")
    elif "Tmeas=" in line:
        line = line.replace("Current", "")
        line = line.replace("Threshold","")
        line = line.replace(";","")
        line = line.replace("Twait", "")
        line = line.replace("Tmeas","")
        line = line.replace("Auto ranging","")
        line = line.replace("Trise", "")
        line = line.replace("m","")
        line = line.replace("A", "")
        line = line.replace("s", "")
        line = line.replace("Ohm","")
        line = line.replace("=","")
        line = line.replace("MOh", "")
        line = line.replace("Voltage")
        line = line.replace("V","")
        param_lines.append(line)        

#    elif re.search(r'\b()')

final_lines = param_lines + filtered_lines
filtered_text = '\n'.join(filtered_lines)

with open(filename, 'w') as file:
    file.write(filtered_text)

print(f"Filtered lines containing 'Passed' or 'Failed' from '{filename}'.")
