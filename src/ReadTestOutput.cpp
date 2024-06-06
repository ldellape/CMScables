// legge l'output .txt e crea i tree per test di continuità/isolamento //
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>
#include "../include/root.h"
#include "../include/def_variables.h"


void ReadTestOutput(std::vector<std::string> &TestNameFile, Int_t j){
    std::ifstream inputFile(TestNameFile[j]);
    std::string line;
    std::vector<std::tuple<std::string, std::string, double>> continuityData;
    std::vector<std::tuple<std::string, std::string, double, double>> insulationData;
    Bool_t FirstTree = false;
    Bool_t SecondTree = false;
    Bool_t ParContTest = false;
    Bool_t ParInsTest = false;
    double i, Thresh, Trise, Twait, Tmeas, Vlimit;
    double r, B;
    std::string AR;

    while(std::getline(inputFile, line)){
      std::istringstream iss(line);
     if(line.find("ContinuityTest") != std::string::npos){
        FirstTree = true;
        SecondTree = false;
        ParContTest = true;
        ParInsTest = false;
     }
     else if (line.find("InsulationTest") != std::string::npos){
        FirstTree = false;
        SecondTree = true;
        ParContTest = false;
        ParInsTest = true;
     }
     else{
        // parameters for test //
        // results of the test //
        std::string str1, str2;
        std::istringstream iss(line);
        if(ParContTest || ParInsTest){
            if(iss>> i >> Thresh >> Trise >> Twait >> Tmeas >> AR >> Vlimit){
               if(FirstTree) ParametersContinuity.push_back(std::make_tuple(i,Thresh, Trise, Twait, Tmeas, AR, Vlimit));
               }
             //  else if(SecondTree) ParametersContinuity.push_back(std::make_tuple(i,Thresh, Trise, Twait, Tmeas, AR, Vlimit));
               ParContTest = false;
               ParInsTest = false;
        }
        
        else{
          if(FirstTree){
            if(iss>>str1>>str2>>r) continuityData.push_back(std::make_tuple(str1,str2,r));
          }
          else if(SecondTree){
          if(iss>>str1>>str2>>r)  insulationData.push_back(std::make_tuple(str1,str2,r,B));
          }
       }
     }
  }    
    // fill continuity tree //
    if(test_type == 0){
    for(const auto&it : continuityData){
        if(std::get<0>(it) == "Passed") statusCon = 1;
        else if(std::get<0>(it) == "Failed") statusCon = 0;
        channelCon = std::get<1>(it);
        resistenceCon = std::get<2>(it);
        cableCon = j;
        ContinuityTree->Fill();
    }
    }
    // fill Insulation Tree //
    else if(test_type == 1){
    for(const auto& it : insulationData){
        if(std::get<0>(it) == "Passed") statusIns = 1;
        else if(std::get<0>(it) == "Failed") statusIns = 0;
        channelIns = std::get<1>(it);
        resistenceIns = std::get<2>(it);
        FieldIns = std::get<3>(it);
        cableIns = j;
        InsulationTree->Fill();
    }
    }
    else if(test_type == 2){
       for(const auto&it : continuityData){
        if(std::get<0>(it) == "Passed") statusCon = 1;
        else if(std::get<0>(it) == "Failed") statusCon = 0;
        channelCon = std::get<1>(it);
        resistenceCon = std::get<2>(it);
        cableCon = j;
        ContinuityTree->Fill();
    }
       for(const auto& it : insulationData){
        if(std::get<0>(it) == "Passed") statusIns = 1;
        else if(std::get<0>(it) == "Failed") statusIns = 0;
        channelIns = std::get<1>(it);
        resistenceIns = std::get<2>(it);
        FieldIns = std::get<3>(it);
        cableIns = j;
        InsulationTree->Fill();
    }    
    }
    
    }



