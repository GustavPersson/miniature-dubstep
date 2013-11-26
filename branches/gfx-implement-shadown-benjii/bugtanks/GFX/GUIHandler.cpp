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

#include "GUIHandler.h"

using namespace gfx;
using std::vector;

GUIHandler::GUIHandler(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight) : EventListener()
{
	mpBaseInstance = pBaseInstance;
	mClientWidth = clientWidth;
	mClientHeight = clientHeight;
	mMouseX = 0;
	mMouseY = 0;

	//Register events.
	logic::EventListenerPtr handlerPtr = boost::shared_ptr<GUIHandler>(this);
	logic::safeAddListener(handlerPtr, logic::EvtData_System_MouseMove::mEventType);
	logic::safeAddListener(handlerPtr, logic::EvtData_System_MouseKeyDown::mEventType);
	logic::safeAddListener(handlerPtr, logic::EvtData_System_MouseKeyUp::mEventType);
}

GUIHandler::GUIHandler(const GUIHandler &handler) : EventListener()
{
	mpBaseInstance = handler.mpBaseInstance;
	mClientWidth = handler.mClientWidth;
	mClientHeight = handler.mClientHeight;
	mMouseX = handler.mMouseX;
	mMouseY = handler.mMouseY;

	//Register events.
	logic::EventListenerPtr handlerPtr = boost::shared_ptr<GUIHandler>(this);
	logic::safeAddListener(handlerPtr, logic::EvtData_System_MouseMove::mEventType);
	logic::safeAddListener(handlerPtr, logic::EvtData_System_MouseKeyDown::mEventType);
	logic::safeAddListener(handlerPtr, logic::EvtData_System_MouseKeyUp::mEventType);
}

GUIHandler::~GUIHandler()
{
	removeAllFrames();
}

const GUIHandler &GUIHandler::operator=(const GUIHandler &handler)
{
	if(this != &handler)
	{
		mpBaseInstance = handler.mpBaseInstance;
		mClientWidth = handler.mClientWidth;
		mClientHeight = handler.mClientHeight;
		mMouseX = handler.mMouseX;
		mMouseY = handler.mMouseY;
	}
	return *this;
}

const char *GUIHandler::getName()
{
	return "GUIHandler";
}

bool GUIHandler::handleEvent(const logic::EventData &eventMessage)
{
	if(eventMessage.getEventType() == logic::EvtData_System_MouseMove::mEventType)
	{
		const logic::EvtData_System_MouseMove &mouseMove = (const logic::EvtData_System_MouseMove &)eventMessage;
		mMouseX = mouseMove.mMouseX;
		mMouseY = mouseMove.mMouseY;
		for(vector<GUIFrame *>::iterator frameIter = mFrameList.begin();
			frameIter != mFrameList.end(); ++frameIter)
		{
			if((*frameIter)->getFlagMouseFocus())
			{
				if(!(*frameIter)->pointInFrame(mMouseX, mMouseY))
				{
					(*frameIter)->onMouseExit(mMouseX, mMouseY);
				}
			}
			else
			{
				if((*frameIter)->pointInFrame(mMouseX, mMouseY))
				{
					(*frameIter)->onMouseEnter(mMouseX, mMouseY);
				}
			}
		}
	}
	else if(eventMessage.getEventType() == logic::EvtData_System_MouseKeyDown::mEventType)
	{
		const logic::EvtData_System_MouseKeyDown &keyDown = (const logic::EvtData_System_MouseKeyDown &)eventMessage;
		for(vector<GUIFrame *>::iterator frameIter = mFrameList.begin();
			frameIter != mFrameList.end(); ++frameIter)
		{
			if((*frameIter)->pointInFrame(mMouseX, mMouseY))
			{
				if(keyDown.mKeys & logic::EvtData_System_MouseKeyDown::MouseKeyType_Left)
				{
					(*frameIter)->onMouseLeftDown(mMouseX, mMouseY);
				}
				if(keyDown.mKeys & logic::EvtData_System_MouseKeyDown::MouseKeyType_Middle)
				{
					(*frameIter)->onMouseMiddleDown(mMouseX, mMouseY);
				}
				if(keyDown.mKeys & logic::EvtData_System_MouseKeyDown::MouseKeyType_Right)
				{
					(*frameIter)->onMouseRightDown(mMouseX, mMouseY);
				}
				if(keyDown.mKeys & logic::EvtData_System_MouseKeyDown::MouseKeyType_LeftDouble)
				{

				}
				if(keyDown.mKeys & logic::EvtData_System_MouseKeyDown::MouseKeyType_MiddleDouble)
				{

				}
				if(keyDown.mKeys & logic::EvtData_System_MouseKeyDown::MouseKeyType_RightDouble)
				{

				}
			}
		}
	}
	else if(eventMessage.getEventType() == logic::EvtData_System_MouseKeyUp::mEventType)
	{
		const logic::EvtData_System_MouseKeyUp &keyUp = (const logic::EvtData_System_MouseKeyUp &)eventMessage;
		for(vector<GUIFrame *>::iterator frameIter = mFrameList.begin();
			frameIter != mFrameList.end(); ++frameIter)
		{
			if((*frameIter)->pointInFrame(mMouseX, mMouseY))
			{
				if(keyUp.mKeys & logic::EvtData_System_MouseKeyUp::MouseKeyType_Left)
				{
					(*frameIter)->onMouseLeftUp(mMouseX, mMouseY);
				}
				if(keyUp.mKeys & logic::EvtData_System_MouseKeyUp::MouseKeyType_Middle)
				{
					(*frameIter)->onMouseMiddleUp(mMouseX, mMouseY);
				}
				if(keyUp.mKeys & logic::EvtData_System_MouseKeyUp::MouseKeyType_Right)
				{
					(*frameIter)->onMouseRightUp(mMouseX, mMouseY);
				}
			}
		}
	}
	return false;
}

void GUIHandler::updateFrames(float deltaTime)
{
	for(vector<GUIFrame *>::iterator frameIter = mFrameList.begin();
		frameIter != mFrameList.end(); ++frameIter)
	{
		(*frameIter)->update(deltaTime);
	}
}

void GUIHandler::init()
{

}

void GUIHandler::addFrame(GUIFrame *pFrame)
{
	mFrameList.push_back(pFrame);
}

void GUIHandler::removeAllFrames()
{
	for(vector<GUIFrame *>::iterator frameIter = mFrameList.begin();
		frameIter != mFrameList.end(); ++frameIter)
	{
		if((*frameIter))
		{
			delete (*frameIter);
			(*frameIter) = NULL;
		}
	}
	mFrameList.clear();
}