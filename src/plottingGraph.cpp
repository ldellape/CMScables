#include <string>
#include <vector>
#include "../include/root.h"
#include "../include/def_variables.h"

void plottingGraph(std::vector<std::pair<std::string, TGraph*>> &gr, Int_t NumberCable, std::string Title){
    TString name;
    name = TestName[NumberCable].substr( TestName[NumberCable].rfind("/") +1, TestName[NumberCable].rfind(".") - TestName[NumberCable].rfind("/")-1);
    sPDFTitle = name + "____" + currentDate;
    TCanvas *c_graph = new TCanvas(Form("c_graph_%i_%s", NumberCable, Title.c_str()), Form("c_graph_%i_%s",NumberCable, Title.c_str()), 1000,3000);
    int it=0;
    c_graph->Divide(1, gr.size(), 0.001, 0.001);

    for(const auto& pair : gr){
        it++;
        TPad *padGraph = (TPad*) c_graph->cd(it);
        padGraph->Draw();
        padGraph->SetLogy(1);
        padGraph->cd(it);
        pair.second->SetTitle((pair.first).c_str());
        pair.second->SetMarkerSize(1);
        pair.second->GetXaxis()->SetTitle("t [s]");
        pair.second->GetXaxis()->SetTickSize(0.02);
        pair.second->GetYaxis()->SetTitle("R [#Omega]");
        //pair.second->SetMinimum(0);
        //pair.second->SetMaximum(1e+18);
        pair.second->GetYaxis()->SetNdivisions(8);
        TLatex textCable;
        textCable.SetTextSize(0.03);
        textCable.DrawLatexNDC(0.02, 0.05, name);
        pair.second->Draw("ACP");

        c_graph->SaveAs("./output/plotsTimeResistence/graph_TimeResistence" + Title + "_" + sPDFTitle + ".pdf");

    }

}
