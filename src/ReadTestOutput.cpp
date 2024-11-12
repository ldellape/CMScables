#include "../include/def_variables.h"
#include "../include/root.h"
#include "../include/Classes.h"
#include <fstream>
#include <sstream>


void ReadTestOutput(Int_t mode, TString path, TString option){
    TString TestNameFile = path;
    std::vector<std::tuple<double,double,double,double,double, ::TString, double>> ParametersContinuity;
    std::vector<std::tuple<double,double,double,double,double, TString, double, TString, double, double>> ParametersInsulationInitial;
    std::ifstream inputFile(TestNameFile.Data()); 
    TString line;
    TString cabletype = option;
    std::vector<std::tuple<bool, TString, double>> continuityData;
    std::vector<std::tuple<bool, TString, double>> insulationData;
    std::vector<std::vector<double>> ParametersInsulationHV;  
    std::vector<std::vector<double>> ParametersInsulationLV;
    std::vector<std::vector<double>> ParametersInsulationTsensor;
    std::vector<double> Bfield;
    Bool_t FirstTree = false;
    Bool_t SecondTree = false;
    double i, Thresh, Trise, Twait, Tmeas, Vlimit, V, Vramp, Tmeasfact;
    double r, B;
    TString AR, str1, str2, TmeasRed;
    int lineCounter = 0;
    int secondtree = 0;
 
    // ---------------------------------------------------------------------------------------------------  //
    // reading output of the test                                                                           //
    // ---------------------------------------------------------------------------------------------------  //
    while (inputFile.good() && line.ReadLine(inputFile)) {  
        std::istringstream iss(line.Data()); 
        if(option == "NONE"){
         //find cable type //
         if(line.Contains("PSPP1")) cabletype = "PSPP1";
         else if(line.Contains("OCTOPUS")) cabletype="OCTOPUS";
        }
        if (line.Contains("ContinuityTest")) {
            FirstTree = true;
            SecondTree = false;
        } else if (line.Contains("InsulationTest")) {
            FirstTree = false;
            SecondTree = true;
        } else {
            if(lineCounter<5){
            if(lineCounter==0 && iss >> i >> Thresh >> Trise >> Twait >> Tmeas >> AR >> Vlimit)
            ParametersContinuity.push_back(std::make_tuple(i,Thresh,Trise,Twait,Tmeas,AR,Vlimit));
             if(lineCounter==1 && iss >> V >> Thresh >> Trise >> Twait >> Tmeas >> AR >> i >> TmeasRed >> Tmeasfact >> Vramp){
              ParametersInsulationInitial.push_back(std::make_tuple(V, Thresh, Trise, Twait, Tmeas,AR, i, TmeasRed, Tmeasfact, Vramp));
             }
             else if(lineCounter==2 && iss>> Trise >> Twait >> Tmeas) ParametersInsulationLV.push_back({Trise, Twait, Tmeas});
             else if(lineCounter == 3 && iss >> V >> Thresh >> Trise >> Tmeas) ParametersInsulationHV.push_back({V,Thresh,Trise,Tmeas});
             else if(lineCounter == 4 && iss>> V >> Thresh >> Trise >> Tmeas) ParametersInsulationTsensor.push_back({V,Thresh,Trise,Tmeas});            
               lineCounter++;
            }
            else {
                if (FirstTree) {
                    if (iss >> str1 >> str2 >> r)
                        continuityData.push_back(std::make_tuple((str1 == "Passed"), str2, r));
                } else if (SecondTree) {
                    secondtree++;
                    if (iss >> str1 >> str2 >> r){
                    //    Bfield.push_back(B);
                        insulationData.push_back(std::make_tuple((str1 == "Passed"), str2, r));
                    }
                }   
            }
        }
    }  // end while
    // ---------------------------------------------------------------------------------------------------  //
    // ---------------------------------------------------------------------------------------------------  //
    


    // ---------------------------------------------------------------------------------------------------  //
    //  fill class objects. 
    // ---------------------------------------------------------------------------------------------------  //
    int lastSlash = TestNameFile.Last('/');
    TString testTitle = TestNameFile(lastSlash + 1, TestNameFile.Length() - lastSlash - 5);
    if (option == "PSPP1") {
      // continuity //
      if(ContinuityTest && !continuityData.empty()){
        TestContinuityPSPP1.push_back(new PSPP1("continuity", continuityData, testTitle));
        if(!ParametersContinuity.empty()) (TestContinuityPSPP1.back())->SetParameters(ParametersContinuity.back());
        (TestContinuityPSPP1.back())->SetPath(TestNameFile);
        (TestContinuityPSPP1.back())->SetTestType("continuity");
      }
      // isolation //
      if(InsulationTest && !insulationData.empty()){
      std::cout<<"isolation"<<std::endl;
        TestIsolationPSPP1.push_back(new PSPP1("isolation", insulationData, testTitle));
        if(!ParametersInsulationInitial.empty()) (TestIsolationPSPP1.back())->SetInitialParameters(ParametersInsulationInitial.back());
        if(!ParametersInsulationLV.empty()) (TestIsolationPSPP1.back())->SetIsolationPar("LV", ParametersInsulationLV.back());
        if(!ParametersInsulationHV.empty()) (TestIsolationPSPP1.back())->SetIsolationPar("HV", ParametersInsulationHV.back());
        if(!ParametersInsulationTsensor.empty()) (TestIsolationPSPP1.back())->SetIsolationPar("Tsensor", ParametersInsulationTsensor.back());
        (TestIsolationPSPP1.back())->SetPath(TestNameFile);
        (TestIsolationPSPP1.back())->SetTestType("isolation");
      }
    }
    else if(option == "OCTOPUS"){
    }
    // ---------------------------------------------------------------------------------------------------  //
    // ---------------------------------------------------------------------------------------------------  //


    // ---------------------------------------------------------------------------------------------------  //
    // switch to continuity/isolation only or end the script if test results not matched with input option  //
    // ---------------------------------------------------------------------------------------------------  //
    if(ContinuityTest && InsulationTest && insulationData.empty() && !continuityData.empty()){
     std::cout<<"Isolation Test not present. Switch to continuity only"<<std::endl;
     InsulationTest = false;
     Ins_Time = false;
    }
    else if(ContinuityTest && InsulationTest && !insulationData.empty() && continuityData.empty()){
     std::cout<<"Continuity Test not present. Switch to isolation only"<<std::endl;
     ContinuityTest = false;
    }
    else if(InsulationTest && !ContinuityTest && insulationData.empty()){
     std::cout<<"Isolation Test not present for "<<testTitle<<". Stop."<<std::endl;
     gROOT->ProcessLine(".q");
    }
    else if(ContinuityTest && !InsulationTest && continuityData.empty()){
     std::cout<<"Continuity Test not present for "<<testTitle<< ". Stop"<<std::endl;
     gROOT->ProcessLine(".q");
    }
    // ---------------------------------------------------------------------------------------------------  //
    // ---------------------------------------------------------------------------------------------------  //
}

