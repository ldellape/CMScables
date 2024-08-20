#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <tuple>
#include "../include/root.h"
#include "../include/def_variables.h"
#include "../include/py_run.h"

TGraph* ReadTestTime(std::string pathFile){
    int point=0;
    //TGraph *gr_Time;
    std::vector<double> ResTime;
    std::vector<double> number_value;
    std::string line;
    Python::PSPP1::ChangeTextFileINI(pathFile);
    std::ifstream inputTimeTesolution(pathFile);
    if(!inputTimeTesolution.is_open()) return nullptr;
    while(std::getline(inputTimeTesolution, line)){
        std::stringstream ss(line);
        double value;
        if(ss >> value){
            point++;
            number_value.push_back(point);
            ResTime.push_back(value);
        }
    }
    inputTimeTesolution.close();
    // fill graph //
    TGraph *gr_Time = new TGraph(point, &number_value[0], &ResTime[0]);
    return gr_Time;
}