#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include "TROOT.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TFile.h"
#include "TLatex.h"
#include "TTree.h"
#include "TSystem.h"
#include "TProfile.h"
#include "ROOT/RDataFrame.hxx"

std::string output_dir = "./../cableDB/static";

std::string join(const std::vector<std::string>& vec, const std::string& separator) {
    std::string result;
    for (size_t i = 0; i < vec.size(); ++i) {
        result += vec[i];
        if (i != vec.size() - 1) {
            result += separator;
        }
    }
    return result;
}

void PlotHistogram(const std::string& type, const std::string& variable, const std::string& condition, const std::string& title, bool WriteFile = false) {
    int Nbins = (variable.find("Passed") != std::string::npos || variable.find("Failed") != std::string::npos) ? 2 : 50;
    ROOT::RDataFrame rdf_Continuity("df_Continuity", "/home/ldellape/CMScables/stat/continuity_rdf.root");
    ROOT::RDataFrame rdf_Isolation("df_Isolation", "/home/ldellape/CMScables/stat/isolation_rdf.root");
    
    ROOT::RDataFrame rdf = (type == "Ins") ? rdf_Isolation : (type == "Con") ? rdf_Continuity : rdf_Continuity;
    
    auto hist = rdf.Filter(condition.c_str()).Histo1D({title.c_str(), title.c_str(), Nbins, rdf.Filter(condition.c_str()).Max(variable.c_str()).GetValue(), rdf.Filter(condition.c_str()).Min(variable.c_str()).GetValue()}, variable.c_str());
    
    TCanvas canvas(title.c_str(), title.c_str(), 1000, 1000);
    hist->Draw();
    hist->GetXaxis()->SetTitle("R[#Omega]");
    hist->GetYaxis()->SetTitle("Entries");
    
    std::string output_image = output_dir + "/" + title + ".png";
    canvas.SaveAs(output_image.c_str());
    std::cout << output_image << std::endl;

    if (WriteFile) {
        std::string output_data = output_dir + "/rawdata_" + condition + ".txt";
        std::ofstream file(output_data);
        auto entries = rdf.Count().GetValue();
        auto columns = rdf.GetColumnNames();
        file << rdf.Filter(condition.c_str()).Display(columns, entries)->AsString();
        
        std::ifstream read_file(output_data);
        std::string line;
        std::vector<std::string> new_lines;
        new_lines.push_back("filter: " + condition);
        while (std::getline(read_file, line)) {
            std::replace(line.begin(), line.end(), '-', ' '); // Remove '-' and '+' as in the original scrit
            std::replace(line.begin(), line.end(), '+', ' ');
            std::replace(line.begin(), line.end(), '|', ' ');
            std::string replaced_line = line;
            std::replace(replaced_line.begin(), replaced_line.end(), 't', '1'); // replacing 't' and 'f'
            std::replace(replaced_line.begin(), replaced_line.end(), 'f', '0');
            new_lines.push_back(replaced_line);
        }
        std::ofstream write_file(output_data);
        for (const auto& new_line : new_lines) {
            write_file << new_line << std::endl;
        }
        
        std::cout << output_data << std::endl;
    }
}

std::string BuiltCondition(const std::string& query, bool WriteFile = false) {
    std::istringstream ss(query);
    std::string segment;
    std::vector<std::string> segments;
    
    while (std::getline(ss, segment, '&')) {
        segments.push_back(segment);
    }

    std::string cable = segments[0].substr(segments[0].find(":") + 1);
    std::string type = segments[1].substr(segments[1].find(":") + 1);
    std::string condition = segments[2].substr(segments[2].find(":") + 1);
    std::cout<<condition<<std::endl;

    std::vector<std::string> filter_conditions;
    filter_conditions.push_back(cable + "_test_" + type + " == true");
    
    std::vector<std::string> channel_conditions;
    
    std::istringstream condition_ss(condition);
    std::string part;
    while (std::getline(condition_ss, part, '_')) {
        if (part == "AND") {
            channel_conditions.push_back(" || ");
        } else {
            channel_conditions.push_back("channel" + part + "_" + type + " == true");
        }
    }

    std::string channel_condition_str = join(channel_conditions, " "); 
    std::cout<<channel_condition_str<<std::endl;
    filter_conditions.push_back("(" + channel_condition_str + ")");
    
    std::string full_condition = join(filter_conditions, " && ");
    std::cout<<full_condition<<std::endl;
    std::string variable = "resistence" + type;
    std::string title = "h_" + query;
    
    PlotHistogram(type, variable, full_condition, title, WriteFile);
    
    return title;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <query> [--write]" << std::endl;
        return 1;
    }
    bool WriteFile = false;
    if (argc == 3 && std::string(argv[2]) == "--write") {
        WriteFile = true;
    }
    std::string query=argv[1];
    BuiltCondition(query, WriteFile);
    return 0;
}

