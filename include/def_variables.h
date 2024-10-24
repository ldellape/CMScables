#ifndef DEF_VARIABLES_H
#define DEF_VARIABLES_H

#include "root.h"
#include <string>
#include <vector>

//////////////////////////////////////////////////
// defined in Makefile
#ifdef WORKDIR
#else
#define WORKDIR "."
#endif


//////////////////////////////////////////////////
// classes and objects of the classes
namespace Continuity{ class PSPP1; class OCTOPUS; class PP0;}
namespace Isolation {  class PSPP1; class OCTOPUS; class PP0;}
extern std::vector<Continuity::PSPP1*> TestContinuityPSPP1;
extern std::vector<Isolation::PSPP1*> TestIsolationPSPP1;
extern std::vector<Continuity::OCTOPUS*> TestContinuityOctopus;
extern std::vector<Isolation::OCTOPUS*> TestIsolationOctopus;
extern std::vector<Continuity::PP0*> TestContinuityPP0;
extern std::vector<Isolation::PP0*> TestIsolationPP0;
//////////////////////////////////////////////////


//////////////////////////////////////////////////
// control variables 
extern Bool_t InsulationTest; 
extern Bool_t ContinuityTest;
extern Bool_t Ins_Time;
extern Bool_t CommandLine;
extern Bool_t PSPP1test;
extern Bool_t OCTOPUStest;
extern Bool_t PP0test;
extern Bool_t CHAINtest;
//////////////////////////////////////////////////


//////////////////////////////////////////////////
// input-output paths variables 
extern const std::string sInputTestDir;
extern const std::string sInputTimeAcquisition;
extern const std::string sOutputRoot; 
extern std::vector<std::string> TestName; // path to original txt file from CEETIS 
extern std::vector<std::string> TestPath; // path to temporaly txt file from CEETIS
extern std::vector<std::string> TestNameTimeAcquisition; // paths to .INI files from CEETIS 
extern std::string currentDate; // date of execution
extern std::string sPDFTitle; // final pdf name (name of the test + currentDaate)
//////////////////////////////////////////////////


//////////////////////////////////////////////////
extern Int_t IterationTest; // number of processed test (needed ??)
extern const int MaxCables;
extern const int NumberHVcables;
extern const int NumberLVcables;
//////////////////////////////////////////////////


//////////////////////////////////////////////////
// canvases for final plots
extern TCanvas *c_plot;
extern TCanvas *c_graph;
//////////////////////////////////////////////////


//////////////////////////////////////////////////
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
//////////////////////////////////////////////////


//////////////////////////////////////////////////
// channel labels
extern const char *labelLV_con[];
extern const char *labelHV_con[];
extern const char *labelLV_iso[];
extern const char *labelHV_iso[];
//////////////////////////////////////////////////





#endif // DEF_VARIABLES_H
