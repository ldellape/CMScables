#include <string>
#include <vector>
#include "../include/root.h"
#include "../include/def_variables.h"

void plottingGraph(std::vector<TGraph*> &gr, Int_t NumberTest, Int_t LVchannel){

for(int i=0; i<LVchannel; i++){
    std::cout<<i<<std::endl;
    TPad *padGraph = (TPad*) c_graph[NumberTest]->cd(i);
    padGraph->Draw();
    padGraph->SetLogy(1);
    padGraph->cd();
    gr[i]->SetTitle("");
    gr[i]->SetLineColor(i+1);
    gr[i]->SetLineWidth(1);
    gr[i]->SetMarkerStyle(i+1);
    gr[i]->SetMarkerSize(1.);
    gr[i]->GetXaxis()->SetTitle("t [s]");
    gr[i]->GetYaxis()->SetTitle("R [#Omega]");
    gr[i]->GetYaxis()->SetLimits(0, 1e+15);
    gr[i]->GetYaxis()->SetNdivisions(8);
    gr[i]->Draw("ACP");
}
c_graph[NumberTest]->SaveAs(("graph" + std::to_string(NumberTest) + ".pdf").c_str());

}