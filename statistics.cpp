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


//#define TemperaturePlot

TFile *f_StatOut;



void UpdateHisto(ROOT::RDataFrame df, std::string_view column, std::string sTitle, std::string_view cut, Bool_t update, Int_t Nbins){
    if(!update){
     ROOT::RDF::RResultPtr<::TH1D> *h;
     h = new ROOT::RDF::RResultPtr<::TH1D>(df.Filter(cut).Histo1D({sTitle.c_str(), sTitle.c_str(), Nbins, df.Filter(cut).Min(column).GetValue(), df.Filter(cut).Max(column).GetValue()}, column));
     TH1D *hh = (TH1D*) h->GetPtr();
     if(hh->GetEntries()!=0) hh->Write();
    }
    else if(update){
      TH1D *h = (TH1D*) f_StatOut->Get(sTitle.c_str());  
     if(h){ 
      double minX = std::min(h->GetXaxis()->GetXmin(), df.Filter(cut).Min(column).GetValue());
      double maxX = std::max(h->GetXaxis()->GetXmax(), df.Filter(cut).Min(column).GetValue());
      auto h_temp = df.Filter(cut).Histo1D({sTitle.c_str(), sTitle.c_str(), Nbins, h->GetXaxis()->GetXmin(), h->GetXaxis()->GetXmax()}, column);
      h->Add(h_temp.GetPtr());  
      if(h->GetEntries()!=0) h->Write("", TObject::kOverwrite);
     } 
    }
     else{
      std::cerr << "Histogram " << sTitle << " not found!" << std::endl;
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
    Bool_t pspp1_test_Con, fullchain_test_Con;
    Bool_t pspp1_test_Ins, fullchain_test_Ins;

    TestResultContinuity->Branch("resistenceCon", &resistenceCon, "resistenceCon/F");
    TestResultContinuity->Branch("channelLV_Con", &channelLV_Con, "channelLV_Con/O");
    TestResultContinuity->Branch("channelHV_Con", &channelHV_Con, "channelHV_Con/O");
    TestResultContinuity->Branch("statusCon", &statusCon, "statusCon/O");
    TestResultContinuity->Branch("channelPHR_Con", &channelPHR_Con, "channelPHR_Con/O");
    TestResultContinuity->Branch("channelTsensor_Con", &channelTsensor_Con, "channelTsensor_Con/O");
    TestResultContinuity->Branch("pspp1_test_Con", &pspp1_test_Con, "pspp1_test_Con/O");
    TestResultContinuity->Branch("fullchain_test_Con", &fullchain_test_Con, "fullchain_test_Con/O");

    TestResultIsolation->Branch("resistenceIns", &resistenceIns, "resistenceIns/F");
    TestResultIsolation->Branch("channelLV_Ins", &channelLV_Ins, "channelLV_Ins/O");
    TestResultIsolation->Branch("channelHV_Ins", &channelHV_Ins, "channelHV_Ins/O");
    TestResultIsolation->Branch("statusIns", &statusIns, "statusIns/O");
    TestResultIsolation->Branch("channelPHR_Ins", &channelPHR_Ins, "PHR_Ins/O");
    TestResultIsolation->Branch("channelTsensor_Ins", &channelTsensor_Ins, "channelTsensor_Ins/O");
    TestResultIsolation->Branch("pspp1_test_Ins", &pspp1_test_Ins, "pspp1_test_Ins/O");
    TestResultIsolation->Branch("fullchain_test_Ins", &fullchain_test_Ins, "fullchain_test_Ins/O");

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
        pspp1_test_Con = false;
        fullchain_test_Con = false;
        if( std::get<1>(it).find("LV") != std::string::npos) channelLV_Con = true;
        else if (std::get<1>(it).find("HV") != std::string::npos) channelHV_Con = true; 
        else if( std::get<1>(it).find("PH") != std::string::npos) channelPHR_Con = true;
        else if( std::get<1>(it).find("Tsensor") != std::string::npos) channelTsensor_Con = true;
        resistenceCon = std::get<2>(it);
        if(option.CompareTo("PSPP1")==0) pspp1_test_Con = true;
        if(option.CompareTo("FULL_CHAIN")==0) fullchain_test_Con = true;
        TestResultContinuity->Fill();
    }
    for (const auto& it : insulationData) {
        statusIns = (std::get<0>(it) == "Passed");
        channelHV_Ins = false;
        channelLV_Ins = false;
        channelPHR_Ins = false;
        channelTsensor_Ins = false;
        pspp1_test_Ins = false;
        fullchain_test_Ins = false;
        if( std::get<1>(it).find("LV") != std::string::npos) channelLV_Ins = true;
        else if (std::get<1>(it).find("HV") != std::string::npos) channelHV_Ins = true; 
        else if( std::get<1>(it).find("PH") != std::string::npos) channelPHR_Ins = true;
        else if( std::get<1>(it).find("Tsensor") != std::string::npos) channelTsensor_Ins = true;
        resistenceIns = std::get<2>(it);
        if(option.CompareTo("PSPP1")==0 || option.CompareTo("LIC")) pspp1_test_Ins = true;
        if(option.CompareTo("FULL_CHAIN")==0) fullchain_test_Ins = true;
        TestResultIsolation->Fill();
    }
    TestResultContinuity->Write();
    TestResultIsolation->Write();
    f_output->Close(); 
}
// ******************************************************************** //



int main(int argc, char* argv[]){
 
 Bool_t UpdateStat=false;
 Bool_t PS_PP1_statistics = false;
 Bool_t OCTOPUS_statistics = false;
 Bool_t FULLCHAIN_statistics = false;

 if(argc == 2){
    std::string type = argv[1];
    if(type == "PSPP1") PS_PP1_statistics = true;
    else if(type == "OCTOPUS") OCTOPUS_statistics = true;
    else if(type == "FULL_CHAIN") FULLCHAIN_statistics = true;
 }

 ROOT::EnableImplicitMT();
 std::system("mkdir  stat_root");
 std::system("mkdir -p stat");
 std::set<std::string> tests;

 std::vector<std::string> FileNames;
 std::vector<std::string> FileNamesProcessed;
 std::vector<std::string> sInputDir;
 std::vector<TString> cable;
 std::vector<Bool_t> controls;

 #ifndef DB
    const char *TestProcessedTXT = "./stat/processedTest.txt";
    const char *pathOutFile = "./stat/statistics.root";
    sInputDir.push_back("/home/ldellape/cableDB/static/data/");
    if(PS_PP1_statistics){
     sInputDir.push_back("/input/FULL_TEST_su_cavo_ps_pp1_V3/");
     cable.push_back("PSPP1");
     controls.push_back(true);
    }
    else if(FULLCHAIN_statistics){ 
     sInputDir.push_back("/input/FULL_CHAIN/");
     cable.push_back("FULL_CHAIN");
     controls.push_back(true);
    }
    else{
     sInputDir = {"/input/FULL_TEST_su_cavo_ps_pp1_V3/", "/input/FULL_CHAIN/"};
     cable = {"PSPP1", "FULL_CHAIN"};
     controls = {true,true};
    }

 #else 
    const char *TestProcessedTXT = "./stat/processedTestDB.txt";
    const char *pathOutFile = "./stat/statistics_DB.root";
    sInputDir.push_back("/home/ldellape/cableDB/static/data/");


 #endif

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
   UpdateStat=true;

}
else{
    std::cout << "creating statistics.root ..." << std::endl;
    f_StatOut = new TFile(pathOutFile, "RECREATE");
}




//PSPP1, FULL_CHAIN
#ifndef DB
        for(int ii=0; ii < int(cable.size()); ii++){
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

        if(std::all_of(controls.begin(), controls.end(), [](bool value){ return !value; }) && ii==int(cable.size()) - 1){
        std::cout << "\033[32mstatistics is already up to date. End. \033[0m " << std::endl;
            std::system("rm -r stat_root");
            return 0;
        }
        Int_t file = 0;
        for (int j = 0; j < int(FileNames.size()); j++) {
            std::cout<<"\033[32mnew test will be added: \033[0m "<<std::endl;
            std::cout<<FileNamesProcessed[j]<<std::endl;
            if(!std::filesystem::exists(FileNamesProcessed[j].c_str())){
                std::string command = "python3 " + std::string(WORKDIR) + "/py/ManageTXT.py ";
                std::system((command + FileNames[j]).c_str());
            }
            if(!gSystem->AccessPathName(FileNamesProcessed[j].c_str())){  ++file;  ReadOutput(FileNamesProcessed[j], file, cable[j]);}
            UpdateTests << FileNames[j] << "\n";
            tests.insert(FileNames[j]);
        }
        std::cout<<file<<std::endl;
        UpdateTests.close();
        if(file == 0){ std::cout<<"No file "<<std::endl; return 0;}
        }
#else
        for ( const auto& entry : std::filesystem::recursive_directory_iterator(sInputDir[0])){
            if ( ! entry.is_regular_file()) continue;
            std::string filename = entry.path().filename().string();
            std::string fullpath = entry.path().string();
            if( filename != "tmp" && fullpath.find("VALORI") == std::string::npos && tests.find(fullpath) == tests.end() && fullpath.find(".pdf") == std::string::npos && fullpath.find(".root") == std::string::npos){
                std::string processedText = entry.path().parent_path().string() + "/tmp/processed_" + filename;
                std::cout<<processedText<<std::endl;
                FileNamesProcessed.push_back(processedText);
                FileNames.push_back(fullpath);
            }
        }
        std::ofstream UpdateTests(TestProcessedTXT, std::ios::app);
        if(FileNames.empty()){
            std::cout << "\033[32mstatistics is already up to date. End. \033[0m " << std::endl;
            std::system("rm -r stat_root");
            return 0;
        }

        Int_t file = 0;
        for (int j = 0; j < int(FileNames.size()); j++) {
            std::cout<<"\033[32mnew test will be added: \033[0m "<<std::endl;
            std::cout<<FileNamesProcessed[j]<<std::endl;
            if(!std::filesystem::exists(FileNamesProcessed[j].c_str())){
                std::string command = "python3 " + std::string(WORKDIR) + "/py/ManageTXT.py ";
                std::system((command + FileNames[j]).c_str());
            }
            if(!gSystem->AccessPathName(FileNamesProcessed[j].c_str())){  ++file;  ReadOutput(FileNamesProcessed[j], file, "PSPP1");}
            UpdateTests << FileNames[j] << "\n";
            tests.insert(FileNames[j]);
        }
        if(file==0 ) { std::system("rm -rf stat_root"); std::cout<<"\033[31mError occurred with test\033[0m "<<std::endl; return 0;}
        UpdateTests.close();
#endif

// ********************************************************************* //
// ************** END INPUT FILES AND OUTPUT DEFINITIONS *************** //
// ********************************************************************* //


TChain inputChain_continuity("TestResultContinuity");
TChain inputChain_isolation("TestResultIsolation");
inputChain_continuity.Add("./stat_root/*.root");
inputChain_isolation.Add("./stat_root/*.root");
ROOT::RDataFrame df_Continuity(inputChain_continuity);
ROOT::RDataFrame df_Isolation(inputChain_isolation);
f_StatOut->cd();


// ----------------------------------------------------------------------- //
// set flags for histograms                                                //
// ----------------------------------------------------------------------- //
df_Continuity.Foreach([&FULLCHAIN_statistics](const Bool_t fc){if(fc == true){ FULLCHAIN_statistics == true; return;}}, {"fullchain_test_Con"});
df_Continuity.Foreach([&PS_PP1_statistics](const Bool_t ps){if(ps==true){PS_PP1_statistics == true; return;}}, {"pspp1_test_Con"});              

// PSPP1 plots
if(PS_PP1_statistics){
    UpdateHisto(df_Continuity, "statusCon", "h_PassedFailedContinuity_pspp1", "pspp1_test_Con == true", UpdateStat, 2);
    UpdateHisto(df_Isolation, "statusIns", "h_PassedFailedIsolation_pspp1", "pspp1_test_Ins == true", UpdateStat, 2);
    UpdateHisto(df_Continuity, "resistenceCon", "h_LV_ResistenceContinuity_pspp1", "(channelLV_Con == true || channelPHR_Con == true) && pspp1_test_Con == true", UpdateStat, 75);
    UpdateHisto(df_Isolation, "resistenceIns", "h_LV_ResistenceIsolation_pspp1", "(channelLV_Ins == true || channelPHR_Ins == true) && pspp1_test_Ins == true", UpdateStat, 75);
    UpdateHisto(df_Continuity, "resistenceCon", "h_HV_ResistenceContinuity_pspp1", "(channelHV_Con == true || channelTsensor_Con == true) && pspp1_test_Con == true", UpdateStat, 75);
    UpdateHisto(df_Isolation, "resistenceIns", "h_HV_ResistenceIsolation_pspp1", "(channelHV_Ins == true || channelTsensor_Ins == true) && pspp1_test_Ins == true", UpdateStat, 75);
    UpdateHisto(df_Isolation, "resistenceIns", "h_Tsensor_ResistenceIsolation_pspp1", "(channelTsensor_Ins == true && pspp1_test_Ins == true)", UpdateStat, 25);
    UpdateHisto(df_Continuity, "resistenceCon", "h_Tsensor_ResistenceContinuity_pspp1", "(channelTsensor_Con == true && pspp1_test_Con == true)", UpdateStat, 25);
    UpdateHisto(df_Isolation, "resistenceIns", "h_PH_ResistenceIsolation_pspp1", "(channelPHR_Ins == true && pspp1_test_Ins == true)", UpdateStat, 25);
    UpdateHisto(df_Continuity, "resistenceCon", "h_PH_ResistenceContinuity_pspp1", "(channelPHR_Con == true && pspp1_test_Con == true)", UpdateStat, 25);
    #ifdef TemperaturePlot
    UpdateHisto(df_Continuity, "resistenceCon", "h_LV_Continuity_Temperature_pspp1", "(channelLV_Con == true || channelPHR_Con == true) && pspp1_test_Con == true", UpdateStat, 50);
    UpdateHisto(df_Continuity, "resistenceCon", "h_HV_Continuity_Temperature_pspp1", "(channelHV_Con == true || channelTsensor_Con == true) && pspp1_test_Con == true", UpdateStat, 50);
    UpdateHisto(df_Isolation, "resistenceIns", "h_LV_Isolation_Temperature_pspp1", "(channelLV_Ins == true || channelPHR_Ins == true) && pspp1_test_Ins == true", UpdateStat, 50);
    UpdateHisto(df_Isolation, "resistenceIns", "h_HV_Isolation_Temperature_pspp1", "(channelHV_Ins == true || channelTsensor_Ins == true) && pspp1_test_Ins == true", UpdateStat, 50);
    UpdateHisto(df_Continuity, "resistenceCon", "h_LV_Continuity_Humidity_pspp1", "(channelLV_Con == true || channelPHR_Con == true) && pspp1_test_Con == true", UpdateStat, 50);
    UpdateHisto(df_Continuity, "resistenceCon", "h_HV_Continuity_Humidity_pspp1", "(channelHV_Con == true || channelTsensor_Con == true) && pspp1_test_Con == true", UpdateStat, 50);
    UpdateHisto(df_Isolation, "resistenceIns", "h_LV_Isolation_Humidity_pspp1", "(channelLV_Ins == true || channelPHR_Ins == true) && pspp1_test_Ins == true", UpdateStat, 50);
    UpdateHisto(df_Isolation, "resistenceIns", "h_HV_Isolation_Humidity_pspp1", "(channelHV_Ins == true || channelTsensor_Ins == true) && pspp1_test_Ins == true", UpdateStat, 50);
#endif
}


////////////////////////////////////////
// OCTOPUS


////////////////////////////////////////
// PP0

////////////////////////////////////////
// FULL-CHAIN
if(FULLCHAIN_statistics){
    UpdateHisto(df_Continuity, "statusCon", "h_PassedFailedContinuity_fullchain", "fullchain_test_Con == true", UpdateStat, 2);
    UpdateHisto(df_Isolation, "statusIns", "h_PassedFailedIsolation_fullchain", "fullchain_test_Ins == true", UpdateStat, 2);
    UpdateHisto(df_Continuity, "resistenceCon", "h_LV_ResistenceContinuity_fullchain", "(channelLV_Con == true || channelPHR_Con == true) && fullchain_test_Con == true", UpdateStat, 25);
    UpdateHisto(df_Isolation, "resistenceIns", "h_LV_ResistenceIsolation_fullchain", "(channelLV_Ins == true || channelPHR_Ins == true) && fullchain_test_Ins == true", UpdateStat, 25);
    UpdateHisto(df_Continuity, "resistenceCon", "h_HV_ResistenceContinuity_fullchain", "(channelHV_Con == true || channelTsensor_Con == true) && fullchain_test_Con == true", UpdateStat, 25);
    UpdateHisto(df_Isolation, "resistenceIns", "h_HV_ResistenceIsolation_fullchain", "(channelHV_Ins == true || channelTsensor_Ins == true) && fullchain_test_Ins == true", UpdateStat, 25);
    UpdateHisto(df_Isolation, "resistenceIns", "h_Tsensor_ResistenceIsolation_fullchain", "(channelTsensor_Ins == true && fullchain_test_Ins == true)", UpdateStat, 25);
    UpdateHisto(df_Continuity, "resistenceCon", "h_Tsensor_ResistenceContinuity_fullchain", "(channelTsensor_Con == true && fullchain_test_Con == true)", UpdateStat, 25);
    UpdateHisto(df_Isolation, "resistenceIns", "h_PH_ResistenceIsolation_fullchain", "(channelPHR_Ins == true && fullchain_test_Ins == true)", UpdateStat, 25);
    UpdateHisto(df_Continuity, "resistenceCon", "h_PH_ResistenceContinuity_fullchain", "(channelPHR_Con == true && fullchain_test_Con == true)", UpdateStat, 25);
    #ifdef TemperaturePlot
    UpdateHisto(df_Continuity, "resistenceCon", "h_LV_Continuity_Temperature_fullchain", "(channelLV_Con == true || channelPHR_Con == true) && fullchain_test_Con == true", UpdateStat, 50);
    UpdateHisto(df_Continuity, "resistenceCon", "h_HV_Continuity_Temperature_fullchain", "(channelHV_Con == true || channelTsensor_Con == true) && fullchain_test_Con == true", UpdateStat, 50);
    UpdateHisto(df_Isolation, "resistenceIns", "h_LV_Isolation_Temperature_fullchain", "(channelLV_Ins == true || channelPHR_Ins == true) && fullchain_test_Ins == true", UpdateStat, 50);
    UpdateHisto(df_Isolation, "resistenceIns", "h_HV_Isolation_Temperature_fullchain", "(channelHV_Ins == true || channelTsensor_Ins == true) && fullchain_test_Ins == true", UpdateStat, 50);
    UpdateHisto(df_Continuity, "resistenceCon", "h_LV_Continuity_Humidity_fullchain", "(channelLV_Con == true || channelPHR_Con == true) && fullchain_test_Con == true", UpdateStat, 50);
    UpdateHisto(df_Continuity, "resistenceCon", "h_HV_Continuity_Humidity_fullchain", "(channelHV_Con == true || channelTsensor_Con == true) && fullchain_test_Con == true", UpdateStat, 50);
    UpdateHisto(df_Isolation, "resistenceIns", "h_LV_Isolation_Humidity_fullchain", "(channelLV_Ins == true || channelPHR_Ins == true) && fullchain_test_Ins == true", UpdateStat, 50);
    UpdateHisto(df_Isolation, "resistenceIns", "h_HV_Isolation_Humidity_fullchain", "(channelHV_Ins == true || channelTsensor_Ins == true) && fullchain_test_Ins == true", UpdateStat, 50);
    #endif
}
std::string continuityRoot = "./stat/continuity_rdf.root";
std::string isolationRoot = "./stat/isolation_rdf.root";
df_Isolation.Snapshot("df_Isolation", isolationRoot);
df_Continuity.Snapshot("df_Continuity", continuityRoot);
f_StatOut->Close(); 



std::cout<<"\033[32mdone \033[0m"<<std::endl;
std::system("rm -rf stat_root");

#ifndef DB
std::system("rm -rf ./input/*/*/tmp");
#else 
std::system("rm -rf /home/ldellape/cableDB/static/data/tmp");
std::system("rm -rf /home/ldellape/cableDB/static/data/*/*/tmp");
#endif

return 0;
gROOT->ProcessLine(".q");

}