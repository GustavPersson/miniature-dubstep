#include "SoundApplication.h"

void testSoundApplication();

int main()
{
	initMemoryCheck();

	setVerbosityLevel(LEVEL_LOWEST, OUTPUT_CONSOLE);

	testSoundApplication();

	return 0;
}

void testSoundApplication()
{
	SoundApplication::createInstance();

	logic::Application* pApp = logic::Application::getInstance();

	if (pApp->initInstance())
	{
		pApp->runGame();
		pApp->onClose();
	}

	SAFE_DELETE(pApp);
}

