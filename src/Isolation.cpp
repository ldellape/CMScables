#include "../include/def_variables.h"
#include "../include/root.h"
#include "../include/Isolation.h"
#include "TError.h"

using namespace Isolation;

 //////////////////////////////////////////////////////////////////////
 // default constructor
 PSPP1::PSPP1(){}

 //////////////////////////////////////////////////////////////////////
 // constructor of the class
 PSPP1::PSPP1(std::vector<std::tuple<bool, std::string, double, double>> &TestOutput, ::TString TestName){
    SetName(TestName);
    for(int i=0; i<int(TestOutput.size()); i++){
        status.push_back(std::get<0>(TestOutput[i]));
        channel.push_back(std::get<1>(TestOutput[i]));
        resistence.push_back(std::get<2>(TestOutput[i]));
        FieldB.push_back(std::get<3>(TestOutput[i]));
    }
 }

 //////////////////////////////////////////////////////////////////////
 // set channel labels 
 void PSPP1::SetChannels(std::vector<::TString> &channelNames){ 
  channel = channelNames;
 }

 //////////////////////////////////////////////////////////////////////
 // set status (passed/failed)
 void PSPP1::SetStatus(std::vector<Bool_t> &statusChannels){ 
  status = statusChannels;
 }

 //////////////////////////////////////////////////////////////////////
 // set measured resistence 
 void PSPP1::SetResistence(std::vector<double> &resistenceChannel){ 
  resistence = resistenceChannel; 
 }

 //////////////////////////////////////////////////////////////////////
 // set name of the test (date and which cable)
 void PSPP1::SetName(::TString name){ 
  CableName=name;
 }

 //////////////////////////////////////////////////////////////////////
 // set magnetic field for LV channels 
 void PSPP1::SetField(std::vector<double> &B){
    B = FieldB;
 }

 //////////////////////////////////////////////////////////////////////
 // set initial parameters of the test 
 void PSPP1::SetInitialParameters(std::tuple<double,double,double,double,double,double,double> param){ 
        InitialParameters.push_back(std::get<0>(param));
    InitialParameters.push_back(std::get<1>(param));
    InitialParameters.push_back(std::get<2>(param));
    InitialParameters.push_back(std::get<3>(param));
    InitialParameters.push_back(std::get<4>(param));
    InitialParameters.push_back(std::get<5>(param));
 }


 //////////////////////////////////////////////////////////////////////
 // functions to get class fields 
 TString PSPP1::GetName(){ 
  return CableName;
 }

 
//////////////////////////////////////////////////////////////////////
// get measured values of resistence based on channel option
 std::vector<double> PSPP1::GetResistence(::TString option){ 
 std::vector<double> vec;
 option.ToUpper();
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
return resistence; }

//////////////////////////////////////////////////////////////////////
// get measured status (passed or failed) based on channel option
 std::vector<Bool_t> PSPP1::GetStatus(::TString option){ 
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


 std::vector<double> PSPP1::GetFieldB(::TString option){
  return FieldB;
 }

 std::vector<double> PSPP1::GetInitialParameters(){
    return InitialParameters;
 }

 //////////////////////////////////////////////////////////////////////
 // get mean values of resistence. option = LV, HV
 Float_t PSPP1::GetMean(TString option){
 std::cout<<option<<std::endl;
 if(option == "LV"){
  double count=0;
  double sum=0;
    for(int i=0; i<NumberHVcables; i++){
     if(channel[i].Contains("LV") || channel[i].Contains("PH")){
      sum+= resistence[i];
      count++;
     }
    }
  return sum/count;
 }
 else if(option == "HV"){
 double count=0;
 double sum=0;
    for(int i=0; i<NumberLVcables; i++){
     if(channel[i].Contains("HV") || channel[i].Contains("Tsensor") && resistence[i] != 0){
      std::cout<<"mean " <<  channel[i] << std::endl;
      sum+= resistence[i];
      count++;
     }
    }
 return sum/count;
 }
 else{
    Error("Isolation::PSPP1::GetMean()", "invalid option");
    return 0.0;
 }
}

 //////////////////////////////////////////////////////////////////////
 // get mean values of resistence. option = LV, HV
 Float_t PSPP1::GetMean(TH1F *h){
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
Float_t PSPP1::GetStdDev(TH1F* h){
  Float_t mean = this->GetMean(h);
  Float_t std = 0;
  for(int i=0; i<h->GetNbinsX(); i++){
    std+= TMath::Power((h->GetBinContent(i) - mean),2);
  }
  return std;
}


//////////////////////////////////////////////////////////////////////
// create histogram for resistence versus channel (to appear on final report)
 TH1F* PSPP1::FillResistenceChannelHistogram(::TString title, ::TString option){
    option.ToUpper();
    std::cout<<"fill resistence "<<option<<std::endl;
    TH1F *h_temp;
    if(option == "LV"){
        h_temp = new TH1F(title,title, NumberLVcables, 0, NumberLVcables);
        // find position //
       for(int j=0; j<int(resistence.size()); j++){
        for(int i=0; i< NumberLVcables; i++ ){
          if(channel[j].CompareTo(labelLV_iso[i]) == 0){
            h_temp->SetBinContent(i+1, resistence[j]);
            std::cout<<" entry "<< channel[j] << "   " << resistence[j] << std::endl;
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
          if(channel[j].CompareTo(labelHV_iso[i]) == 0){
            h_temp->SetBinContent(i+1, resistence[j]);
            std::cout<<" entry "<< channel[j] << "   " << resistence[j] << std::endl;
            break;
          }
        }
       }   
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
// create histogram of measured resistence based on HV, LV, Tsensor...
TH1F* PSPP1::FillResistenceHistogram(::TString title, ::TString option){
    option.ToUpper();
    TH1F *h_temp;
    if(option == "LV"){
     std::pair<double,double> ResMaxMin = FindMaxMinResistence("LV");
     h_temp = new TH1F("h_Resistence_LV", "h_Resistence_LV", 100, ResMaxMin.first, ResMaxMin.second);
     std::vector<double> LVresistence = FilterChannel<double>("LV", "resistence");   
     for(int kk=0; kk< int(LVresistence.size()); kk++){
        h_temp->Fill(LVresistence[kk]);
     }
    }
    else if(option == "HV"){
     std::pair<double,double> ResMaxMin = FindMaxMinResistence("HV");
     h_temp = new TH1F("h_Resistence_HV", "h_Resistence_HV", 100, ResMaxMin.first, ResMaxMin.second);
     std::vector<double> HVresistence = FilterChannel<double>("HV","resistence");
     for(int kk=0; kk< int(HVresistence.size()); kk++){
        h_temp->Fill(HVresistence[kk]);
     }
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
/*
//////////////////////////////////////////////////////////////////////
// get standard deviation
Float_t PSPP1::GetStdDev(TString option){
 option.ToUpper();
 if(option == "LV"){
    double mean = GetMean("LV");
    double std = 0;
    for(int j=0; j<int(resistence.size()); j++){
     if(channel[j].Contains("LV") || channel[j].Contains("PH")){
        std+= TMath::Power((resistence[j] - mean),2);
     }
    }
  std::cout<<std<<std::endl;
  return std;
 }
 else if(option == "HV"){
    double mean = GetMean("HV");
    double std = 0;
    for(int j=0; j<int(resistence.size()); j++){
     if(channel[j].Contains("LV") || channel[j].Contains("PH")){
        std+= TMath::Power((resistence[j] - mean),2);
     }
    }
   return std;
 }
 else{
    Error("Isolation::PSPP1::GetStdDev()", "invalid option");
    return 0.0;
 }
}

*/

