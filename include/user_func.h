#ifndef USERFUNC_H
#define USERFUNC_H

#include "def_variables.h"
#include "root.h"

//////////////////////////////////////////////////////////////////////
// Functions defined in the src directory 
void plottingGraph(std::vector<std::pair<std::string, TGraph*>> gr[], std::string Title);
void ReadTestOutput(std::vector<std::string> &TestNameFile, int j);
TGraph* ReadTestTime(std::string pathFile);
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// Functions used only from the main script, source files not needed 
void start(int number_arg, char *argument[]);
void printlogo();
std::vector<std::string> listAndChooseFiles();
void TestType();
Bool_t TimeAcquisition();
std::vector<std::string> listAndChooseFilesTimeAcquisition();
std::vector<std::string> DirTimeAcquisition();
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// print logo and store date and time of execution
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
  std::cout<<"*****************************************"<<std::endl;
 std::system(" mkdir -v ./output && mkdir -v ./output/rootFiles && mkdir -v ./output/plots && mkdir -v ./output/plots/SingleCable && mkdir -v ./output/plots/CheckCable && mkdir -p ./output/report && mkdir -p ./output/plotsTimeResistence" );
 std::cout<<"*****************************************"<<std::endl;
 std::cout<<"Input Directory ---> " ;
 std::cout<<sInputTestDir<<std::endl;
 std::cout<<"Histograms will be saved in -----> "+ std::string(WORKDIR)+ "/output/rootFiles/" << std::endl; 
 std::cout<<"Plots will be saved in -----> " + std::string(WORKDIR) + "/output/plots/" << std::endl; 
 std::cout<<"Final Report will be saved in -----> " + std::string(WORKDIR)+ "/output/report" << std::endl; 
 std::cout<<"****************************************"<<std::endl;
}


//////////////////////////////////////////////////////////////////////
// only for mode 1
std::vector<std::string> listAndChooseFiles() {
    Int_t number_test = 0;
    Int_t number_test2 = 0;
    std::string command, remove_temp;
    TString str;
    std::vector<std::string> TestTemp;
    std::vector<std::pair<Int_t, std::string>> FileName;
    std::vector<std::pair<Int_t, std::string>> FileName2;

    while (number_test != -1) {
        command = "cd " + sInputTestDir + " && ls > tempFilesName.txt";
        remove_temp = "rm " + sInputTestDir + "tempFilesName.txt";
        std::system(command.c_str());

        std::ifstream inputFile((sInputTestDir + "tempFilesName.txt").c_str());
        int count = 0;
        if (inputFile.is_open()) {
            std::string fileName;
            while (std::getline(inputFile, fileName)) {
                ++count;
                if (fileName != "tempFilesName.txt") {
                    FileName.push_back(std::make_pair(count, fileName));
                }
            }
            inputFile.close();
        }
        std::system(remove_temp.c_str());
        std::cout << "************** Which cable to use? (enter number) *****************" << std::endl;
        for (int ii = 0; ii < int(FileName.size()); ++ii) {
            std::cout << "Nr. " << FileName[ii].first << "   " << FileName[ii].second << std::endl;
        }
         std::cout << "*******************************************************************" << std::endl;

        std::cin >> number_test;
        command = "cd " + sInputTestDir + FileName[number_test - 1].second + " && ls *.txt > tempFileName2.txt";

        std::system(command.c_str());

        std::ifstream inputFile2((sInputTestDir + FileName[number_test - 1].second + "/tempFileName2.txt").c_str());
        count = 0;
        if (inputFile2.is_open()) {
            std::string fileName2;
            while (std::getline(inputFile2, fileName2)) {
                ++count;
                if (fileName2 != "tempFileName2.txt") {
                    FileName2.push_back(std::make_pair(count, fileName2));
                }
            }
            inputFile2.close();
        }
        std::system(("rm " + sInputTestDir + FileName[number_test - 1].second + "/tempFileName2.txt").c_str());

        std::cout <<"************** Which test to use? (enter number) ******************" << std::endl;
        for (int i = 0; i < int(FileName2.size()); ++i) {
            std::cout << "Nr. " << FileName2[i].first << "   " << FileName2[i].second << std::endl;
        }
        std::cout << "*******************************************************************" << std::endl;
        std::cin >> number_test2;
        TestTemp.push_back(sInputTestDir + FileName[number_test - 1].second + "/" + FileName2[number_test2 - 1].second);
        TestPath.push_back(sInputTestDir + FileName[number_test - 1].second + "/tmp/processed_" + FileName2[number_test2 - 1].second);
        number_test = 0;
        std::cout << "************** Use other cables for comparison? (y/n) ****************" << std::endl;
        std::cin >> str;
        if (str == "y") {
            number_test = 0;
        } else if (str == "n") {
            number_test = -1;
        }
        std::cout<<"*****************************************"<<std::endl;

        FileName2.clear();
        FileName.clear();
    }
    return TestTemp;
}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// only for mode 1
void TestType() {
    Int_t test_type;
    std::cout << "0 for continuity test, 1 for insulation test, 2 for both: " << std::endl;
    std::cout<<"*****************************************"<<std::endl;
    std::cin >> test_type;
    if (test_type == 0) {
        ContinuityTest = true;
        std::cout << "Plotting Histograms for ----> \033[32mCONTINUITY TEST\033[0m" << std::endl<<std::endl;
    } else if (test_type == 1) {
        InsulationTest = true;
        std::cout << "Plotting Histograms for ----> \033[32mISOLATION TEST\033[0m" << std::endl<<std::endl;
    } else if (test_type == 2) {
        ContinuityTest = true;
        InsulationTest = true;
        std::cout << "Plotting Histograms for ----> \033[32mCONTINUITY TEST && ISOLATION TEST\033[0m" << std::endl;
    }
    if (!ContinuityTest && !InsulationTest) {
        gROOT->ProcessLine(".q");
    }
}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// only for mode 1
Bool_t TimeAcquisition() {
    TString risp;
    Bool_t choice;
    std::cout << "Plot resistance vs time? (y/n)" << std::endl;
    std::cin >> risp;
    risp.ToUpper();
    if (risp == "N") {
        choice = false;
    } 
    else if (risp == "Y") {
    choice = true;    
    }
    

return choice;
}
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// only for TimeRes = true, form command line in mode 1 or from makefile 
std::vector<std::string> DirTimeAcquisition(){
 std::vector<std::string> Directories;
  for(int ii=0; ii<int(TestName.size()); ii++){
        std::string DirValues = TestName[ii].substr(0 , TestName[ii].size()- 31);
        size_t lastSlash = TestName[ii].find_last_of("/");
        size_t lastDot = TestName[ii].find_last_of(".");
        std::string Date = TestName[ii].substr(lastSlash + 9, lastDot - lastSlash - 9);
        Directories.push_back((DirValues + "/VALORI/" +  Date).c_str());
        
    }
 return Directories;
}
//////////////////////////////////////////////////////////////////////


#endif



