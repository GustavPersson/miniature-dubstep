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

#include "GUIFrame.h"

using namespace gfx;
using std::vector;

GUIFrame::GUIFrame() : TempSprite()
{
	mFrameType = FrameTypes::FRAME;
	mFrameID = -1;
	mFrameName = "Unknown";
	mFlagVisible = true;
}

GUIFrame::GUIFrame(const GUIFrame &frame) : TempSprite(frame)
{
	mFrameType = frame.mFrameType;
	mFrameID = frame.mFrameID;
	mFrameName = frame.mFrameName;
	mFlagVisible = frame.mFlagVisible;
}

GUIFrame::~GUIFrame()
{
	removeAllSubFrames();
}

const GUIFrame &GUIFrame::operator=(const GUIFrame &frame)
{
	if(this != &frame)
	{
		TempSprite::operator=(frame);
		mFrameType = frame.mFrameType;
		mFrameID = frame.mFrameID;
		mFrameName = frame.mFrameName;
		mFlagVisible = frame.mFlagVisible;
	}
	return *this;
}

void GUIFrame::onMouseEnter()
{

}

void GUIFrame::onMouseExit()
{

}

void GUIFrame::onMouseLeftClick()
{

}

void GUIFrame::onMouseRightClick()
{

}

void GUIFrame::addSubFrames(std::vector<GUIFrame *> frameList) const
{
	for(vector<GUIFrame *>::const_iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		frameList.push_back((*frameIter));
		(*frameIter)->addSubFrames(frameList);
	}
}

void GUIFrame::addVisibleSubFrames(std::vector<GUIFrame *> frameList) const
{
	for(vector<GUIFrame *>::const_iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		if((*frameIter)->isVisible())
		{
			frameList.push_back((*frameIter));
			(*frameIter)->addVisibleSubFrames(frameList);
		}
	}
}

void GUIFrame::resizeFrameAbsolute(float sizeX, float sizeY)
{

}

void GUIFrame::resizeFrameFactor(float factorX, float factorY)
{

}

void GUIFrame::removeAllSubFrames()
{
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		if((*frameIter))
		{
			delete (*frameIter);
			(*frameIter) = NULL;
		}
	}
	mSubFrameList.clear();
}