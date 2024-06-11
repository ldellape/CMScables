from PyPDF2 import PdfMerger
import sys

def merge_pdfs(output_path, pdf_list):
    merger = PdfMerger()

    for pdf in pdf_list:
        merger.append(pdf)

    merger.write(output_path)
    merger.close()

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python merge_pdfs.py output.pdf input1.pdf input2.pdf ...")
        sys.exit(1)

    output_path = sys.argv[1]
    pdfs = sys.argv[2:]
    merge_pdfs(output_path, pdfs)
    print(f"PDFs merged into '{output_path}'")
