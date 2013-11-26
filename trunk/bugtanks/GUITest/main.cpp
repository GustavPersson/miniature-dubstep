#include <iostream>
#include "TestApp.h"

using namespace guitest;
using namespace std;

int main()
{
	initMemoryCheck();

	//setVerbosityLevel(LEVEL_LOWEST, OUTPUT_CONSOLE);

	setVerbosityLevel(LEVEL_LOWEST, OUTPUT_CONSOLE);
	setOutputTargetDebugMessage(OUTPUT_CONSOLE | OUTPUT_FILE);
	setVerbosityLevel(LEVEL_LOWEST, OUTPUT_FILE);

	guitest::TestApp::createInstance();
	logic::Application* pApp = guitest::TestApp::getInstance();

	if (pApp->initInstance())
	{
		pApp->runGame();
		pApp->onClose();
	}

#pragma warning(push)
#ifdef _DEBUG
#pragma warning(disable: 4189)
	int * banan = myNew int;
#endif
	
	SAFE_DELETE(pApp);

	return 0;
}
#pragma warning(pop)