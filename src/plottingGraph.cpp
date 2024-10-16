#include <string>
#include <vector>
#include <iostream>
#include <TString.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TGraph.h>
#include <TLatex.h>
#include "root.h"
#include "def_variables.h"
#include "../include/style.h"

void plottingGraph(std::vector<std::pair<std::string, TGraph*>> gr[], std::string Title) {
    gStyle->SetOptStat(0);

    // Preparing name of the test
    TString name[IterationTest];
    for (int j = 0; j < IterationTest; j++) {
        name[j] = TestName[j].substr(TestName[j].rfind("/") + 1, TestName[j].rfind(".") - TestName[j].rfind("/") - 1);
    }

    TCanvas *c[IterationTest];
    for (int kk = 0; kk < IterationTest; kk++) {
        c[kk] = new TCanvas(("c_" + Title + std::to_string(kk)).c_str(), ("c_" + Title + std::to_string(kk)).c_str(), 3000, 3500);
        c[kk]->SetFillStyle(4000);
        const int Nx = 3;
        int Ny = (int(gr[kk].size()) + Nx - 1) / Nx; // Calculate the number of rows needed

        Float_t lMargin = 0.06;
        Float_t rMargin = 0.06;
        Float_t bMargin = 0.04;
        Float_t tMargin = 0.01;

        CanvasPartition(c[kk], Nx, Ny, lMargin, rMargin, bMargin, tMargin);

        TPad* pad[Nx][Ny];
        int graphIndex = 0;

        for (int j =0; j < Ny; j++) {
            for (int i =0; i < Nx; i++) {
                if (graphIndex < int(gr[kk].size())) {
                    c[kk]->cd();
                    pad[i][j] = (TPad*)c[kk]->FindObject(TString::Format("pad_%d_%d", i, j).Data());
                    if (pad[i][j] == nullptr) {
                        std::cerr << "Error: Could not find pad pad_" << i << "_" << j << std::endl;
                        continue;
                    }
                    pad[i][j]->Draw();
                    pad[i][j]->SetFillStyle(4000);
                    pad[i][j]->SetFrameFillStyle(4000);
                    pad[i][j]->SetLogy(1);
                    pad[i][j]->cd();

                    // Assuming the pair is <string, TGraph*> in the vector
                    const std::string& channel = gr[kk][graphIndex].first;
                    TGraph* graph = gr[kk][graphIndex].second;
                    if (graph == nullptr) {
                        std::cerr << "Error: Null graph pointer at index " << graphIndex << std::endl;
                        continue;
                    }

                    graph->SetTitle("");
                    graph->SetMarkerStyle(kk + 3);
                    graph->SetMarkerSize(3);
                    graph->SetMarkerColor(kk + 3);
                    graph->SetLineColor(kk + 3);
                    graph->SetLineWidth(1);
                    graph->Draw("ACP");
                    graph->GetXaxis()->SetTitle("t");
                    graph->GetYaxis()->SetTitle("R [#Omega]");

                    TLatex text;
                    text.SetTextSize(0.05);
                   // text.SetTextColor(kRed);
                    text.DrawTextNDC(XtoPad(0.80), YtoPad(0.90), channel.c_str());
                    graphIndex++;
                  if(i==0 && j==0){
                  TLatex legend;
              
                  legend.SetTextSize(0.05);
                  legend.SetTextColor(kk + 3);
                  legend.DrawLatexNDC(0.10, 0.10, name[kk]);
                  }   
                    }
                }
            }
        if(int(gr[kk].size())>0) c[kk]->SaveAs((std::string(WORKDIR) + "/output/plotsTimeResistence/graph_TimeResistence" + Title + "_" + currentDate + "_Cable" + std::to_string(kk) + ".pdf").c_str());
    }
}
