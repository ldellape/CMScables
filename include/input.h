#include<cstdlib>
#ifndef INPUT_H
#define INPUT_H

#define linux
//#define windows

// list of commands to be execute //


std::vector<std::string> listAndChooseFiles() {
    std::cout << sInputTestDir << std::endl;
    Int_t number_test = 0;
    Int_t number_test2 = 0;
    Int_t first_choice;
    std::string command, remove_temp;
    TString str;
    std::vector<std::string> TestTemp;
    std::vector<std::pair<Int_t, std::string>> FileName;
    std::vector<std::pair<Int_t, std::string>> FileName2;

    while (number_test != -1) {
        // Platform-specific command to list files and store in tempFilesName.txt
#ifdef linux
        command = "cd " + sInputTestDir + " && ls > tempFilesName.txt";
        remove_temp = "rm " + sInputTestDir + "tempFilesName.txt";
#elif defined _WIN32
        command = "powershell Get-ChildItem -Path " + sInputTestDir + " -Name > " + sInputTestDir + "tempFilesName.txt";
        remove_temp = "powershell Remove-Item -Path " + sInputTestDir + "tempFilesName.txt";
#endif
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
        for (int ii = 0; ii < FileName.size(); ++ii) {
            std::cout << "Nr. " << FileName[ii].first << "   " << FileName[ii].second << std::endl;
        }
         std::cout << "*******************************************************************" << std::endl;

        std::cin >> number_test;

#ifdef linux
        command = "cd " + sInputTestDir + FileName[number_test - 1].second + " && ls *.txt > tempFileName2.txt";
#elif defined _WIN32
        command = "powershell Get-ChildItem -Path " + sInputTestDir + FileName[number_test - 1].second + " -Filter \"*.txt\" -Name > " + sInputTestDir + FileName[number_test - 1].second + "\\tempFileName2.txt";
#endif
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
#ifdef linux
        std::system(("rm " + sInputTestDir + FileName[number_test - 1].second + "/tempFileName2.txt").c_str());
#elif defined _WIN32
        std::system(("powershell Remove-Item -Path " + sInputTestDir + FileName[number_test - 1].second + "\\tempFileName2.txt").c_str());
#endif

        for (int i = 0; i < FileName2.size(); ++i) {
            std::cout << "Nr. " << FileName2[i].first << "   " << FileName2[i].second << std::endl;
        }
        std::cout << "Which test (enter number)? " << std::endl;
        std::cin >> number_test2;
        TestTemp.push_back(sInputTestDir + FileName[number_test - 1].second + "/" + FileName2[number_test2 - 1].second);
        number_test = 0;
        std::cout << "************** Use other cables for comparison? (y/n) ****************" << std::endl;
        std::cin >> str;
        if (str == "y") {
            number_test = 0;
        } else if (str == "n") {
            number_test = -1;
        }
        FileName2.clear();
        FileName.clear();
    }
    return TestTemp;
}

void TestType() {
    std::cout << "0 for continuity test, 1 for insulation test, 2 for both: " << std::endl<<std::endl;
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

void TimeAcquisition() {
    TString risp;
    std::cout << "Plot resistance vs time? (y/n)" << std::endl;
    std::cin >> risp;
    risp.ToUpper();
    if (risp == "n") {
        return;
    } else if (risp == "y") {
        Ins_Time = true;
    }
}

#endif