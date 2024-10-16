#include "../include/def_variables.h"
#include "../include/root.h"
#include "../include/Continuity.h"
#include "TError.h"

using namespace Continuity;

 //////////////////////////////////////////////////////////////////////
 // default constructor
 PSPP1::PSPP1(){};

 //////////////////////////////////////////////////////////////////////
 // constructor of the class
 PSPP1::PSPP1(std::vector<std::tuple<Bool_t, std::string, double>> &TestOutput, ::TString TestName){
    CableName = TestName;
    for(int i=0; i<int(TestOutput.size()); i++){
     status.push_back(std::get<0>(TestOutput[i]));
     channel.push_back(std::get<1>(TestOutput[i]));
     resistence.push_back(std::get<2>(TestOutput[i]));
    }
 }

 //////////////////////////////////////////////////////////////////////
 // 
 void PSPP1::SetChannels(std::vector<TString> &channelNames){ 
  channel = channelNames; 
 }

//////////////////////////////////////////////////////////////////////
// 
 void PSPP1::SetStatus(std::vector<Bool_t> &statusChannels){ 
  status = statusChannels;
 }

 //////////////////////////////////////////////////////////////////////
 //
 void PSPP1::SetResistence(std::vector<double> &resistenceChannel){
  resistence = resistenceChannel;
 }

 //////////////////////////////////////////////////////////////////////
 //
 void PSPP1::SetName(TString name){ 
  CableName=name;
 }
 //////////////////////////////////////////////////////////////////////
 //
void PSPP1::SetParameters(std::tuple<double,double,double,double,double,std::string,double> param){ 
    Parameters = param;
}
void PSPP1::SetPath(std::string path){
  TestPath = path;
}

TString PSPP1::GetName(){ return CableName;}

std::string PSPP1::GetPath(){ return TestPath; }

//////////////////////////////////////////////////////////////////////
// 
void PSPP1::SetThreshold(::TString option, Float_t Thresh){
option.ToUpper();
if(option == "LV") ThreshContLV = Thresh;
else if(option == "HV") ThreshContHV = Thresh;
}

//////////////////////////////////////////////////////////////////////
// get measured values of resistence based on channel option
std::vector<double> PSPP1::GetResistence(TString option){
 std::vector<double> vec;
 option.ToUpper();
 std::cout<<option<<std::endl;
 if(option == "LV" && !resistence.empty()){
   for(int i=0; i<int(resistence.size()); i++){
    if(channel[i].Contains("LV")|| channel[i].Contains("PH")){
      vec.push_back(resistence[i]);
    }
   }
 return vec;
 }
 else if(option == "HV" && !resistence.empty()){
  for(int i=0; i<int(resistence.size()); i++){
    if(channel[i].Contains("HV")  || channel[i].Contains("Tsensor")){
      vec.push_back(resistence[i]);
    }
   }
  }
 else if(option == "all"){
  for(int i=0; i<int(resistence.size()); i++){
    vec.push_back(resistence[i]);
  }
  return vec;
 }    
return vec;
}

//////////////////////////////////////////////////////////////////////
// get entries over threshold
std::vector<std::pair<TString, double>> PSPP1::GetOverThreshold(::TString option){
   option.ToUpper();
   std::vector<std::pair<TString, double>> vec;
   if(option == "LV"){
      for(int i=0; i<int(resistence.size()); i++){
        if(resistence[i]>ThreshContLV){
          vec.push_back(std::make_pair(channel[i], resistence[i]));
        }
      }
    return vec;
   }
   else if(option == "HV"){
      for(int i=0; i<int(resistence.size()); i++){
        if(resistence[i]>ThreshContHV){
          vec.push_back(std::make_pair(channel[i], resistence[i]));
        }
      }
    return vec;
   }
   else{
     Error("Continuity::PSPP1::GetOverThreshold(::TString option)", "Invalid Option");
     return vec;
   }
}

 //////////////////////////////////////////////////////////////////////
// get measured status (passed or failed) based on channel option
std::vector<Bool_t> PSPP1::GetStatus(TString option){
    std::vector<Bool_t> vec;
    option.ToUpper();
    if(option == "LV" && !resistence.empty()){
        for(int i=0; i<int(resistence.size()); i++){
            if(channel[i].Contains("LV")|| channel[i].Contains("PH")){
                vec.push_back(status[i]);
            }
        }
    return vec;
    }
    else if(option == "HV" && !resistence.empty()){
        for(int i=0; i<int(resistence.size()); i++){
            if(channel[i].Contains("HV") || channel[i].Contains("Tsensor")){
                vec.push_back(status[i]);
            }
        }
    return vec;
    }
return status;
}

 //////////////////////////////////////////////////////////////////////
 // get mean value of measured resistence
Double_t PSPP1::GetMean(TString option){
    option.ToUpper();
    if(option != "LV" && option != "HV"){
        Error("Continuity::PSPP1::GetMean", "invalid option");
        return 0.0;
    }
    Float_t sum=0;
    Float_t count=0;
    if(option == "LV"){
    for(int i=0; i<int(resistence.size()); i++){
        if((channel[i].Contains("LV") || channel[i].Contains("PH"))){
              if(resistence[i] < ThreshContLV){
                sum+= resistence[i];
                count++;
              }
        }
    }
    }
    else if(option == "HV"){
    for(int i=0; i<int(resistence.size()); i++){
     if(channel[i].Contains("HV") || channel[i].Contains("Tsensor")){
              if(resistence[i] < ThreshContHV){
                sum+= resistence[i];
                count++;
              }
        }
    }
    }
 return sum/count;    
}

 //////////////////////////////////////////////////////////////////////
 // get mean values of resistence. option = LV, HV
 Double_t PSPP1::GetMean(TH1F *h){
  Float_t sum =0;
  Float_t count=0;
  for(int i=0; i<h->GetNbinsX(); i++){
    sum+=h->GetBinContent(i);
    count++;
  }
  return sum/count;
}

 //////////////////////////////////////////////////////////////////////
 // get mean values of resistence. option = LV, HV
Double_t PSPP1::GetStdDev(TH1F* h){
  Double_t mean = this->GetMean(h);
  Double_t std = 0;
  for(int i=0; i<h->GetNbinsX(); i++){
    std+= TMath::Power((h->GetBinContent(i) - mean),2);
  }
  return std;
}

  //////////////////////////////////////////////////////////////////////
  // get threshold values 
Double_t PSPP1::GetThreshold(TString option){
  option.ToUpper();
  if(option == "LV") return ThreshContLV;
  else if(option == "HV") return ThreshContHV;
  else{
    Error("Isolation::PSPP1::GetThreshold", "invalid option");
  }
}



//////////////////////////////////////////////////////////////////////
// get the param entry of the parameters vector
/*
 PSPP1::GetParameter(int param){ 
 return std::get<param>(Parameters);
}
*/
//////////////////////////////////////////////////////////////////////
// get vector of parameters
std::tuple<double,double,double,double,double, std::string, double> PSPP1::GetParameters(){ 
 return Parameters;
}


//////////////////////////////////////////////////////////////////////
// create histogram for resistence versus channel (to appear on final report)
 TH1F* PSPP1::FillResistenceChannelHistogram(::TString title, ::TString option){
    option.ToUpper();
    TH1F *h_temp;
    if(option == "LV"){
        h_temp = new TH1F(title,title, NumberLVcables, 0, NumberLVcables);
        // find position //
       for(int j=0; j<int(resistence.size()); j++){
        for(int i=0; i< NumberLVcables; i++ ){
          if(channel[j].CompareTo(labelLV_con[i]) == 0){
            h_temp->SetBinContent(i+1, resistence[j]);
            break;
          }
        }
       }
    return h_temp;
    }
    else if(option == "HV"){
        h_temp = new TH1F(title, title, NumberHVcables, 0, NumberHVcables);
        // find position //
       for(int j=0; j<int(resistence.size()); j++){
        for(int i=0; i< NumberHVcables; i++ ){
          if(channel[j].CompareTo(labelHV_con[i]) == 0){
            h_temp->SetBinContent(i+1, resistence[j]);
            break;
          }
        }
       }   
    return h_temp;
    }
   else{
    Error("Continuity::PSPP1::FillResistenceChannelHistogram():", "invalid option");
    return h_temp;
   } 
}

//////////////////////////////////////////////////////////////////////
// create histogram for result of the test (Passed/Failed/Add)
TH1I* PSPP1::FillStatusHistogram(::TString title, ::TString option){
    option.ToUpper();
    TH1I *h_temp;
    if(option == "LV"){
        h_temp = new TH1I(title, title, 2, 0, 2);
        for(int i=0; i<int(status.size()); i++){
            if(channel[i].Contains("LV") || channel[i].Contains("PH")){
            h_temp->Fill(status[i]);
            }
        }
    return h_temp;
    }
    else if(option == "HV"){
        h_temp = new TH1I(title, title, 2, 0, 2);
        for(int i=0; i<int(status.size()); i++){
            if(channel[i].Contains("HV") || channel[i].Contains("Tsensor")){
            h_temp->Fill(status[i]);
            }
        }
    return h_temp;
    }
    else{
     h_temp = new TH1I(title, title, 2, 0, 2);
    for(int i=0; i<int(status.size()); i++){
     h_temp->Fill(status[i]);
     }
     return h_temp;
    }
}

//////////////////////////////////////////////////////////////////////
// private method: find max and min to set automatically histogram's ranges
std::pair<double,double> PSPP1::FindMaxMinResistence(::TString Option){
    Option.ToUpper();
    double min = resistence[0];
    double max = resistence[0];
    if(Option == "LV"){
    for(int k=0; k<int(resistence.size()); k++){
     if(channel[k].Contains("LV") || channel[k].Contains("PH")){
        if(resistence[k] < min) min = resistence[k];
        if(resistence[k] > max) max = resistence[k];
     }
    }
    }
    else if(Option == "HV"){
    for(int k=0; k<int(resistence.size()); k++){
     if(channel[k].Contains("HV") || channel[k].Contains("Tsensor")){
        if(resistence[k] < min) min = resistence[k];
        if(resistence[k] > max) max = resistence[k];
     }
    }
    }
    return std::make_pair(min, max);
}

//////////////////////////////////////////////////////////////////////
// fill resistence histogram based on channel option
TH1F* PSPP1::FillResistenceHistogram(::TString option){
    option.ToUpper();
    TH1F *h_temp;
    if(option == "LV"){
     std::pair<double,double> ResMaxMin = FindMaxMinResistence("LV");
     h_temp = new TH1F("h_Resistence_LV", "h_Resistence_LV", 100, ResMaxMin.first, ResMaxMin.second);
     std::vector<double> LVresistence = FilterChannel<double>("LV", "resistence");   
     for(int kk=0; kk< int(LVresistence.size()); kk++){
        h_temp->Fill(LVresistence[kk]);
     }
     return h_temp;
    }
    else if(option == "HV"){
     std::pair<double,double> ResMaxMin = FindMaxMinResistence("HV");
     h_temp = new TH1F("h_Resistence_HV", "h_Resistence_HV", 100, ResMaxMin.first, ResMaxMin.second);
     std::vector<double> HVresistence = FilterChannel<double>("HV","resistence");
     for(int kk=0; kk< int(HVresistence.size()); kk++){
        h_temp->Fill(HVresistence[kk]);
     }
     return h_temp;
    }
    else{
     Error("Continuity::PSPP1::FillResistenceHistogram", "invalid option");
     return h_temp;
    }
}


//////////////////////////////////////////////////////////////////////
// compute cable lenght based on measured resistence
Double_t PSPP1::GetLenght(TString option){
 if(option == "LV"){
    double mean = GetMean("LV");
    return (mean*TMath::Pi()*TMath::Power(diamLV, 2))/(4*ResistivityLV);
 }
 else if(option == "HV"){
    double mean = GetMean("HV");
    return (mean*TMath::Pi()*TMath::Power(diamHV, 2))/(4*ResistivityHV);
}
else{
    Error("Continuity::PSPP1::GetLenght", "invalid option");
    return 0.0;
}
}
Double_t PSPP1::GetLenght(TH1F *h){
 TString name(h->GetTitle());
 double mean = this->GetMean(h);
 if(name.Contains("LV")) return (mean*TMath::Pi()*TMath::Power(diamLV, 2))/(4*ResistivityLV);
 else if(name.Contains("HV")) return (mean*TMath::Pi()*TMath::Power(diamHV, 2))/(4*ResistivityHV);
 else{
   Error("Isolation::PSPP1::GetLenght(TH1F *h, TString option):", "invalid option");
   return 0;
 }
}

//////////////////////////////////////////////////////////////////////
// get RMS of measured resistence based on channel 
Double_t PSPP1::GetStdDev(TString option){
 double std = 0;
 if(option == "LV"){
    Double_t mean = this->GetMean("LV");
    for(int j=0; j<int(resistence.size()); j++){
     if((channel[j].Contains("LV") || channel[j].Contains("PH")) && resistence[j] < ThreshContLV){
        std+= TMath::Power((resistence[j] - mean),2);
     }
    }
 }
 else if(option == "HV"){
    Double_t mean = this->GetMean("HV");
    for(int j=0; j<int(resistence.size()); j++){
     if((channel[j].Contains("HV") || channel[j].Contains("Tsensor")) && resistence[j] < ThreshContHV){
        std+= TMath::Power((resistence[j] - mean),2);
     }
    }
 }
 else {
    Error("Continuity::PSPP1::GetMean", "invalid option");
 }
 return std;
}

