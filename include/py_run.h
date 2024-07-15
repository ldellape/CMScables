#ifndef PY_RUN
#define PY_RUN

// python scirpts, defined in py directory //


#include <string>

namespace Python{
 namespace PS_PP1{
 void ChangeTextFile(std::string TestName);
 void ChangeTextFileINI(std::string path);
 void WriteFinalReport(std::string PDFname, std::string CableName);
 void ChangeHTML(std::string ReportTitle);
 }
}


#endif
