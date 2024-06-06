#include <vector>
#include "../include/root.h"
#include "../include/def_variables.h"

void WritePDF(std::vector<TCanvas*> &canvas, std::string sPDFTitle){
    TPDF report(("./output/pdf_ceetis/" + sPDFTitle + ".pdf").c_str());
    report.TextNDC(0.1,0.1, "Plot per il Test di Continuità");
    for(const auto& it : canvas){
     if(it){
        it->Draw();
        //it->Print((report.GetName()));
       if(it != canvas.back())  report.NewPage();
    }
    report.Text(100,10, "Plot per il Test di Continuità");

    }
    report.Print("c_plot.pdf");
    report.Close();
}