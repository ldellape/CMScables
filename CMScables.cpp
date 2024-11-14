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



// ***************** SERIALE **********************
// PS-PP1 ---------> OT_LIC_30208X                *
// PP1-PP0 --------> OT_MSC_30208X_X              *
// OCTOPUS --------> OT_OCT_XXXXX_X               *  
// CHAIN -------> OT_CHN_30208X_30208X_X_XXXXX_X  * 
// ************************************************ 



int main(int argc, char* argv[]){

 if(argc<2)  printlogo();

// ****************************************************************************************** //
// INPUT                                                                                      //
// ****************************************************************************************** //
 if(argc>1) start(argc, argv); // mode from command line ./CMScables --input ...

 else if(!CommandLine){ // choose manually type and number of test
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
std::cout<<"Tests:   "<<std::endl;
std::string name[IterationTest];
 for(int j=0; j<IterationTest; j++){
    std::size_t LastDot = TestName[j].find_last_of(".");
    name[j] = TestName[j].substr(0, LastDot);
    std::cout<<name[j]<<std::endl;
 }
std::cout<<"*******************************************************************"<<std::endl;
// ****************************************************************************************** //
// ****************************************************************************************** //




// ****************************************************************************************** //
//// STORE CONTINUITY/ISOLATION CLASS OBJECTS                                                 //
// ****************************************************************************************** //
std::cout<<"preparing text files..." <<std::endl;
for(int i=0; i<int(TestName.size()); i++){
std::cout<<"testname: "<<TestName[i]<<std::endl;
Python::PSPP1::ChangeTextFile(TestName[i]);
}
std::cout<<"Input Tests:"<<std::endl;
std::cout<<"Iterationt test size: "<<IterationTest<<std::endl;

for(int i=0; i<IterationTest; ++i){
  std::cout<<Form("%i",i+1)<<"-"<<TestPath[i]<<std::endl;
  TString serial = TestPath[i](TestPath[i].Last('/') + 1, TestPath[i].Length() - TestPath[i].Last('/') - 5);
  if(serial.Contains("LIC")) ReadTestOutput(TestPath[i], "PSPP1");
  else if(serial.Contains("OCT")) ReadTestOutput(TestPath[i], "OCTOPUS");
  else if(serial.Contains("CHN")) ReadTestOutput(TestPath[i], "FULL CHAIN");
  else if(serial.Contains("MSC")) ReadTestOutput(TestPath[i], "PP0");
  else ReadTestOutput(TestPath[i], "PSPP1");
  std::cout<<"done"<<std::endl;
  std::cout<<"****************************************"<<std::endl;
}
// ***************************************************************************************** //
// ***************************************************************************************** //




// ***************************************************************************************** //
//// HISTOGRAMS (FILLILING AND PLOTTING)                                                     //
// ***************************************************************************************** //
std::cout<<IterationTest<<std::endl;
for(int it = 0; it< IterationTest; it++){
   TString serial = TestPath[it](TestPath[it].Last('/') + 1, TestPath[it].Length() - TestPath[it].Last('/') - 5);
  if(ContinuityTest && InsulationTest){
   //if(serial.Contains("LIC")){
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
   //}
    if(serial.Contains("OCT") || serial.Contains("CHN")){
     hIns_Difference_LV[it] = TestIsolationOctopus[it]->FillResistenceLengthDifference(Form("h_DeltaResistenceLV_isolation_%i", it +1), "LV");
     hIns_Difference_HV[it] = TestIsolationOctopus[it]->FillResistenceLengthDifference(Form("h_DeltaResistenceHV_isolation_%i", it +1), "HV");
     hCon_Difference_LV[it] = TestContinuityOctopus[it]->FillResistenceLengthDifference(Form("h_DeltaResistenceLV_continuity_%i", it +1), "LV");
     hCon_Difference_HV[it] = TestContinuityOctopus[it]->FillResistenceLengthDifference(Form("h_DeltaResistenceHV_continuity_%i", it +1), "HV");
    }
  }
  else if(ContinuityTest && !InsulationTest){
  // if(serial.Contains("LIC")){
    h_passedHV_Cont[it] = TestContinuityPSPP1[it]->FillStatusHistogram(Form("h_passedHV_continuity_%i", it+1), "HV");
    h_passedLV_Cont[it] = TestContinuityPSPP1[it]->FillStatusHistogram(Form("h_passedLV_continuity_%i", it+1), "LV");
    hCont_ResChannel_HV[it] = TestContinuityPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceLV_continuity_%i", it+1), "HV");
    hCont_ResChannel_LV[it] = TestContinuityPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceHV_continuity_%i", it+1), "LV");
   // }
   if(serial.Contains("OCT") || serial.Contains("CHN")){
     hCon_Difference_LV[it] = TestContinuityOctopus[it]->FillResistenceLengthDifference(Form("h_DeltaResistenceLV_continuity_%i", it +1), "LV");
     hCon_Difference_HV[it] = TestContinuityOctopus[it]->FillResistenceLengthDifference(Form("h_DeltaResistenceHV_continuity_%i", it +1), "HV");
    }
  }
  else if(InsulationTest && !ContinuityTest){
  // if(serial.Contains("LIC")){
    h_passedIns_tot[it] = TestIsolationPSPP1[it]->FillStatusHistogram(Form("h_passed_Isolation_all_%i", it +1));
    h_passedHV_Ins[it] = TestIsolationPSPP1[it]->FillStatusHistogram(Form("h_passedHV_Isolation_%i",it+1), "HV");
    h_passedLV_Ins[it] = TestIsolationPSPP1[it]->FillStatusHistogram(Form("h_passedLV_Isolation_%i",it+1), "LV");
    hIns_ResChannel_HV[it] = TestIsolationPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceHV_isolation_%i",it+1), "HV");
    hIns_ResChannel_LV[it] = TestIsolationPSPP1[it]->FillResistenceChannelHistogram(Form("h_resistenceLV_isolation_%i",it+1), "LV");
   //}
   if(serial.Contains("OCT") || serial.Contains("CHN")){
     hCon_Difference_LV[it] = TestContinuityOctopus[it]->FillResistenceLengthDifference(Form("h_DeltaResistenceLV_continuity_%i", it +1), "LV");
     hCon_Difference_HV[it] = TestContinuityOctopus[it]->FillResistenceLengthDifference(Form("h_DeltaResistenceHV_continuity_%i", it +1), "HV");
    }
  }
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
// ***************************************************************************************** //
// ***************************************************************************************** //




// ***************************************************************************************** //
// RESISTENCE VERSUS TIME ACQUISITION HISTOGRAMS                                             //
// ***************************************************************************************** //
gErrorIgnoreLevel = kWarning;
std::cout<<"****************************************"<<std::endl;
if (Ins_Time && InsulationTest) {
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
std::cout<<"*******************************************"<<std::endl;
// ***************************************************************************************** //
// ***************************************************************************************** //



// ***************************************************************************************** //
// SAVE THE OUTPUTS (ROOT + REPORT)
// ***************************************************************************************** //
std::cout<<"Output: "<<std::endl;
std::cout<<"\033[32mroot histograms have been saved in "<< sOutputRoot << sPDFTitle <<".root\033[0m"<<std::endl;
if(IterationTest==1){
  std::cout<<"\033[32mplots has been saved as " + std::string(WORKDIR) +"/output/plots/SingleCable/"<< sPDFTitle<<".pdf\033[0m"<<std::endl;
}
else{
  std::cout<<"\033[32mplot pdf has been saved as" + std::string(WORKDIR) +"/output/plots/CheckCable/"<< sPDFTitle <<".pdf\033[0m"<<std::endl;
}
std::cout<<"*****************************************"<<std::endl;


TString CreateReport;
if(!CommandLine){
  std::cout<<"Do you want to create the final report? (y/n)" <<std::endl;
  std::cin>>CreateReport;
  if(CreateReport == "y")  Python::PSPP1::WriteFinalReport(sPDFTitle, name[0]);
}
else if(CommandLine){
  CreateReport = "y";
   Python::PSPP1::WriteFinalReport(sPDFTitle, name[0]);
}
TFile *f_OutPut = new TFile((sOutputRoot + sPDFTitle + ".root").c_str(), "RECREATE");
for(int i=0; i<IterationTest; ++i){
 if(ContinuityTest){
   h_passedLV_Cont[i]->Write(); h_passedHV_Cont[i]->Write(); hCont_ResChannel_HV[i]->Write(); hCont_ResChannel_LV[i]->Write(); 
 }
 if(InsulationTest){ h_passedHV_Ins[i]->Write(); h_passedLV_Ins[i]->Write();  hIns_ResChannel_HV[i]->Write(); hIns_ResChannel_LV[i]->Write();
 }
}
f_OutPut->Write();
f_OutPut->Close();
std::cout<<"removing temporary files...";
std::system(("rm -r " + sInputTestDir + "*/tmp").c_str());
std::system(("rm -r " + sInputTestDir + "*/*/tmp").c_str());
std::cout<<"done"<<std::endl;
// ***************************************************************************************** //
// ***************************************************************************************** //




// ***************************************************************************************** //
// UPDATE/CREATE CUMULATIVE STATISTICS
// ***************************************************************************************** //
std::cout<<"*****************************************"<<std::endl;
std::cout<<"Updating comulative statistics..."<<std::endl;
if(!CommandLine) std::system("./statistics");
else if(CommandLine){
    // update the corresponding histograms //
    TString serial = TestPath[0](TestPath[0].Last('/') + 1, TestPath[0].Length() - TestPath[0].Last('/') - 5);
    if(serial.Contains("CHN")) std::system("./statistics FULL_CHAIN");
    else if(serial.Contains("LIC")) std::system("./statistics PSPP1");
    else std::system("./statistics");
}
std::cout<<"*****************************************"<<std::endl;
std::cout<<"done"<<std::endl;
std::cout<<"*****************************************"<<std::endl;
std::cout<<"\033[32mTHE END\033[0m" << std::endl;
// ***************************************************************************************** //
// ***************************************************************************************** //



return 0;
gROOT->ProcessLine(".q");

}

