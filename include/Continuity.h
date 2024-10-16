#ifndef CONTINUITY_CLASS_H
#define CONTINUITY_CLASS_H 

#include <vector>
#include "root.h"
#include "def_variables.h"

namespace Continuity{
 class PSPP1{
  private:
    TString CableName;
    std::vector<Bool_t> status;
    std::vector<::TString> channel;
    std::vector<double> resistence;
    std::string TestPath;
    std::tuple<double,double,double,double,double, std::string, double> Parameters;
    const double diamLV = 0.0012;
    const double diamHV = 0.00038;
    Float_t ThreshContLV = 0.65;
    Float_t ThreshContHV = 13;
    const double ResistivityLV = 1.724*TMath::Power(10, -8);
    const double ResistivityHV = 1.724*TMath::Power(10,-8);
    std::pair<double,double> FindMaxMinResistence(::TString option);
  public:
    PSPP1();
    PSPP1(std::vector<std::tuple<Bool_t, std::string, double>> &TestOutput, ::TString TestTitle);
    void SetPath(std::string path);
    void SetChannels(std::vector<::TString> &channelNames);
    void SetStatus(std::vector<Bool_t> &statusChannels);
    void SetResistence(std::vector<double> &resistenceChannel);
    void SetName(::TString name);
    void SetParameters(std::tuple<double,double,double,double,double,std::string,double> param); 
    void SetThreshold(::TString option, Float_t Thresh);
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
    std::string GetPath();
    std::vector<std::pair<TString, double>> GetOverThreshold(::TString option);
    double GetParameter(int param);
    std::tuple<double,double,double,double,double, std::string, double> GetParameters();
    TH1F* FillResistenceChannelHistogram(::TString title, ::TString option);
    TH1F* FillResistenceHistogram(::TString option= "all");
    TH1I* FillStatusHistogram(::TString title, ::TString option = "all");

    // ************************************************** //
    // ---> template method
 //   template<typename histo> histo* Add(histo *h, ::TString option, PSPP1 *cable1, Float_t c1);
    template <typename T> std::vector<T> FilterChannel(::TString option, ::TString vector);
 };
// ************ template method implementation ************ //
// ******************************************************** //

//////////////////////////////////////////////////////////////////////
// sum/difference operation between two cables
/*
 template<typename histo> histo* PSPP1::Add(histo *h, ::TString option, PSPP1 *cable1, Float_t c1){
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
     Error("Continuity::PSPP1::Add", "histograms are of different type");
     return h;
    }
}
*/

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


class Octopus {

};
}

/*
 class Octopus{
    private: 
    TString OctopusName;
    const int NumberBranch = 12;
    std::vector<std::pair<int, TString>> Channel;
    std::vector<bool> status;
    std::string TestPath;
    public:
    Octopus();
    Octopus(std::vector<std::tuple<bool, std::string, double, double>> &TestOutput, ::TString TestTitle);
    void SetPath(std::string path);
    void SetName(std::string )

 };
*/




#endif