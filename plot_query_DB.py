import os
import sys
import subprocess
import ROOT

fInputContinuity = "/home/ldellape/CMScables/stat/continuity_rdf.root"
fInputIsolation = "/home/ldellape/CMScables/stat/isolation_rdf.root"
rdf_Continuity = ROOT.RDataFrame("df_Continuity", fInputContinuity)
rdf_Isolation = ROOT.RDataFrame("df_Isolation", fInputIsolation)

output_dir = "/home/ldellape/cableDB/static/"
ROOT.gErrorIgnoreLevel = ROOT.kError

def PlotHistogram(type, variable, condition, title):
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
        (title, title, Nbins, rdf.Max(variable).GetValue(), rdf.Min(variable).GetValue()), 
        variable
    )
    canvas = ROOT.TCanvas(title,title, 1000, 1000)
    hist.Draw()
    hist.GetXaxis().SetTitle("R[#Omega]")
    hist.GetYaxis().SetTitle("Entries")
    output_image = f"{output_dir}{title}.png"
    canvas.SaveAs(output_image)
    print(output_image)
    return output_image


def BuiltCondition(query):
    
    split_query = query.split("_")

    cable = split_query[0].split(":")[1] 
    type = split_query[1].split(":")[1]  
    condition = split_query[2].split(":")[1].replace("_AND_", " && ")  

    filter_conditions = [f"{cable}_test_{type} == true"]

    channels = condition.split(" && ")
    for channel in channels:
        filter_conditions.append(f"channel{channel}_{type} == true")

    result = " && ".join(filter_conditions)

    variable = f"resistence{type}"
    title = f"h_{query}"
    
    return PlotHistogram(type, variable, result, title)

if __name__ == "__main__":
    if len(sys.argv) != 2: 
        sys.exit(1)
    query = sys.argv[1]
    BuiltCondition(query)
