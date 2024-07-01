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
#include "./include/root.h"
#include "./include/def_variables.h"
#include "./include/user_func.h"
#include "./include/input.h"

#ifdef AUTO_TEST // automatic test--> report for the more recently txt file 
#define AutoTest 
#elif defined(INTER_TEST) // --> choose test from command line
#define InterTest
#endif



int main(int argc, char* argv[]){

InsulationTest = false;
ContinuityTest = false;
Ins_Time = false;
test_type = 0;
std::string pathTEST;


if(argc>1) start(argc, argv);
printlogo();

std::this_thread::sleep_for(std::chrono::seconds(3));
std::cout<<"*****************************************"<<std::endl;
std::cout<<"Input Directory ---> " ;
std::cout<<sInputTestDir<<std::endl;
std::cout<<"Histograms will be saved in -----> ./output/rootFiles/" << std::endl; 
std::cout<<"Plots will be saved in -----> ./output/plots/" << std::endl; 
std::cout<<"Final Report will be saved in -----> ./output/report" << std::endl; 
std::cout<<"****************************************"<<std::endl;

if(!CommandLine){
#ifdef InterTest
 TestName = listAndChooseFiles();
 TestType();
 IterationTest = TestName.size();
//if(test_type != 0){
//Ins_Time = TimeAcquisition();
//TestNameTimeAcquisition = DirTimeAcquisition();
//if(Ins_Time){ std::cout << "ok" << std::endl; TestNameTimeAcquisition = listAndChooseFilesTimeAcquisition();}
//}
#elif defined(AutoTest)
 std::string commandTXT = "cd " + sInputTestDir + " && find . -type f -name \"*.txt\" -exec stat --format='%Y %n' {} + | sort -nr | head -n 1 | cut -d' ' -f2- > temp";
 std::system(commandTXT.c_str());
 std::ifstream pathTemp((sInputTestDir + "temp").c_str());
 std::string recentTEST;
 while(std::getline(pathTemp, recentTEST)){
    std::cout<<" line: " << recentTEST<<std::endl;
    pathTEST = sInputTestDir + recentTEST;
 }
TestName.push_back(pathTEST);
  pathTemp.close();
std::system(("cd " + sInputTestDir + " && rm temp").c_str());
  test_type=2;
#endif
}
IterationTest = TestName.size();

std::cout<<"Test Processati   "<<std::endl;
std::cout<<TestName[0] << std::endl;
TString name[IterationTest];
 for(int j=0; j<IterationTest; j++){
      name[j] = TestName[j].substr( TestName[j].rfind("/") +1, TestName[j].rfind(".") - TestName[j].rfind("/")-1);
  //    std::cout<<j+1<< "-" << name[j]<<std::endl;
    }
std::cout<<"*****************************************"<<std::endl;
std::cout<<"preparing text files..." <<std::endl;
for(int i=0; i<int(TestName.size()); i++){
    std::string commandPy = "python3 ./src/ManageTXT.py " + TestName[i];
    std::system(commandPy.c_str());
}
//std::this_thread::sleep_for(std::chrono::seconds(1));

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
std::cout<<"**************************************** "<<std::endl;
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

/*
//*******Histogram for Resistance Versus Time Acquisition*********
if (Ins_Time) {
std::vector<TGraph*> grRes_Time[IterationTest];
    for(int i = 0; i < IterationTest; ++i) {
    grRes_Time[i] = std::vector<TGraph*>(100, nullptr);     
}
float OverThreshHV[100] = {0};
    std::vector<std::vector<double>> ResTime(NumberLVcables);
    std::vector<std::vector<double>> x(NumberLVcables);

    for (int k = 0; k < IterationTest; k++) {
        int Iteration = 0;
        for (const auto& pair : LVcables) {
            std::string line;
            std::cout << (TestNameTimeAcquisition[k] + "/" + pair.first + std::to_string(pair.second) + ".ini").c_str() << std::endl;
            std::ifstream inputTimeResolution((TestNameTimeAcquisition[k] + "/" + pair.first + std::to_string(pair.second) + ".ini").c_str());

            if (inputTimeResolution.is_open()) {
                ++Iteration;
                while (std::getline(inputTimeResolution, line)) {
                    std::stringstream ss(line);
                    double number_acquisition, value;
                    if (ss >> number_acquisition >> value) { // Read a double from the line
                        ResTime[Iteration - 1].push_back(value);
                    }
                }
                inputTimeResolution.close();
            }
        }

        for (int i = 0; i < Iteration; i++) {
            for (int ii = 0; ii < int(ResTime[i].size()); ii++) {
                x[i].push_back(ii);
            }
            grRes_Time[i] = new TGraph(int(ResTime[i].size()), &x[i][0], &ResTime[i][0]); // graph for each LV cable 
        }

        plottingGraph(grRes_Time[k], k, grRes_Time.size());  
        std::cout<< "number of graphs : " << std::cout << grRes_Time.size() << std::endl;
        c_graph[k] = new TCanvas(Form("c_graph%i", k + 1), Form("c_graph%i", k + 1), 3000, 1000);
        c_graph[k]->Divide(grRes_Time.size());
        grRes_Time.clear();
    }
}
*/


// ***************************************


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
//plotting(h_passedHV_Ins , "InsulationTest_HV_Passed-Failed");
//plotting(h_passedLV_Ins , "InsulationTest_LV_Passed-Failed");
plotting(hIns_ResChannel_HV , "InsulationTest_HV_Resistence",3);
plotting(hIns_ResChannel_LV , "InsulationTest_LV_Resistence",4);
//plotting(h_passedHV_Cont ,"ContinuityTest_HV_Passed-Failed");
//plotting(h_passedLV_Cont , "ContinuityTest_LV_Passed-Failed");
plotting(hCont_ResChannel_HV,"ContinuityTest_ResistenceHV",5);
plotting(hCont_ResChannel_LV,"ContinuityTest_ResistenceLV",6);
//plotting(h_passedHV_Cont ,"ContinuityTest_HV_Passed-Failed");
}

gErrorIgnoreLevel = kPrint;
//std::string PDF_Name = "ContinuityPDF";
//WritePDF(CanvasPlots, "ContinuityPDF"); not fully implemented yet


// writing out histograms //
TFile *f_OutPut = new TFile(sOutputRoot + sPDFTitle + ".root", "RECREATE");

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
std::cout<<"*******************************************"<<std::endl;
TString CreateReport;
std::cout<<"Do you want to create the final report? (y/n)" <<std::endl;
std::cin>>CreateReport;

f_OutPut->Write();
std::cout<<"*******************************************"<<std::endl;
std::cout<<"Output: "<<std::endl;
std::cout<<"\033[32mroot histograms have been saved in "<< sOutputRoot << sPDFTitle <<".root\033[0m"<<std::endl;
if(IterationTest==1){
std::cout<<"\033[32mplot pdf has been saved as ./output/plots/SingleCable/"<< sPDFTitle <<".pdf\033[0m"<<std::endl;
}
else{
std::cout<<"\033[32mplot pdf has been saved as ./output/plots/CheckCable/"<< sPDFTitle <<".pdf\033[0m"<<std::endl;
}
f_OutPut->Close();

std::cout<<"*****************************************"<<std::endl;


if(CreateReport == "y"){
std::cout<<"*****************************************"<<std::endl;
std::cout<<"creating Final Report..." <<std::endl;
std::cout<<"*****************************************"<<std::endl;

sPDFTitle = "prova_report";
std::string PythonCommand;
if(IterationTest == 1){
    PythonCommand = "python3 ./src/WritePDF.py ./output/report/Report_" + sPDFTitle + ".pdf ./input/pdf_ceetis/" + name[0] + ".pdf ./output/plots/SingleCable/" + sPDFTitle + ".pdf"; 
}
else if(IterationTest > 1){
    PythonCommand = "python3 ./src/WritePDF.py ./output/report/Report_" + sPDFTitle + ".pdf ./input/pdf_ceetis/" + name[0] + ".pdf ./output/plots/CheckCable/" + sPDFTitle + ".pdf"; 
}
std::cout<< PythonCommand << std::endl;
std::system((PythonCommand).c_str());
std::cout<<"\033[32mFinal REPORT saved as ./output/report/"+ sPDFTitle +"\033[0m" <<std::endl;
}
return 0;
gROOT->ProcessLine(".q");


}