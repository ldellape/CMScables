//#include "../include/"
#include "TH1.h"
#include<filesystem>
#include "./src/ReadTestOutput"
#include "TTree.h"
#include "TFile.h"

const int NumberLVcables = 26;
const int NumberHVcables = 12;
const int NumberHVRTNwires = 3;

void StatCable(){
    TFile *f_statTest = TFile::Open("f_statTest", "update");
    TTree *ContTest;
    TTree *IsoTest;
    double resIso, std::string channelIso, std::string statusIso;
    double resCont, std::string channelCont, std::string statusCont;
    IsoTest->Branch("channelIso", &channelIso);
    IsoTest->Branch("statusIso", &statusIso);
    IsoTest->Branch("resIso", &resIso, "resIso/F");
    ContTest->Branch("channelCont", &ChannelCont);
    ContTest->Branch("statusCont", &statusCont);
    ContTest->Branch("resCont", &resCont, "resCont/F");

    // Define Histograms // 
    TH1F *h_Total_PassedFailed = new TH1F("h_Total_PassedFailed", "h_Total_PassedFailed", NumberLVcables + NumberHVcables, 0, NumberHVcables + NumberLVcables);
    TH1F *h_lenghtLVcable = new TH1F("h_lenghtLVcable" , "h_lenghtLVcable", 100, 35, 60);
    TH1F *h_lenghtHVcable = new TH1F("h_lenghtHVcable", "h_lenghtHVcable", 100, 0, 100);
    TH1F *h_meanLVcables = new TH1F("h_meanLVcables", "h_meanLVcables", 100, 0 , 15);
    TH1F *h_meanHVcables = new TH1F("h_meanHVcables", "h_meanHVcables", 100, 0, 30);
    TH1F *h_sigmaLVcables = new TH1F("h_sigmaLVcables", "h_sigmaLVcables", 100, 0, 15);
    TH1F *h_sigmaHVcables = new TH1F("h_sigmaHVcables", "h_sigmaHVcables", 100, 0, 15);
    // ******************* //

    std::vector<std::string> FileNames;
    std::string sInputDir = "./input/FULL_RTES";
    for(int i=0; i<3; i++){
        std::string dir = sInputDir + Form("Cable0%i", i+1);
        for(const auto&entry : std::filesystem::directory_iterator(dir.c_str())){
             FileNames.push_back(entry.dir().filename().string());
        }
    }
    for(int i=0; i<int(FileNames.size()); i++){
        ReadTestOutput(FileNames[i], i);
    }



    
    
}

TH1::





}