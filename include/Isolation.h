#ifndef ISOLATION_CLASS_H
#define ISOLATION_CLASS_H

#include <vector>
#include <tuple>
#include <string>
#include "root.h"
#include "TError.h"
#include "def_variables.h"

namespace Isolation{
 class PSPP1{
  private: 
    std::pair<double,double> FindMaxMinResistence(::TString Option = "all");
    std::string CableName;
    std::vector<bool> status;
    std::vector<::TString> channel;
    std::vector<double> resistence;
    std::vector<double> InitialParameters;
    std::vector<double> FieldB;
    std::tuple<double,double,double, double> IsolationParLV;
    std::tuple<double,double,double,double> IsolationParHV;
    std::tuple<double,double,double,double> IsolationParTsensor;
 public:
    PSPP1();
    PSPP1(std::vector<std::tuple<bool, std::string, double, double>> &TestOutput, ::TString TestName);
    void SetChannels(std::vector<::TString> &channelNames);
    void SetStatus(std::vector<Bool_t> &statusChannels);
    void SetResistence(std::vector<double> &resistenceChannel);
    void SetField(std::vector<double> &B);
    void SetName(::TString name);
    void SetInitialParameters(std::tuple<double,double,double,double,double,double,double> param);
    TString GetName();
    std::vector<double> GetResistence(::TString option= "all");
    std::vector<Bool_t> GetStatus(::TString option = "all");
    std::vector<double> GetFieldB(::TString option = "all");
    template <typename T> std::vector<T> FilterChannel(::TString option, ::TString vector);
    //Float_t GetStdDev(::TString option = "all");
    Float_t GetStdDev(TH1F *h);
    Float_t GetMean(TH1F *h);
    Float_t GetMean(::TString option = "all");
    std::vector<double> GetInitialParameters();
    TH1F* FillResistenceChannelHistogram(::TString title, ::TString option= "all");
    TH1I* FillStatusHistogram(::TString title, ::TString option = "all");
    TH1F* FillResistenceHistogram(::TString title, ::TString option= "all");

    // *************************************************** //
    // ---> template method, implemented in this file //
    template<typename... TT> std::tuple<TT ...> GetIsolationPar(TString option); 
    template<typename... TT> void SetIsolationPar(::TString option, std::tuple<TT...> pars);
    template<typename histo> histo* Add(::TString option, PSPP1 *cable1, Float_t c1 = 1);
 };

//////////////////////////////////////////////////////////////////////
// set isolation parameters 
template<typename... TT> void PSPP1::SetIsolationPar(::TString option, std::tuple<TT ...> pars){
    option.ToUpper();
    if(option == "LV"){
        IsolationParLV = pars;
    }
    else if(option == "HV"){
        IsolationParHV = pars;
    }
    else if(option == "TSENSOR" || option=="TSENSORS"){
        IsolationParTsensor = pars;
    }
    else{
        Error("Isolation::PSPP1::SetIsolationPar", "invalid option");
        return;
    }
}

//////////////////////////////////////////////////////////////////////
// get parameters of isolation test for LV,HV and Tsensor (not the initial parameters)
template<typename...TT> std::tuple<TT...>  PSPP1::GetIsolationPar(TString option){
    option.ToUpper();
    if(option == "LV"){
        return IsolationParLV;
    }
    else if(option == "HV"){
        return IsolationParHV;
    }
    else if (option == "TSENSOR"){
        return IsolationParTsensor;
    }
}

//////////////////////////////////////////////////////////////////////
// sum/difference operation between two cables
 template<typename histo> histo* PSPP1::Add(::TString option,PSPP1 *cable1, Float_t c1){
    option.ToUpper();
    histo *Histo2;
    if(std::is_same<histo, TH1F>::value){
     Histo2 = (histo*) cable1->FillResistenceChannelHistogram(option);
     this->histo::Add(Histo2, c1);
     return *this;
    }
    else if(std::is_same<histo, TH1I>::value){
     Histo2 = (histo*) cable1->FillStatusHistogram(option);
      this->histo::Add(Histo2);
      return *this;
    }
    else{
     Error("Isolation::PSPP1::Add", "histograms are of different type, addition/subtraction not performed");
     return *this;
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

}
#endif