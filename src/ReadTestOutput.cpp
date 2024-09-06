#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>
#include "../include/root.h"
#include "../include/def_variables.h"
#include "../include/Isolation.h"
#include "../include/Continuity.h"

void ReadTestOutput(std::vector<std::string> &TestNameFile, int j){
    std::ifstream inputFile(TestNameFile[j]);
    std::string line;
    std::vector<std::tuple<bool, std::string, double>> continuityData;
    std::vector<std::tuple<bool, std::string, double, double>> insulationData;
    Bool_t FirstTree = false;
    Bool_t SecondTree = false;
    double i, Thresh, Trise, Twait, Tmeas, Vlimit, V, Vramp;
    double r, B;
    std::string AR, str1, str2;
    int lineCounter=0;
    int secondtree=0;

    while(std::getline(inputFile, line)){
      std::istringstream iss(line);
     if(line.find("ContinuityTest") != std::string::npos){
        FirstTree = true;
        SecondTree = false;
     }
     else if (line.find("InsulationTest") != std::string::npos){
        FirstTree = false;
        SecondTree = true;
     }
     else{
        if(lineCounter<5){
            if(lineCounter==0 && iss >> i >> Thresh >> Trise >> Twait >> Tmeas >> AR >> Vlimit)           
            ParametersContinuity.push_back(std::make_tuple(i,Thresh,Trise,Twait,Tmeas,AR));
            else if(lineCounter==1 && iss >> V >> Thresh >> Trise >> Twait >> Tmeas >> i >> Vramp)
            ParametersInsulationInitial.push_back(std::make_tuple(V, Thresh, Trise, Twait, Tmeas, i, Vramp));
            else if(lineCounter==2 && iss>> Trise >> Twait >> Tmeas)
            ParametersInsulationLV.push_back(std::make_tuple(Trise, Twait,  Tmeas, Tmeas));
            else if(lineCounter == 3 && iss >> V >> Thresh >> Trise >> Tmeas)
            ParametersInsulationHV.push_back(std::make_tuple(V, Thresh, Trise, Tmeas));
            else if(lineCounter == 4 && iss>> V >> Thresh >> Trise >> Tmeas)
            ParametersInsulationTsensor.push_back(std::make_tuple(V, Thresh, Trise, Tmeas));
            lineCounter++;
        }
        else{
          if(FirstTree){
            if(iss>>str1>>str2>>r) continuityData.push_back(std::make_tuple((str1 == "Passed"),str2,r));
          }
          else if(SecondTree){
          secondtree++;
          if(iss>>str1>>str2>>r)  insulationData.push_back(std::make_tuple((str1 == "Passed"),str2,r,B));
          }
       }
     }
    }//while
    TString path(TestNameFile[j].c_str());
    int lastSlash = path.Last('/');
    TString testTitle = path( lastSlash+1, path.Length()-lastSlash-4);
    if(test_type == 1 || test_type == 2){
     TestIsolationPSPP1[j] = new Isolation::PSPP1(insulationData, testTitle);
     TestIsolationPSPP1[j]->SetInitialParameters(ParametersInsulationInitial[j]);
     TestIsolationPSPP1[j]->SetIsolationPar("LV", ParametersInsulationLV[j]);
    }
    /*
    TestIsolationPSPP1[j]->SetIsolationPar("HV", ParametersInsulationHV[j]);
    TestIsolationPSPP1[j]->SetIsolationPar("Tsensor", ParametersInsulationTsensor[j]);
    */
    
    if(test_type == 0 || test_type == 2){
    TestContinuityPSPP1[j] = new Continuity::PSPP1(continuityData, testTitle);
   // TestContinuityPSPP1[j]->SetParameters(ParametersContinuity[j]);
    }
  
  }
