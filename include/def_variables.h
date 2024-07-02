// DECLARATION OF GLOBAL VARIABLES 

#ifndef DEF_VARIABLES_H
#define DEF_VARIABLES_H

#include "root.h"
#include <string>
#include <vector>

// input-output paths, to be change in the Makefile //
extern const std::string sInputTestDir;
extern const std::string sInputTimeAcquisition;
extern const std::string sOutputRoot;


extern Int_t IterationTest; // number of cable to be test
extern TString sPDFTitle; // final pdf name 
extern std::string currentDate; 

// vectors of paths to cable to be test //
extern std::vector<std::string> TestName;
extern std::vector<std::string> TestNameTimeAcquisition;
extern const int MaxCables;

extern const int NumberHVcables;
extern const int NumberLVcables;
extern const int NumberSensorWire;
extern const int NumberDrainWire;
extern const int NumberHVRTNwires;

extern const Float_t ThreshContLV;
extern const Float_t ThreshContHV;
extern const Float_t ThreshIsoLV;
extern const Float_t ThreshIsoHV;

// param to compute lenght of cables //
extern Float_t diamLV;
extern Float_t diamHV;
extern Float_t ResistivityLV;
extern Float_t ResistivityHV;

// control vairables //
extern Bool_t InsulationTest; 
extern Bool_t ContinuityTest;
extern Bool_t Ins_Time;
extern Int_t test_type;
extern Int_t CompareTest;
extern Bool_t CommandLine;

// I/O variables //
extern TTree *ContinuityTree;
extern TTree *InsulationTree;
extern Int_t statusCon, statusIns;
extern std::string channelIns, channelCon;
extern Float_t resistenceIns, resistenceCon;
extern Int_t cableIns, cableCon;
extern Float_t FieldIns;

// cables variables (needed??) //
extern std::vector<std::pair<std::string,Int_t>> HVcables;
extern std::vector<std::pair<std::string, Int_t>> LVcables; 
extern std::vector<std::pair<std::string,Int_t>> HVcables_rtn;
extern std::vector<std::pair<std::string, Int_t>> LVcables_rtn;
extern std::vector<std::pair<std::string,Int_t>> TSensor;
extern std::vector<std::pair<std::string,Int_t>> Drain;

// test parameters //
extern std::vector<std::tuple<double,double,double,double,double, std::string, double>> ParametersContinuity;
extern std::vector<std::tuple<double,double,double,double,double, double, double>> ParametersInsulationInitial;
extern std::vector<std::tuple<double,double,double>> ParametersInsulationLV;
extern std::vector<std::tuple<double,double,double,double>> ParametersInsulationHV;
extern std::vector<std::tuple<double,double,double,double>> ParametersInsulationTsensor;

extern TCanvas *c_plot;
extern std::vector<TCanvas*> c_graph;

extern std::vector<TH1F*> h_passedHV_Cont;
extern std::vector<TH1F*> h_passedLV_Cont;
extern std::vector<TH1F*> hCont_ResChannel_HV;
extern std::vector<TH1F*> hCont_ResChannel_LV; 
extern std::vector<TH1F*> h_passedCont_tot;
extern std::vector<TH1F*> hCont_ResHV;
extern std::vector<TH1F*> hCont_ResLV; 
extern std::vector<TH1F*> h_passedHV_Ins;
extern std::vector<TH1F*> h_passedLV_Ins;
extern std::vector<TH1F*> h_passedIns_tot;
extern std::vector<TH1F*> hIns_ResHV;
extern std::vector<TH1F*> hIns_ResLV;
extern std::vector<TH1F*> hIns_ResTot;
extern std::vector<TH1F*> hIns_ResChannel_LV;
extern std::vector<TH1F*> hIns_ResChannel_HV;
extern float OverThreshLV[];
extern float OverThreshHV[];
extern float BelowThreshHV[];
extern float BelowThreshLV[];
extern Float_t ResMaxHV_ins[];
extern Float_t ResMaxLV_ins[];
extern Float_t ResMinHV_ins[];
extern Float_t ResMinLV_ins[];
extern Float_t ResMaxHV_cont[];
extern Float_t ResMaxLV_cont[];
extern Float_t ResMinHV_cont[];
extern Float_t ResMinLV_cont[];
extern const char *labelsContHV[];
extern const char *labelsContLV[];
extern const char *labelsInsHV[];
extern const char *labelsInsLV[];

extern std::vector<std::pair<std::string, TGraph*>> grRes_Time;


#endif // DEF_VARIABLES_H
