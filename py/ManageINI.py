import sys
import re
import os
import warnings

warnings.filterwarnings("ignore")
filename = sys.argv[1]
base, ext = os.path.splitext(filename)
output_dir = os.path.dirname(filename)
output_filename = f"{os.path.basename(base)}_tmp{ext}"

try:
    with open(filename, 'r', encoding='utf-8') as file:
        first_line = file.readline().strip()
        if "[Test]" not in first_line:
            sys.exit(1)
except UnicodeDecodeError:
    with open(filename, 'r', encoding='iso-8859-1') as file:
        first_line = file.readline().strip()
        if "[Test]" not in first_line:
            sys.exit(1)

try:
    with open(filename, 'r', encoding='utf-8') as file:
        lines = file.readlines()
except UnicodeDecodeError:
    with open(filename, 'r', encoding='iso-8859-1') as file:
        lines = file.readlines()

filtered_numbers = []


for line in lines:
    line = line.strip()
    if "Count" in line or "ValueResult" in line:
        continue 
    match = re.search(r'=(.*)', line)
    if match:
        number = match.group(1)
        filtered_numbers.append(number)

filtered_text = '\n'.join(filtered_numbers)

with open(output_dir + "/" + output_filename, 'w' , encoding='utf-8') as file:
    file.write(filtered_text)

    


