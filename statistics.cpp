#include <vector>
#include <string>
#include <iostream>
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
#include "TProfile.h"
#include "ROOT/RDataFrame.hxx"

#ifdef WORKDIR
#else
#define WORKDIR "."
#endif


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

void ReadOutput(const std::string TestNameFile, Int_t file, TString option) {
    TFile *f_output = new TFile(("stat_root/" + std::to_string(file) + ".root").c_str(), "RECREATE");
    TTree *TestResultContinuity = new TTree("TestResultContinuity", "TestResultContinuity");
    TTree *TestResultIsolation = new TTree("TestResultIsolation", "TestResultIsolation");
    Float_t resistenceCon, resistenceIns;
    Bool_t channelLV_Con, channelLV_Ins;
    Bool_t channelHV_Con, channelHV_Ins, channelPHR_Con, channelTsensor_Con;
    Bool_t statusCon, statusIns, channelPHR_Ins, channelTsensor_Ins;
    Bool_t pspp1_test_con, fullchain_test_con;
    Bool_t pspp1_test_ins, fullchain_test_ins;

    TestResultContinuity->Branch("resistenceCon", &resistenceCon, "resistenceCon/F");
    TestResultContinuity->Branch("channelLV_Con", &channelLV_Con, "channelLV_Con/O");
    TestResultContinuity->Branch("channelHV_Con", &channelHV_Con, "channelHV_Con/O");
    TestResultContinuity->Branch("statusCon", &statusCon, "statusCon/O");
    TestResultContinuity->Branch("channelPHR_Con", &channelPHR_Con, "channelPHR_Con/O");
    TestResultContinuity->Branch("channelTsensor_Con", &channelTsensor_Con, "channelTsensor_Con/O");
    TestResultContinuity->Branch("pspp1_test_con", &pspp1_test_con, "pspp1_test_con/O");
    TestResultContinuity->Branch("fullchain_test_con", &fullchain_test_con, "fullchain_test_con/O");

    TestResultIsolation->Branch("resistenceIns", &resistenceIns, "resistenceIns/F");
    TestResultIsolation->Branch("channelLV_Ins", &channelLV_Ins, "channelLV_Ins/O");
    TestResultIsolation->Branch("channelHV_Ins", &channelHV_Ins, "channelHV_Ins/O");
    TestResultIsolation->Branch("statusIns", &statusIns, "statusIns/O");
    TestResultIsolation->Branch("channelPHR_Ins", &channelPHR_Ins, "PHR_Ins/O");
    TestResultIsolation->Branch("channelTsensor_Ins", &channelTsensor_Ins, "channelTsensor_Ins/O");
    TestResultIsolation->Branch("pspp1_test_ins", &pspp1_test_ins, "pspp1_test_ins/O");
    TestResultIsolation->Branch("fullchain_test_ins", &fullchain_test_ins, "fullchain_test_ins/O");

    std::ifstream inputFile(TestNameFile);
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
        } 
        else if (line.find("InsulationTest") != std::string::npos) {
            FirstTree = false;
            SecondTree = true;
        } 
        else if (lineCounter > 4) {
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
        pspp1_test_con = false;
        fullchain_test_con = false;
        if( std::get<1>(it).find("LV") != std::string::npos) channelLV_Con = true;
        else if (std::get<1>(it).find("HV") != std::string::npos) channelHV_Con = true; 
        else if( std::get<1>(it).find("PH") != std::string::npos) channelPHR_Con = true;
        else if( std::get<1>(it).find("Tsensor") != std::string::npos) channelTsensor_Con = true;
        resistenceCon = std::get<2>(it);
        if(option.CompareTo("PSPP1")==0) pspp1_test_con = true;
        if(option.CompareTo("FULL_CHAIN")==0) fullchain_test_con = true;
        TestResultContinuity->Fill();
    }
    for (const auto& it : insulationData) {
        statusIns = (std::get<0>(it) == "Passed");
        channelHV_Ins = false;
        channelLV_Ins = false;
        channelPHR_Ins = false;
        channelTsensor_Ins = false;
        pspp1_test_ins = false;
        fullchain_test_ins = false;
        if( std::get<1>(it).find("LV") != std::string::npos) channelLV_Ins = true;
        else if (std::get<1>(it).find("HV") != std::string::npos) channelHV_Ins = true; 
        else if( std::get<1>(it).find("PH") != std::string::npos) channelPHR_Ins = true;
        else if( std::get<1>(it).find("Tsensor") != std::string::npos) channelTsensor_Ins = true;
        resistenceIns = std::get<2>(it);
        if(option.CompareTo("PSPP1")==0) pspp1_test_ins = true;
        if(option.CompareTo("FULL_CHAIN")==0) fullchain_test_ins = true;
        TestResultIsolation->Fill();
    }
    TestResultContinuity->Write();
    TestResultIsolation->Write();
    f_output->Close(); 
}
// ******************************************************************** //



int main(){

 ROOT::EnableImplicitMT();
 std::system("mkdir  stat_root");
 std::system("mkdir -p stat");
 std::set<std::string> tests;
 const char *pathOutFile = "./stat/statistics.root";
 const char *TestProcessedTXT = "./stat/processedTest.txt";

// ******************************************************************** //
// ************* INPUT FILES AND OUTPUT FILES DEFINITIONS ************* //
// ******************************************************************** //
if(!gSystem->AccessPathName(pathOutFile)){
    std::cout << "searching for new test(s)..." << std::endl;
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
std::vector<std::string> FileNamesProcessed;

//PSPP1, FULL_CHAIN
std::string sInputDir[2]={"/input/FULL_TEST_su_cavo_ps_pp1_V3/", "/input/FULL_CHAIN/"};
TString cable[2] = {"PSPP1", "FULL_CHAIN"};
Bool_t controls[2] = {true};
for(int ii=0; ii<2; ii++){
 FileNames.clear();
 FileNamesProcessed.clear();
 for (const auto& entry : std::filesystem::recursive_directory_iterator(WORKDIR + sInputDir[ii])) {
        if (!entry.is_regular_file()) continue; 
        std::string filename = entry.path().filename().string();
        std::string fullpath = entry.path().string();

        if (filename != "tmp" && fullpath.find("VALORI") == std::string::npos && tests.find(fullpath) == tests.end() && fullpath.find(".pdf") == std::string::npos) {
            std::string processedText = entry.path().parent_path().string() + "/tmp/processed_" + filename;
            FileNamesProcessed.push_back(processedText);
            FileNames.push_back(fullpath);
        }
 }
 if(FileNames.empty()) controls[ii] = false;
 std::ofstream UpdateTests(TestProcessedTXT, std::ios::app);
 if(ii==1 && !controls[0] && !controls[1]){
   std::cout << "\033[32mstatistics is already up to date. End. \033[0m " << std::endl;
    std::system("rm -r stat_root");
    return 0;
 }


Int_t file = 0;
for (int j = 0; j < int(FileNames.size()); j++) {
    std::cout<<"\033[32mnew test will be added: \033[0m "<<std::endl;
    std::cout<<FileNamesProcessed[j]<<std::endl;
    ++file;
    if(!std::filesystem::exists(FileNamesProcessed[j].c_str())){
        std::string command = "python3 " + std::string(WORKDIR) + "/py/ManageTXT.py ";
        std::system((command + FileNames[j]).c_str());
    }
    ReadOutput(FileNamesProcessed[j], file, cable[ii]);
    UpdateTests << FileNames[j] << "\n";
    tests.insert(FileNames[j]);
}
UpdateTests.close();
}

// ********************************************************************* //
// ************** END INPUT FILES AND OUTPUT DEFINITIONS *************** //
// ********************************************************************* //



TChain inputChain_continuity("TestResultContinuity");
TChain inputChain_isolation("TestResultIsolation");
inputChain_continuity.Add("./stat_root/*.root");
inputChain_isolation.Add("./stat_root/*.root");
ROOT::RDataFrame df_Continuity(inputChain_continuity);
ROOT::RDataFrame df_Isolation(inputChain_isolation);
////////////////////////////////////////
// PS-PP1
auto pspp1 = df_Continuity.Histo1D({"h","h",2,0,2},"statusCon");
auto h_PassedFailedContinuity_pspp1 = df_Continuity.Filter("pspp1_test_con == true").Histo1D({"h_PassedFailedContinuity_pspp1", "Continuity Test Passed/Failed PS-PP1", 2, 0, 2},"statusCon");
auto h_PassedFailedIsolation_pspp1 = df_Isolation.Filter("pspp1_test_ins == true").Histo1D({"h_PassedFailedIsolation_pspp1", "Isolation Test Passed/Failed PS-PP1", 2,0,2}, "statusIns");
auto h_LV_ResistenceContinuity_pspp1 = df_Continuity.Filter("(channelLV_Con == true || channelPHR_Con == true) && pspp1_test_con == true").Histo1D({"h_LV_ResistenceContinuity_pspp1", "LV PS-PP1 Resistence Continuity", 25, 0.53, 0.60}, "resistenceCon");
auto h_LV_ResistenceIsolation_pspp1 = df_Isolation.Filter("(channelLV_Ins == true || channelPHR_Ins == true) && pspp1_test_ins == true").Histo1D({"h_LV_ResistenceIsolation_pspp1", "LV PS-PP1 Resistence Isolation", 25, 1e+05, 1e+09}, "resistenceIns");
auto h_HV_ResistenceContinuity_pspp1 = df_Continuity.Filter("(channelHV_Con == true || channelTsensor_Con == true) && pspp1_test_con==true").Histo1D({"h_HV_ResistenceContinuity_pspp1", "HV PS-PP1 Resistence Continuity", 25, 10, 13},"resistenceCon");
auto h_HV_ResistenceIsolation_pspp1 = df_Isolation.Filter("(channelHV_Ins == true || channelTsensor_Ins == true) && pspp1_test_ins==true").Histo1D({"h_HV_ResistenceIsolation_pspp1", "HV PS-PP1 Resistence Isolation", 25, 1e+06, 1e+10},"resistenceIns");
/* remove comment here for temperature and humidity histos
auto h_LV_Continuity_Temperature_pspp1 = df_Continuity.Filter("(channelLV_Con == true || channelPHR_Con == true) && pspp1_test_con == true").Profile1D({"h_LV_Continuity_Temperature_pspp1", "LV PS-PP1 Profile Resistence Continuity", 50, 10, 30}, "resistenceCon", "Temperature");
auto h_HV_Continuity_Temperature_pspp1 = df_Continuity.Filter("(channelHV_Con == true || channelTsensor_Con == true) && pspp1_test_con == true").Profile1D({"h_HV_Continuity_Temperature_pspp1", "HV PS-PP1 Profile Resistence Continuity", 50, 10, 30}, "resistenceCon", "Temperature");
auto h_LV_Isolation_Temperature_pspp1 = df_Isolation.Filter("(channelLV_Ins == true || channelPHR_Ins == true) && pspp1_test_ins == true").Profile1D({"h_LV_Isolation_Temperature_pspp1", "LV PS-PP1 Profile Resistence Isolation", 50, 10, 30}, "resistenceIns", "Temperature");
auto h_HV_Isolation_Temperature_pspp1 = df_Isolation.Filter("(channelHV_Ins == true || channelTsensor_Ins == true) && pspp1_test_ins == true").Profile1D({"h_LV_Isolation_Temperature_pspp1", "HV PS-PP1 Profile Resistence Isolation", 50, 10, 30}, "resistenceIns", "Temperature");

auto h_LV_Continuity_Humidity_pspp1 = df_Continuity.Filter("(channelLV_Con == true || channelPHR_Con == true) && pspp1_test_con == true").Profile1D({"h_LV_Continuity_Humidity_pspp1", "LV PS-PP1 Profile Resistence Continuity", 50, 10, 30}, "resistenceCon", "Humidity");
auto h_HV_Continuity_Humidity_pspp1 = df_Continuity.Filter("(channelHV_Con == true || channelTsensor_Con == true) && pspp1_test_con == true").Profile1D({"h_HV_Continuity_Humidity_pspp1", "HV PS-PP1 Profile Resistence Continuity", 50, 10, 30}, "resistenceCon", "Humidity");
auto h_LV_Isolation_Humidity_pspp1 = df_Isolation.Filter("(channelLV_Ins == true || channelPHR_Ins == true) && pspp1_test_ins == true").Profile1D({"h_LV_Isolation_Humidity_pspp1", "LV PS-PP1 Profile Resistence Isolation", 50, 10, 30}, "resistenceIns", "Humidity");
auto h_HV_Isolation_Humidity_pspp1 = df_Isolation.Filter("(channelHV_Ins == true || channelTsensor_Ins == true) && pspp1_test_ins == true").Profile1D({"h_LV_Isolation_Humidity_pspp1", "HV PS-PP1 Profile Resistence Isolation", 50, 10, 30}, "resistenceIns", "Humidity");
*/



////////////////////////////////////////
// OCTOPUS

////////////////////////////////////////
// PP0

////////////////////////////////////////
// FULL-CHAIN
auto h_PassedFailedContinuity_fullchain = df_Continuity.Filter("fullchain_test_con==true").Histo1D({"h_PassedFailedContinuity:fullchain", "Continuity Test Passed/Failed Full-Chain", 2, 0, 2},"statusCon");
auto h_PassedFailedIsolation_fullchain = df_Isolation.Filter("fullchain_test_ins == true").Histo1D({"h_PassedFailedIsolation_fullchain", "Isolation Test Passed/Failed Full-Chain", 2,0,2}, "statusIns");
auto h_LV_ResistenceContinuity_fullchain = df_Continuity.Filter("(channelLV_Con == true || channelPHR_Con == true) && fullchain_test_con == true").Histo1D({"h_LV_ResistenceContinuity_fullchain", "LV Full-Chain Resistence Continuity", 25, 0.53, 0.60}, "resistenceCon");
auto h_LV_ResistenceIsolation_fullchain = df_Isolation.Filter("(channelLV_Ins == true || channelPHR_Ins == true) && fullchain_test_ins == true").Histo1D({"h_LV_ResistenceIsolation_fullchain", "LV Full-Chain Resistence Isolation", 25, 1e+05, 1e+09}, "resistenceIns");
auto h_HV_ResistenceContinuity_fullchain = df_Continuity.Filter("(channelHV_Con == true || channelTsensor_Con == true) && fullchain_test_con == true").Histo1D({"h_HV_ResistenceContinuity_fullchain", "HV Full-Chain Resistence Continuity",25, 10, 15}, "resistenceCon");
auto h_HV_ResistenceIsolation_fullchain = df_Isolation.Filter("(channelHV_Ins == true || channelTsensor_Ins == true) && fullchain_test_ins==true").Histo1D({"h_HV_ResistenceIsolation_fullchain", "HV Full-Chain Resistence Isolation", 25, 1e+06, 1e+10},"resistenceIns");
/* remove comment here for temeprature and humidity histos
auto h_LV_Continuity_Temperature_fullchain = df_Continuity.Filter("(channelLV_Con == true || channelPHR_Con == true) && fullchain_test_con == true").Profile1D({"h_LV_Continuity_Temperature_fullchain", "LV Full-Chain Profile Resistence Continuity", 50, 10, 30}, "resistenceCon", "Temperature");
auto h_HV_Continuity_Temperature_fullchain = df_Continuity.Filter("(channelHV_Con == true || channelTsensor_Con == true) && fullchain_test_con == true").Profile1D({"h_HV_Continuity_Temperature_fullchain", "HV Full-Chain Profile Resistence Continuity", 50, 10, 30}, "resistenceCon", "Temperature");
auto h_LV_Isolation_Temperature_fullchain = df_Isolation.Filter("(channelLV_Ins == true || channelPHR_Ins == true) && fullchain_test_ins == true").Profile1D({"h_LV_Isolation_Temperature_fullchain", "LV Full-Chain Profile Resistence Isolation", 50, 10, 30}, "resistenceIns", "Temperature");
auto h_HV_Isolation_Temperature_fullchain = df_Isolation.Filter("(channelHV_Ins == true || channelTsensor_Ins == true) && fullchain_test_ins == true").Profile1D({"h_LV_Isolation_Temperature_fullchain", "HV Full-Chain Profile Resistence Isolation", 50, 10, 30}, "resistenceIns", "Temperature");

auto h_LV_Continuity_Humidity_fullchain = df_Continuity.Filter("(channelLV_Con == true || channelPHR_Con == true) && fullchain_test_con == true").Profile1D({"h_LV_Continuity_Humidity_fullchain", "LV Full-Chain Profile Resistence Continuity", 50, 10, 30}, "resistenceCon", "Humidity");
auto h_HV_Continuity_Humidity_fullchain = df_Continuity.Filter("(channelHV_Con == true || channelTsensor_Con == true) && fullchain_test_con == true").Profile1D({"h_HV_Continuity_Humidity_fullchain", "HV Full-Chain Profile Resistence Continuity", 50, 10, 30}, "resistenceCon", "Humidity");
auto h_LV_Isolation_Humidity_fullchain = df_Isolation.Filter("(channelLV_Ins == true || channelPHR_Ins == true) && fullchain_test_ins == true").Profile1D({"h_LV_Isolation_Humidity_fullchain", "LV Full-Chain Profile Resistence Isolation", 50, 10, 30}, "resistenceIns", "Humidity");
auto h_HV_Isolation_Humidity_fullchain = df_Isolation.Filter("(channelHV_Ins == true || channelTsensor_Ins == true) && fullchain_test_ins == true").Profile1D({"h_LV_Isolation_Humidity_fullchain", "HV Full-Chain Profile Resistence Isolation", 50, 10, 30}, "resistenceIns", "Humidity");
*/



////////////////////////////////////////
// DRAW PLOTS
f_StatOut->cd();

// PS-PP1
DrawPlot("Isolation Test, passed/failed channels, PS-PP1", h_PassedFailedIsolation_pspp1, "hist");
DrawPlot("Isolation Test HV and Tsensors channels resistence, PS-PP1", h_HV_ResistenceIsolation_pspp1, "hist");
DrawPlot("Continuity Test HV and Tsensors channels resistence, PS-PP1", h_HV_ResistenceContinuity_pspp1, "hist");
DrawPlot("Isolation Test LV and PH channels resistence, PS-PP1", h_LV_ResistenceIsolation_pspp1, "hist");
DrawPlot("Continuity Test LV and PH channels resistence, PS-PP1", h_LV_ResistenceContinuity_pspp1, "hist");

// FULL-CHAIN
DrawPlot("Isolation Test, passed/failed channels, Full-Chain", h_PassedFailedContinuity_fullchain, "hist");
DrawPlot("Isolation Test HV and Tsensors channels resistence, Full-Chain", h_HV_ResistenceIsolation_fullchain, "hist");
DrawPlot("Continuity Test HV and Tsensors channels resistence, Full-Chain", h_HV_ResistenceContinuity_fullchain, "hist");
DrawPlot("Isolation Test LV and PH channels resistence, Full-Chain", h_LV_ResistenceIsolation_fullchain, "hist");
DrawPlot("Continuity Test LV and PH channels resistence, Full-Chain", h_LV_ResistenceContinuity_fullchain, "hist");

f_StatOut->Close(); 
std::cout<<"\033[32mdone \033[0m"<<std::endl;

std::system("rm -rf stat_root");
std::system("rm -rf ./input/*/*/tmp");

return 0;
gROOT->ProcessLine(".q");

}