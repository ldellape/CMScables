#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <filesystem>
#include <tuple>
#include "TROOT.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TFile.h"
#include "TLatex.h"
#include "TTree.h"
#include "TSystem.h"
#include "ROOT/RDataFrame.hxx"


// ROOT file to appear on web page, defined in main  //
TFile *f_StatOut;

void DrawPlot(TString sTitle, ROOT::RDF::RResultPtr<::TH1D> histo, std::string option){
    TCanvas *c = new TCanvas(sTitle, sTitle, 2000, 1500);
    f_StatOut->cd();
    if(sTitle.Contains("passed")){
        histo->GetXaxis()->SetBinLabel(1, "Failed");
        histo->GetXaxis()->SetBinLabel(2, "Passed");
        histo->Draw(option.c_str());
        TLatex textTitle;
        textTitle.SetTextSize(0.03);
        textTitle.DrawLatexNDC(0.02, 0.92, sTitle);
        c->Write();
    }
    else{
        histo->Draw();
        histo->GetXaxis()->SetTitle("R [#Omega]");
        histo->GetYaxis()->SetTitle("Entries");
        histo->SetTitle("");
        TLatex textTitle;
        textTitle.SetTextSize(0.03);
        textTitle.DrawLatexNDC(0.02, 0.92, sTitle);
        c->Write();
    }
}

void ReadOutput(const std::string TestNameFile, Int_t file) {
    TFile *f_output = new TFile(("stat_root/" + std::to_string(file) + ".root").c_str(), "RECREATE");
    
    TTree *TestResultContinuity = new TTree("TestResultContinuity", "TestResultContinuity");
    TTree *TestResultIsolation = new TTree("TestResultIsolation", "TestResultIsolation");

    Float_t resistenceCon, resistenceIns;
    Bool_t channelLV_Con, channelLV_Ins;
    Bool_t channelHV_Con, channelHV_Ins, channelPHR_Con, channelTsensor_Con;
    Bool_t statusCon, statusIns, channelPHR_Ins, channelTsensor_Ins;

    TestResultContinuity->Branch("resistenceCon", &resistenceCon, "resistenceCon/F");
    TestResultContinuity->Branch("channelLV_Con", &channelLV_Con, "channelLV_Con/O");
    TestResultContinuity->Branch("channelHV_Con", &channelHV_Con, "channelHV_Con/O");
    TestResultContinuity->Branch("statusCon", &statusCon, "statusCon/O");
    TestResultContinuity->Branch("channelPHR_Con", &channelPHR_Con, "channelPHR_Con/O");
    TestResultContinuity->Branch("channelTsensor_Con", &channelTsensor_Con, "channelTsensor_Con/O");

    TestResultIsolation->Branch("resistenceIns", &resistenceIns, "resistenceIns/F");
    TestResultIsolation->Branch("channelLV_Ins", &channelLV_Ins, "channelLV_Ins/O");
    TestResultIsolation->Branch("channelHV_Ins", &channelHV_Ins, "channelHV_Ins/O");
    TestResultIsolation->Branch("statusIns", &statusIns, "statusIns/O");
    TestResultIsolation->Branch("channelPHR_Ins", &channelPHR_Ins, "PHR_Ins/O");
    TestResultIsolation->Branch("channelTsensor_Ins", &channelTsensor_Ins, "channelTsensor_Ins/O");

    std::ifstream inputFile(TestNameFile);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << TestNameFile << std::endl;
        return;
    }

    std::string line;
    std::vector<std::tuple<std::string, std::string, double>> continuityData;
    std::vector<std::tuple<std::string, std::string, double, double>> insulationData;
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
        } else if (lineCounter > 4) {
            std::string str1, str2;
            double r, B;
            if (FirstTree && (iss >> str1 >> str2 >> r)) {
                continuityData.emplace_back(str1, str2, r);
            } else if (SecondTree && (iss >> str1 >> str2 >> r )) {
                insulationData.emplace_back(str1, str2, r, B);
            }
        }
        lineCounter++;
    }

    for (const auto& it : continuityData) {
        statusCon = (std::get<0>(it) == "Passed");
        channelHV_Con = false;
        channelLV_Con = false;
        channelPHR_Con = false;
        channelTsensor_Con = false;
        if( std::get<1>(it).find("LV") != std::string::npos) channelLV_Con = true;
        else if (std::get<1>(it).find("HV") != std::string::npos) channelHV_Con = true; 
        else if( std::get<1>(it).find("PH") != std::string::npos) channelPHR_Con = true;
        else if( std::get<1>(it).find("Tsensor") != std::string::npos) channelTsensor_Con = true;
        resistenceCon = std::get<2>(it);
        TestResultContinuity->Fill();
    }
    for (const auto& it : insulationData) {
        statusIns = (std::get<0>(it) == "Passed");
        channelHV_Ins = false;
        channelLV_Ins = false;
        channelPHR_Ins = false;
        channelTsensor_Ins = false;
        if( std::get<1>(it).find("LV") != std::string::npos) channelLV_Ins = true;
        else if (std::get<1>(it).find("HV") != std::string::npos) channelHV_Ins = true; 
        else if( std::get<1>(it).find("PH") != std::string::npos) channelPHR_Ins = true;
        else if( std::get<1>(it).find("Tsensor") != std::string::npos) channelTsensor_Ins = true;
        resistenceIns = std::get<2>(it);
        TestResultIsolation->Fill();
    }
    TestResultContinuity->Write();
    TestResultIsolation->Write();
    f_output->Close(); 
}
// ******************************************************************** //



int main(){

 ROOT::EnableImplicitMT();

// ******************************************************************** //
// ************* INPUT FILES AND OUTPUT FILES DEFINITIONS ************* //
// ******************************************************************** //
 std::system("mkdir -p stat_root");
 std::set<std::string> tests;
 const char *pathOutFile = "./docs/statistics.root";
 const char *TestProcessedTXT = "./docs/statistics_tests.txt";
 if(!gSystem->AccessPathName(pathOutFile)){
    std::cout << "check if new tests exist..." 
    f_StatOut = TFile::Open(pathOutFile, "UPDATE");
    std::ifstream TestProcessed(TestProcessedTXT);
    std::string str;
    while (TestProcessed >> str){
        tests.insert(str);
    }
    TestProcessed.close();
 }
 else{
    std::cout << "creating statistics.root ..." << std::endl;
    f_StatOut = new TFile(pathOutFile, "RECREATE");
 }
 
 std::vector<std::string> FileNames;
 std::string sInputDir = "./input/FULL_TEST_su_cavo_ps_pp1_V3/";
 for (int i = 0; i < 3; i++) {
    std::string dir = sInputDir + Form("Cable0%i", i + 1);
    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        std::string fullpath = sInputDir + Form("Cable0%i/", i + 1) + entry.path().filename().string();
        if(entry.path().filename() != "VALORI" && tests.find(fullpath) == tests.end()){
         std::cout<<" new test(s) will be added : "<<fullpath << std::endl;
         FileNames.push_back(fullpath);
        }
    }
 }
if(FileNames.empty()){
 std::cout << "statistics is already updated. End." << std::endl;
 return 0;
}
Int_t file = 0;
std::ofstream UpdateTests(TestProcessedTXT, std::ios::app);
for (int j=0; j<int(FileNames.size()); j++) {
    ++file;
    ReadOutput(FileNames[j], file);
    UpdateTests << FileNames[j] << "\n";
}
UpdateTests.close();
// ********************************************************************* //
// ************** END INPUT FILES AND OUTPUT DEFINITIONS *************** //
// ********************************************************************* //



TChain inputChain_continuity("TestResultContinuity");
TChain inputChain_isolation("TestResultIsolation");
inputChain_continuity.Add("./stat_root/*.root");
inputChain_isolation.Add("./stat_root/*.root");
ROOT::RDataFrame df_Continuity(inputChain_continuity);
ROOT::RDataFrame df_Isolation(inputChain_isolation);


auto h_PassedFailedContinuity = df_Continuity.Histo1D({"h_PassedFailedContinuity", "Continuity Test Passed/Failed", 2, 0, 2},"statusCon");
auto h_PassedFailedIsolation = df_Isolation.Histo1D({"h_PassedFailedIsolation", "Isolation Test Passed/Failed", 2,0,2}, "statusIns");
auto h_LV_ResistenceContinuity = df_Continuity.Filter("channelLV_Con == true || channelPHR_Con == true").Histo1D({"h_LV_ResistenceContinuity", "LV Resistence Continuity", 50, 0.53, 0.60}, "resistenceCon");
auto h_LV_ResistenceIsolation = df_Isolation.Filter("channelLV_Ins == true || channelPHR_Ins == true").Histo1D({"h_LV_ResistenceIsolation", "LV Resistence Isolation", 50, 1e+05, 1e+09}, "resistenceIns");
auto h_HV_ResistenceContinuity = df_Continuity.Filter("channelHV_Con == true || channelTsensor_Con == true").Histo1D({"h_HV_ResistenceContinuity", "HV Resistence Continuity", 50, 10, 13},"resistenceCon");
auto h_HV_ResistenceIsolation = df_Isolation.Filter("channelHV_Ins == true || channelTsensor_Ins == true").Histo1D({"h_HV_ResistenceIsolation", "HV Resistence Isolation", 25, 1e+06, 1e+10},"resistenceIns");

// *********** //

f_StatOut->cd();
DrawPlot("Continuity Test, passed/failed, all cables", h_PassedFailedContinuity, "p");
DrawPlot("Isolation Test, passed/failed channels, all cables", h_PassedFailedIsolation, "p");
DrawPlot("Isolation Test HV and Tsensors channels resistence, all cables", h_HV_ResistenceIsolation, "hist");
DrawPlot("Continuity Test HV and Tsensors channels resistence, all cables", h_HV_ResistenceContinuity, "hist");
DrawPlot("Isolation Test LV and PH channels resistence, all cables", h_LV_ResistenceIsolation, "hist");
DrawPlot("Continuity Test LV and PH channels resistence, all cables", h_LV_ResistenceContinuity, "hist");
f_StatOut->Close(); 

std::system("rm -r stat_root");

return 0;
gROOT->ProcessLine(".q");

}
