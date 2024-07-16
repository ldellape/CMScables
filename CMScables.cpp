#include <iostream>
#include <utility>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <tuple>
#include <filesystem>
#include "./include/root.h"
#include "./include/def_variables.h"
#include "./include/user_func.h"
#include "./include/input.h"
#include "./include/py_run.h"



#ifdef AUTO_TEST // automatic test--> report for the more recently txt file 
#define AutoTest 
#elif defined(INTER_TEST) // --> choose test from command line
#define InterTest
#endif

int main(int argc, char* argv[]){

#ifdef TIME_RES
 Ins_Time = true;
#endif

printlogo();
if(argc>1) start(argc, argv);

std::this_thread::sleep_for(std::chrono::seconds(3));
std::cout<<"*****************************************"<<std::endl;
std::cout<<"Input Directory ---> " ;
std::cout<<sInputTestDir<<std::endl;
std::cout<<"Histograms will be saved in -----> "+ std::string(WORKDIR)+ "/output/rootFiles/" << std::endl; 
std::cout<<"Plots will be saved in -----> " + std::string(WORKDIR) + "/output/plots/" << std::endl; 
std::cout<<"Final Report will be saved in -----> " + std::string(WORKDIR)+ "/output/report" << std::endl; 
std::cout<<"****************************************"<<std::endl;


std::string pathTEST;
if(!CommandLine){
#ifdef InterTest
 TestName = listAndChooseFiles();
 TestType();
 IterationTest = TestName.size();
 if(test_type != 0){
   Ins_Time = TimeAcquisition();
 if(Ins_Time){
  TestNameTimeAcquisition = DirTimeAcquisition();
 }
}
#elif defined(AutoTest)
  std::string commandTXT = "cd " + sInputTestDir + " && find . -type f -name \"*.txt\" -exec stat --format='%Y %n' {} + | sort -nr | head -n 1 | cut -d' ' -f2- > temp";
  std::system(commandTXT.c_str());
  std::ifstream pathTemp((sInputTestDir + "temp").c_str());
  std::string recentTEST;
  while(std::getline(pathTemp, recentTEST)){
    pathTEST = sInputTestDir + recentTEST;
  } 
  TestName.push_back(pathTEST);
  pathTemp.close();
  std::system(("cd " + sInputTestDir + " && rm temp").c_str());
  test_type=2;
  std::cout << "Plotting Histograms for ----> \033[32mCONTINUITY TEST && ISOLATION TEST\033[0m" << std::endl;
  TestNameTimeAcquisition = DirTimeAcquisition();
#endif
}
IterationTest = TestName.size();

std::cout<<"*****************************************"<<std::endl;
std::cout<<"Test Processati:   "<<std::endl;
std::string name[IterationTest];
 for(int j=0; j<IterationTest; j++){
      name[j] = TestName[j].substr( TestName[j].rfind("/") +1, TestName[j].rfind(".") - TestName[j].rfind("/")-1);
      std::cout<<TestName[j]<<std::endl;
    }
std::cout<<"*****************************************"<<std::endl;


// ****Preparing Text files **** //
std::cout<<"preparing text files..." <<std::endl;
for(int i=0; i<int(TestName.size()); i++){
    Python::PS_PP1::ChangeTextFile(TestName[i]);
}


//********************************
fill_HVcables(HVcables);
fill_HVcables_RTN(HVcables_rtn);
fill_LVcables(LVcables);
fill_LVcables_RTN(LVcables_rtn);
fill_Tsensors(TSensor);
fill_DRAINcables(Drain);
//********************************


// ***** define and fill trees ***** //
ContinuityTree = new TTree("ContinuityTree", "ContinuityTree");
InsulationTree = new TTree("InsulationTree", "InsulationTree");
if(test_type == 0){
 ContinuityTree->Branch("statusCon", &statusCon, "statusCon/I");
 ContinuityTree->Branch("channelCon", &channelCon);
 ContinuityTree->Branch("resistenceCon", &resistenceCon, "resistenceCon/F");
 ContinuityTree->Branch("cableCon", &cableCon, "cableCon/I");
}
else if(test_type == 1){
 InsulationTree->Branch("statusIns", &statusIns,"statusIns/I");
 InsulationTree->Branch("channelIns", &channelIns);
 InsulationTree->Branch("resistenceIns", &resistenceIns, "resistenceIns/F");
 InsulationTree->Branch("cableIns", &cableIns, "cableIns/I");
 InsulationTree->Branch("FiledIns",&FieldIns , "FiledIns/F");
}
else if(test_type == 2){
 ContinuityTree->Branch("statusCon", &statusCon, "statusCon/I");
 ContinuityTree->Branch("channelCon", &channelCon);
 ContinuityTree->Branch("resistenceCon", &resistenceCon, "resistenceCon/F");
 ContinuityTree->Branch("cableCon", &cableCon, "cableCon/I");
 InsulationTree->Branch("statusIns", &statusIns,"statusIns/I");
 InsulationTree->Branch("channelIns", &channelIns);
 InsulationTree->Branch("resistenceIns", &resistenceIns, "resistenceIns/F");
 InsulationTree->Branch("cableIns", &cableIns, "cableIns/I");
 InsulationTree->Branch("FiledIns",&FieldIns , "FiledIns/F");
}
std::cout<<"*****************************************"<<std::endl;

std::cout<<"Filling tree(s)...";
for(int i=0; i<IterationTest; ++i){
  ReadTestOutput(TestName, i);
}
std::this_thread::sleep_for(std::chrono::seconds(1));
std::cout<<"done"<<std::endl;
std::cout<<"****************************************"<<std::endl;
std::cout<<"Filling Histograms... "<<std::endl;

std::this_thread::sleep_for(std::chrono::seconds(2));

// preparing histograms //
for(int i=0; i<IterationTest; i++){
 ResMaxHV_cont[i] = FindMax(ContinuityTree, i, "HV_cont");
 ResMinHV_cont[i] = FindMin(ContinuityTree, i, "HV_cont");
 ResMaxLV_cont[i] = FindMax(ContinuityTree, i, "LV_cont");
 ResMinLV_cont[i] = FindMin(ContinuityTree, i, "LV_cont");
 ResMaxHV_ins[i] = FindMax(ContinuityTree, i, "HV_ins");
 ResMinHV_ins[i] = FindMin(ContinuityTree, i, "HV_ins");
 ResMaxLV_ins[i] = FindMax(ContinuityTree, i, "LV_ins");
 ResMinLV_ins[i] = FindMin(ContinuityTree, i, "LV_ins");
 h_passedHV_Cont[i] = new TH1F(Form("h_passedHV_Cont%i",i), Form("h_passedHV_Cont%i",i), 2, 0, 2);  
 h_passedLV_Cont[i] = new TH1F(Form("h_passedLV_Cont%i",i), Form("h_passedLV_Cont%i",i), 2, 0, 2);
 h_passedCont_tot[i] = new TH1F(Form("h_passedCont_tot%i",i), Form("h_passedCont_tot%i",i), 2, 0, 2);
 hCont_ResChannel_HV[i] = new TH1F(Form("hCont_ResChannel_HV%i", i), Form("hCont_ResChannel_HV%i",i), NumberHVcables + NumberHVRTNwires + NumberSensorWire, 0 , NumberHVcables + NumberHVRTNwires + NumberSensorWire);
 hCont_ResChannel_LV[i] = new TH1F(Form("hCont_ResChannel_LV%i",i), Form("hCont_ResChannel_LV%i",i), NumberLVcables, 0 , NumberLVcables );
 hCont_ResHV[i] = new TH1F(Form("hCont_ResHV%i",i), Form("hCont_ResHV%i", i),100, ResMinHV_cont[i]-0.1, ResMaxHV_cont[i]+0.1);
 hCont_ResLV[i] = new TH1F(Form("hCont_ResLV%i",i), Form("hCont_ResLV%i", i), 100, ResMinLV_cont[i]-0.1,  ResMaxLV_cont[i]+0.1);

 h_passedHV_Ins[i] = new TH1F(Form("h_passedHV_Ins%i",i), Form("h_passedHV_Ins%i",i), 2, 0, 2);
 h_passedLV_Ins[i] = new TH1F(Form("h_passedLV_Ins%i",i), Form("h_passedLV_Ins%i",i), 2, 0, 2);
 h_passedIns_tot[i] = new TH1F(Form("h_passedIns_tot%i",i), Form("h_passedIns_tot%i",i), 2, 0, 2);
 hIns_ResHV[i] = new TH1F(Form("hIns_ResHV%i", i), Form("hIns_ResHV%i", i), 100, ResMinHV_ins[i] - 0.1, ResMaxHV_ins[i] + 0.1);
 hIns_ResLV[i] = new TH1F(Form("hIns_ResLV%i", i), Form("hIns_ResLV%i", i), 100, ResMinLV_ins[i] - 0.1, ResMaxLV_ins[i] + 0.1);
 hIns_ResChannel_LV[i] = new TH1F(Form("hIns_ResChannel_LV%i",i), Form("hIns_ResChannel_LV%i",i), NumberLVcables, 0 , NumberLVcables);
 hIns_ResChannel_HV[i] = new TH1F(Form("hIns_ResChannel_HV%i",i), Form("hIns_ResChannel_HV%i",i), NumberHVcables + NumberHVRTNwires + NumberSensorWire, 0 , NumberHVcables + NumberHVRTNwires + NumberSensorWire);
}
// ******filling histograms********
std::vector<int> countHV(IterationTest+1, 0); 
std::vector<int> countLV(IterationTest+1, 0);
std::vector<int> countLV2(IterationTest+1, 0);
std::vector<int> countHV2(IterationTest+1, 0);

if(test_type == 0 || test_type == 2){
 for(int i=0; i<ContinuityTree->GetEntries(); ++i){
    ContinuityTree->GetEntry(i);
    if(channelCon.find("LV") != std::string::npos || channelCon.find("PH") != std::string::npos){
        ++countLV[cableCon];
        h_passedLV_Cont[cableCon]->Fill(statusCon);    
        hCont_ResLV[cableCon]->Fill(resistenceCon);
        if(resistenceCon < 2*ThreshContLV){
         hCont_ResChannel_LV[cableCon]->SetBinContent(countLV[cableCon], resistenceCon);
        }
        else{
         hCont_ResChannel_LV[cableCon]->SetBinContent(countLV[cableCon], ThreshContLV);
         std::cout<<"\033[31mAlert in channel " << channelCon << ", cable "<< cableCon <<"\033[0m: resistence over Threshold for Continuity Test"<<std::endl;
         std::cout<<"Resistence = "<<resistenceCon<<" ---> set to threshold value "<< ThreshContLV << std::endl;
        OverThreshLV[countLV[cableCon]] = resistenceCon;
        }
        labelsContLV[countLV[0]-1] = strdup(channelCon.c_str());
    }
    else if(channelCon.find("H") != std::string::npos || channelCon.find("Tsensor") != std::string::npos){
        ++countHV[cableCon];
        h_passedHV_Cont[cableCon]->Fill(statusCon);
        hCont_ResLV[cableCon]->Fill(resistenceCon);
        if(resistenceCon < 2*ThreshContHV){
         hCont_ResChannel_HV[cableCon]->SetBinContent(countHV[cableCon], resistenceCon);
        }
        else{
         hCont_ResChannel_HV[cableCon]->SetBinContent(countHV[cableCon], ThreshContHV);
         std::cout<<"\033[31mAlert in channel " << channelCon << ", cable "<< cableCon <<"\033[0m: resistence over Threshold for Continuity Test"<<std::endl;
         std::cout<<"Resistence = "<<resistenceCon<<" ---> set to threshold value "<< ThreshContHV << std::endl;
         OverThreshHV[countHV[cableCon]] = resistenceCon;
        }
        labelsContHV[countHV[0]-1] = strdup(channelCon.c_str());
    }
    h_passedCont_tot[cableCon]->Fill(statusCon);
}
}
if(test_type == 1 || test_type == 2){
for(int i=0; i<InsulationTree->GetEntries(); ++i){
    InsulationTree->GetEntry(i);
    if(channelIns.find("LV") != std::string::npos || channelIns.find("PH") != std::string::npos){
        ++countLV2[cableIns];
        h_passedLV_Ins[cableIns]->Fill(statusIns); 
        hIns_ResHV[cableIns]->Fill(resistenceIns);   
        if( resistenceIns >= ThreshIsoLV){
        hIns_ResChannel_LV[cableIns]->SetBinContent(countLV2[cableIns], resistenceIns);
        }
        else{
         hIns_ResChannel_LV[cableIns]->SetBinContent(countHV2[cableIns], ThreshIsoLV);
         std::cout<<"\033[31mAlert in channel " << channelCon << ", cable "<<cableIns << "\033[0m: resistence below Threshold for Isolation Test"<<std::endl;
         std::cout<<"Resistence = "<<resistenceCon<<" --> set to threshold value " << ThreshIsoLV << std::endl;
         BelowThreshLV[countLV[0]-1] = resistenceIns;
        }
        labelsInsLV[countLV2[0]-1] = strdup(channelIns.c_str());
    }
    else if(channelIns.find("H") != std::string::npos || channelIns.find("Tsensor") != std::string::npos){
        ++countHV2[cableIns];
        h_passedHV_Ins[cableIns]->Fill(statusIns);
        hIns_ResLV[cableIns]->Fill(resistenceIns);
        if(resistenceIns >= ThreshIsoHV){
        hIns_ResChannel_HV[cableIns]->SetBinContent(countHV2[cableIns], resistenceIns);
        }
        else{
         hIns_ResChannel_LV[cableIns]->SetBinContent(countHV2[cableIns], ThreshIsoHV);
         std::cout<<"\033[31mAlert in channel " << channelCon << " , cable "<< cableIns << "\033[0m: resistence below Threshold for Isolation Test"<<std::endl;
         std::cout<<"Resistence = "<<resistenceIns<<" --> " << "set to threshold value " << ThreshIsoHV << std::endl;
        BelowThreshHV[countHV[0]-1] = resistenceIns;
        }
        labelsInsHV[countHV2[0]-1] = strdup(channelIns.c_str());
    }
    h_passedIns_tot[cableIns]->Fill(statusIns);
}
}
std::cout<<"*****************************************"<<std::endl;
std::this_thread::sleep_for(std::chrono::seconds(2));



// *******Histogram for Resistance Versus Time Acquisition*********
if (Ins_Time) {
 std::cout << "Plotting LV cables resistance versus time acquisition..."<<std::endl;
 std::vector<std::pair<std::string, TGraph*>> grRes_TimeLV[IterationTest];
 std::vector<std::pair<std::string, TGraph*>> grRes_TimeLVR[IterationTest];
 for (int k = 0; k < IterationTest; k++) {
  if(!std::filesystem::exists(TestNameTimeAcquisition[k])){
      std::cout<<"\033[33mLV: no available time measurements for "<<TestNameTimeAcquisition[k] <<"\033[0m"<< std::endl;
      continue;
    }
  for (const auto& pair : LVcables) {
    std::string pathINI = TestNameTimeAcquisition[k] + "/" + pair.first + std::to_string(pair.second) + ".ini";
    gErrorIgnoreLevel = kError;
    TGraph *gr_temp = ReadTestTime(pathINI);
    gErrorIgnoreLevel = kWarning;
    if (gr_temp != nullptr) {
     grRes_TimeLV[k].push_back(std::make_pair(pair.first + std::to_string(pair.second), gr_temp));
    }
   }
 }
 plottingGraph(grRes_TimeLV, "LV");
 for (int k = 0; k < IterationTest; k++) {
 if(!std::filesystem::exists(TestNameTimeAcquisition[k])){
      std::cout<<"\033[33mLVR: no available time measurements for "<<TestNameTimeAcquisition[k] <<"\033[0m" <<std::endl;
      continue;
    }
  for (const auto& pair : LVcables_rtn) {
    std::string pathINI = TestNameTimeAcquisition[k] + "/" + pair.first + std::to_string(pair.second) + ".ini";
    gErrorIgnoreLevel = kError;
    TGraph *gr_temp = ReadTestTime(pathINI);
    gErrorIgnoreLevel = kWarning;
    if (gr_temp != nullptr) {
     grRes_TimeLVR[k].push_back(std::make_pair(pair.first + std::to_string(pair.second), gr_temp));
    }
   }
 }
  plottingGraph(grRes_TimeLVR, "LVR");
    std::cout << "done" << std::endl;
    std::cout << "*****************************************" << std::endl;
}


// **********Drawing Plots****************
// ***************************************

gErrorIgnoreLevel = kWarning;
std::cout<<"************ Creating PDF...**************" <<std::endl;
std::cout<<"Drawn and Saved Histograms: "<<std::endl;
// Passed/Failed Histograms //
c_plot = new TCanvas("c_plot","c_plot", 3000, 3500);
c_plot->Divide(2,3);

if(test_type == 1){
plotting(h_passedIns_tot , "InsulationTest_All_Passed-Failed", 1);
plotting(h_passedHV_Ins , "InsulationTest_HV_Passed-Failed", 2);
plotting(h_passedLV_Ins , "InsulationTest_LV_Passed-Failed", 3);
plotting(hIns_ResChannel_HV , "InsulationTest_HV_Resistence", 4);
plotting(hIns_ResChannel_LV , "InsulationTest_LV_Resistence", 5);
}

// Channel vs Resistence //
if(test_type == 0 ){
plotting(h_passedHV_Cont ,"ContinuityTest_HV_Passed-Failed", 1);
plotting(h_passedLV_Cont , "ContinuityTest_LV_Passed-Failed", 2);
plotting(h_passedCont_tot ,"ContinuityTest_All_Passed-Failed", 3);
plotting(hCont_ResChannel_HV,"ContinuityTest_ResistenceHV", 4);
plotting(hCont_ResChannel_LV,"ContinuityTest_ResistenceLV", 5);
}

if(test_type == 2){
plotting(h_passedCont_tot ,"ContinuityTest_All_Passed-Failed",1);
plotting(h_passedIns_tot , "InsulationTest_All_Passed-Failed",2);
plotting(hIns_ResChannel_HV , "InsulationTest_HV_Resistence",3);
plotting(hIns_ResChannel_LV , "InsulationTest_LV_Resistence",4);
plotting(hCont_ResChannel_HV,"ContinuityTest_ResistenceHV",5);
plotting(hCont_ResChannel_LV,"ContinuityTest_ResistenceLV",6);
}

gErrorIgnoreLevel = kPrint;

std::cout<<"*******************************************"<<std::endl;
TString CreateReport;
std::cout<<"Do you want to create the final report? (y/n)" <<std::endl;
std::cin>>CreateReport;

f_OutPut->Write();
std::cout<<"*******************************************"<<std::endl;
std::cout<<"Output: "<<std::endl;
std::cout<<"\033[32mroot histograms have been saved in "<< sOutputRoot << sPDFTitle <<".root\033[0m"<<std::endl;
if(IterationTest==1){
std::cout<<"\033[32mplots has been saved as " + std::string(WORKDIR) +"/output/plots/SingleCable/"<< sPDFTitle <<".pdf\033[0m"<<std::endl;
}
else{
std::cout<<"\033[32mplot pdf has been saved as" + std::string(WORKDIR) +"/output/plots/CheckCable/"<< sPDFTitle <<".pdf\033[0m"<<std::endl;
}
f_OutPut->Close();

std::cout<<"*****************************************"<<std::endl;


if(CreateReport == "y") {
Python::PS_PP1::WriteFinalReport(sPDFTitle, name[0]);
Python::PS_PP1::ChangeHTML(sPDFTitle);
}

TFile *f_OutPut = new TFile((sOutputRoot + sPDFTitle + ".root").c_str(), "RECREATE");
ContinuityTree->Write();
InsulationTree->Write();
for(int i=0; i<IterationTest; ++i){
 if(test_type == 0 || test_type == 2){
 h_passedLV_Cont[i]->Write();
 h_passedHV_Cont[i]->Write();
 hCont_ResChannel_HV[i]->Write();
 hCont_ResChannel_LV[i]->Write();
 hCont_ResLV[i]->Write();
 hCont_ResHV[i]->Write();
 h_passedCont_tot[i]->Write();
 }

if(test_type == 1 || test_type == 2){
 hIns_ResHV[i]->Write();
 hIns_ResLV[i]->Write();
 h_passedHV_Ins[i]->Write();
 h_passedLV_Ins[i]->Write();
 h_passedIns_tot[i]->Write();
 hIns_ResChannel_HV[i]->Write();
 hIns_ResChannel_LV[i]->Write();
}
}

return 0;
gROOT->ProcessLine(".q");


}