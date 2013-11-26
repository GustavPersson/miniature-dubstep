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
	init();
}

GUIHandlerTest::GUIHandlerTest(const GUIHandlerTest &handler) : GUIHandler(handler)
{
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
	GUIFrame *frame = myNew GUIFrame(getInstanceBase(), getClientWidth(), getClientHeight());
	frame->setRelativePosition(D3DXVECTOR2(0.5f, 0.3f));
	frame->setRelativeSize(D3DXVECTOR2(0.4f, 0.2f));
	addFrame(frame);

	GUIFrame *frame2 = myNew GUIFrame(getInstanceBase(), getClientWidth(), getClientHeight());
	frame2->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	frame2->setRelativeSize(D3DXVECTOR2(0.2f, 0.9f));
	addFrame(frame2);
}