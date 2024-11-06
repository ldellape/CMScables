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
#include "./include/Classes.h"
#include "./include/plotting.h"


int main(int argc, char* argv[]){
 if(argc<2)  printlogo();
  // ************************************* //
  // ************ INPUT TESTS ************ //
  // ************************************* //
 if(argc>1) start(argc, argv); // auto mode
 else if(!CommandLine){ // input from command line
   TestName = listAndChooseFiles();
   TestType();
   IterationTest = TestName.size();
   if(InsulationTest){
    Ins_Time = TimeAcquisition();
    if(Ins_Time){
      TestNameTimeAcquisition = DirTimeAcquisition();
    }
   }
 }

 
IterationTest = TestName.size();
std::cout<<"*******************************************************************"<<std::endl;
std::cout<<"Test Processati:   "<<std::endl;
std::string name[IterationTest];
 for(int j=0; j<IterationTest; j++){
    std::size_t LastDot = TestName[j].find_last_of(".");
    name[j] = TestName[j].substr(0, LastDot);
    std::cout<<name[j]<<std::endl;
 }
std::cout<<"*******************************************************************"<<std::endl;


// *************************************************** //
// *************************************************** //

// ********* changing txt files and store Continuity/Isolation class objects **************** //
// ****************************************************************************************** //
std::cout<<"preparing text files..." <<std::endl;
for(int i=0; i<int(TestName.size()); i++){
Python::PSPP1::ChangeTextFile(TestName[i]);
}
std::cout<<"Input Tests:"<<std::endl;
std::cout<<"Iterationt test size: "<<IterationTest<<std::endl;

for(int i=0; i<IterationTest; ++i){
  std::cout<<Form("%i",i+1)<<"-"<<TestPath[i]<<std::endl;
  if(ContinuityTest) ReadTestOutput("PSPP1", 0, TestPath[i]);
  if(InsulationTest) ReadTestOutput("PSPP1", 1, TestPath[i]);
}
std::cout<<TestIsolationPSPP1[0]->GetName()<<std::endl;

std::cout<<"done"<<std::endl;
std::cout<<"****************************************"<<std::endl;
// ***************************************************************************************** //

// ***************************************************** //
// *** HISTOGRAMS (FILLILING AND PLOTTING) ************* //
// ***************************************************** //
std::cout<<IterationTest<<std::endl;
for(int it = 0; it< IterationTest; it++){
  if(ContinuityTest && InsulationTest){
   h_passedCont_tot[it] = TestContinuityPSPP1[it]->FillStatusHistogram(Form("h_passed_continuity_all_%i", it+1));
   h_passedHV_Cont[it] = TestContinuityPSPP1[it]->FillStatusHistogram(Form("h_passedHV_continuity_%i", it+1), "HV");
   h_passedLV_Cont[it] = TestContinuityPSPP1[it]->FillStatusHistogram(Form("h_passedLV_continuity_%i", it+1), "LV");
   hCont_ResChannel_HV[it] = TestContinuityPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceLV_continuity_%i", it+1), "HV");
   hCont_ResChannel_LV[it] = TestContinuityPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceHV_continuity_%i", it+1), "LV");
   h_passedIns_tot[it] = TestIsolationPSPP1[it]->FillStatusHistogram(Form("h_passed_Isolation_all_%i", it +1));
   h_passedHV_Ins[it] = TestIsolationPSPP1[it]->FillStatusHistogram(Form("h_passedHV_Isolation_%i",it+1), "HV");
   h_passedLV_Ins[it] = TestIsolationPSPP1[it]->FillStatusHistogram(Form("h_passedLV_Isolation_%i",it+1), "LV");
   hIns_ResChannel_HV[it] = TestIsolationPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceHV_isolation_%i",it+1), "HV");
   hIns_ResChannel_LV[it] = TestIsolationPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceLV_isolation_%i",it+1), "LV");
  }
  else if(ContinuityTest && !InsulationTest){
   h_passedHV_Cont[it] = TestContinuityPSPP1[it]->FillStatusHistogram(Form("h_passedHV_continuity_%i", it+1), "HV");
   h_passedLV_Cont[it] = TestContinuityPSPP1[it]->FillStatusHistogram(Form("h_passedLV_continuity_%i", it+1), "LV");
   hCont_ResChannel_HV[it] = TestContinuityPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceLV_continuity_%i", it+1), "HV");
   hCont_ResChannel_LV[it] = TestContinuityPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceHV_continuity_%i", it+1), "LV");
  }
  else if(InsulationTest && !ContinuityTest){
   h_passedIns_tot[it] = TestIsolationPSPP1[it]->FillStatusHistogram(Form("h_passed_Isolation_all_%i", it +1));
   h_passedHV_Ins[it] = TestIsolationPSPP1[it]->FillStatusHistogram(Form("h_passedHV_Isolation_%i",it+1), "HV");
   h_passedLV_Ins[it] = TestIsolationPSPP1[it]->FillStatusHistogram(Form("h_passedLV_Isolation_%i",it+1), "LV");
   hIns_ResChannel_HV[it] = TestIsolationPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceHV_isolation_%i",it+1), "HV");
   hIns_ResChannel_LV[it] = TestIsolationPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceLV_isolation_%i",it+1), "LV");
  }
//}  
/*
else if(CableType[it] == "OCTOPUS"){
  //histograms for octopus cable
}
else if(CableType[it] == "PP0"){
  //histograms for PP0 cable
}
else if(CableType[it] == "CHAIN"){
  //histograsm for full chain test
}
*/
}


gErrorIgnoreLevel = kWarning;
std::cout<<"\033[32mDRAWING HISTOGRAMS...\033[0m" <<std::endl;
std::cout<<"Drawn and Saved Histograms: "<<std::endl;
c_plot = new TCanvas("c_plot","c_plot", 3000, 3500);
c_plot->Divide(2,3);

if(InsulationTest && ContinuityTest){
  sPDFTitle =  TestIsolationPSPP1[0]->GetName();
  plotting<TH1I*>(h_passedCont_tot ,"ContinuityTest_All_Passed-Failed",1);
  plotting<TH1I*>(h_passedIns_tot , "InsulationTest_All_Passed-Failed",2);
  plotting<TH1F*>(hIns_ResChannel_HV , "InsulationTest_HV_Resistence",3);
  plotting<TH1F*>(hIns_ResChannel_LV , "InsulationTest_LV_Resistence",4);
  plotting<TH1F*>(hCont_ResChannel_HV, "ContinuityTest_ResistenceHV",5);
  plotting<TH1F*>(hCont_ResChannel_LV, "ContinuityTest_ResistenceLV", 6);
}
else if(InsulationTest && !ContinuityTest){
  sPDFTitle =  TestIsolationPSPP1[0]->GetName();
  plotting<TH1I*>(h_passedHV_Ins , "InsulationTest_HV_Passed-Failed", 1);
  plotting<TH1I*>(h_passedLV_Ins , "InsulationTest_LV_Passed-Failed", 2);
  plotting<TH1F*>(hIns_ResChannel_HV , "InsulationTest_HV_Resistence", 3);
  plotting<TH1F*>(hIns_ResChannel_LV , "InsulationTest_LV_Resistence", 4);
}
else if(!InsulationTest && ContinuityTest){
  sPDFTitle =  TestContinuityPSPP1[0]->GetName();
  std::cout<<"PDF title "<<sPDFTitle<<std::endl;
  plotting<TH1I*>(h_passedHV_Cont ,"ContinuityTest_HV_Passed-Failed", 1);
  plotting<TH1I*>(h_passedLV_Cont , "ContinuityTest_LV_Passed-Failed", 2);
  plotting<TH1F*>(hCont_ResChannel_HV,"ContinuityTest_ResistenceHV", 3);
  plotting<TH1F*>(hCont_ResChannel_LV,"ContinuityTest_ResistenceLV", 4);
}
gErrorIgnoreLevel = kPrint;
// ***************************************************** //
// ***************************************************** //


// ***************************************************** //
// *** RESISTENCE VERSUS TIME ACQUISITION HISTOGRAMS *** //
// ***************************************************** //
gErrorIgnoreLevel = kWarning;
std::cout<<"****************************************"<<std::endl;
if (Ins_Time) {
 std::cout <<"\033[32mPlotting LV cables resistance versus time acquisition...\033[0m"<<std::endl;
 std::vector<std::pair<std::string, TGraph*>> grRes_TimeLV[IterationTest];
 std::vector<std::pair<std::string, TGraph*>> grRes_TimeLVR[IterationTest];
 for (int k = 0; k < IterationTest; k++){
  grRes_TimeLV[k] = TestIsolationPSPP1[k]->FillGraphTimeResistence("LV");
  grRes_TimeLVR[k] = TestIsolationPSPP1[k]->FillGraphTimeResistence("LVR");
}
plottingGraph(grRes_TimeLV, "LV");
plottingGraph(grRes_TimeLVR, "LVR");
std::cout<<"\033[32mDONE\033[0m"<<std::endl;
}
gErrorIgnoreLevel = kPrint;
// ***************************************************** //
// ***************************************************** //
std::cout<<"*******************************************"<<std::endl;
std::cout<<"Output: "<<std::endl;
//std::cout<<"\033[32mroot histograms have been saved in "<< sOutputRoot << sPDFTitle <<".root\033[0m"<<std::endl;
if(IterationTest==1){
  std::cout<<"\033[32mplots has been saved as " + std::string(WORKDIR) +"/output/plots/SingleCable/"<< sPDFTitle<<".pdf\033[0m"<<std::endl;
}
else{
  std::cout<<"\033[32mplot pdf has been saved as" + std::string(WORKDIR) +"/output/plots/CheckCable/"<< sPDFTitle <<".pdf\033[0m"<<std::endl;
}
std::cout<<"*****************************************"<<std::endl;


TString CreateReport;
#ifdef AutoTest
  CreateReport = "y";
#else
if(!CommandLine){
std::cout<<"Do you want to create the final report? (y/n)" <<std::endl;
std::cin>>CreateReport;
}
else if (CommandLine){
CreateReport = "y";
}
#endif

#ifndef AutoTest
if(CreateReport == "y") {
 Python::PSPP1::WriteFinalReport(sPDFTitle, name[0]);
}
#else
 for(int i=0; i<IterationTest; i++){
 std::cout<<name[i]<<std::endl;
 Python::PSPP1::WriteFinalReport(sPDFTitle[i], name[i]);
}
#endif

/*
TFile *f_OutPut = new TFile((sOutputRoot + sPDFTitle + ".root").c_str(), "RECREATE");
for(int i=0; i<IterationTest; ++i){
 if(ContinuityTest){
  h_passedLV_Cont[i]->Write(); h_passedHV_Cont[i]->Write(); hCont_ResChannel_HV[i]->Write(); hCont_ResChannel_LV[i]->Write(); 
 }
 if(InsulationTest){ 
  h_passedHV_Ins[i]->Write(); h_passedLV_Ins[i]->Write();  hIns_ResChannel_HV[i]->Write(); hIns_ResChannel_LV[i]->Write();
 }
}
f_OutPut->Write();
f_OutPut->Close();
*/


std::cout<<"done"<<std::endl;
std::cout<<"*****************************************"<<std::endl;
std::cout<<"Updating comulative statistics..."<<std::endl;
std::system("./statistics");
std::cout<<"*****************************************"<<std::endl;
std::cout<<"removing temporary files...";
std::system(("rm -r " + sInputTestDir + "/*/tmp").c_str());
std::system(("rm -r " + sInputTestDir + "/tmp").c_str());
std::cout<<"done"<<std::endl;
std::cout<<"*****************************************"<<std::endl;
std::cout<<"\033[32mTHE END\033[0m" << std::endl;



return 0;

gROOT->ProcessLine(".q");

}

