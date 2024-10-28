#ifndef ISOLATION_CLASS_H
#define ISOLATION_CLASS_H

#include <vector>
#include <tuple>
#include <string>
#include <array>
#include <map>
#include "root.h"
#include "TError.h"
#include "def_variables.h"

namespace Isolation{
 class Cable{
    public:
        Cable();
        virtual ~Cable(){}
        void ReadTestOutput(std::vector<TString> &TestNameFile, int j);
        TString FindCableType(TString TestNameFile);
    };



 class PSPP1 : public Cable{
  private: 
    TString CableName;
    std::vector<bool> status;
    std::vector<::TString> channel;
    std::vector<double> resistence;
    std::string TestPath;
    std::tuple<double,double,double,double,double, std::string, double, std::string, double, double> InitialParameters;
    std::vector<double> FieldB;
    std::vector<double> IsolationParLV;
    std::vector<double> IsolationParHV;
    std::vector<double> IsolationParTsensor;
    std::pair<double,double> FindMaxMinResistence(::TString Option = "all");
    
    Float_t ThreshIsoLV = 10000;
    Float_t ThreshIsoHV = 1e+9;
 public:
    PSPP1();
    PSPP1(std::vector<std::tuple<bool, std::string, double, double>> &TestOutput, ::TString TestTitle);
    void SetChannels(std::vector<::TString> &channelNames);
    void SetPath(std::string path);
    void SetStatus(std::vector<Bool_t> &statusChannels);
    void SetResistence(std::vector<double> &resistenceChannel);
    void SetField(std::vector<double> &B);
    void SetName(::TString name);
    void SetInitialParameters(std::tuple<double,double,double,double,double, std::string, double, std::string, double, double> param);
    void SetThreshold(Float_t thresh, ::TString option);
    void SetIsolationPar(::TString option, std::vector<double> &pars);
    TString GetName();
    std::vector<double> GetResistence(::TString option= "all");
    std::vector<Bool_t> GetStatus(::TString option = "all");
    std::vector<double> GetFieldB(::TString option = "all");
    std::vector<double> GetIsolationPar(TString option);
    std::string GetPath();
    Double_t GetStdDev(::TString option );
    Double_t GetStdDev(TH1F *h);
    Double_t GetMean(TH1F *h);
    Double_t GetMean(::TString option = "all");
    Double_t GetThreshold(::TString option);
    TString GetPathTimeRes();
    std::tuple<double,double,double,double,double, std::string, double, std::string, double, double> GetInitialParameters();
    TH1F* FillResistenceChannelHistogram(::TString title, ::TString option= "all");
    TH1I* FillStatusHistogram(::TString title, ::TString option = "all");
    TH1F* FillResistenceHistogram(::TString title, ::TString option= "all");
    std::vector<std::pair<std::string, TGraph*>> FillGraphTimeResistence(TString option);
    //////////////////////////////////////////////////////////////////////
    // template methods 
    template <typename T> std::vector<T> FilterChannel(TString option, TString vector);
    template<typename histo> histo* Add(histo *h, ::TString option, PSPP1 *cable1, Float_t c1 = 1);
 };


//////////////////////////////////////////////////////////////////////
// sum/difference operation between two cables
 template<typename histo> 
 histo* PSPP1::Add(histo *h, ::TString option, PSPP1 *cable1, Float_t c1){
    option.ToUpper();
    histo *Histo2;
    if(std::is_same<histo, TH1F>::value){
     Histo2 = (histo*) cable1->FillResistenceChannelHistogram(option);
     h->histo::Add(Histo2, c1);
     return *this;
    }
    else if(std::is_same<histo, TH1I>::value){
     Histo2 = (histo*) cable1->FillStatusHistogram(option);
      h->histo::Add(Histo2, c1);
      return *this;
    }
    else{
     Error("Isolation::PSPP1::Add", "histograms are of different type");
     return h;
    }
}

//////////////////////////////////////////////////////////////////////
// Filter entries base on HV or LV 
template<typename T> std::vector<T> PSPP1::FilterChannel(::TString option, ::TString vector) {
 std::vector<T> filtered;
 if(vector== "resistence"){
    for(int i=0; i< int(resistence.size()); i++){
        if((channel[i].Contains("LV") || channel[i].Contains("PH")) && option == "LV"){
            filtered.push_back(resistence[i]);
        }
        else if((channel[i].Contains("HV") || channel[i].Contains("Tsensor")) && option =="HV"){
            filtered.push_back(resistence[i]);
        }
    }
  return filtered;
 }
 else if(vector == "status"){
     for(int i=0; i< int(status.size()); i++){
        if((channel[i].Contains("LV") || channel[i].Contains("PH")) && option == "LV"){
            filtered.push_back(status[i]);
        }
        else if((channel[i].Contains("HV") || channel[i].Contains("Tsensor")) && option =="HV"){
            filtered.push_back(status[i]);
        }
    }
  return filtered;
 }
}

class OCTOPUS : public Cable{
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
}

#endif