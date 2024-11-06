#include <string>
#include <vector>
#include "../include/root.h"
#include "../include/def_variables.h"
#include "../include/Classes.h"


template <typename T> 
void plotting(std::vector<T> &h, std::string title, Int_t number_pad){
  TString sTitle(title.c_str());
  std::cout<<sTitle<<std::endl;
  TPad *pad = (TPad*) c_plot->cd(number_pad);
  pad->Draw();    
 
  TLatex text, textThresh;
  TLatex textMean[IterationTest], textLenght[IterationTest], textStdDev[IterationTest];
  TLatex textOverThreshHV[20]; TLatex textOverThreshLV[20];
  TLine *lineMean[IterationTest];
  Float_t ThreshHVIso[IterationTest];
  Float_t ThreshLVIso[IterationTest];
  Float_t ThreshHVCon[IterationTest]; 
  Float_t ThreshLVCon[IterationTest];
   
// *****DRAWING HISTOS, PARAMETERS OF THE TEST, SETTING LABELS**************** //
if(sTitle.Contains("LV") && !sTitle.Contains("Passed")){
  for(int k=0; k<h[0]->GetNbinsX(); k++){
    h[0]->GetXaxis()->SetBinLabel(k+1, labelLV_con[k]);
  }
 }
 else if(sTitle.Contains("HV") && !sTitle.Contains("Passed")){
  for(int k=0; k<h[0]->GetNbinsX(); k++){
    h[0]->GetXaxis()->SetBinLabel(k+1, labelHV_con[k]);
  }
 }
 else if(sTitle.Contains("Passed")){
  h[0]->GetXaxis()->SetBinLabel(1, "Failed"); h[0]->GetXaxis()->SetBinLabel(2, "Passed");
 }

for(int i=0; i<IterationTest; i++){
 if(i==0){
  h[i]->SetStats(0);
  if(!sTitle.Contains("Passed")) h[i]->Draw("  p");
  else h[i]->Draw("hist ");
   h[i]->SetTitle(""); h[i]->SetMarkerColor(i+3); h[i]->SetLineColor(i+3); h[i]->SetMarkerStyle(3); h[i]->SetMarkerSize(2);
 }
 else{
   h[i]->Draw("same p"); h[i]->SetMarkerColor(i+3); h[i]->SetMarkerStyle(22); h[i]->SetMarkerSize(1.5);
 }
}
text.SetTextSize(0.04);
text.DrawLatexNDC(0.15, 0.92, sTitle);
// *************************************************************************** //

// *************************************************************************** //
// ********************** INSULATION PLOTS *********************************** //
// *************************************************************************** //
if(sTitle == "InsulationTest_HV_Resistence" || sTitle == "InsulationTest_LV_Resistence"){
  std::tuple<double,double,double,double,double, TString, double, TString, double, double> ParamInitial = TestIsolationPSPP1[0]->GetInitialParameters();
  TLatex textParIns; textParIns.SetTextSize(0.020); textParIns.SetTextFont(52);
  textParIns.DrawLatexNDC(0.01, 0.02, Form("Initial Parameters : V=%i kV, Thresh.= %i G #Omega, T_{rise} = %i s, T_{wait} = %i s, T_{meas} = %i s, AR = %s, i_{lim}= %.2f mA, T_{meas} red.=%s, T_{meas} fact.=%i, V_{ramp}= %i V/s", 
      int(std::get<0>(ParamInitial)), 
      int(std::get<1>(ParamInitial)), 
      int(std::get<2>(ParamInitial)), 
      int(std::get<3>(ParamInitial)),
      int(std::get<4>(ParamInitial)), 
      std::get<5>(ParamInitial).Data(),
      std::get<6>(ParamInitial),
      std::get<7>(ParamInitial).Data(),
      int(std::get<8>(ParamInitial)),
      int(std::get<9>(ParamInitial))
  ));

  TAxis *x = h[0]->GetXaxis();
  TAxis *y = h[0]->GetYaxis();
  y->SetTitle("R [#Omega]");
  y->SetNdivisions(8);
  pad->SetLogy(1);
  
  if(sTitle == "InsulationTest_HV_Resistence"){
    ThreshHVIso[0] = TestIsolationPSPP1[0]->GetThreshold("HV");
    y->SetRangeUser(100,1e+12);
    TLine *lineTsensor = new TLine(4*x->GetBinWidth(2), 0, 4*x->GetBinWidth(2), 1e+12);
    lineTsensor->SetLineWidth(1); lineTsensor->SetLineStyle(kDashed); 
    lineTsensor->Draw("same");
    
    std::vector<double> ParamTsensor = TestIsolationPSPP1[0]->GetIsolationPar("Tsensor");
    TLatex textTsensors; textTsensors.SetTextSize(0.01); textTsensors.SetTextFont(52);
    textTsensors.DrawLatex(0.3*x->GetBinWidth(2), 1e+07 , Form("V=%dV, Thresh=%i M #Omega, T_{rise}=%i s, T_{meas}=%i s",
       int(ParamTsensor[0]), 
       int(ParamTsensor[1]),
       int(ParamTsensor[2]),
       int(ParamTsensor[3])
    ));

    std::vector<double> ParamHV = TestIsolationPSPP1[0]->GetIsolationPar("HV");
    TLatex textHVcable; textHVcable.SetTextSize(0.01); textHVcable.SetTextFont(52);
    textHVcable.DrawLatex(8*x->GetBinWidth(2), 1e+07, Form("T_{rise}=%i s, T_{wait}=%i s, T_{meas}=%i s", 
        int(ParamHV[0]),
        int(ParamHV[1]),
        int(ParamHV[2])
    ));
    
    TLine *lineThresh = new TLine(4*x->GetBinWidth(2),ThreshHVIso[0], x->GetXmax(), ThreshHVIso[0]);
    lineThresh->SetLineColor(46); lineThresh->SetLineStyle(kDashed);
    lineThresh->Draw("same");
    textThresh.SetTextSize(0.02); textThresh.SetTextFont(52);
    textThresh.DrawLatex(0.8, 1e+06, "Tsensor threshold");
  
    TLine *lineThreshTsensor = new TLine(x->GetXmin(), 1e+06, 4*x->GetBinWidth(2), 1e+06);
    textThresh.DrawLatex(4.5*x->GetBinWidth(2), ThreshHVIso[0], "HV threshold");
    lineThreshTsensor->SetLineColor(46); lineThreshTsensor->SetLineStyle(kDashed);
    lineThreshTsensor->Draw("same");
    
    TArrow *arrow = new TArrow(x->GetXmax()-0.4, ThreshHVIso[0]+0.9*1e+09, x->GetXmax()-0.4, ThreshHVIso[0]-0.9*1e+09, 0.005, "<" );
    arrow->SetLineColor(kRed); arrow->SetFillColor(kRed); arrow->SetLineWidth(1);
    arrow->Draw();
   
    TArrow *arrow2 = new TArrow(4*x->GetBinWidth(2)-0.2, 1e+06-0.5*1e+06, 4*x->GetBinWidth(2)-0.2, 1e+06+0.8*1e+06, 0.005, ">");
    arrow2->SetLineColor(kRed); arrow2->SetFillColor(kRed); arrow2->SetLineWidth(1);
    arrow2->Draw();

    //draw mean, RMS.
    for(int jj=0; jj<IterationTest; jj++){
      double mean = TestIsolationPSPP1[jj]->GetMean("HV");
      double StdDev = TestIsolationPSPP1[jj]->GetMean("HV");

      textMean[jj].SetTextSize(0.02); textMean[jj].SetTextColor(jj+3); 
      textStdDev[jj].SetTextSize(0.02); textStdDev[jj].SetTextColor(jj+3);

      textMean[jj].DrawLatex(2.95*(jj+1)*x->GetBinWidth(2) + x->GetBinWidth(2) , 1e+03, Form("#mu_{%i} = %.2e", jj+1,mean));
      textStdDev[jj].DrawLatex(2.95*(jj+1)*x->GetBinWidth(2) + x->GetBinWidth(2), 1e+03 - 0.5*1e+03, Form("#sigma_{%i} = %.2e", jj+1, StdDev));

      lineMean[jj] = new TLine (x->GetXmin(),mean, x->GetXmax(), mean);
      lineMean[jj]->SetLineColor(jj+3); lineMean[jj]->SetLineWidth(1); lineMean[jj]->Draw("same");
    }
  }
  else if(sTitle == "InsulationTest_LV_Resistence"){
    y->SetRangeUser(100,1e+11);
    ThreshLVIso[0] = TestIsolationPSPP1[0]->GetThreshold("LV");
    TLine *lineThresh = new TLine(x->GetXmin(), ThreshLVIso[0], x->GetXmax(), ThreshLVIso[0]);
    lineThresh->SetLineColor(46); lineThresh->SetLineStyle(kDashed);
    lineThresh->Draw("same");
  
    textThresh.SetTextSize(0.02); textThresh.SetTextFont(52);
    textThresh.DrawLatex(0.8, ThreshLVIso[0], "threshold");

    TArrow *arrow = new TArrow(x->GetXmax()-0.4, ThreshLVIso[0]+8000, x->GetXmax()-0.4, ThreshLVIso[0]-8000, 0.005, "<" );
    arrow->SetLineColor(kRed);
    arrow->SetFillColor(kRed);
    arrow->SetLineWidth(1);
    arrow->Draw();
    
    for(int j=0; j<IterationTest; j++){
     double mean = TestIsolationPSPP1[j]->GetMean("LV");
     double StdDev = TestIsolationPSPP1[j]->GetMean("LV");

     textMean[j].SetTextSize(0.02); textMean[j].SetTextColor(j+3); 
     textStdDev[j].SetTextSize(0.02); textStdDev[j].SetTextColor(j+3);

     textMean[j].DrawLatex(2.95*(j+1)*x->GetBinWidth(2) + x->GetBinWidth(2) , 1e+03, Form("#mu_{%i} = %.2e", j+1,mean));
     textStdDev[j].DrawLatex(2.95*(j+1)*x->GetBinWidth(2) + x->GetBinWidth(2), 1e+03 - 0.5*1e+03, Form("#sigma_{%i} = %.2e", j+1, StdDev));

     lineMean[j] = new TLine(x->GetXmin(),mean, x->GetXmax(), mean);
     lineMean[j]->SetLineColor(j+3); lineMean[j]->SetLineWidth(1); lineMean[j]->Draw("same");
    }
  }
}
// *************************************************************************** //
// *******************END INSULATION PLOTS************************************ //
// *************************************************************************** //


// *************************************************************************** //
// ***************** CONTINUITY PLOTS **************************************** //
// *************************************************************************** //
else if(sTitle == "ContinuityTest_ResistenceHV" || sTitle == "ContinuityTest_ResistenceLV"){
  TAxis *x = h[0]->GetXaxis();
  TAxis *y = h[0]->GetYaxis();

  std::tuple<double,double,double,double,double, TString, double> Param = TestContinuityPSPP1[0]->GetParameters();
  TLatex textPar; textPar.SetTextSize(0.020); textPar.SetTextFont(52);
  textPar.DrawLatexNDC(0.01, 0.02, Form("Parameters : i=%i, Thresh.= %i, T_{rise} = %i s, T_{wait} = %i s, T_{meas} = %i s, AR=%s, V_{lim}= %i", 
        int(std::get<0>(Param)),
        int(std::get<1>(Param)), 
        int(std::get<2>(Param)), 
        int(std::get<3>(Param)),
        int(std::get<4>(Param)),
        std::get<5>(Param).Data(), 
        int(std::get<6>(Param))
  ));

  y->SetTitle("R[#Omega]");

  if(sTitle == "ContinuityTest_ResistenceHV"){
   ThreshHVCon[0] = TestContinuityPSPP1[0]->GetThreshold("HV");
   y->SetRangeUser(9,14);
   TLine *lineThresh = new TLine(x->GetXmin(), ThreshHVCon[0] , x->GetXmax(), ThreshHVCon[0]);
   lineThresh->SetLineColor(46); lineThresh->SetLineStyle(kDashed);
   lineThresh->Draw("same");
   
   textThresh.SetTextSize(0.02); textThresh.SetTextFont(52);
   textThresh.DrawLatex(0.8, ThreshHVCon[0], "threshold");

   TArrow *arrow = new TArrow(x->GetXmax()-0.4, ThreshHVCon[0], x->GetXmax()-0.4, ThreshHVCon[0]-0.5, 0.005, ">" );
   arrow->SetLineColor(kRed); arrow->SetFillColor(kRed); arrow->SetLineWidth(1);
   arrow->Draw();

   // draw mean, RMS, lenght //
   for(int jj =0; jj<IterationTest; jj++){
     double mean = TestContinuityPSPP1[jj]->GetMean("HV");
     double StdDev = TestContinuityPSPP1[jj]->GetStdDev("HV");
     double lenght = TestContinuityPSPP1[jj]->GetLenght("HV");

     textMean[jj].SetTextSize(0.02); textMean[jj].SetTextColor(jj+3); 
     textStdDev[jj].SetTextSize(0.02); textStdDev[jj].SetTextColor(jj+3);
     textLenght[jj].SetTextSize(0.02); textStdDev[jj].SetTextColor(jj+3);
     
     textStdDev[jj].DrawLatex(2*(jj+1)*x->GetBinWidth(2), 9.95, Form("#sigma_{%i} = %.3f", jj+1,mean));
     textMean[jj].DrawLatex(2*(jj+1)*x->GetBinWidth(2), 9.8, Form("#mu_{%i} = %.3f" ,jj+1, StdDev));
     textLenght[jj].DrawLatex(2*(jj+1)*x->GetBinWidth(2), 9.65, Form("L_{%i} = %.3f" , jj+1, lenght));

     lineMean[jj] = new TLine(x->GetXmin(),mean, x->GetXmax(),mean);
     lineMean[jj]->SetLineColor(jj+3); lineMean[jj]->SetLineWidth(1); lineMean[jj]->Draw("same");
   }
  }
  else if(sTitle == "ContinuityTest_ResistenceLV"){
   y->SetRangeUser(0,1.3);
   ThreshLVCon[0] = TestContinuityPSPP1[0]->GetThreshold("LV");
   TLine *lineThresh = new TLine(x->GetXmin(), ThreshLVCon[0], x->GetXmax(), ThreshLVCon[0]);
   lineThresh->SetLineColor(46); lineThresh->SetLineStyle(kDashed);
   lineThresh->Draw("same");

   textThresh.SetTextSize(0.02); textThresh.SetTextFont(52);
   textThresh.DrawLatex(0.15, ThreshLVCon[0]+0.001, "threshold");

   TArrow *arrow = new TArrow(x->GetXmax()-0.4, ThreshLVCon[0] + 0.05, x->GetXmax()-0.4, ThreshLVCon[0] -0.05, 0.005, ">" );
   arrow->SetLineColor(kRed); arrow->SetFillColor(kRed); arrow->SetLineWidth(1);
   arrow->Draw();

   for(int jj =0; jj<IterationTest; jj++){
     double mean = TestContinuityPSPP1[jj]->GetMean("LV");
     double StdDev = TestContinuityPSPP1[jj]->GetStdDev("LV");
     double lenght = TestContinuityPSPP1[jj]->GetLenght("LV");

     textMean[jj].SetTextSize(0.02); textMean[jj].SetTextColor(jj+3); 
     textStdDev[jj].SetTextSize(0.02); textStdDev[jj].SetTextColor(jj+3);
     textLenght[jj].SetTextSize(0.02); textStdDev[jj].SetTextColor(jj+3);
     textStdDev[jj].DrawLatex(2.5*(jj+1)*x->GetBinWidth(2), 0.23, Form("#sigma_{%i} = %.3f", jj+1, StdDev));
     textMean[jj].DrawLatex(2.5*(jj+1)*x->GetBinWidth(2), 0.20, Form("#mu_{%i} = %.3f" ,jj+1, mean));
     textLenght[jj].DrawLatex(2.5*(jj+1)*x->GetBinWidth(2), 0.17, Form("L_{%i} = %.3f" , jj+1, lenght));

     lineMean[jj] = new TLine(x->GetXmin(),mean, x->GetXmax(),mean);
     lineMean[jj]->SetLineColor(jj+3); lineMean[jj]->SetLineWidth(1); lineMean[jj]->Draw("same");
   }
  }
}
// *************************************************************************** //
// ************* END CONTINUITY PLOTS **************************************** //
// *************************************************************************** //


TString name[IterationTest];
TLegend *l = new TLegend(0.80,0.9,0.9,1.0);
l->SetTextSize(0.018);
for(int i=0; i<IterationTest; ++i){
 if(ContinuityTest) name[i] = TestContinuityPSPP1[i]->GetName();
 else if(InsulationTest) name[i] = TestIsolationPSPP1[i]->GetName();
 else name[i] = TestIsolationPSPP1[i]->GetName();
 if(!sTitle.Contains("Passed")){ 
  l->AddEntry(h[i], name[i], "p");
 }
 else{
  l->AddEntry(h[i], name[i], "p");
 }
}
l->Draw("same");

if(IterationTest == 1) c_plot->SaveAs((std::string(WORKDIR) + "/output/plots/SingleCable/" + sPDFTitle +".pdf").c_str());
else if(IterationTest >1) c_plot->SaveAs((std::string(WORKDIR) + "/output/plots/CheckCable/" + sPDFTitle +".pdf").c_str());

}

