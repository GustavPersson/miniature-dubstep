#include "EditorApplication.h"
//#include "../GFX/MeshLoader2.h"

#define UREF(x) x

int main(int nArgs, const char **pArgs)
{
	UREF(nArgs);
	UREF(pArgs);

	initMemoryCheck();
	setVerbosityLevel(LEVEL_NONE, OUTPUT_CONSOLE);

	EditorApplication::createInstance();

	logic::Application *pApp = logic::Application::getInstance();

	if (pApp->initInstance())
	{
		pApp->runGame();
		pApp->onClose();
	}

	SAFE_DELETE(pApp);

	return 0;
}