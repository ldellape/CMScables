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
#include "./include/py_run.h"
#include "./include/user_func.h"
#include "./include/Continuity.h"
#include "./include/Isolation.h"
#include "./include/plotting.h"

#ifdef AUTO_TEST // automatic test--> report for the more recently txt file 
#define AutoTest 
#elif defined(INTER_TEST) // --> choose test from command line
#define InterTest
#endif


int main(int argc, char* argv[]){
 printlogo();
 std::this_thread::sleep_for(std::chrono::seconds(2));
 if(argc>1) start(argc, argv); // mode 3, from command line 

 #ifdef TIME_RES
 Ins_Time = true;
 #endif

// ************************************* //
// ************ INPUT TESTS ************ //
// ************************************* //
// mode 1 and mode 2 //
std::string pathTEST;
if(!CommandLine){
  #ifdef InterTest // mode 1 
   TestName = listAndChooseFiles();
   TestType();
   IterationTest = TestName.size();
   if(test_type != 0){
    Ins_Time = TimeAcquisition();
    if(Ins_Time){
      TestNameTimeAcquisition = DirTimeAcquisition();
    }
   }
  #elif defined(AutoTest) // mode 2 
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
// *************************************************** //
// *************************************************** //


// ********* changing txt files and store Continuity/Isolation class objects **************** //
// ***************************************************************************************** //
std::cout<<"preparing text files..." <<std::endl;
for(int i=0; i<int(TestName.size()); i++){
//Python::PSPP1::ChangeTextFile(TestName[i]); to be test 
std::cout<<TestPath[i]<<std::endl;
}
for(int i=0; i<IterationTest; ++i){
  ReadTestOutput(TestName, i);
}
std::cout<<"done"<<std::endl;
std::cout<<"****************************************"<<std::endl;
// ***************************************************************************************** //


// ***************************************************** //
// ****************Drawing Plots************************ //
// ***************************************************** //
for(int it = 0; it< IterationTest; it++){
  if(test_type == 0 || test_type == 2){
  // h_passedCont_tot[it] = TestContinuityPSPP1[it]->FillResistenceChannelHistogram(Form("h_passed_continuity_all_%i", it+1));
   h_passedHV_Cont[it] = TestContinuityPSPP1[it]->FillStatusHistogram(Form("h_passedHV_continuity_%i", it+1), "HV");
   h_passedLV_Cont[it] = TestContinuityPSPP1[it]->FillStatusHistogram(Form("h_passedLV_continuity_%i", it+1), "LV");
   hCont_ResChannel_HV[it] = TestContinuityPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceLV_continuity_%i", it+1), "HV");
   hCont_ResChannel_LV[it] = TestContinuityPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceHV_continuity_%i", it+1), "LV");
  }
  else if( test_type = 1 || test_type==2){
   h_passedIns_tot[it] = TestIsolationPSPP1[it]->FillStatusHistogram(Form("h_passed_Isolation_all_%i", it +1));
   h_passedHV_Ins[it] = TestIsolationPSPP1[it]->FillStatusHistogram(Form("h_passedHV_Isolation_%i",it+1), "HV");
   h_passedLV_Ins[it] = TestIsolationPSPP1[it]->FillStatusHistogram(Form("h_passedLV_Isolation_%i",it+1), "LV");
   hIns_ResChannel_HV[it] = TestIsolationPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceHV_isolation_%i",it+1), "HV");
   hIns_ResChannel_LV[it] = TestIsolationPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceLV_isolation_%i",it+1), "LV");
  }
}       
gErrorIgnoreLevel = kWarning;
std::cout<<"************ Drawing Plots...**************" <<std::endl;
std::cout<<"Drawn and Saved Histograms: "<<std::endl;
c_plot = new TCanvas("c_plot","c_plot", 3000, 3500);
c_plot->Divide(2,3);

if(test_type == 1){
 sPDFTitle = TestIsolationPSPP1[0]->GetName() + "___" + currentDate;
 plotting<TH1I*>(h_passedHV_Ins , "InsulationTest_HV_Passed-Failed", 1);
 plotting<TH1I*>(h_passedLV_Ins , "InsulationTest_LV_Passed-Failed", 2);
 plotting<TH1F*>(hIns_ResChannel_HV , "InsulationTest_HV_Resistence", 3);
 plotting<TH1F*>(hIns_ResChannel_LV , "InsulationTest_LV_Resistence", 4);
}
else if(test_type == 0 ){
 sPDFTitle = TestContinuityPSPP1[0]->GetName() + "___" + currentDate;
 plotting<TH1I*>(h_passedHV_Cont ,"ContinuityTest_HV_Passed-Failed", 1);
 plotting<TH1I*>(h_passedLV_Cont , "ContinuityTest_LV_Passed-Failed", 2);
 plotting<TH1F*>(hCont_ResChannel_HV,"ContinuityTest_ResistenceHV", 3);
 plotting<TH1F*>(hCont_ResChannel_LV,"ContinuityTest_ResistenceLV", 4);
}
else if(test_type == 2){
 sPDFTitle = TestIsolationPSPP1[0]->GetName() + "___" + currentDate;
 plotting<TH1I*>(h_passedCont_tot ,"ContinuityTest_All_Passed-Failed",1);
 plotting<TH1I*>(h_passedIns_tot , "InsulationTest_All_Passed-Failed",2);
 plotting<TH1F*>(hIns_ResChannel_HV , "InsulationTest_HV_Resistence",3);
 plotting<TH1F*>(hIns_ResChannel_LV , "InsulationTest_LV_Resistence",4);
 plotting<TH1F*>(hCont_ResChannel_HV,"ContinuityTest_ResistenceHV",5);
 plotting<TH1F*>(hCont_ResChannel_LV,"ContinuityTest_ResistenceLV", 6);
}
gErrorIgnoreLevel = kPrint;
// ***************************************************** //
// ***************************************************** //


// ***************************************************** //
// *** RESISTENCE VERSUS TIME ACQUISITION HISTOGRAMS *** //
// ***************************************************** //
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
// ***************************************************** //
// ***************************************************** //


TString CreateReport;
#ifdef AutoTest
  CreateReport = "y";
#else
std::cout<<"*******************************************"<<std::endl;
std::cout<<"Do you want to create the final report? (y/n)" <<std::endl;
std::cin>>CreateReport;
#endif
std::cout<<"*******************************************"<<std::endl;
std::cout<<"Output: "<<std::endl;
std::cout<<"\033[32mroot histograms have been saved in "<< sOutputRoot << sPDFTitle <<".root\033[0m"<<std::endl;
if(IterationTest==1){
std::cout<<"\033[32mplots has been saved as " + std::string(WORKDIR) +"/output/plots/SingleCable/"<< sPDFTitle <<".pdf\033[0m"<<std::endl;
}
else{
std::cout<<"\033[32mplot pdf has been saved as" + std::string(WORKDIR) +"/output/plots/CheckCable/"<< sPDFTitle <<".pdf\033[0m"<<std::endl;
}

std::cout<<"*****************************************"<<std::endl;


if(CreateReport == "y") {
 Python::PSPP1::WriteFinalReport(sPDFTitle, name[0]);
 Python::PSPP1::UpdateHTML(sPDFTitle);
}

TFile *f_OutPut = new TFile((sOutputRoot + sPDFTitle + ".root").c_str(), "RECREATE");
for(int i=0; i<IterationTest; ++i){
 if(test_type == 0 || test_type == 2){
  h_passedLV_Cont[i]->Write(); h_passedHV_Cont[i]->Write(); hCont_ResChannel_HV[i]->Write(); hCont_ResChannel_LV[i]->Write(); 
 }
 if(test_type == 1 || test_type == 2){ 
  h_passedHV_Ins[i]->Write(); h_passedLV_Ins[i]->Write();  hIns_ResChannel_HV[i]->Write(); hIns_ResChannel_LV[i]->Write();
 }
}
f_OutPut->Write();
f_OutPut->Close();


std::cout<<"removing temporary files...";
std::system(("rm -r " + sInputTestDir + "/\*/tmp").c_str());
std::cout<<"done"<<std::endl;
std::cout<<"Updating comulative statistics..."<<std::endl;
std::system("./statistics");
std::cout<<"done"<<std::endl;


return 0;

gROOT->ProcessLine(".q");


}
