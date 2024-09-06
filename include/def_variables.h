// DECLARATION OF GLOBAL VARIABLES 

#ifndef DEF_VARIABLES_H
#define DEF_VARIABLES_H

#include "root.h"
#include <string>
#include <vector>

// defined in Makefile, absolute path//
#ifdef WORKDIR
#else
#define WORKDIR "."
#endif


// ************************************************************** //
// ******classes and objects of the classes declaration********** //
namespace Continuity{
    class PSPP1;
    class Octopus;
}
namespace Isolation{
    class PSPP1;
    class Octopus;
}

extern std::vector<Continuity::PSPP1*> TestContinuityPSPP1;
extern std::vector<Isolation::PSPP1*> TestIsolationPSPP1;
// ************************************************************** //
// ************************************************************** //


// ************************************************************** //
// ************ input-output paths variables ******************** //
extern const std::string sInputTestDir;
extern const std::string sInputTimeAcquisition;
extern const std::string sOutputRoot; 
extern std::vector<std::string> TestName; // path to original txt file from CEETIS 
extern std::vector<std::string> TestPath; // path to modified txt file from CEETIS
extern std::vector<std::string> TestNameTimeAcquisition; // paths to .INI files from CEETIS 
extern std::string currentDate; // date of execution
extern std::string sPDFTitle; // final pdf name (name of the test + currentDaate)
// ************************************************************** //
// ************************************************************** //



extern Int_t IterationTest; // number of cable to be test


extern const int MaxCables;


// ************************************************************** //
// ***********parameters of tests******************************** //
extern const int NumberHVcables;
extern const int NumberLVcables;
extern const Float_t ThreshContLV;
extern const Float_t ThreshContHV;
extern const Float_t ThreshIsoLV;
extern const Float_t ThreshIsoHV;
extern Float_t diamLV;
extern Float_t diamHV;
extern Float_t ResistivityLV;
extern Float_t ResistivityHV;
// ************************************************************** //
// ************************************************************** //

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
