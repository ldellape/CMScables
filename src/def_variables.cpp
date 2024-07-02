// inizialization of global variables //
#include "../include/def_variables.h"

Int_t IterationTest;
TString sPDFTitle = "Report";
std::string currentDate;

std::vector<std::string> TestName;
std::vector<std::string> TestNameTimeAcquisition;

const int MaxCables = 200;
const std::string sInputTestDir = std::string(WORKDIR) + "/input/FULL_TEST_su_cavo_ps_pp1_V3/";
const std::string sInputTimeAcquisition = std::string(WORKDIR) + (sInputTestDir + "/VALORI/").c_str();
const std::string sOutputRoot= std::string(WORKDIR) + "/output/rootFiles/";

const int NumberHVcables = 12;
const int NumberLVcables = 26;
const int NumberSensorWire = 4;
const int NumberDrainWire = 4;
const int NumberHVRTNwires = 3;

const Float_t ThreshContLV = 0.65;
const Float_t ThreshContHV = 13;
const Float_t ThreshIsoLV = 10000;
const Float_t ThreshIsoHV= 1e+9;

Float_t diamLV = 0.0012;
Float_t diamHV = 0.00038;
Float_t ResistivityLV = 1.724*TMath::Power(10,-8);
Float_t ResistivityHV = 1.724*TMath::Power(10,-8);

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

std::vector<std::tuple<double,double,double,double,double, std::string, double>> ParametersContinuity;
std::vector<std::tuple<double,double,double,double,double, double, double>> ParametersInsulationInitial;
std::vector<std::tuple<double,double,double>> ParametersInsulationLV;
std::vector<std::tuple<double,double,double,double>> ParametersInsulationHV;
std::vector<std::tuple<double,double,double,double>> ParametersInsulationTsensor;

TCanvas *c_plot;
std::vector<TCanvas*> c_graph(100, nullptr);

std::vector<TH1F*> h_passedHV_Cont(MaxCables, nullptr);
std::vector<TH1F*> h_passedLV_Cont(MaxCables, nullptr);
std::vector<TH1F*> hCont_ResChannel_HV(MaxCables, nullptr);
std::vector<TH1F*> hCont_ResChannel_LV(MaxCables, nullptr); 
std::vector<TH1F*> h_passedCont_tot(MaxCables, nullptr);
std::vector<TH1F*> hCont_ResHV(MaxCables, nullptr);
std::vector<TH1F*> hCont_ResLV(MaxCables, nullptr); 
std::vector<TH1F*> h_passedHV_Ins(MaxCables, nullptr);
std::vector<TH1F*> h_passedLV_Ins(MaxCables, nullptr);
std::vector<TH1F*> h_passedIns_tot(MaxCables, nullptr);
std::vector<TH1F*> hIns_ResHV(MaxCables, nullptr);
std::vector<TH1F*> hIns_ResLV(MaxCables, nullptr);
std::vector<TH1F*> hIns_ResTot(MaxCables, nullptr);
std::vector<TH1F*> hIns_ResChannel_LV(MaxCables, nullptr);
std::vector<TH1F*> hIns_ResChannel_HV(MaxCables, nullptr);

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
const char *labelsContHV[NumberHVcables + NumberHVRTNwires + NumberSensorWire];
const char *labelsContLV[NumberLVcables];
const char *labelsInsHV[NumberHVcables + NumberHVRTNwires + NumberSensorWire];
const char *labelsInsLV[NumberLVcables];

 std::vector<std::pair<std::string, TGraph*>> grRes_Time;


