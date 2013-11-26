#include "LogicApplication.h"
void testLogicApplication();

int main()
{
	initMemoryCheck();
	setVerbosityLevel(LEVEL_LOWEST, OUTPUT_CONSOLE);
	setOutputTargetDebugMessage(OUTPUT_CONSOLE | OUTPUT_FILE);
	setVerbosityLevel(LEVEL_LOWEST, OUTPUT_FILE);
	setOutputTargetError(OUTPUT_FILE);

	testLogicApplication();
	
	

	return 0;
}

void testLogicApplication()
{
	LogicApplication::createInstance();

	logic::Application* pApp = logic::Application::getInstance();

	if (pApp->initInstance())
	{
		pApp->runGame();
		pApp->onClose();
	}

	SAFE_DELETE(pApp);		
}