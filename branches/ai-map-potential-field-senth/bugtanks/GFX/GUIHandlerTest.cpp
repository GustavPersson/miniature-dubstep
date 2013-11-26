/**
 * @file
 * @author Victor Lundberg <lundberg.victor@gmail.com>
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
 */

#include "GUIHandlerTest.h"

using namespace gfx;

GUIHandlerTest::GUIHandlerTest(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight)
			   : GUIHandler(pBaseInstance, clientWidth, clientHeight)
{
	mpFPS = NULL;
	init();
}

GUIHandlerTest::GUIHandlerTest(const GUIHandlerTest &handler) : GUIHandler(handler)
{
	mpFPS = NULL;
	init();
}

GUIHandlerTest::~GUIHandlerTest()
{

}

const GUIHandlerTest &GUIHandlerTest::operator=(const GUIHandlerTest &handler)
{
	if(this != &handler)
	{
		GUIHandler::operator=(handler);
	}
	return *this;
}

const char *GUIHandlerTest::getName()
{
	return "GUIHandlerTest";
}

void GUIHandlerTest::init()
{
	mpFPS = myNew GUIText(getInstanceBase(), getClientWidth(), getClientHeight(), 0);
	mpFPS->setRelativePosition(D3DXVECTOR2(0.05f, 0.1f));
	mpFPS->setRelativeSize(D3DXVECTOR2(0.1f, 0.1f));
	mpFPS->setTextFormat(GUIText::TextFormat_FixedLine);
	mpFPS->setTextSize(D3DXVECTOR2(20.0f, 40.0f));
	mpFPS->setTextSpaceAbsolute(D3DXVECTOR2(1.0f, 1.0f));
	mpFPS->setText("0");
	addFrame(mpFPS);
}