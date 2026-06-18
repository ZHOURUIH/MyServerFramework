// auto generate start
#include "GameHeader.h"

void ExcelRegister::registeAll()
{
	mExcelAchivement = mExcelManager->registeExcel<ExcelAchivement>("Achivement");
	mExcelTest = mExcelManager->registeExcel<ExcelTest>("Test");
}
// auto generate end