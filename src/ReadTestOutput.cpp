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
            ParametersContinuity.push_back(std::make_tuple(i,Thresh,Trise,Twait,Tmeas,AR,Vlimit));
            else if(lineCounter==1 && iss >> V >> Thresh >> Trise >> Twait >> Tmeas >> i >> Vramp)
            ParametersInsulationInitial.push_back(std::make_tuple(V, Thresh, Trise, Twait, Tmeas, i, Vramp));
            else if(lineCounter==2 && iss>> Trise >> Twait >> Tmeas)
            ParametersInsulationLV.push_back(std::make_tuple(Trise, Twait,  Tmeas));
            else if(lineCounter == 3 && iss >> V >> Thresh >> Trise >> Tmeas)
            ParametersInsulationHV.push_back(std::make_tuple(V, Thresh, Trise, Tmeas));
            else if(lineCounter == 4 && iss>> V >> Thresh >> Trise >> Tmeas)
            ParametersInsulationTsensor.push_back(std::make_tuple(V, Thresh, Trise, Tmeas));
            lineCounter++;
        }
        
        else{
          if(FirstTree){
            if(iss>>str1>>str2>>r) continuityData.push_back(std::make_tuple(str1,str2,r));
          }
          else if(SecondTree){
          secondtree++;
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



