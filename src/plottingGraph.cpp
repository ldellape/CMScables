#include <string>
#include <vector>
#include "../include/root.h"
#include "../include/def_variables.h"
#include "../include/style.h"

void plottingGraph(std::vector<std::tuple<int, std::string, TGraph*>> &gr, std::string Title) {
    gStyle->SetOptStat(0);
    c_graph = new TCanvas(("c_graph_" + Title).c_str(), ("c_graph_" + Title).c_str(), 3000, 3500);
    c_graph->SetFillStyle(4000);

    // preparing name of the test //
    TString name[IterationTest];
    for(int j = 0; j < IterationTest; j++) {
        name[j] = TestName[j].substr(TestName[j].rfind("/") + 1, TestName[j].rfind(".") - TestName[j].rfind("/") - 1);
    }
    sPDFTitle = name[0] + "____" + currentDate;
    
    const int Nx = 3;
    int Ny = (gr.size() + Nx - 1) / Nx; // Calculate the number of rows needed

    Float_t lMargin = 0.06;
    Float_t rMargin = 0.06;
    Float_t bMargin = 0.04;
    Float_t tMargin = 0.01;

    CanvasPartition(c_graph, Nx, Ny, lMargin, rMargin, bMargin, tMargin);
    
    TPad *pad[Nx][Ny];
    int graphIndex = 0;

    for(int j = 0; j < Ny; j++) {
        for(int i = 0; i < Nx; i++) {
            if (graphIndex < int(gr.size())) {
                c_graph->cd();
                pad[i][j] = (TPad*) c_graph->FindObject(TString::Format("pad_%d_%d", i, j).Data()); 
                pad[i][j]->Draw();
                pad[i][j]->SetFillStyle(4000);
                pad[i][j]->SetFrameFillStyle(4000);
                pad[i][j]->SetLogy(1);
                pad[i][j]->cd();

                int Cable = std::get<0>(gr[graphIndex]);
                std::string channel = std::get<1>(gr[graphIndex]);
                TGraph *graph = std::get<2>(gr[graphIndex]);
                graph->SetTitle("");
                graph->SetMarkerStyle(Cable + 1);
                graph->SetMarkerSize(3);
                graph->SetMarkerColor(Cable +1);
                graph->SetLineColor(Cable + 1);
                graph->SetLineWidth(1);
                graph->Draw("ACP");
                graph->GetXaxis()->SetTitle("t");
                graph->GetYaxis()->SetTitle("R [#Omega]");
                TLatex text;
                text.SetTextSize(0.05);
                text.SetTextColor(kRed);
                text.DrawTextNDC(XtoPad(0.80), YtoPad(0.90), channel.c_str());
                graphIndex++;
                if(i==0 && j==0){
                    TLatex legend[IterationTest];
                    for(int kk=0; kk<IterationTest; kk++){
                        legend[kk].SetTextSize(0.03);
                        legend[kk].SetTextColor(Cable+1);
                        legend[kk].DrawLatexNDC(0.10,0.10-Cable*0.01, name[kk]); 
                    }
                
                }
            }
        }
    }
        c_graph->SaveAs((std::string(WORKDIR) + "/output/plotsTimeResistence/graph_TimeResistence" + Title + "_" + sPDFTitle + ".pdf").c_str());
        std::cout<<std::string(WORKDIR) + "/output/plotsTimeResistence/graph_TimeResistence" + Title + "_" + sPDFTitle + ".pdf"<<std::endl;
    }
