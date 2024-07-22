//#include "../include/"
#include "TH1.h"
#include <filesystem>
#include "./src/ReadTestOutput.cpp"
#include "TTree.h"
#include "TFile.h"

const int NumberLVcables = 26;
const int NumberHVcables = 12;
const int NumberHVRTNwires = 3;
std::vector<std::tuple<double,double,double,double,double, std::string, double>> ParametersContinuity;
std::vector<std::tuple<double,double,double,double,double, double, double>> ParametersInsulationInitial;
std::vector<std::tuple<double,double,double>> ParametersInsulationLV;
std::vector<std::tuple<double,double,double,double>> ParametersInsulationHV;
std::vector<std::tuple<double,double,double,double>> ParametersInsulationTsensor;


void StatCable(){
    TTree *ContinuityTree = new TTree("ContinuityTree", "ContinuityTree");
TTree *InsulationTree = new TTree("InsulationTree", "InsulationTree");
Int_t statusCon, statusIns;
std::string channelIns, channelCon;
Float_t resistenceIns, resistenceCon;
Int_t cableIns, cableCon;
Float_t FieldIns;
/*
ContinuityTree->Branch("statusCon", &statusCon, "statusCon/I");
ContinuityTree->Branch("channelCon", &channelCon);
ContinuityTree->Branch("resistenceCon", &resistenceCon, "resistenceCon/F");
ContinuityTree->Branch("cableCon", &cableCon, "cableCon/I");
InsulationTree->Branch("statusIns", &statusIns,"statusIns/I");
InsulationTree->Branch("channelIns", &channelIns);
InsulationTree->Branch("resistenceIns", &resistenceIns, "resistenceIns/F");
InsulationTree->Branch("cableIns", &cableIns, "cableIns/I");
InsulationTree->Branch("FiledIns",&FieldIns , "FiledIns/F");
    TFile *f_statTest = TFile::Open("./docs/f_statTest", "update");
    std::vector<std::string> FileNames;
    std::string sInputDir = "./input/FULL_TEST_su_cavo_ps_pp1_V3/";
    for(int i=0; i<3; i++){
        std::string dir = sInputDir + Form("Cable0%i", i+1);
        for(const auto&entry : std::filesystem::directory_iterator(dir.c_str())){
             FileNames.push_back(entry.path().filename().string());
        }
    }
    for(int i=0; i<int(FileNames.size()); i++){
        ReadTestOutput(FileNames, i);
        std::cout<<FileNames[i]<<std::endl;
    }

    TH1I *h_HV_PassedFailedCont = new TH1I("h_HV_PassedFailedCont", "h_HV_PassedFailedCont", 2, 0, 2);
    TH1I *h_LV_PassedFailedCont = new TH1I("h_LV_PassedFailedCont", "h_LV_PassedFailedCont", 2, 0, 2);
    TH1I *h_HV_PassedFailedIns = new TH1I("h_HV_PassedFailedIns", "h_HV_PassedFailedIns", 2, 0, 2);
    TH1I *h_LV_PassedFailedIns = new TH1I("h_LV_PassedFailedIns", "h_LV_PassedFailedLV", 2, 0, 2);
    TH1F *h_LV_resistenceCon = new TH1F("h_LV_resistenceCont", "h_LV_resistenceCont", 100, 0.52, 0.60);
    TH1F *h_LV_resistenceIns = new TH1F("h_LV_resistenceIns", "h_LV_resistenceIns", 100, 1e+09, 1e+11);
    TH1F *h_HV_resistenceCon = new TH1F("h_HV_resistenceCont", "h_HV_resistenceCont", 100, 9, 13);
    TH1F *h_HV_resistenceIns = new TH1F("h_HV_resistenceIns", "h_LV_resistenceIns", 100, 1e+09, 1e+11);


    // filling histograms //
    for(int i=0; i<ContinuityTree->GetEntries(); i++){
        ContinuityTree->GetEntry(i);
        if(channelCon.find("LV") != std::string::npos){
            h_LV_resistenceCon->Fill(resistenceCon);
            h_LV_PassedFailedCont->Fill(statusCon);        
        }    
        else if(channelCon.find("HV") != std::string::npos){
            h_HV_resistenceCon->Fill(resistenceCon);
            h_HV_PassedFailedCont->Fill(statusCon);
        }
    }
    for(int i=0; i<InsulationTree->GetEntries(); i++){
        InsulationTree->GetEntry(i);
        if(channelIns.find("LV")!=std::string::npos){
            h_LV_resistenceIns->Fill(resistenceIns);
            h_LV_PassedFailedIns->Fill(statusIns);
        }
        else if(channelIns.find("HV")!= std::string::npos){
            h_HV_resistenceIns->Fill(resistenceIns);
            h_HV_PassedFailedIns->Fill(statusIns);
        }
    }
    h_LV_PassedFailedCont->Write();
    h_LV_PassedFailedIns->Write();
    h_HV_PassedFailedIns->Write();
    h_LV_PassedFailedIns->Write();
    h_HV_resistenceIns->Write();
    h_HV_resistenceCon->Write();
    h_LV_resistenceCon->Write();
    h_LV_resistenceIns->Write();
    f_statTest->Close();
    gROOT->ProcessLine(".q");    
    */
}






