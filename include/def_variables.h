#ifndef DEF_VARIABLES_H
#define DEF_VARIABLES_H
#include "root.h"

Int_t IterationTest;
TString sPDFTitle = "Report";
std::string currentDate;

std::vector<std::string> TestName;

const int MaxCables = 200;
// directory where Test Output are stored //
const std::string sInputTestDir = "./input/FULL_TEST_su_cavo_ps_pp1_V3/";
const std::string sInputTimeAcquisition = (sInputTestDir + "/VALORI/").c_str();
// output root file //
const std::string sOutputRoot="./output/rootFiles/";


// Number of different type cables //
const int NumberHVcables = 12;
const int NumberLVcables = 26;
const int NumberSensorWire = 4;
const int NumberDrainWire = 4;
const int NumberHVRTNwires = 3;

//******* Threshold per test continuità LV, HV **********//
const Float_t ThreshContLV = 0.65;
const Float_t ThreshContHV = 13;

//********* Threshold per test isolamento LV, HV ********//
const Float_t ThreshIsoLV = 10000; //MOhm
const Float_t ThreshIsoHV= 1e+9; //GOhm

//********** Sezione e Resistività ***********//ù
Float_t diamLV = 0.0012;
Float_t diamHV = 0.00038;
Float_t ResistivityLV = 1.724*TMath::Power(10,-8);
Float_t ResistivityHV = 1.724*TMath::Power(10,-8);

//******Control variables******//
Bool_t InsulationTest; 
Bool_t ContinuityTest;
Bool_t Ins_Time;
Int_t test_type; // continuity, insulation, both
Int_t CompareTest; // confronto tra cavi o no //

//*****trees per output misure******//
TTree *ContinuityTree;
TTree *InsulationTree;
Int_t statusCon, statusIns; // passed = 1, failed = 0 //
std::string channelIns, channelCon; // LV, HV, Tsensor ecc. 
Float_t resistenceIns, resistenceCon; // resistenza misurata
Int_t cableIns, cableCon; // numero del cavo, se CompareTest != 0
Float_t FieldIns;

//*****servono veramente?******//
std::vector<std::pair<std::string,Int_t>> HVcables;
std::vector<std::pair<std::string, Int_t>> LVcables; 
std::vector<std::pair<std::string,Int_t>> HVcables_rtn;
std::vector<std::pair<std::string, Int_t>> LVcables_rtn;
std::vector<std::pair<std::string,Int_t>> TSensor;
std::vector<std::pair<std::string,Int_t>> Drain;
//std::vecotr<std::pair<std::string,Int_t> PCI; // probe cable insertion //


//Test Parameters //
std::vector<std::tuple<double,double,double,double,double, std::string, double>> ParametersContinuity;
std::vector<std::tuple<double,double,double,double,double, std::string, double>> ParametersInsulation;

TCanvas *c_plot;

// histos for Isolation Test (log scale)//
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
 float OverThreshHV[100] = {0};
 float OverThreshLV[100] = {0};
 float BelowThreshHV[100] = {0};
 float BelowThreshLV[100] = {0};
 TGraph *grRes_Time[NumberLVcables];
 Float_t ResMaxHV_ins[MaxCables], ResMaxLV_ins[MaxCables];
 Float_t ResMinHV_ins[MaxCables], ResMinLV_ins[MaxCables];
 Float_t ResMaxHV_cont[MaxCables], ResMaxLV_cont[MaxCables];
 Float_t ResMinHV_cont[MaxCables], ResMinLV_cont[MaxCables];
 const char *labelsContHV[NumberHVcables + NumberHVRTNwires + NumberSensorWire];
 const char *labelsContLV[NumberLVcables];
 const char *labelsInsHV[NumberHVcables + NumberHVRTNwires + NumberSensorWire];
 const char *labelsInsLV[NumberLVcables];

std::vector<TCanvas*> CanvasPlots;
std::vector<TCanvas*> IsolationPlots(MaxCables*4, nullptr);




// histo insulation resistance contro tempo ( file .ini), per tutti i cavi //

#endif
