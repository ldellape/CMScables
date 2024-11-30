import os
import sys
import subprocess
import ROOT
from ROOT import TTree
import argparse  

fInputContinuity = "/home/ldellape/CMScables/stat/continuity_rdf.root"
fInputIsolation = "/home/ldellape/CMScables/stat/isolation_rdf.root"
rdf_Continuity = ROOT.RDataFrame("df_Continuity", fInputContinuity)
rdf_Isolation = ROOT.RDataFrame("df_Isolation", fInputIsolation)

output_dir = "/home/ldellape/cableDB/static"

ROOT.gErrorIgnoreLevel = ROOT.kError
def PlotHistogram(type, variable, condition, title, WriteFile=False):
    if "Passed" in variable or "Failed" in variable:
        Nbins = 2  
    else:
        Nbins = 50 
    
    if type == "Ins":
        rdf = rdf_Isolation
    elif type == "Con":
        rdf = rdf_Continuity
    else:
        return
    hist = rdf.Filter(condition).Histo1D(
        (title, title, Nbins, rdf.Filter(condition).Max(variable).GetValue(), rdf.Filter(condition).Min(variable).GetValue()), 
        variable
    )
    canvas = ROOT.TCanvas(title, title, 1000, 1000)
    hist.Draw()
    hist.GetXaxis().SetTitle("R[#Omega]")
    hist.GetYaxis().SetTitle("Entries")
    output_image = f"{output_dir}/{title}.png"
    canvas.SaveAs(output_image)
    print(output_image)

    if WriteFile:  
        output_data = f"{output_dir}/data.txt"
        with open(output_data, "w") as file:
            Entries = rdf.Count().GetValue()
            columns = rdf.GetColumnNames()
            file.write(rdf.Filter(condition).Display(list(columns), Entries).AsString())
        with open(output_data, "r+") as file:
            lines = file.readlines() 
            new_lines = []
            new_lines.append(f"filter: {condition}")
            for line in lines:
                line = line.strip() 
                line = line.replace("true", "1") 
                line = line.replace("false", "0")  
                line = line.replace("-", "")
                line= line.replace("+", "")
                line = line.replace("|", "")
                new_lines.append(line)
            file.seek(0)
            file.write("\n".join(new_lines) + "\n") 
            file.truncate() 
            print(output_data)
            return output_image, output_data
    else:
        return output_image


def BuiltCondition(query, WriteFile=False):  
    split_query = query.split("&")
    cable = split_query[0].split(":")[1] 
    type = split_query[1].split(":")[1]  
    condition = split_query[2].split(":")[1]
    channels = condition.split("_AND_")

    filter_conditions = [f"{cable}_test_{type} == true"]

    channel_conditions = []
    for channel in channels:
        channel_conditions.append(f"channel{channel}_{type} == true")

    channel_condition_str = " || ".join(channel_conditions)

    filter_conditions.append(f"({channel_condition_str})")

    result = " && ".join(filter_conditions)

    variable = f"resistence{type}"
    title = f"h_{query}"

    return PlotHistogram(type, variable, result, title, WriteFile)


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Process data and plot histograms.")
    parser.add_argument("query", help="The query string to process.")
    parser.add_argument("--write", action="store_true", help="Enable writing to file (test.txt).")

    args = parser.parse_args()

    BuiltCondition(args.query, WriteFile=args.write)
