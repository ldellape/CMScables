#ifndef CONTINUITY_CLASS_H
#define CONTINUITY_CLASS_H 

#include <vector>
#include "root.h"
#include "def_variables.h"

namespace Continuity{
 class PSPP1{
  private:
    const double diamLV = 0.0012;
    const double diamHV = 0.00038;
    const double ResistivityLV = 1.724*TMath::Power(10, -8);
    const double ResistivityHV = 1.724*TMath::Power(10,-8);
    std::pair<double,double> FindMaxMinResistence(::TString option);
  protected:
    TString CableName;
    std::vector<Bool_t> status;
    std::vector<::TString> channel;
    std::vector<double> resistence;
    std::vector<double> Parameters;
    std::string ParString;
  public:
    PSPP1();
    PSPP1(std::vector<std::tuple<Bool_t, std::string, double>> &TestOutput, ::TString TestName);
    void SetChannels(std::vector<::TString> &channelNames);
    void SetStatus(std::vector<Bool_t> &statusChannels);
    void SetResistence(std::vector<double> &resistenceChannel);
    void SetName(::TString name);
    void SetParameters(std::tuple<double,double,double,double,double,std::string,double> param); 
    ::TString GetName();
    std::vector<double> GetResistence(::TString option= "all");
    std::vector<Bool_t> GetStatus(::TString = "all");
    Float_t GetMean(::TString option = "all");
    Float_t GetMean(TH1F *h);
    Float_t GetLenght(::TString option);
    Float_t GetLenght(TH1F* h);
    Float_t GetStdDev(::TString option);
    Float_t GetStdDev(TH1F *h);
    double GetParameter(int param);
    std::vector<double> GetParameters();
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



 class Octopus{
    public:
    Octopus();
 };

}

#endif