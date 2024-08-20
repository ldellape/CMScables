import sys
import re

def add_file_to_const_files(html_file, new_file):
    with open(html_file, 'r', encoding='utf-8') as file:
        html_content = file.read()

    pattern = r'const files = \[\s*(.*?)\];'

    match = re.search(pattern, html_content, re.DOTALL)

    if match:
        current_files = match.group(1).strip()
        updated_files = current_files.rstrip(',') + f",\n    \"{new_file}\""
        updated_html = html_content.replace(current_files, updated_files)

        with open(html_file, 'w', encoding='utf-8') as file:
            file.write(updated_html)
    else:
        print(f"Error: Couldn't find const files array in {html_file}.")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python add_file_to_const_files.py <html_file> <new_file>")
        sys.exit(1)
    
    html_file = sys.argv[1]
    new_file = sys.argv[2]
    
    add_file_to_const_files(html_file, new_file)
