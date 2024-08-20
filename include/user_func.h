// identified by pair of letter and number //
#ifndef USERFUNC_H
#define USERFUNC_H
#include "def_variables.h"
#include "root.h"

//***** functions defined in src directory *****//
void plottingGraph(std::vector<std::pair<std::string, TGraph*>> gr[], std::string Title);
void ReadTestOutput(std::vector<std::string> &TestNameFile, int j);
TGraph* ReadTestTime(std::string pathFile);
// ******************************************* //


void printlogo(){    
    TDatime dt;
    int day = dt.GetDay();
    int year = dt.GetYear();
    int month = dt.GetMonth();
    int hour = dt.GetHour();
    int minute = dt.GetMinute();
    int second = dt.GetSecond();
    
    // Format the date and time string
    currentDate = Form("%02d_%02d_%04d_%02d_%02d_%02d", day, month, year, hour, minute, second);    
    const std::string red = "\033[33m";
    const std::string green = "\033[92m";
    // ANSI escape code to reset the color
    const std::string reset = "\033[0m";
    
    const std::string textCMS =  R"(
************************************************************ 
*                                                          *
*  CMS Cables                   ÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆ          *
*  Sezione di Perugia    ÆÆÆÆÆÆÆÆ                ÆÆÆÆ      *
*                  ÆÆÆÆÆÆ                          ÆÆ      *
*              ÆÆÆÆÆ                                Æ      *
*            ÆÆÆÆÆ                                   Æ     * 
*        ÆÆÆÆÆ       ÆÆ   ÆÆ     Æ   ÆÆÆÆÆÆÆ  ÆÆÆ    ÆÆ    *
*     ÆÆÆÆ           ÆÆ   ÆÆÆ    Æ   ÆÆ       ÆÆÆÆ   ÆÆ    *
*   ÆÆÆÆ             ÆÆ   ÆÆ ÆÆ  Æ   ÆÆÆÆÆÆÆ  ÆÆ ÆÆ  ÆÆ    *
*  ÆÆÆÆÆ             ÆÆ   ÆÆ  ÆÆ Æ   ÆÆ       ÆÆ  ÆÆÆÆÆ    *
* ÆÆÆÆÆ              ÆÆ   ÆÆ   ÆÆÆ   ÆÆ       ÆÆ    ÆÆÆ    *
* ÆÆÆÆÆ              ÆÆ   ÆÆ    ÆÆ   ÆÆ       ÆÆ     ÆÆ    *
* ÆÆÆÆÆÆ                                                   *
*   ÆÆÆÆÆÆ  https://github.com/ldellape/CMScables   v.4.0  *   
*      ÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆÆ                                   *                                                         
* )"+currentDate + R"(                                      *
************************************************************ )"; 
                              

std::cout << green << textCMS  << reset << std::endl;
std::cout<< green << "Test su cavo PS-PP1" << reset <<std::endl;
std::system("mkdir -p ./output/plots && mkdir -p ./output/plots/SingleCable && mkdir -p ./output/plots/CheckCable && mkdir -p ./output/report && mkdir -p ./output/plotsTimeResistence" );
std::cout<<"*****************************************"<<std::endl;
std::cout<<"Input Directory ---> " ;
std::cout<<sInputTestDir<<std::endl;
std::cout<<"Histograms will be saved in -----> "+ std::string(WORKDIR)+ "/output/rootFiles/" << std::endl; 
std::cout<<"Plots will be saved in -----> " + std::string(WORKDIR) + "/output/plots/" << std::endl; 
std::cout<<"Final Report will be saved in -----> " + std::string(WORKDIR)+ "/output/report" << std::endl; 
std::cout<<"****************************************"<<std::endl;
}

void start(int number_arg, char *argument[]){
    Bool_t ValidOption= false;
    for (int i = 1; i < number_arg; ++i) {
    std::string arg = argument[i];
    std::string inputPath;
    std::string outputPath;
    if(arg== "--help" || arg=="-h"){
        std::cout<<" CMScables --help, -h : print this message"<<std::endl;
        std::cout<<" CMScables --doc, -D : print documentation on what it is expected as input and what you can expect as output"<<std::endl;
 std::cout<<" CMScables --input/-I <path> --mode/-M [OPTION]: perform test giving the input text file, path should be of type CableXX/TEST_NAME.txt " << std::endl;
        std::cout<<"[OPTION] = 0 for continuity test, 1 for isolation test, 2 for both. Default is continuity test" <<std::endl;
        gROOT->ProcessLine(".q");
    }
    else if(arg == "--doc" || arg == "-D"){
        std::ifstream readme(".doc");
        std::string readme_line;
        while(std::getline(readme, readme_line)){
        if(readme_line.find("COSA SERVE:") != std::string::npos ){
            break;
        }
        else std::cout<<readme_line<<std::endl;
        }
        readme.close();
        gROOT->ProcessLine(".q");
    }
    else if(arg == "--input" || arg=="-I") {
     while (i + 1 < number_arg && argument[i + 1][0] != '-') {
                inputPath = argument[++i];
                TestName.push_back((sInputTestDir + inputPath).c_str());
            }
    CommandLine = true;
    ValidOption=true;
    Ins_Time = false;
    } 
    else if((arg == "--mode" || arg=="-M") && CommandLine==true){
        test_type = std::stoi(argument[++i]);
        if(test_type==0){
         std::cout << "Plotting Histograms for ----> \033[32mCONTINUITY TEST\033[0m" << std::endl<<std::endl;
         ContinuityTest=true;
        }
        else if(test_type==1){
         InsulationTest=true;
         std::cout << "Plotting Histograms for ----> \033[32mISOLATION TEST\033[0m" << std::endl<<std::endl;
        }
        else if(test_type ==2){
         ContinuityTest=true;
         InsulationTest=true;
         std::cout << "Plotting Histograms for ----> \033[32mCONTINUITY TEST && ISOLATION TEST\033[0m" << std::endl;
        }
        else if(test_type != 0 && test_type != 1 && test_type != 2){
        std::cout<<"Mode not found! "<<std::endl;
        std::cout<<"possible tests are: "<<std::endl;
        std::cout<<"0 for continuity, 1 for isolation, 2 for both"<<std::endl;
        gROOT->ProcessLine(".q");
        }
    }
    else if(!ValidOption){
        std::cout<<" command not found, possible command are: "<<std::endl;
        std::cout<<" CMScables --help, -h : print this message"<<std::endl;
        std::cout<<" CMScables --doc, -D : print documentation on what it is expected as input and what you can expect as output"<<std::endl;
        std::cout<<" CMScables --input/-I <path> --mode/-M [OPTION]: perform test giving the input text file, path should be of type CableXX/TEST_NAME.txt " << std::endl;
        std::cout<<"[OPTION] = 0 for continuity test, 1 for isolation test, 2 for both. Default is continuity test" <<std::endl;
        gROOT->ProcessLine(".q");
    }
    }

}



#endif
