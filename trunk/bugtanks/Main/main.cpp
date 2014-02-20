/**
 * @file
 * @author Matteus Magnusson <senth.wallace@gmail.com>
 * @author Gustav Persson <gustav@alltomminrov.se>
 * @version 1.0
 * Copyright (©) A-Team.
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 * 
 * @section DESCRIPTION
 *
 * Here rests main peacefully. Before it dies it will give birth to all
 * possible things, both imaginable and unimaginable.
 */

#include "../Utilities/Macros.h"
#include "../Logic/Application.h"

//TODO: Before going Gold:
//		Remove Debug preprocessors in Application
//		Remove Debug entries in Config.ini

int main()
{
	initMemoryCheck();

	setVerbosityLevel(LEVEL_LOW, OUTPUT_CONSOLE);
	setOutputTargetDebugMessage(OUTPUT_FILE);
	setVerbosityLevel(LEVEL_LOWEST, OUTPUT_FILE);

	logic::Application::createInstance();
	logic::Application* pApp = logic::Application::getInstance();

	if (pApp->initInstance())
	{
		pApp->runGame();
		pApp->onClose();
	}

	SAFE_DELETE(pApp);

#pragma warning(push)
#ifdef _DEBUG
#pragma warning(disable: 4189)
	int * banan = myNew int; //bevisa att minnesläckhittaren funkar.
#endif
	return 0;
}
#pragma warning(pop)