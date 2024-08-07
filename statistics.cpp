#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <tuple>
#include "TROOT.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TFile.h"
#include "TLatex.h"
#include "TTree.h"
#include "ROOT/RDataFrame.hxx"


int file=0;
using namespace ROOT;

void ReadOutput(const std::string TestNameFile) {
    TFile *f_output = new TFile(("stat_root/" + std::to_string(file) + ".root").c_str(), "RECREATE");
    
    // Create trees in the file
    TTree *TestResultContinuity = new TTree("TestResultContinuity", "TestResultContinuity");
    TTree *TestResultIsolation = new TTree("TestResultIsolation", "TestResultIsolation");

    Float_t resistenceCon, resistenceIns;
    std::string channelCon, channelIns;
    Bool_t statusCon, statusIns;

    TestResultContinuity->Branch("resistenceCon", &resistenceCon, "resistenceCon/F");
    TestResultContinuity->Branch("channelCon", &channelCon);
    TestResultContinuity->Branch("statusCon", &statusCon, "statusCon/O");

    TestResultIsolation->Branch("resistenceIns", &resistenceIns, "resistenceIns/F");
    TestResultIsolation->Branch("channelIns", &channelIns);
    TestResultIsolation->Branch("statusIns", &statusIns, "statusIns/O");

    std::ifstream inputFile(TestNameFile);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << TestNameFile << std::endl;
        return;
    }

    std::string line;
    std::vector<std::tuple<std::string, std::string, double>> continuityData;
    std::vector<std::tuple<std::string, std::string, double>> insulationData;
    bool FirstTree = false;
    bool SecondTree = false;
    int lineCounter = 0;

    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        if (line.find("ContinuityTest") != std::string::npos) {
            FirstTree = true;
            SecondTree = false;
        } else if (line.find("InsulationTest") != std::string::npos) {
            FirstTree = false;
            SecondTree = true;
        } else if (lineCounter > 5) {
            std::string str1, str2;
            double r;
            if (FirstTree && (iss >> str1 >> str2 >> r)) {
                continuityData.emplace_back(str1, str2, r);
            } else if (SecondTree && (iss >> str1 >> str2 >> r)) {
                insulationData.emplace_back(str1, str2, r);
            }
        }
        lineCounter++;
    }

    for (const auto& it : continuityData) {
        statusCon = (std::get<0>(it) == "Passed");
        channelCon = std::get<1>(it);
        resistenceCon = std::get<2>(it);
        TestResultContinuity->Fill();
    }
    for (const auto& it : insulationData) {
        statusIns = (std::get<0>(it) == "Passed");
        channelIns = std::get<1>(it);
        resistenceIns = std::get<2>(it);
        TestResultIsolation->Fill();
    }

    TestResultContinuity->Write();
    TestResultIsolation->Write();
    f_output->Close(); 
}

bool LV_filter(const std::string& channel) {
    return channel.find("LV") != std::string::npos;
}

bool HV_filter(const std::string& channel) {
    return channel.find("HV") != std::string::npos;
}

void statistics() {
    ROOT::EnableImplicitMT();

    std::vector<std::string> FileNames;
    std::string sInputDir = "./input/FULL_TEST_su_cavo_ps_pp1_V3/";
    for (int i = 0; i < 3; i++) {
        std::string dir = sInputDir + Form("Cable0%i", i + 1);
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
           if(entry.path().filename() != "VALORI") FileNames.push_back(sInputDir + Form("Cable0%i/", i + 1) + entry.path().filename().string());
        }
    }
    for (int j=0; j<int(FileNames.size()); j++) {
        ++file;
        ReadOutput(FileNames[j]);
    }

    TChain inputChain_continuity("TestResultContinuity");
    TChain inputChain_isolation("TestResultIsolation");
    inputChain_continuity.Add("./stat_root/*.root");
    inputChain_isolation.Add("./stat_root/*.root");


    ROOT::RDataFrame df_Continuity(inputChain_continuity);
    ROOT::RDataFrame df_Isolation(inputChain_isolation);
    auto nEntries = df_Continuity.Count().GetValue();  // Get total number of entries
    auto nEntries2 = df_Isolation.Count().GetValue();  // Get total number of entries
  for (const auto& name : df_Continuity.GetColumnNames()) {
    std::cout << "Column in Continuity DataFrame: " << name << std::endl;
}
for (const auto& name : df_Isolation.GetColumnNames()) {
    std::cout << "Column in Isolation DataFrame: " << name << std::endl;
}


auto h_PassedFailedContinuity = df_Continuity.Histo1D(
{"h_PassedFailedContinuity", "Continuity Test Passed/Failed", 2, 0, 2},
    "statusCon"
);
auto h_PassedFailedIsolation = df_Isolation.Histo1D({"h_PassedFailedIsolation", "Isolation Test Passed/Failed", 2,0,2}, "statusIns");
auto h_LV_ResistenceContinuity = df_Continuity.Filter(LV_filter, {"channelCon"}).Histo1D({
"h_LV_ResistenceContinuity", "LV Resistence Continuity", 25, 0.53, 0.60}, "resistenceCon");
    auto h_LV_ResistenceIsolation = df_Isolation.Filter(LV_filter, {"channelIns"}).Histo1D({
"h_LV_ResistenceIsolation", "LV Resistence Isolation", 50, 1e+07, 1e+10}, "resistenceIns");

auto h_HV_ResistenceContinuity = df_Continuity.Filter(HV_filter, {"channelCon"}).Histo1D({
"h_HV_ResistenceContinuity", "HV Resistence Continuity", 50, 10, 13},"resistenceCon");
auto h_HV_ResistenceIsolation = df_Isolation.Filter(HV_filter, {"channelIns"}).Histo1D({
"h_HV_ResistenceIsolation", "HV Resistence Isolation", 25, 1e+06, 1e+10},"resistenceIns");

    TFile *f_StatOut = new TFile("./docs/statistic.root", "RECREATE");

    TCanvas *c = new TCanvas("h_LV_ResistenceContinuity", "h_LV_ResistenceContinuity", 1);
    h_LV_ResistenceContinuity->Draw();
    h_LV_ResistenceContinuity->SetTitle("");
    h_LV_ResistenceContinuity->GetXaxis()->SetTitle("R [#Omega]");
    h_LV_ResistenceContinuity->GetYaxis()->SetTitle("Entries");
    TLatex text;
    text.SetTextSize(0.03);
    text.DrawLatexNDC( 0.15,0.93, "Continuity Test LV channels resistence, all cables");
    c->SetTitle("");
    c->Write();

    TCanvas *c2 = new TCanvas("h_LV_ResistenceIsolation", "h_LV_ResistenceIsolation", 1);
    h_LV_ResistenceIsolation->Draw();
    h_LV_ResistenceIsolation->SetTitle("");
    h_LV_ResistenceIsolation->GetXaxis()->SetTitle("R [#Omega]");
    h_LV_ResistenceIsolation->GetYaxis()->SetTitle("Entries");
    TLatex text2;
    text2.SetTextSize(0.03);
    text2.DrawLatexNDC( 0.15,0.93, "Isolation Test LV channels resistence, all cables");
    c2->SetTitle("");
    c2->Write();

    TCanvas *c3 = new TCanvas("h_HV_ResistenceContinuity", "h_HV_ResistenceContinuity", 1);
    h_HV_ResistenceContinuity->Draw();
    h_HV_ResistenceIsolation->SetTitle("");
    h_HV_ResistenceContinuity->SetTitle("");
    h_HV_ResistenceContinuity->GetXaxis()->SetTitle("R [#Omega]");
    h_HV_ResistenceContinuity->GetYaxis()->SetTitle("Entries");
    TLatex text3;
    text3.SetTextSize(0.03);
    text3.DrawLatexNDC( 0.15,0.93, "Continuity Test HV channels resistence, all cables");
    c3->SetTitle("");
    c3->Write();

    TCanvas *c4 = new TCanvas("h_HV_ResistenceIsolation", "h_HV_ResistenceIsolation", 1);
    h_HV_ResistenceIsolation->Draw();
    h_HV_ResistenceIsolation->GetXaxis()->SetTitle("R [#Omega]");
    h_HV_ResistenceIsolation->GetYaxis()->SetTitle("Entries");
    TLatex text4;
    text4.SetTextSize(0.03);
    text4.DrawLatexNDC( 0.15,0.93, "Isolation Test HV channels resistence, all cables");
    c4->SetTitle("");
    c4->Write();

     TCanvas *c5 = new TCanvas("h_PassedFailedIsolation", "h_PassedFailedIsolation", 1);
    c5->SetTitle(""); 
    h_PassedFailedIsolation->Draw();
    h_PassedFailedIsolation->SetTitle("");
    h_PassedFailedIsolation->GetXaxis()->SetTitle("");
    h_PassedFailedIsolation->GetYaxis()->SetTitle("Entries");
    TLatex text5;
    text5.SetTextSize(0.03);
    text5.DrawLatexNDC(0.15, 0.93, "Isolation Test, passed/failed channels, all cables");
    c5->Write();

    
     TCanvas *c6 = new TCanvas("h_PassedFailedContinuity", "h_PassedFailedContinuity", 1);
    c6->SetTitle(""); 
    h_PassedFailedContinuity->Draw();
    h_PassedFailedContinuity->SetTitle("");
    h_PassedFailedContinuity->GetXaxis()->SetTitle("");
    h_PassedFailedContinuity->GetYaxis()->SetTitle("Entries");
    TLatex text6;
    text6.SetTextSize(0.03);
    text6.DrawLatexNDC(0.15, 0.93, "Continuity Test, passed/failed channels, all cables");
    c6->Write();




    f_StatOut->Close(); 
    gROOT->ProcessLine(".q");
}
