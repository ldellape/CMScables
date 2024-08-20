// DECLARATION OF GLOBAL VARIABLES 

#ifndef DEF_VARIABLES_H
#define DEF_VARIABLES_H

#include "root.h"
#include <string>
#include <vector>

namespace Continuity{
    class PSPP1;
    class Octopus;
}
namespace Isolation{
    class PSPP1;
    class Octopus;
}

// declare in Makefile //
#ifdef WORKDIR
#else
#define WORKDIR "."
#endif

extern std::vector<Continuity::PSPP1*> TestContinuityPSPP1;
extern std::vector<Isolation::PSPP1*> TestIsolationPSPP1;

// input-output paths //
extern const std::string sInputTestDir;
extern const std::string sInputTimeAcquisition;
extern const std::string sOutputRoot;

extern Int_t IterationTest; // number of cable to be test
extern std::string sPDFTitle; // final pdf name 
extern std::string currentDate; // date of execution

// vectors of paths to cable to be test //
extern std::vector<std::string> TestName;
extern std::vector<std::string> TestNameTimeAcquisition;
extern std::vector<std::string> TestPath;
extern const int MaxCables;

extern const int NumberHVcables;
extern const int NumberLVcables;

// treshold values //
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

// cables variables (needed??) //
extern std::vector<std::pair<std::string,Int_t>> HVcables;
extern std::vector<std::pair<std::string, Int_t>> LVcables; 
extern std::vector<std::pair<std::string,Int_t>> HVcables_rtn;
extern std::vector<std::pair<std::string, Int_t>> LVcables_rtn;
extern std::vector<std::pair<std::string,Int_t>> TSensor;
extern std::vector<std::pair<std::string,Int_t>> Drain;

// test parameters //
extern std::vector<std::tuple<double,double,double,double,double, std::string>> ParametersContinuity;
extern std::vector<std::tuple<double,double,double,double,double, double, double>> ParametersInsulationInitial;
extern std::vector<std::tuple<double,double,double, double>> ParametersInsulationLV;
extern std::vector<std::tuple<double,double,double,double>> ParametersInsulationHV;
extern std::vector<std::tuple<double,double,double,double>> ParametersInsulationTsensor;

// canvas for report and report resistence vs time //
extern TCanvas *c_plot;
extern TCanvas *c_graph;

// histograms and histograms variables for range, maximum/minimum values to be stored 
extern std::vector<TH1I*> h_passedHV_Cont;
extern std::vector<TH1I*> h_passedLV_Cont;
extern std::vector<TH1F*> hCont_ResChannel_HV;
extern std::vector<TH1F*> hCont_ResChannel_LV; 
extern std::vector<TH1I*> h_passedCont_tot;
extern std::vector<TH1F*> hCont_ResHV;
extern std::vector<TH1F*> hCont_ResLV; 
extern std::vector<TH1I*> h_passedHV_Ins;
extern std::vector<TH1I*> h_passedLV_Ins;
extern std::vector<TH1I*> h_passedIns_tot;
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

extern const char *labelLV_con[];
extern const char *labelHV_con[];
extern const char *labelLV_iso[];
extern const char *labelHV_iso[];





#endif // DEF_VARIABLES_H
