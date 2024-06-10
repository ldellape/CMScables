// identified by pair of letter and number //
#ifndef USERFUNC_h
#define USERFUNC_h
#include "def_variables.h"
#include "root.h"

// functions defined in src directory //
void plotting(std::vector<TH1F*> &h, std::string sTitle, Int_t number_pad);
void plottingGraph(std::vector<TGraph*> &gr, Int_t Iteration, Int_t LVchannel);
void ReadTestOutput(std::vector<std::string> &TestNameFile, Int_t j);
void WritePDF(std::vector<TCanvas*> &canvas, std::string sPDFTitle);

// functions in this header files //
void fill_LVcables(std::vector<std::pair<std::string, Int_t>> &v);
void fill_LVcables_RTN(std::vector<std::pair<std::string,Int_t>> &v);
void fill_HVcables(std::vector<std::pair<std::string, Int_t>> &v);
void fill_HVcables_RTN(std::vector<std::pair<std::string, Int_t>> &v);
void fill_DRAINcables(std::vector<std::pair<std::string, Int_t>> &v);
void fill_Tsensors(std::vector<std::pair<std::string, Int_t>> &v);
Float_t FindMax(TTree *tree, Int_t Cable, TString Option);
Float_t FindMin(TTree *tree, Int_t Cable, TString Option);
void start();


void fill_LVcables(std::vector<std::pair<std::string, Int_t>> &v){
    std::string name = "LV";
    for(int i=0; i<NumberLVcables; ++i){
    v.push_back(make_pair(name, i+1));
    }
}
void fill_LVcables_RTN(std::vector<std::pair<std::string,Int_t>> &v){
    std::string name = "LVR";
    for(int i=0; i<NumberLVcables; ++i){
    v.push_back(make_pair(name, i+1));
    }
}
void fill_HVcables(std::vector<std::pair<std::string, Int_t>> &v){
    std::string name = "HV_RTN";
    for(int i=0; i<NumberHVcables; ++i){
    v.push_back(make_pair(name, i+1));
    }
}
void fill_HVcables_RTN(std::vector<std::pair<std::string, Int_t>> &v){
    std::string name = "HV_RTN";
    for(int i=0; i<NumberHVRTNwires; ++i){
    v.push_back(make_pair(name, i+1));
    }
}
void fill_DRAINcables(std::vector<std::pair<std::string, Int_t>> &v){
    std::string name = "D";
    for(int i=0; i<NumberDrainWire; ++i){
    v.push_back(make_pair(name, i+1));
    }
}
void fill_Tsensors(std::vector<std::pair<std::string, Int_t>> &v){
    std::string name = "T";
    for(int i=0; i<NumberSensorWire; ++i){
    v.push_back(make_pair(name, i+1));
    }
}


// FindMax, FindMin --> automatic setting of histograms //
Float_t FindMax(TTree *tree, Int_t Cable, TString Option){
    Option.ToUpper();
    Float_t max;
    if(Option == "HV_CONT" || Option == "LV_CONT"){
        tree->GetEntry(0);
        max = resistenceCon;
        for(int i=0; i<tree->GetEntries(); i++){
            tree->GetEntry(i);
            if( Option == "HV_CONT" && resistenceCon > max && channelCon.find("HV")) max = resistenceCon;
            else if(Option == "LV_CONT" && resistenceCon > max && channelCon.find("HV")) max = resistenceCon;
        }
    }
    else if(Option == "HV_INS" || Option == "LV_INS"){
      tree->GetEntry(0);
      Float_t max = resistenceIns;
        for(int i=0; i<tree->GetEntries(); i++){
            tree->GetEntry(i);
            if( Option == "HV_INS" && resistenceIns > max && channelIns.find("HV")) max = resistenceIns;
            else if(Option == "LV_INS" && resistenceIns > max && channelIns.find("HV")) max = resistenceIns;
        }
    }
 return max;
}

Float_t FindMin(TTree *tree, Int_t Cable, TString Option){
    Option.ToUpper();
    Float_t min;
    if(Option == "HV_CONT" || Option == "LV_CONT"){
        tree->GetEntry(0);
        Float_t min = resistenceCon;
        for(int i=0; i<tree->GetEntries(); i++){
            tree->GetEntry(i);
            if( Option == "HV_CONT" && resistenceCon < min && channelCon.find("HV")) min = resistenceCon;
            else if(Option == "LV_CONT" && resistenceCon < min && channelCon.find("HV")) min = resistenceCon;
        }
    }
    else if(Option == "HV_INS" || Option == "LV_INS"){
      tree->GetEntry(0);
      Float_t min = resistenceIns;
        for(int i=0; i<tree->GetEntries(); i++){
            tree->GetEntry(i);
            if( Option == "HV_INS" && resistenceIns < min && channelIns.find("HV")) min = resistenceIns;
            else if(Option == "LV_INS" && resistenceIns < min && channelIns.find("HV")) min = resistenceIns;
        }
    }
 return min;
}



void start(){
/*
    std::cout << "  *******************************************" << std::endl;
    std::cout << "  *                                         *" << std::endl;
    std::cout << "  *           \033[1;32m CMS  Cables\033[0m                  *" << std::endl;
    std::cout << "  *          INFN  Sez.Perugia              *" << std::endl;
    std::cout << "  *                                         *" << std::endl;
    std::cout << "  *                                         *" << std::endl;
    std::cout << "  *******************************************" << std::endl;
    std::cout << "                                             " << std::endl;
    std::cout<< "-------------------------------------------- "<< std::endl; 
    */
    
     TDatime dt;
    int day = dt.GetDay();
    int year = dt.GetYear();
    int month = dt.GetMonth();
    int hour = dt.GetHour();
    int minute = dt.GetMinute();
    int second = dt.GetSecond();
    
    // Format the date and time string
    currentDate = Form("%02d_%02d_%04d %02d:%02d:%02d", day, month, year, hour, minute, second);    
    const std::string red = "\033[33m";
    const std::string green = "\033[92m";
    // ANSI escape code to reset the color
    const std::string reset = "\033[0m";
    
    const std::string textCMS =  R"(
************************************************************ 
*                                                          *
*  CMS Cables                   ÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆ          *
*  Sezione di Perugia    ÆÆÆÆÆÆÆÆ                ÆÆÆÆ      *
*                  ÆÆÆÆÆÆ                          ÆÆ      *
*              ÆÆÆÆÆ                                Æ      *
*            ÆÆÆÆÆ                                   Æ     * 
*        ÆÆÆÆÆ       ÆÆ   ÆÆ     Æ   ÆÆÆÆÆÆÆ  ÆÆÆ    ÆÆ    *
*     ÆÆÆÆ           ÆÆ   ÆÆÆ    Æ   ÆÆ       ÆÆÆÆ   ÆÆ    *
*   ÆÆÆÆ             ÆÆ   ÆÆ ÆÆ  Æ   ÆÆÆÆÆÆÆ  ÆÆ ÆÆ  ÆÆ    *
*  ÆÆÆÆÆ             ÆÆ   ÆÆ  ÆÆ Æ   ÆÆ       ÆÆ  ÆÆÆÆÆ    *
* ÆÆÆÆÆ              ÆÆ   ÆÆ   ÆÆÆ   ÆÆ       ÆÆ    ÆÆÆ    *
* ÆÆÆÆÆ              ÆÆ   ÆÆ    ÆÆ   ÆÆ       ÆÆ     ÆÆ    *
* ÆÆÆÆÆÆ                                                   *
*   ÆÆÆÆÆÆ  https://github.com/ldellape/CMScables   v.2.4  *   
*      ÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆ                                   *                                                         
* )"+currentDate + R"(                                      *
************************************************************ )";                               



std::cout << green << textCMS  << reset << std::endl;
std::cout<< green << "Test su cavo PS-PP1" << reset <<std::endl;

std::system("mkdir -p ./output/plots && mkdir -p ./output/plots/SingleCable && mkdir -p ./output/plots/CheckCable && mkdir -p ./output/report" );

}





#endif
