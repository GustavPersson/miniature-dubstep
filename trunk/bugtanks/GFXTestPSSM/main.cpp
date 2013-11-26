#include "GFXTestPSSMApplication.h"

void testGFXTestPSSMApplication();

int main()
{
	srand(static_cast<unsigned int>(time(0)));
	initMemoryCheck();
	setVerbosityLevel(LEVEL_LOWEST, OUTPUT_CONSOLE);
	testGFXTestPSSMApplication();

	return 0;
}

void testGFXTestPSSMApplication()
{
	GFXTestPSSMApplication::createInstance();

	logic::Application* pApp = logic::Application::getInstance();

	if (pApp->initInstance())
	{
		pApp->runGame();
		pApp->onClose();
	}

	SAFE_DELETE(pApp);
}