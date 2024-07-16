#include "../include/root.h"
#include "../include/def_variables.h"
void Stat_Cables(Int_t NumberCable){
    std::string directory = std::string(WORKDIR) + Form("/input/FULL_TEST_su_cavo_ps_pp1_V3/Cable0%i/",NumberCable);
    std::vector<std::string> Tests;
    command = "cd " + directory + " && ls > tempFilesName.txt";







}