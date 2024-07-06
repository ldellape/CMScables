#ifndef PY_RUN
#define PY_RUN

// python scirpts, defined in py directory //


#include <string>

namespace Python{
 void ChangeTextFile(std::string TestName);
 void ChangeTextFileINI(std::string path);
 void WriteFinalReport(std::string PDFname, std::string CableName);
 }


#endif
