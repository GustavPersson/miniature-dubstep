#include "BTMayaExporter.h"

int main(int nArgs, const char *pArgs)
{
	BTMayaExporter *pExporter = new BTMayaExporter();

	pExporter->startExporter();

	cout << "Press any key to exit." << endl;
	_getch();
	delete pExporter;
	return 0;
}