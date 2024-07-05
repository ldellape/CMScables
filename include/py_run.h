#ifndef PY_RUN
#define PY_RUN

#include <string>

namespace Python{
 void ChangeTextFile(std::string TestName);
 void ChangeTextFileINI(std::string path);
 void WriteFinalReport(std::string PDFname, std::string CableName);
 }


#endif
