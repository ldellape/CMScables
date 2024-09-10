#include <string>
#include "../include/def_variables.h"
#include "../include/root.h"

namespace Python{
 namespace PSPP1{
    // change text file mfrom ceetis, new files store in temporarly dirs
    void ChangeTextFile(std::string TestName){
      std::string pythonScript = "python3 " + std::string(WORKDIR) + "/py/ManageTXT.py ";
      std::system((pythonScript + TestName).c_str());
      return;
    }


    // changing .INI files from CEETIS for time vs resistenc plots 
    void ChangeTextFileINI(std::string path){
      std::string PythonINIcommand = "python3 " + std::string(WORKDIR) +"/py/ManageINI.py " + path + " 2>/dev/null";
      std::system(PythonINIcommand.c_str());
      return;
    }

    // building final pdf file
    void WriteFinalReport(std::string PDFname, std::string CableName){
      std::string command;
      std::cout<<"creating Final Report..." <<std::endl;
      std::cout<<"*****************************************"<<std::endl;
      std::string pythonScript = "python3 " + std::string(WORKDIR) + "/py/WriteReport.py ";
      std::string sOutputReport = std::string(WORKDIR) + "/output/report/Report_" + PDFname + ".pdf ";
      std::string sInputPDFCeetis = std::string(WORKDIR) + "/input/pdf_ceetis/" + CableName + ".pdf ";
      std::string sInputPlotsSingle = std::string(WORKDIR) + "/output/plots/SingleCable/" + PDFname + ".pdf ";
      std::string sInputPlotsCheck = std::string(WORKDIR)  + "/output/plots/CheckCable/" + PDFname + ".pdf ";
      std::string sInputPlotsTimeResLV = std::string(WORKDIR) + "/output/plotsTimeResistence/graph_TimeResistenceLV_" + currentDate + "_Cable0.pdf ";
      std::string sInputPlotsTimeResLVR = std::string(WORKDIR) + "/output/plotsTimeResistence/graph_TimeResistenceLVR_" + currentDate + "_Cable0.pdf ";
      if(IterationTest > 1){
        for(int i=1; i<IterationTest; i++){
         sInputPlotsTimeResLV+= " " + std::string(WORKDIR) + "/output/plotsTimeResistence/graph_TimeResistenceLV_" + currentDate + "_Cable"+std::to_string(i)+".pdf ";
         sInputPlotsTimeResLVR+= " " + std::string(WORKDIR) + "/output/plotsTimeResistence/graph_TimeResistenceLVR_" + currentDate + "_Cable" +std::to_string(i)+ ".pdf ";
        }
      }
      if( IterationTest == 1 && Ins_Time){
        command = pythonScript + sOutputReport + sInputPDFCeetis 
        + sInputPlotsSingle + sInputPlotsTimeResLV + sInputPlotsTimeResLVR;
      }
      else if( IterationTest > 1 && Ins_Time){
        command = pythonScript + sOutputReport + sInputPDFCeetis 
        + sInputPlotsCheck + sInputPlotsTimeResLV + sInputPlotsTimeResLVR;
      }
      else if( IterationTest == 1 && !Ins_Time){
        command = pythonScript + sOutputReport + sInputPDFCeetis 
        + sInputPlotsSingle;
      }
      else if( IterationTest >1 && !Ins_Time){
        command = pythonScript + sOutputReport + sInputPDFCeetis 
        + sInputPlotsCheck;
      }
      std::cout<<command<<std::endl;
      std::system(command.c_str());
      std::cout<<"*****************************************"<<std::endl;
      std::cout<<"\033[32mFinal REPORT saved as "<< std::string(WORKDIR) <<"/output/report/Report_"+ PDFname +"\033[0m" <<std::endl;
      return;
    }

    // updating the HTML page with new processed test (push once a day)
    void UpdateHTML(std::string ReportTitle){
      std::system(" mv index.html index.txt");
      std::string command;
      command = "python3 " + std::string(WORKDIR) +"/py/UpdateHTML.py index.txt Report_" + ReportTitle+".pdf";
      std::system(command.c_str());
      std::system(" mv index.txt index.html");
    }
 }//namespace PSPP1

 namespace Octopus{

 }//namespace Octopus 
}//namespace python 

