#include "../include/def_variables.h"
#include "../include/root.h"
#include "../include/Classes.h"
#include <fstream>
#include <sstream>


void ReadTestOutput(TString option, Int_t mode, TString path){
    TString TestNameFile = path;
    std::vector<std::tuple<double,double,double,double,double, ::TString, double>> ParametersContinuity;
    std::vector<std::tuple<double,double,double,double,double, TString, double, TString, double, double>> ParametersInsulationInitial;
    std::ifstream inputFile(TestNameFile.Data()); 
    TString line;
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
 
    while (inputFile.good() && line.ReadLine(inputFile)) {  
        std::istringstream iss(line.Data()); 
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

    // ***** get cable name ******* //
    int lastSlash = TestNameFile.Last('/');
    TString testTitle = TestNameFile(lastSlash + 1, TestNameFile.Length() - lastSlash - 5);

    if (option == "PSPP1") {
      // continuity //
      if((mode==0 || mode ==2) && !continuityData.empty()){
        TestContinuityPSPP1.push_back(new PSPP1("continuity", continuityData, testTitle));
        (TestContinuityPSPP1.back())->SetParameters(ParametersContinuity.back());
        (TestContinuityPSPP1.back())->SetPath(TestNameFile);
        (TestContinuityPSPP1.back())->SetTestType("continuity");
      }
      // isolation //
      if((mode==1 || mode==2) && !insulationData.empty()){
        TestIsolationPSPP1.push_back(new PSPP1("isolation", insulationData, testTitle));
        (TestIsolationPSPP1.back())->SetInitialParameters(ParametersInsulationInitial.back());
        (TestIsolationPSPP1.back())->SetIsolationPar("LV", ParametersInsulationLV.back());
        (TestIsolationPSPP1.back())->SetIsolationPar("HV", ParametersInsulationHV.back());
        (TestIsolationPSPP1.back())->SetIsolationPar("Tsensor", ParametersInsulationTsensor.back());
        (TestIsolationPSPP1.back())->SetPath(TestNameFile);
        (TestIsolationPSPP1.back())->SetTestType("isolation");
        
    //    (TestIsolationPSPP1.back())->SetField(Bfield);
      }
    }
    else if(option == "OCTOPUS"){ 
    }
}

