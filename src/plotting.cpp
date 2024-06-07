#include <string>
#include <vector>
#include "../include/root.h"
#include "../include/user_func.h"

template<typename T> 
void plotting(std::vector<T*> &h, std::string sTitle, Int_t number_pad){
  std::cout<<sTitle<<std::endl;
    // choosing subcanvas //
    TPad *pad = (TPad*) c_plot->cd(number_pad);
    pad->Draw();    
    TLatex text;
    TLine *lineThresh;
    TLine *lineMean[IterationTest];
    TLatex textThresh;
    TLatex textMean[IterationTest], textLenght[IterationTest], textStdDev[IterationTest];
    TLatex textOverThreshHV[20];
    TLatex textOverThreshLV[20];
   // TArrow *arrow;
    


double mean[20], stdDev[20], lenght[20];

//******STORE MEAN, RMS, LENGHT*****************//
for(int ii=0; ii<IterationTest; ++ii){
 //h[ii]->ResetStats();
double m=0;
double std=0;
Float_t count =0;
for(int j=1; j<h[ii]->GetNbinsX(); j++){
  m+= h[ii]->GetBinContent(j);
  count++;
 }
 mean[ii] = m/count;
 for(int jj=1; jj<h[ii]->GetNbinsX(); jj++){
 std+=TMath::Power((h[ii]->GetBinContent(jj) - mean[ii]),2);
 }
 stdDev[ii] = sqrt((1/count)*std);
 if(sTitle == "ContinuityTest_ResistenceLV")  lenght[ii] = (mean[ii]*TMath::Pi()*TMath::Power(diamLV,2))/(4*ResistivityLV);
 else if(sTitle == "ContinuityTest_ResistenceHV") lenght[ii] = (mean[ii]*TMath::Pi()*TMath::Power(diamHV,2))/(4*ResistivityHV);
}
/**************************************************/

// prepare name of file //
TString name[IterationTest];
for(int j=0; j<IterationTest; j++){
  name[j] = TestName[j].substr( TestName[j].rfind("/") +1, TestName[j].rfind(".") - TestName[j].rfind("/")-1);
}
sPDFTitle = name[0] + "____" + currentDate;

//*********SET LABELS***********************//
    text.SetTextSize(0.03);
    char const *labels[2]={"Failed", "Passed"};
    auto *x = h[0]->GetXaxis();
    auto *y = h[0]->GetYaxis();
    if((h[0]->GetNbinsX())== 2){
     for(int i=1; i<3; i++){
      x->SetBinLabel(i,labels[i-1]); 
    }
    }
    else if((h[0]->GetNbinsX()) == NumberLVcables){
      for(int i=1; i<NumberLVcables+1; i++){
       x->SetBinLabel(i, labelsContLV[i-1]);
      }
    }
    else if((h[0]->GetNbinsX()) == (NumberHVcables + NumberHVRTNwires + NumberSensorWire)){
      for(int i=1; i<(NumberHVcables + NumberHVRTNwires + NumberSensorWire+1); i++){

      x->SetBinLabel(i, labelsContHV[i-1]);
     }  
     }
//*******************************************//
    

//*************DRAWING HISTOS AND PARAMETERS OF THE TEST****************//
  for(int i=0; i<IterationTest; i++){
    if(i==0){
     h[i]->SetStats(0);
     if(sTitle != "ContinuityTest_All_Passed-Failed" && sTitle != "InsulationTest_All_Passed-Failed") h[i]->Draw("  p");
     else h[i]->Draw("hist ");
     h[i]->SetTitle("");
     h[i]->SetMarkerColor(i+3);
     h[i]->SetLineColor(i+3);
     h[i]->SetMarkerStyle(3);
     h[i]->SetMarkerSize(2);
     x->SetLabelSize(0.035);
     y->SetLabelSize(0.035);
    }
    else{
     // if(sTitle != "ContinuityTest_All_Passed-Failed" && sTitle != "InsulationTest_All_Passed-Failed") 
     // else h[i]->Draw(" same hist");
      h[i]->Draw("same p");
      h[i]->SetMarkerColor(i+3);
      h[i]->SetMarkerStyle(22);
      h[i]->SetMarkerSize(1.5);
  //    h[i]->SetLineColor(kRed);
    }
    }
    text.DrawLatexNDC(0.15, 0.92, (sTitle).c_str());
    if(sTitle.find("Continuity") != std::string::npos){
     TLatex textPar[IterationTest];
      for(int i=0; i<IterationTest; i++){
        textPar[i].SetTextSize(0.020);
        textPar[i].SetTextFont(52);
      for(auto &it : ParametersContinuity){
        textPar[i].DrawLatexNDC(0.01, 0.02, Form("Parameters : i=%i, Thresh.= %i, T_{rise} = %i s, T_{wait} = %i s, T_{meas} = %i s, V_{lim}= %i", int(std::get<0>(it)), int(std::get<1>(it)), int(std::get<2>(it)), int(std::get<3>(it)), int(std::get<4>(it)), int(std::get<6>(it))));
      }
     }
    }
    else if(sTitle.find("Insulation") != std::string::npos){
      TLatex textParIns;
      textParIns.SetTextSize(0.020);
      textParIns.SetTextFont(52);
      if(sTitle != "InsulationTest_HV_Resistence"){
        textParIns.DrawLatexNDC(0.01, 0.02, "Parameters : V=50 V, Thresh.= 100 M #Omega, T_{rise} = 1 s, T_{wait} = 8 s, T_{meas} = 8 s, i_{lim}= 1.95 mA, V_{ramp}=120 V/s");
      }
      else if(sTitle == "InsulationTest_HV_Resistence"){
        textParIns.DrawLatexNDC(0.01, 0.02, "Initial Parameters : V=1.2 kV, Thresh.= 1 G #Omega, T_{rise} = 10 s, T_{wait} = 8 s, T_{meas} = 1 s, i_{lim}= 1.95 mA, V_{ramp}=120 V/s");
        TLine *lineTsensor = new TLine(4*x->GetBinWidth(2), 0, 4*x->GetBinWidth(2), 1e+12);
        lineTsensor->SetLineWidth(1);
        lineTsensor->SetLineStyle(kDashed);
        lineTsensor->Draw("same");
        TLatex textTsensor[2];
        textTsensor[0].SetTextSize(0.02);
        textTsensor[0].SetTextFont(52);
        textTsensor[0].DrawLatex(0.3*x->GetBinWidth(2), 1e+07 , "V=50V, Thresh=100 M #Omega");
        textTsensor[1].SetTextSize(0.02);
        textTsensor[1].SetTextFont(52);
        textTsensor[1].DrawLatex(0.3*x->GetBinWidth(2), 0.4*1e+07 , "T_{rise}=1s, T_{meas}=100 2s");
        TLatex textHVcable;
        textHVcable.SetTextSize(0.02);
        textHVcable.SetTextFont(52);
        textHVcable.DrawLatex(8*x->GetBinWidth(2), 1e+07, "T_{rise}=1 s, T_{wait}=8 s, T_{meas}=8 s" );
      }

    }
//****************************************************************//

/**********************************************/
TLegend *l = new TLegend(0.65,0.9,0.9,1.0);
l->SetTextSize(0.018);
if(sTitle != "ContinuityTest_All_Passed-Failed" && sTitle != "InsulationTest_All_Passed-Failed") l->AddEntry(h[0], name[0], "p");
else{  l->AddEntry(h[0], name[0], "l");}
for(int i=1; i<IterationTest; ++i){
 if(sTitle != "ContinuityTest_All_Passed-Failed" && sTitle != "InsulationTest_All_Passed-Failed") l->AddEntry(h[i], name[i], "p");
 else  l->AddEntry(h[i], name[i], "p");
}
l->Draw("same");
//leg->Draw("same");
/***********************************************/


textThresh.SetTextSize(0.02);
textThresh.SetTextFont(52);

if(sTitle == "ContinuityTest_ResistenceHV"){
  y->SetRangeUser(9,14);
  y->SetTitle("R[#Omega]");

  lineThresh = new TLine(x->GetXmin(), ThreshContHV, x->GetXmax(), ThreshContHV);
  lineThresh->SetLineColor(46);
lineThresh->SetLineStyle(kDashed);
lineThresh->Draw("same");
textThresh.DrawLatex(0.8, ThreshContHV, "threshold");
TArrow *arrow = new TArrow(x->GetXmax()-0.4, ThreshContHV+0.2, x->GetXmax()-0.4, ThreshContHV-0.5, 0.005, ">" );
arrow->SetLineColor(kRed);
arrow->SetFillColor(kRed);
arrow->SetLineWidth(1);
arrow->Draw();
}
else if(sTitle == "ContinuityTest_ResistenceLV"){
  y->SetRangeUser(0,1.3);
  y->SetTitle("R[#Omega]");

  lineThresh = new TLine(x->GetXmin(), ThreshContLV, x->GetXmax(), ThreshContLV);
  lineThresh->SetLineColor(46);
lineThresh->SetLineStyle(kDashed);
lineThresh->Draw("same");
textThresh.DrawLatex(0.15, ThreshContLV+0.001, "threshold");
TArrow *arrow = new TArrow(x->GetXmax()-0.4, ThreshContLV+0.05, x->GetXmax()-0.4, ThreshContLV-0.05, 0.005, ">" );
arrow->SetLineColor(kRed);
arrow->SetFillColor(kRed);
arrow->SetLineWidth(1);
arrow->Draw();
} 
else if(sTitle == "InsulationTest_LV_Resistence"){
  y->SetTitle("R [#Omega]");
  y->SetRangeUser(100,1e+11);
  y->SetNdivisions(8);
  pad->SetLogy(1);
  lineThresh = new TLine(x->GetXmin(), ThreshIsoLV, x->GetXmax(), ThreshIsoLV);
  lineThresh->SetLineColor(46);
  lineThresh->SetLineStyle(kDashed);
  lineThresh->Draw("same");
  textThresh.DrawLatex(0.8, ThreshIsoLV, "threshold");
TArrow *arrow = new TArrow(x->GetXmax()-0.4, ThreshIsoLV+8000, x->GetXmax()-0.4, ThreshIsoLV-8000, 0.005, "<" );
arrow->SetLineColor(kRed);
arrow->SetFillColor(kRed);
arrow->SetLineWidth(1);
arrow->Draw();
}

else if(sTitle == "InsulationTest_HV_Resistence"){
  y->SetTitle("R [#Omega]");
  y->SetRangeUser(100,1e+12);
  y->SetNdivisions(8);
  pad->SetLogy(1);
  lineThresh = new TLine(4*x->GetBinWidth(2),ThreshIsoHV, x->GetXmax(), ThreshIsoHV);
  lineThresh->SetLineColor(46); 
  lineThresh->SetLineStyle(kDashed);
  lineThresh->Draw("same");
  textThresh.DrawLatex(0.8, 1e+06, "Tsensor threshold");
    lineThresh = new TLine(4*x->GetBinWidth(2), 1e+06, x->GetXmax(), 1e+06);

  TLine *lineThreshTsensor = new TLine(x->GetXmin(), 1e+06, 4*x->GetBinWidth(2), 1e+06);
    textThresh.DrawLatex(4.5*x->GetBinWidth(2), ThreshIsoHV, "HV threshold");

  lineThreshTsensor->SetLineColor(46); 
  lineThreshTsensor->SetLineStyle(kDashed);
  lineThreshTsensor->Draw("same");
  TArrow *arrow = new TArrow(x->GetXmax()-0.4, ThreshIsoHV+0.9*1e+09, x->GetXmax()-0.4, ThreshIsoHV-0.9*1e+09, 0.005, "<" );
arrow->SetLineColor(kRed);
arrow->SetFillColor(kRed);
arrow->SetLineWidth(1);
arrow->Draw();
TArrow *arrow2 = new TArrow(4*x->GetBinWidth(2)-0.2, 1e+06-0.5*1e+06, 4*x->GetBinWidth(2)-0.2, 1e+06+0.8*1e+06, 0.005, ">");
arrow2->SetLineColor(kRed);
arrow2->SetFillColor(kRed);
arrow2->SetLineWidth(1);
arrow2->Draw();
}


if(sTitle.find("Passed-Failed") == std::string::npos){
//******Print mean, rms, lenght******************//
for(int j=0; j<IterationTest; j++){
  textMean[j].SetTextSize(0.02);
  textLenght[j].SetTextSize(0.02);
  textStdDev[j].SetTextSize(0.02);
  if(sTitle == "ContinuityTest_ResistenceHV"){
        for(int k=0; k<100; k++){
         if(OverThreshHV[k] != 0 ){
         cout<<"okok"<<endl;
         TLatex textOverThresh;
         textOverThresh.SetTextSize(0.02);
         textOverThresh.DrawLatex((x->GetBinWidth(1)*(k-1)), ThreshContHV-0.25, Form("Over T. %.2e", OverThreshHV[k]) );
       }
      }
      if(IterationTest == 1){
       textStdDev[j].DrawLatex(x->GetXmax()+0.2, mean[j]+0.1, Form("#sigma = %.3f", stdDev[j]));
       textMean[j].DrawLatex(x->GetXmax()+0.2, mean[j], Form("#mu = %.3f" , mean[j]));
       textLenght[j].DrawLatex(x->GetXmax()+0.2, mean[j]-0.1, Form("L = %.3f" , lenght[j]));
    }
     else{
       textMean[j].SetTextColor(j+3);
       textStdDev[j].SetTextColor(j+3);
       textLenght[j].SetTextColor(j+3);
       textStdDev[j].DrawLatex(2*(j+1)*x->GetBinWidth(2), 9.95, Form("#sigma_{%i} = %.3f", j+1, stdDev[j]));
       textMean[j].DrawLatex(2*(j+1)*x->GetBinWidth(2), 9.8, Form("#mu_{%i} = %.3f" ,j+1, mean[j]));
       textLenght[j].DrawLatex(2*(j+1)*x->GetBinWidth(2), 9.65, Form("L_{%i} = %.3f" , j+1,lenght[j]));
       }
  }
  else if(sTitle == "ContinuityTest_ResistenceLV"){
    if(IterationTest == 1){
      textStdDev[j].DrawLatex(x->GetXmax()+0.2, mean[j]+0.04, Form("#sigma = %.3f", stdDev[j]));
      textMean[j].DrawLatex(x->GetXmax()+0.2, mean[j], Form("#mu = %.3f" , mean[j]));
      textLenght[j].DrawLatex(x->GetXmax()+0.2, mean[j]-0.04, Form("L = %.3f" , lenght[j]));
    }
    else{
      textMean[j].SetTextColor(j+3);
      textStdDev[j].SetTextColor(j+3);
      textLenght[j].SetTextColor(j+3);
      textStdDev[j].DrawLatex(2.5*(j+1)*x->GetBinWidth(2), 0.23, Form("#sigma_{%i} = %.3f", j+1, stdDev[j]));
      textMean[j].DrawLatex(2.5*(j+1)*x->GetBinWidth(2), 0.20, Form("#mu_{%i} = %.3f" ,j+1, mean[j]));
      textLenght[j].DrawLatex(2.5*(j+1)*x->GetBinWidth(2), 0.17, Form("L_{%i} = %.3f" , j+1,lenght[j]));
      for(int k=0; k<100; k++){
      if(OverThreshLV[k] != 0){
        TLatex textOverThresh;
        textOverThresh.SetTextSize(0.02);
        textOverThresh.DrawLatex((x->GetBinWidth(1)*(k-1)), ThreshContLV+0.10, Form("Over T. %.2e", OverThreshLV[k]));
      }
     }
    }
  }
  else if(sTitle == "InsulationTest_LV_Resistence" || sTitle == "InsulationTest_HV_Resistence"){
   if(sTitle == "InsulationTest_HV_Resistence"){
    for(int k=0; k<100; k++){
         if(BelowThreshHV[k] != 0 ){
         cout<<"okok"<<endl;
         TLatex textBelowThresh;
         textBelowThresh.SetTextSize(0.02);
         if(k<4){
     //    textBelowThresh.DrawLatex((x->GetBinWidth(1)*(k-1)), 1e+06-0.25*1e+06, Form("Below T. %f", BelowThreshHV[k]) );
         }
         else{
    //      textBelowThresh.DrawLatex((x->GetBinWidth(1)*(k-1)), ThreshIsoHV-0.8*1e+10, Form("Below T. %.2e", BelowThreshHV[k]) );
         }
       }
      }
    }
  else if(sTitle == "InsulationTest_LV_Resistence"){
        for(int k=0; k<100; k++){
         if(BelowThreshLV[k] != 0 ){
         TLatex textBelowThresh;
         textBelowThresh.SetTextSize(0.02);
         textBelowThresh.DrawLatex((x->GetBinWidth(1)*(k-1)), ThreshIsoLV-0.25, Form("Over T. %.2e", BelowThreshLV[k]) );
       }
      }
  }
   if(IterationTest==1){
     textMean[j].DrawLatex(x->GetXmax()+0.2, mean[j], Form("#mu = %.2e" , mean[j]));
     if(sTitle == "InsulationTest_HV_Resistence") textStdDev[j].DrawLatex(x->GetXmax()+0.2, mean[j]-0.3*1e+11, Form("#sigma = %.2e", stdDev[j]));
     else if (sTitle == "InsulationTest_LV_Resistence") textStdDev[j].DrawLatex(x->GetXmax()+0.2, mean[j]-0.1*1e+10, Form("#sigma = %.2e", stdDev[j]));
   }
  else{
    textMean[j].SetTextColor(j+3);
    textStdDev[j].SetTextColor(j+3);
    if(sTitle == "InsulationTest_HV_Resistence"){
     textMean[j].DrawLatex(2.6*(j+1)*x->GetBinWidth(2) + 3*x->GetBinWidth(2) , 1e+03, Form("#mu_{%i} = %.2e", j+1, mean[j]));
     textStdDev[j].DrawLatex(2.6*(j+1)*x->GetBinWidth(2) + 3*x->GetBinWidth(2), 1e+03 - 0.5*1e+03, Form("#sigma_{%i} = %.2e", j+1, stdDev[j]));
     }
    if (sTitle == "InsulationTest_LV_Resistence") {
     textMean[j].DrawLatex(2.95*(j+1)*x->GetBinWidth(2) + x->GetBinWidth(2) , 1e+03, Form("#mu_{%i} = %.2e", j+1, mean[j]));
     textStdDev[j].DrawLatex(2.95*(j+1)*x->GetBinWidth(2) + x->GetBinWidth(2), 1e+03 - 0.5*1e+03, Form("#sigma_{%i} = %.2e", j+1, stdDev[j]));
    }
   
   }
 }
  lineMean[j] = new TLine(x->GetXmin(), mean[j], x->GetXmax(), mean[j]);
  lineMean[j]->SetLineColor(j+3);
  //lineMean[j]->SetLineStyle(2);
  lineMean[j]->SetLineWidth(1);
  lineMean[j]->Draw("same");
}
}

if(IterationTest == 1) c_plot->SaveAs("./output/plots/SingleCable/" + sPDFTitle + ".pdf");
else if(IterationTest >1) c_plot->SaveAs("./output/plots/CheckCable/" + sPDFTitle + ".pdf");

 //std::system("pdftk " + sInputTestDir + name[0] + ".pdf ./output/plots/" + sPDFTitle + ".pdf output ./output/report/" + sPDFTitle + ".pdf" );
}


