#include <string>
#include <vector>
#include "../include/root.h"
#include "../include/def_variables.h"
#include "../include/Continuity.h"
#include "../include/Isolation.h"


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
  
  // ************************************************************** //
  // ****** STORE MEAN, RMS, LENGHT ******************************* //
  // ************************************************************** //
  std::vector<Float_t> lenght;
  std::vector<Float_t> mean;
  std::vector<Float_t> StdDev;
  if constexpr (std::is_same<TH1F*, T>::value){
   for(int ii=0; ii<IterationTest; ++ii){
     if((test_type==1 || test_type==2)){
       StdDev.push_back(TestIsolationPSPP1[ii]->GetStdDev(h[ii]));
       mean.push_back(TestIsolationPSPP1[ii]->GetMean(h[ii]));     
     }
     else if((test_type==0 || test_type==2)){
       lenght.push_back(TestContinuityPSPP1[ii]->GetLenght(h[ii]));
       StdDev.push_back(TestContinuityPSPP1[ii]->GetStdDev(h[ii]));
       mean.push_back(TestContinuityPSPP1[ii]->GetMean(h[ii]));
       std::cout<<mean[ii]<< "   " << StdDev[ii] << "  " << lenght[ii] << std::endl;
     }
   }
  }
// *************************************************************** //
// *************************************************************** //


// *****DRAWING HISTOS, PARAMETERS OF THE TEST, SETTING LABELS**************** //
if(sTitle.Contains("LV") && !sTitle.Contains("Passed")){
  for(int k=0; k<h[0]->GetNbinsX(); k++){
    h[0]->GetXaxis()->SetBinLabel(k+1, labelLV_con[k]);
  }
 }
 else if(sTitle.Contains("HV") && !sTitle.Contains("Passed")){
   std::cout<<sTitle<<" bin : " << h[0]->GetNbinsX()<<std::endl;

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
text.DrawLatexNDC(0.15, 0.92, sTitle);
// *************************************************************************** //

// *************************************************************************** //
// ********************** INSULATION PLOTS *********************************** //
// *************************************************************************** //
if(sTitle == "InsulationTest_HV_Resistence" || sTitle == "InsulationTest_LV_Resistence"){
  auto ParamInitial = TestIsolationPSPP1[0]->GetInitialParameters();
  TLatex textParIns; textParIns.SetTextSize(0.020); textParIns.SetTextFont(52);
 // textParIns.DrawLatexNDC(0.01, 0.02, Form("Initial Parameters : V=%i kV, Thresh.= %i G #Omega, T_{rise} = %i s, T_{wait} = %i s, T_{meas} = %i s, i_{lim}= %i mA, V_{ramp}= %i V/s", int(ParamInitial[0]), int(ParamInitial[1]), int(ParamInitial[2]), int(ParamInitial[3]), int(ParamInitial[4]), int(ParamInitial[5])));

  auto ParamLV = TestIsolationPSPP1[0]->GetIsolationPar<double,double,double,double>("LV");
  TAxis *x = h[0]->GetXaxis();
  TAxis *y = h[0]->GetYaxis();
  y->SetTitle("R [#Omega]");
  y->SetNdivisions(8);
  pad->SetLogy(1);
  
  if(sTitle == "InsulationTest_HV_Resistence"){
    y->SetRangeUser(100,1e+12);
    TLine *lineTsensor = new TLine(4*x->GetBinWidth(2), 0, 4*x->GetBinWidth(2), 1e+12);
    lineTsensor->SetLineWidth(1); lineTsensor->SetLineStyle(kDashed); 
    lineTsensor->Draw("same");
    
    auto ParamTsensor = TestIsolationPSPP1[0]->GetIsolationPar<double,double,double,double>("Tsensor");
    
    TLatex textTsensors; textTsensors.SetTextSize(0.02); textTsensors.SetTextFont(52);
   // textTsensors.DrawLatex(0.3*x->GetBinWidth(2), 1e+07 , Form("V=%dV, Thresh=%d M #Omega", int(std::get<0>(ParamTsensor)), int(std::get<1>(ParamTsensor))));

    auto ParamHV = TestIsolationPSPP1[0]->GetIsolationPar<double,double,double,double>("HV");
    TLatex textHVcable; textHVcable.SetTextSize(0.02); textHVcable.SetTextFont(52);
  //  textHVcable.DrawLatex(8*x->GetBinWidth(2), 1e+07, Form("T_{rise}=%d s, T_{wait}=%d s, T_{meas}=%d s", int(std::get<0>(ParamHV)), int(std::get<1>(ParamHV)), int(std::get<2>(ParamHV)) ));
    
    TLine *lineThresh = new TLine(4*x->GetBinWidth(2),ThreshIsoHV, x->GetXmax(), ThreshIsoHV);
    lineThresh->SetLineColor(46); lineThresh->SetLineStyle(kDashed);
    lineThresh->Draw("same");
    textThresh.SetTextSize(0.02); textThresh.SetTextFont(52);
    textThresh.DrawLatex(0.8, 1e+06, "Tsensor threshold");
  
    TLine *lineThreshTsensor = new TLine(x->GetXmin(), 1e+06, 4*x->GetBinWidth(2), 1e+06);
    textThresh.DrawLatex(4.5*x->GetBinWidth(2), ThreshIsoHV, "HV threshold");
    lineThreshTsensor->SetLineColor(46); lineThreshTsensor->SetLineStyle(kDashed);
    lineThreshTsensor->Draw("same");
    
    TArrow *arrow = new TArrow(x->GetXmax()-0.4, ThreshIsoHV+0.9*1e+09, x->GetXmax()-0.4, ThreshIsoHV-0.9*1e+09, 0.005, "<" );
    arrow->SetLineColor(kRed); arrow->SetFillColor(kRed); arrow->SetLineWidth(1);
    arrow->Draw();
   
    TArrow *arrow2 = new TArrow(4*x->GetBinWidth(2)-0.2, 1e+06-0.5*1e+06, 4*x->GetBinWidth(2)-0.2, 1e+06+0.8*1e+06, 0.005, ">");
    arrow2->SetLineColor(kRed); arrow2->SetFillColor(kRed); arrow2->SetLineWidth(1);
    arrow2->Draw();

    //draw mean, RMS.
    for(int jj=0; jj<IterationTest; jj++){
      textMean[jj].SetTextSize(0.02); textMean[jj].SetTextColor(jj+3); 
      textStdDev[jj].SetTextSize(0.02); textStdDev[jj].SetTextColor(jj+3);

      textMean[jj].DrawLatex(2.95*(jj+1)*x->GetBinWidth(2) + x->GetBinWidth(2) , 1e+03, Form("#mu_{%i} = %.2e", jj+1, mean[jj]));
      textStdDev[jj].DrawLatex(2.95*(jj+1)*x->GetBinWidth(2) + x->GetBinWidth(2), 1e+03 - 0.5*1e+03, Form("#sigma_{%i} = %.2e", jj+1, StdDev[jj]));

      lineMean[jj] = new TLine (x->GetXmin(), mean[jj], x->GetXmax(), mean[jj]);
      lineMean[jj]->SetLineColor(jj+3); lineMean[jj]->SetLineWidth(1); lineMean[jj]->Draw("same");
    }
  }
  else if(sTitle == "InsulationTest_LV_Resistence"){
    y->SetRangeUser(100,1e+11);
    TLine *lineThresh = new TLine(x->GetXmin(), ThreshIsoLV, x->GetXmax(), ThreshIsoLV);
    lineThresh->SetLineColor(46); lineThresh->SetLineStyle(kDashed);
    lineThresh->Draw("same");
  
    textThresh.SetTextSize(0.02); textThresh.SetTextFont(52);
    textThresh.DrawLatex(0.8, ThreshIsoLV, "threshold");

    TArrow *arrow = new TArrow(x->GetXmax()-0.4, ThreshIsoLV+8000, x->GetXmax()-0.4, ThreshIsoLV-8000, 0.005, "<" );
    arrow->SetLineColor(kRed);
    arrow->SetFillColor(kRed);
    arrow->SetLineWidth(1);
    arrow->Draw();

    for(int j=0; j<IterationTest; j++){
     textMean[j].SetTextSize(0.02); textMean[j].SetTextColor(j+3); 
     textStdDev[j].SetTextSize(0.02); textStdDev[j].SetTextColor(j+3);

     textMean[j].DrawLatex(2.95*(j+1)*x->GetBinWidth(2) + x->GetBinWidth(2) , 1e+03, Form("#mu_{%i} = %.2e", j+1, mean[j]));
     textStdDev[j].DrawLatex(2.95*(j+1)*x->GetBinWidth(2) + x->GetBinWidth(2), 1e+03 - 0.5*1e+03, Form("#sigma_{%i} = %.2e", j+1, StdDev[j]));
     lineMean[j] = new TLine(x->GetXmin(), mean[j], x->GetXmax(), mean[j]);
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

  //std::vector<double> Param = TestContinuityPSPP1[0]->GetParameters();
  TLatex textPar; textPar.SetTextSize(0.020); textPar.SetTextFont(52);
  //textPar.DrawLatexNDC(0.01, 0.02, Form("Parameters : i=%i, Thresh.= %i, T_{rise} = %i s, T_{wait} = %i s, T_{meas} = %i s, V_{lim}= %i", int(Param[0]), int(Param[1]), int(Param[2]), int(Param[3]), int(Param[4]), int(Param[6])));
  y->SetTitle("R[#Omega]");

  if(sTitle == "ContinuityTest_ResistenceHV"){
   y->SetRangeUser(9,14);
   TLine *lineThresh = new TLine(x->GetXmin(), ThreshContHV, x->GetXmax(), ThreshContHV);
   lineThresh->SetLineColor(46); lineThresh->SetLineStyle(kDashed);
   lineThresh->Draw("same");
   
   textThresh.SetTextSize(0.02); textThresh.SetTextFont(52);
   textThresh.DrawLatex(0.8, ThreshContHV, "threshold");

   TArrow *arrow = new TArrow(x->GetXmax()-0.4, ThreshContHV+0.2, x->GetXmax()-0.4, ThreshContHV-0.5, 0.005, ">" );
   arrow->SetLineColor(kRed); arrow->SetFillColor(kRed); arrow->SetLineWidth(1);
   arrow->Draw();
   // draw mean, RMS, lenght //
   for(int jj =0; jj<IterationTest; jj++){
     textMean[jj].SetTextSize(0.02); textMean[jj].SetTextColor(jj+3); 
     textStdDev[jj].SetTextSize(0.02); textStdDev[jj].SetTextColor(jj+3);
     textLenght[jj].SetTextSize(0.02); textStdDev[jj].SetTextColor(jj+3);

     textStdDev[jj].DrawLatex(2*(jj+1)*x->GetBinWidth(2), 9.95, Form("#sigma_{%i} = %.3f", jj+1, StdDev[jj]));
     textMean[jj].DrawLatex(2*(jj+1)*x->GetBinWidth(2), 9.8, Form("#mu_{%i} = %.3f" ,jj+1, mean[jj]));
     textLenght[jj].DrawLatex(2*(jj+1)*x->GetBinWidth(2), 9.65, Form("L_{%i} = %.3f" , jj+1,lenght[jj]));

     lineMean[jj] = new TLine(x->GetXmin(), mean[jj], x->GetXmax(), mean[jj]);
     lineMean[jj]->SetLineColor(jj+3); lineMean[jj]->SetLineWidth(1); lineMean[jj]->Draw("same");
   }
  }
  else if(sTitle == "ContinuityTest_ResistenceLV"){
   y->SetRangeUser(0,1.3);
   TLine *lineThresh = new TLine(x->GetXmin(), ThreshContLV, x->GetXmax(), ThreshContLV);
   lineThresh->SetLineColor(46); lineThresh->SetLineStyle(kDashed);
   lineThresh->Draw("same");

   textThresh.SetTextSize(0.02); textThresh.SetTextFont(52);
   textThresh.DrawLatex(0.15, ThreshContLV+0.001, "threshold");

   TArrow *arrow = new TArrow(x->GetXmax()-0.4, ThreshContLV+0.05, x->GetXmax()-0.4, ThreshContLV-0.05, 0.005, ">" );
   arrow->SetLineColor(kRed); arrow->SetFillColor(kRed); arrow->SetLineWidth(1);
   arrow->Draw();
  
   for(int jj =0; jj<IterationTest; jj++){
     textMean[jj].SetTextSize(0.02); textMean[jj].SetTextColor(jj+3); 
     textStdDev[jj].SetTextSize(0.02); textStdDev[jj].SetTextColor(jj+3);
     textLenght[jj].SetTextSize(0.02); textStdDev[jj].SetTextColor(jj+3);

     textStdDev[jj].DrawLatex(2.5*(jj+1)*x->GetBinWidth(2), 0.23, Form("#sigma_{%i} = %.3f", jj+1, StdDev[jj]));
     textMean[jj].DrawLatex(2.5*(jj+1)*x->GetBinWidth(2), 0.20, Form("#mu_{%i} = %.3f" ,jj+1, mean[jj]));
     textLenght[jj].DrawLatex(2.5*(jj+1)*x->GetBinWidth(2), 0.17, Form("L_{%i} = %.3f" , jj+1,lenght[jj]));

     lineMean[jj]->SetLineColor(jj+3); lineMean[jj]->SetLineWidth(1); lineMean[jj]->Draw("same");
     lineMean[jj] = new TLine(x->GetXmin(), mean[jj], x->GetXmax(), mean[jj]);
   }
  } 
}
// *************************************************************************** //
// ************* END CONTINUITY PLOTS **************************************** //
// *************************************************************************** //
TString name[IterationTest];
TLegend *l = new TLegend(0.65,0.9,0.9,1.0);
l->SetTextSize(0.018);
for(int i=0; i<IterationTest; ++i){
 if(test_type == 0 || test_type == 2) name[i] = TestContinuityPSPP1[i]->GetName();
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

