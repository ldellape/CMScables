// inizialization of global variables //
#include "../include/def_variables.h"

Int_t IterationTest;
std::string sPDFTitle;
std::string currentDate;
const Float_t ThreshIsoLV = 10000;
const Float_t ThreshIsoHV = 1e+9;
const Float_t ThreshContLV = 0.65;
const Float_t ThreshContHV = 13;
std::vector<std::string> TestName;
std::vector<std::string> TestNameTimeAcquisition;
std::vector<std::string> TestPath;

const int MaxCables = 200;
const std::string sInputTestDir = std::string(WORKDIR) + "/input/FULL_TEST_su_cavo_ps_pp1_V3/";
const std::string sInputTimeAcquisition = std::string(WORKDIR) + (sInputTestDir + "/VALORI/").c_str();
const std::string sOutputRoot= std::string(WORKDIR) + "/output/rootFiles/";


const int NumberSensorWire = 4;
const int NumberHVcables = 19;
const int NumberLVcables = 25;


Bool_t InsulationTest = false; 
Bool_t ContinuityTest = false;
Bool_t CommandLine = false;
Bool_t Ins_Time = false;
Int_t test_type = 0;
Int_t CompareTest;

TTree *ContinuityTree;
TTree *InsulationTree;
Int_t statusCon, statusIns;
std::string channelIns, channelCon;
Float_t resistenceIns, resistenceCon;
Int_t cableIns, cableCon;
Float_t FieldIns;

std::vector<std::pair<std::string,Int_t>> HVcables;
std::vector<std::pair<std::string, Int_t>> LVcables; 
std::vector<std::pair<std::string,Int_t>> HVcables_rtn;
std::vector<std::pair<std::string, Int_t>> LVcables_rtn;
std::vector<std::pair<std::string,Int_t>> TSensor;
std::vector<std::pair<std::string,Int_t>> Drain;

std::vector<std::tuple<double,double,double,double,double, std::string>> ParametersContinuity;
std::vector<std::tuple<double,double,double,double,double, double, double>> ParametersInsulationInitial;
std::vector<std::tuple<double,double,double, double>> ParametersInsulationLV;
std::vector<std::tuple<double,double,double,double>> ParametersInsulationHV;
std::vector<std::tuple<double,double,double,double>> ParametersInsulationTsensor;

TCanvas *c_plot;
TCanvas *c_graph;

std::vector<TH1I*> h_passedHV_Cont(MaxCables, nullptr);
std::vector<TH1I*> h_passedLV_Cont(MaxCables, nullptr);
std::vector<TH1F*> hCont_ResChannel_HV(MaxCables, nullptr);
std::vector<TH1F*> hCont_ResChannel_LV(MaxCables, nullptr); 
std::vector<TH1I*> h_passedCont_tot(MaxCables, nullptr);
std::vector<TH1F*> hCont_ResHV(MaxCables, nullptr);
std::vector<TH1F*> hCont_ResLV(MaxCables, nullptr); 
std::vector<TH1I*> h_passedHV_Ins(MaxCables, nullptr);
std::vector<TH1I*> h_passedLV_Ins(MaxCables, nullptr);
std::vector<TH1I*> h_passedIns_tot(MaxCables, nullptr);
std::vector<TH1F*> hIns_ResHV(MaxCables, nullptr);
std::vector<TH1F*> hIns_ResLV(MaxCables, nullptr);
std::vector<TH1F*> hIns_ResTot(MaxCables, nullptr);
std::vector<TH1F*> hIns_ResChannel_LV(MaxCables, nullptr);
std::vector<TH1F*> hIns_ResChannel_HV(MaxCables, nullptr);
std::vector<Continuity::PSPP1*> TestContinuityPSPP1(100, nullptr);
std::vector<Isolation::PSPP1*> TestIsolationPSPP1(100, nullptr);

float OverThreshHV[100] = {0};
float OverThreshLV[100] = {0};
float BelowThreshLV[100] = {0};
float BelowThreshHV[100] = {0};

Float_t ResMaxHV_ins[MaxCables];
Float_t ResMaxLV_ins[MaxCables];
Float_t ResMinHV_ins[MaxCables];
Float_t ResMinLV_ins[MaxCables];
Float_t ResMaxHV_cont[MaxCables];
Float_t ResMaxLV_cont[MaxCables];
Float_t ResMinHV_cont[MaxCables];
Float_t ResMinLV_cont[MaxCables];

const char *labelLV_iso[] = {
    "LV1", "LVR1", 
    "LV2", "LVR3", 
    "LV4", "LVR4", 
    "LV5", "LVR5", 
    "LV6", "LVR6", 
    "LVR7", 
    "LV8", "LVR8", 
    "LVR9", 
    "LV10", "LVR10", 
    "LV11", "LVR11", 
    "LV12", "LVR12", 
    "PH", "PHR"
};
const char *labelHV_iso[] = {
    "Tsensor1", "Tsensor2", "Tsensor3", "Tsensor4", 
    "HV1", "HV2", "HV3", "HV4", 
    "HV5", "HV6", "HV7", "HV8", 
    "HV9", "HV10", "HV11", "HV12", 
    "HVR1", "HVR2", "HVR3", 
};
const char *labelLV_con[] = {"LV1", "LV1R", 
    "LV2", "LV2R", 
    "LV3", "LV3R", 
    "LV4", "LV4R", 
    "LV5", "LV5R", 
    "LV6", "LV6R", 
    "LV7", "LV7R", 
    "LV8", "LV8R", 
    "LV9R", 
    "LV10", "LV10R", 
    "LV11", "LV11R", 
    "LV12", "LV12R", 
    "PH", "PHR"
};
const char *labelHV_con[] =  {
    "Tsensor1", "Tsensor2", "Tsensor3", "Tsensor4", 
    "H1", "H2", "H3", "H4", 
    "HR1", "H5", "H6", "H7", 
    "H8", "HR2", "H9", 
    "H10", "H11", "H12", 
    "HR3"
};
 


