import os
import sys
from PyPDF2 import PdfMerger

def merge_pdfs(output_path, pdf_list):
    merger = PdfMerger()
    files_appended = False

    for pdf in pdf_list:
        if not os.path.exists(pdf):
            print(f"\033[33mWarning: The file '{pdf}' does not exist and will be skipped.\033[0m")
            continue
        merger.append(pdf)
        files_appended = True

    if files_appended:
        merger.write(output_path)
    else:
        print("Error: No valid PDF files were provided for merging.")
    
    merger.close()

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python merge_pdfs.py output_path.pdf input1.pdf input2.pdf ...")
        sys.exit(1)

    output_path = sys.argv[1]
    pdfs = sys.argv[2:]
    merge_pdfs(output_path, pdfs)
