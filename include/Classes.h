#ifndef CLASSES_H
#define CLASSES_H

#include "root.h"
#include "def_variables.h"

/////////////////////////////////////////////////////
////////////// PSPP1 CLASS //////////////////////////
////////////////////////////////////////////////////

  class PSPP1{
  private:
    TString CableName;
    TString testPath;
    std::vector<Bool_t> status;
    std::vector<::TString> channel;
    std::vector<double> resistence;
    std::vector<double> FieldB;
    TString testType; // isolation or continuity
    // test parameters //
    std::tuple<double,double,double,double,double, TString, double> Parameters;
    std::tuple<double,double,double,double,double, TString, double,TString, double, double> InitialParameters;
    std::vector<double> IsolationParLV;
    std::vector<double> IsolationParHV;
    std::vector<double> IsolationParTsensor;
    // cable parameters //
    const double diamLV = 0.0012;
    const double diamHV = 0.00038;
    Float_t ThreshContLV = 0.65;
    Float_t ThreshContHV = 13;
    Float_t ThreshIsoLV = 10000;
    Float_t ThreshIsoHV = 1e+9;
    const double ResistivityLV = 1.724*TMath::Power(10, -8);
    const double ResistivityHV = 1.724*TMath::Power(10,-8);
    std::pair<double,double> FindMaxMinResistence(::TString option);
  public:
    PSPP1();
    PSPP1(TString testType, std::vector<std::tuple<Bool_t, TString, double>> &TestOutput, TString TestTitle);
    void SetPath(TString path);
    void SetChannels(std::vector<::TString> &channelNames);
    void SetStatus(std::vector<Bool_t> &statusChannels);
    void SetResistence(std::vector<double> &resistenceChannel);
    void SetName(::TString name);
    void SetParameters(std::tuple<double,double,double,double,double,TString,double> param); 
    void SetInitialParameters(std::tuple<double,double,double,double,double, TString, double, TString, double, double> param);
    void SetIsolationPar(::TString option, std::vector<double> &pars);
    void SetThreshold(::TString option, Float_t Thresh);
    void SetField(std::vector<double> &B);
    void SetTestType(TString test);
    ::TString GetName();
    std::vector<double> GetResistence(::TString option= "all");
    std::vector<Bool_t> GetStatus(::TString = "all");
    Double_t GetMean(::TString option = "all");
    Double_t GetMean(TH1F *h);
    Double_t GetLenght(::TString option);
    Double_t GetLenght(TH1F* h);
    Double_t GetStdDev(::TString option);
    Double_t GetStdDev(TH1F *h);
    Double_t GetThreshold(::TString option);
    std::vector<double> GetIsolationPar(TString option);
    TString GetPath();
    std::vector<std::pair<TString, double>> GetOverThreshold(::TString option);
    std::tuple<double,double,double,double,double, TString, double,TString, double, double> GetInitialParameters();
    TString GetPathTimeRes();
    double GetParameter(int param);
    std::tuple<double,double,double,double,double, TString, double> GetParameters();
    TH1F* FillResistenceChannelHistogram(::TString title, ::TString option);
    TH1F* FillResistenceHistogram(::TString option= "all");
    TH1I* FillStatusHistogram(::TString title, ::TString option = "all");
    template <typename T> std::vector<T> FilterChannel(::TString option, ::TString vector);
    std::vector<std::pair<std::string, TGraph*>> FillGraphTimeResistence(TString option);
 };
 /////////////////////////////////////////////////////////////////////
// filter entries based on channel option
template <typename T> std::vector<T> PSPP1::FilterChannel(TString option, TString vector) {
 std::vector<T> filtered;
 if(vector == "RESISTENCE"){
    for(int i=0; i < int(resistence.size()); i++){
        if((channel[i].Contains("LV") || channel[i].Contains("PH")) && option == "LV"){
            filtered.push_back(resistence[i]);
        }
        else if((channel[i].Contains("HV") || channel[i].Contains("Tsensor")) && option =="HV"){
            filtered.push_back(resistence[i]);
        }
    }
 }
 else if(vector == "STATUS"){
     for(int i=0; i< int(status.size()); i++){
        if((channel[i].Contains("LV") || channel[i].Contains("PH")) && option == "LV"){
            filtered.push_back(status[i]);
        }
        else if((channel[i].Contains("HV") || channel[i].Contains("Tsensor")) && option =="HV"){
            filtered.push_back(status[i]);
        }
     }
 }
 else{
    Fatal("Continuity::PSPP1::FilterChannel", "invalid vector name : resistence or status");
    return filtered;
 }
 return filtered;
}


 class OCTOPUS{
    private:
        TString CableName;
        std::vector<Bool_t> status;
        std::string TestPath;
        Float_t Temperature;
        Float_t Humidity;
        std::vector<double> resistence;
        std::vector<TString> channel;
        std::vector<std::pair<TString, double>> LenghtChannelTotal;
        std::vector<std::pair<TString, double>> LenghtChannelBundle;
        std::vector<std::pair<TString, double>> LenghtChannelBranch;
        std::pair<double, double> FindMaxMinResistence(::TString option);
        const double MainLenghts = 1000;
        std::vector<std::tuple<TString, std::vector<TString>, double, double>> OCTOPUSmodules;
        void FillModulesParameter(){
          std::vector<TString> TsensorChannel = {"PH", "PH_RTN"};
          std::vector<TString> PreHeaterChannel = {"Tsens1", "Tsens2"};
          for(int i=0; i<14; i++){
            std::vector<TString> BranchChannel = {"HV" + std::to_string(i+1), "", "LV_RTN" + std::to_string(i+1), "LV1" + std::to_string(i+1)};
            OCTOPUSmodules.emplace_back(std::make_tuple("1", BranchChannel, 1042, 58));
            OCTOPUSmodules.emplace_back(std::make_tuple("2", BranchChannel, 943, 58));
            OCTOPUSmodules.emplace_back(std::make_tuple("3", BranchChannel, 820, 58));
            OCTOPUSmodules.emplace_back(std::make_tuple("4", BranchChannel, 744, 58));
            OCTOPUSmodules.emplace_back(std::make_tuple("5", BranchChannel, 623, 58));
            OCTOPUSmodules.emplace_back(std::make_tuple("6", BranchChannel, 546, 58));
            OCTOPUSmodules.emplace_back(std::make_tuple("7", BranchChannel, 428, 58));
            OCTOPUSmodules.emplace_back(std::make_tuple("8", BranchChannel, 352, 58));
            OCTOPUSmodules.emplace_back(std::make_tuple("9", BranchChannel, 234, 58));
            OCTOPUSmodules.emplace_back(std::make_tuple("10", BranchChannel, 156, 58));
            OCTOPUSmodules.emplace_back(std::make_tuple("11", BranchChannel, 20, 37));
            OCTOPUSmodules.emplace_back(std::make_tuple("12", BranchChannel, 32, 152));
          }
          OCTOPUSmodules.emplace_back(std::make_tuple("Tsensor", TsensorChannel, 32, 145));
          OCTOPUSmodules.emplace_back(std::make_tuple("Pre-Heater", PreHeaterChannel, 32, 145));        
        }
    public: 
        OCTOPUS(); 
        OCTOPUS(std::vector<std::tuple<Bool_t, std::string, double>> &TestOutput, ::TString TestTitle);
        void SetPath(std::string path);
        void SetName(::TString name);
        void SetTemperature(Float_t T);
        void SetHumidity(Float_t H);
        void SetResistence(std::vector<double> &resistenceChannels);

};





#endif