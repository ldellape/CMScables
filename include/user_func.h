#ifndef USERFUNC_H
#define USERFUNC_H

#include "def_variables.h"
#include "root.h"

//////////////////////////////////////////////////////////////////////
// Functions defined in the src directory 
void plottingGraph(std::vector<std::pair<std::string, TGraph*>> gr[], std::string Title);
void ReadTestOutput( TString path, TString option = "NONE");
//////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////
// Functions used only on the main script
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
    currentDate = Form("%02d/%02d/%04d  %02d:%02d:%02d", day, month, year, hour, minute, second);    
    const std::string red = "\033[33m";
    const std::string blu = "\033[36m";
    // ANSI escape code to reset the color
    const std::string reset = "\033[0m";
    
     const std::string blue = "\033[36m";
    const std::string yellow = "\033[33m";

    std::cout << blue <<

    "**********************************************************************************\n"
    "*"<<yellow<<"                                           "<<blue<<" ____ __  __ ____                    *\n"                                                                                                        
    "*"<<yellow<<"              *%@@@@@@@@@@%@@              "<<blue<<"/ ___|  \\/  / ___|                   *\n"      
    "*"<<yellow<<"             =@%. -@-@@@@@@@@@@%.         "<<blue<<"| |   | |\\/| \\___ \\                   *\n" 
    "*"<<yellow<<"            -@@* %@@@@@@@@@@@@@@@@%       "<<blue<<"| |___| |  | |___) | CABLES           *\n"
    "*"<<yellow<<"            %@@ @@@@@@@@@@@@@@@@@@@@@%.   "<<blue<<"\\ ____|_|  |_|____/                   *\n"
    "*"<<yellow<<"            -@- *@@@@@@@@@@@@@@@@%   -@@%                   =%@@@@@@@@@*"<<blue<<"        *\n"
    "*"<<yellow<<"             +@% -@@@@@@@@@@@@@@@  \033[31m**"<<yellow<<"  .@@                *@@@+       =%@*"<<blue<<"      *\n"
    "*"<<yellow<<"               @@@@@@@@@@@@@@@@@@@%-@@@@@              %@@@+           *%="<<blue<<"      *\n"
    "*"<<yellow<<"                  =%@@@@@@%:@@#@@@@@@@@@@@  #=:.@@:-#@@@@*   .+@@@@@%:  %#"<<blue<<"      *\n"
    "*"<<yellow<<"                             #@@@@@@@@@@@@@-##%@@@@@@@%=   %@@%*    =%* %%"<<blue<<"      *\n"
    "*"<<yellow<<"                           +@@@@@@@@@@@@@@@@@          -@@@-         +#%*"<<blue<<"       *\n"
    "*"<<yellow<<"    -@@@@@@@@@@@@@@*--*@@@@@#@@@% %@@= @@@  @@@@@@##@@@@@%            +%#"<<blue<<"       *\n"
    "*"<<yellow<<"   %@@         +@@@@@@@@@@: .@@%  @@@+ #@@@  -@@@@@@@@*            -=%%%"<<blue<<"        *\n"
    "*"<<yellow<<"  *@*                      %@@%   @@@-  %@@@                        =++-"<<blue<<"        *\n"
    "*"<<yellow<<"  #@=                    *@@@-   +@@@    #@@@@                          "<<blue<<"        *\n"
    "*"<<yellow<<"  *@+                 .%@@@*     @@@%     -@@@@@%        -@@@@@@@@@%    "<<blue<<"        *\n"
    "*"<<yellow<<"   %@*          =*#@@@@@%       %@@@-        =@@@@@@@@@@@@@       .@@+  "<<blue<<"        *\n"
    "*"<<yellow<<"    -@@#    -@@@@@@#+-         @@@@         %= @@-=++==. @@@        #%  "<<blue<<"        *\n"
    "*"<<yellow<<"       .- :@@@+              =@@@%            .@  #= =%*@@          *%  "<<blue<<"        *\n"
    "*"<<yellow<<"         .@@+               =@@@-     %@@@%.         :+#@@%#-      +%.  "<<blue<<"        *\n"
    "*"<<yellow<<"         @@*               .@@@          -@@#           #@%%%%+  =-     "<<blue<<"        *\n"
    "*"<<yellow<<"         @@-     :+:       #@@%           =@@                %:         "<<blue<<"        *\n"
    "*"<<yellow<<"         -@@        %*      @@@:          %%%       -%%%%%%@%.          "<<blue<<"        *\n"
    "*"<<yellow<<"          =@@+     .%*      -@@@#       =%@#                            "<<blue<<"        *\n"
    "*"<<yellow<<"            *@@@@@@%*         %@@@@@@@@@@%.                             "<<blue<<"        *\n"
    "*"<<yellow<<"   "<<blue<<"___ _____ ____  _                    ____"<<yellow<<"      "<<blue<<"                              *\n"                              
    "*"<<yellow<<"  "<<blue<<"/ _ \\_   _|  _ \\| |__   __ _ ___  ___|___ \\"<<yellow<<"  "<<blue<<"                                 *\n"                                 
    "*"<<yellow<<" "<<blue<<"| | | || | | |_) | '_ \\ / _` / __|/ _ \\ __) |"<<yellow<<"  "<<blue<<"                                *\n"                                
    "*"<<yellow<<" "<<blue<<"| |_| || | |  __/| | | | (_| \\__ \\  __// __/ "<<yellow<<"        "<<currentDate<<blue<<"      *\n"    
    "*"<<yellow<<"  "<<blue<<"\\___/ |_| |_|   |_| |_|\\__,_|___/\\___|_____|"<<yellow<<"        INFN Sez.Perugia       "<<blue<<"   *\n"    
    "*"<<yellow<<"                                          https://github.com/ldellape/CMScables "<<blue<<"*\n" 
    "**********************************************************************************\n"

    <<reset;
    
 std::system(" mkdir -pv ./output/rootFiles");
 std::system(" mkdir -pv ./output/plots/SingleCable");
 std::system(" mkdir -pv ./output/plots/CheckCable");
 std::system("mkdir -pv ./output/report");
 std::system("mkdir -pv ./output/plotsTimeResistence" );
 std::cout<<"*******************************************************************"<<std::endl;
 std::cout<<"Input Directory ---> " ;
 std::cout<<sInputTestDir<<std::endl;
 std::cout<<"Histograms will be saved in -----> "+ std::string(WORKDIR)+ "/output/rootFiles/" << std::endl; 
 std::cout<<"Plots will be saved in -----> " + std::string(WORKDIR) + "/output/plots/" << std::endl; 
 std::cout<<"Final Report will be saved in -----> " + std::string(WORKDIR)+ "/output/report" << std::endl; 
 std::cout<<"*******************************************************************"<<std::endl;
}


//////////////////////////////////////////////////////////////////////
// only for mode 1
std::vector<std::string> listAndChooseFiles() {
    Int_t number_test = 0;
    Int_t number_test2 = 0;
    Int_t number_test3 = 0; 
    std::string command, remove_temp;
    TString str;
    std::vector<std::string> TestTemp;
    std::vector<std::pair<Int_t, std::string>> DirectoryName; 
    std::vector<std::pair<Int_t, std::string>> FileName;
    std::vector<std::pair<Int_t, std::string>> FileName2;

    while (number_test != -1) {
        // Step 1: List all main directories inside sInputTestDir
        command = "cd " + sInputTestDir + " && ls -d */ > tempDirectoryName.txt";
        remove_temp = "rm " + sInputTestDir + "tempDirectoryName.txt";
        std::system(command.c_str());

        std::ifstream inputDirFile((sInputTestDir + "tempDirectoryName.txt").c_str());
        int count = 0;
        if (inputDirFile.is_open()) {
            std::string dirName;
            while (std::getline(inputDirFile, dirName)) {
                ++count;
                dirName.pop_back(); 
                DirectoryName.push_back(std::make_pair(count, dirName));
            }
            inputDirFile.close();
        }
        std::system(remove_temp.c_str());

        std::cout << "************** Choose directory to use: (enter number) **************" << std::endl;
        for (size_t i = 0; i < DirectoryName.size(); ++i) {
            std::cout << "Nr. " << DirectoryName[i].first << "   " << DirectoryName[i].second << std::endl;
        }
        std::cout << "*********************************************************************" << std::endl;
        std::cin >> number_test3;

        command = "cd " + sInputTestDir + DirectoryName[number_test3 - 1].second + " && ls > tempFilesName.txt";
        std::system(command.c_str());

        std::ifstream inputFile((sInputTestDir + DirectoryName[number_test3 - 1].second + "/tempFilesName.txt").c_str());
        count = 0;
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
        std::system(("rm " + sInputTestDir + DirectoryName[number_test3 - 1].second + "/tempFilesName.txt").c_str());

        std::cout << "************** Which cable to use? (enter number) *****************" << std::endl;
        for (size_t ii = 0; ii < FileName.size(); ++ii) {
            std::cout << "Nr. " << FileName[ii].first << "   " << FileName[ii].second << std::endl;
        }
        std::cout << "*******************************************************************" << std::endl;
        std::cin >> number_test;

        command = "cd " + sInputTestDir + DirectoryName[number_test3 - 1].second + "/" + FileName[number_test - 1].second + " && ls *.txt > tempFileName2.txt";
        std::system(command.c_str());

        std::ifstream inputFile2((sInputTestDir + DirectoryName[number_test3 - 1].second + "/" + FileName[number_test - 1].second + "/tempFileName2.txt").c_str());
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
        std::system(("rm " + sInputTestDir + DirectoryName[number_test3 - 1].second + "/" + FileName[number_test - 1].second + "/tempFileName2.txt").c_str());

        std::cout <<"************** Which test to use? (enter number) ******************" << std::endl;
        for (size_t i = 0; i < FileName2.size(); ++i) {
            std::cout << "Nr. " << FileName2[i].first << "   " << FileName2[i].second << std::endl;
        }
        std::cout << "*******************************************************************" << std::endl;
        std::cin >> number_test2;

        TestTemp.push_back(sInputTestDir + DirectoryName[number_test3 - 1].second + "/" + FileName[number_test - 1].second + "/" + FileName2[number_test2 - 1].second);
        TestPath.push_back(sInputTestDir + DirectoryName[number_test3 - 1].second + "/" + FileName[number_test - 1].second + "/tmp/processed_" + FileName2[number_test2 - 1].second);

        std::cout << "************ Use other cables for comparison? (y/n) ***************" << std::endl;
        std::cin >> str;
        if (str == "y") {
            number_test = 0;
        } else if (str == "n") {
            number_test = -1;
        }
        std::cout << "*******************************************************************" << std::endl;

        FileName2.clear();
        FileName.clear();
        DirectoryName.clear();
    }
    return TestTemp;
}

//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
// only for mode 1
void TestType() {
    Int_t test_type;
    std::cout << "0 for continuity test, 1 for insulation test, 2 for both: ";
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
        std::cout<<"*******************************************************************"<<std::endl;
        std::cout << "\033[32mPlotting Histograms for ----> CONTINUITY TEST && ISOLATION TEST\033[0m" << std::endl;
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
    std::cout<< "*******************************************************************"<<std::endl;
    std::cout << "Plot LV channels resistence vs time? (y/n)" << std::endl;
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


//////////////////////////////////////////////////////////////////////
// command line inputs 
void start(int number_arg, char *argument[]){
     std::system(" mkdir -pv ./output/rootFiles");
     std::system(" mkdir -pv ./output/plots/SingleCable");
     std::system(" mkdir -pv ./output/plots/CheckCable");
     std::system("mkdir -pv ./output/report");
     std::system("mkdir -pv ./output/plotsTimeResistence" );
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
                std::cout<<inputPath<<std::endl;
                TestName.push_back((sInputTestDir + inputPath).c_str());
                std::size_t LastSlash = inputPath.find_last_of("/");
                std::string cable = inputPath.substr(0, LastSlash);
                std::string test = inputPath.substr(LastSlash+1);
                TestPath.push_back((sInputTestDir + cable + "/tmp/processed_" + test).c_str());
                std::cout<<test<<std::endl;
            }
    CommandLine= true;
    ValidOption=true;
    Ins_Time = false;
    } 
    else if((arg == "--mode" || arg=="-M") && CommandLine==true){
        int test_type = std::stoi(argument[++i]);
        if(test_type==0){
         std::cout << "Plotting Histograms for ----> \033[32mCONTINUITY TEST\033[0m" << std::endl<<std::endl;
         ContinuityTest=true;
         InsulationTest=false;
        }
        else if(test_type==1){
         InsulationTest=true;
         ContinuityTest=false;
         std::cout << "Plotting Histograms for ----> \033[32mISOLATION TEST\033[0m" << std::endl<<std::endl;
        }
        else if(test_type ==2){
         ContinuityTest=true;
         InsulationTest=true;
         Ins_Time = true;
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



