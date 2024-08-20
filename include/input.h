#ifndef INPUT_H
#define INPUT_H

// functions to be called when the second mode is selected

// list of commands to be execute //
std::vector<std::string> listAndChooseFiles();
void TestType();
Bool_t TimeAcquisition();
std::vector<std::string> listAndChooseFilesTimeAcquisition();
std::vector<std::string> DirTimeAcquisition();



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

void TestType() {
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


#endif