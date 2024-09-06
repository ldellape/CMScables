#ifndef PY_RUN_H
#define PY_RUN_H

#include<string>

namespace Python{
 namespace PSPP1{
  void ChangeTextFile(std::string TestName);
  void ChangeTextFileINI(std::string path);
  void WriteFinalReport(std::string PDFname, std::string CableName);
  void UpdateHTML(std::string ReportTitle);
 }
 namespace Octopus{}
}

#endif