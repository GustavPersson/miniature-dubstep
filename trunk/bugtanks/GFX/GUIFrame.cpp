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

GUIFrame::GUIFrame(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
		 : mSubFrameList(), mRelativePosition(0.0f, 0.0f), mRelativeSize(0.0f, 0.0f)
{
	mFrameType = GUIFrame::FrameTypes_Frame;
	mpParent = NULL;
	mpBaseInstance = pBaseInstance;
	mClientWidth = clientWidth;
	mClientHeight = clientHeight;
	mAbsoluteX = 0;
	mAbsoluteY = 0;
	mAbsoluteSizeX = 0;
	mAbsoluteSizeY = 0;
	mDepthLevel = parentDepth + 1;
	mFlagUpdateRequired = false;
	mFlagNewUpdateRequired = false;
	mFlagMouseFrameFocus = false;
	mFlagMouseOmniFocus = false;
	mFlagMouseFocus = false;
	mFlagMouseLeftDown = false;
	mFlagMouseMiddleDown = false;
	mFlagMouseRightDown = false;
	mFlagVisible = true;
}

GUIFrame::GUIFrame(GUIFrame *pParent) : mSubFrameList(), mRelativePosition(0.0f, 0.0f), mRelativeSize(0.0f, 0.0f)
{
	mFrameType = GUIFrame::FrameTypes_SubFrame;
	mpParent = pParent;
	mpBaseInstance = pParent->mpBaseInstance;
	mClientWidth = pParent->mClientWidth;
	mClientHeight = pParent->mClientHeight;
	mAbsoluteX = 0;
	mAbsoluteY = 0;
	mAbsoluteSizeX = 0;
	mAbsoluteSizeY = 0;
	mDepthLevel = pParent->mDepthLevel + 1;
	mFlagUpdateRequired = false;
	mFlagNewUpdateRequired = false;
	mFlagMouseFrameFocus = false;
	mFlagMouseOmniFocus = false;
	mFlagMouseFocus = false;
	mFlagMouseLeftDown = false;
	mFlagMouseMiddleDown = false;
	mFlagMouseRightDown = false;
	mFlagVisible = true;
}

GUIFrame::GUIFrame(const GUIFrame &frame) : mSubFrameList(), mRelativePosition(frame.mRelativePosition),
	mRelativeSize(frame.mRelativeSize)
{
	mFrameType = frame.mFrameType;
	mpParent = frame.mpParent;
	mpBaseInstance = frame.mpBaseInstance;
	mClientWidth = frame.mClientWidth;
	mClientHeight = frame.mClientHeight;
	mAbsoluteX = frame.mAbsoluteX;
	mAbsoluteY = frame.mAbsoluteY;
	mAbsoluteSizeX = frame.mAbsoluteSizeX;
	mAbsoluteSizeY = frame.mAbsoluteSizeY;
	mFlagUpdateRequired = false;
	mFlagNewUpdateRequired = false;
	mFlagMouseFrameFocus = false;
	mFlagMouseOmniFocus = false;
	mFlagMouseFocus = false;
	mFlagMouseLeftDown = false;
	mFlagMouseMiddleDown = false;
	mFlagMouseRightDown = false;
	mFlagVisible = true;
}

GUIFrame::~GUIFrame()
{
	removeAllSubFrames();
}

const GUIFrame &GUIFrame::operator=(const GUIFrame &frame)
{
	if(this != &frame)
	{
		mFrameType = frame.mFrameType;
		mpParent = frame.mpParent;
		mpBaseInstance = frame.mpBaseInstance;
		mClientWidth = frame.mClientWidth;
		mClientHeight = frame.mClientHeight;
		mAbsoluteX = frame.mAbsoluteX;
		mAbsoluteY = frame.mAbsoluteY;
		mAbsoluteSizeX = frame.mAbsoluteSizeX;
		mAbsoluteSizeY = frame.mAbsoluteSizeY;
		mFlagUpdateRequired = false;
		mFlagNewUpdateRequired = false;
		mFlagMouseFrameFocus = false;
		mFlagMouseOmniFocus = false;
		mFlagMouseFocus = false;
		mFlagMouseLeftDown = false;
		mFlagMouseMiddleDown = false;
		mFlagMouseRightDown = false;
		mFlagVisible = true;
	}
	return *this;
}

void GUIFrame::update(float deltaTime)
{
	mFlagUpdateRequired = mFlagNewUpdateRequired;
	mFlagNewUpdateRequired = false;
	if(mFlagUpdateRequired)
	{
		updateAbsolutePosition();
		setFlagRender(mFlagVisible);
	}

	//Update all subframes.
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		if(mFlagUpdateRequired)
		{
			(*frameIter)->updateRequired();
		}
		(*frameIter)->update(deltaTime);
	}
}

void GUIFrame::onMouseEnter(long mX, long mY)
{
	mFlagMouseFrameFocus = true;
	bool focus = true;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		(*frameIter)->mFlagMouseFrameFocus = true;
		if((*frameIter)->pointInFrame(mX, mY))
		{
			if(!mFlagMouseOmniFocus)
			{
				focus = false;
			}
			(*frameIter)->onMouseEnter(mX, mY);
		}
	}
	mFlagMouseFocus = focus;
}

void GUIFrame::onMouseExit(long mX, long mY)
{
	mFlagMouseFrameFocus = false;
	mFlagMouseFocus = false;
	mFlagMouseLeftDown = false;
	mFlagMouseMiddleDown = false;
	mFlagMouseRightDown = false;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		(*frameIter)->onMouseExit(mX, mY);
	}
}

void GUIFrame::onMouseMove(long mX, long mY)
{
	bool focus = true;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		if((*frameIter)->pointInFrame(mX, mY))
		{
			if(!mFlagMouseOmniFocus)
			{
				focus = false;
			}
			if((*frameIter)->getFlagMouseFocus())
			{
				(*frameIter)->onMouseMove(mX, mY);
			}
			else
			{
				(*frameIter)->onMouseEnter(mX, mY);
			}
		}
		else
		{
			if((*frameIter)->getFlagMouseFocus())
			{
				(*frameIter)->onMouseExit(mX, mY);
			}
		}
	}
	mFlagMouseFocus = focus;
}

void GUIFrame::onMouseLeftDown(long mX, long mY)
{
	mFlagMouseLeftDown = true;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		if((*frameIter)->pointInFrame(mX, mY))
		{
			(*frameIter)->onMouseLeftDown(mX, mY);
		}
	}
}

void GUIFrame::onMouseMiddleDown(long mX, long mY)
{
	mFlagMouseMiddleDown = true;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		if((*frameIter)->pointInFrame(mX, mY))
		{
			(*frameIter)->onMouseMiddleDown(mX, mY);
		}
	}
}

void GUIFrame::onMouseRightDown(long mX, long mY)
{
	mFlagMouseRightDown = true;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		if((*frameIter)->pointInFrame(mX, mY))
		{
			(*frameIter)->onMouseRightDown(mX, mY);
		}
	}
}

void GUIFrame::onMouseLeftUp(long mX, long mY)
{
	mFlagMouseLeftDown = false;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		if((*frameIter)->pointInFrame(mX, mY))
		{
			(*frameIter)->onMouseLeftUp(mX, mY);
		}
	}
}

void GUIFrame::onMouseMiddleUp(long mX, long mY)
{
	mFlagMouseMiddleDown = false;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		if((*frameIter)->pointInFrame(mX, mY))
		{
			(*frameIter)->onMouseMiddleUp(mX, mY);
		}
	}
}

void GUIFrame::onMouseRightUp(long mX, long mY)
{
	mFlagMouseRightDown = false;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		if((*frameIter)->pointInFrame(mX, mY))
		{
			(*frameIter)->onMouseRightUp(mX, mY);
		}
	}
}

void GUIFrame::onKeyDown(int letterKeys, int keys)
{
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		(*frameIter)->onKeyDown(letterKeys, keys);
	}
}

bool GUIFrame::pointInFrame(long X, long Y)
{
	if(!mFlagVisible)
	{
		return false;
	}
	if(X < mAbsoluteX || X > mAbsoluteX + mAbsoluteSizeX)
	{
		return false;
	}
	if(Y < mAbsoluteY || Y > mAbsoluteY + mAbsoluteSizeY)
	{
		return false;
	}
	return true;
}

void GUIFrame::increaseRelativePosition(const D3DXVECTOR2 &offset)
{
	mRelativePosition += offset;
	mFlagNewUpdateRequired = true;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		(*frameIter)->updateAbsolutePosition();
	}
}

void GUIFrame::increaseRelativeSize(const D3DXVECTOR2 &offset)
{
	mRelativeSize += offset;
	mFlagNewUpdateRequired = true;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		(*frameIter)->updateAbsolutePosition();
	}
}

void GUIFrame::setFlagRender(bool render)
{
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		(*frameIter)->setFlagRender(render);
	}
}

void GUIFrame::setRelativePosition(const D3DXVECTOR2 &position)
{
	mRelativePosition = position;
	mFlagNewUpdateRequired = true;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		(*frameIter)->updateAbsolutePosition();
	}
}

void GUIFrame::setRelativeSize(const D3DXVECTOR2 &size)
{
	mRelativeSize = size;
	mFlagNewUpdateRequired = true;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		(*frameIter)->updateAbsolutePosition();
	}
}

void GUIFrame::setFlagVisible(bool flag)
{
	mFlagVisible = flag;
	mFlagNewUpdateRequired = true;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		(*frameIter)->setFlagVisible(flag);
	}
}

void GUIFrame::init()
{

}

void GUIFrame::notify(int message, GUIFrame *pSender)
{
	//Unrefer parameter.
	pSender;

	if(mpParent)
	{
		mpParent->notify(message, this);
	}
}

void GUIFrame::updateRequired()
{
	mFlagNewUpdateRequired = true;
}

void GUIFrame::updateAbsolutePosition()
{
	if(!mpParent)
	{
		//Calculate absolute coordinates for frames.
		mAbsoluteX = (long)(mRelativePosition.x * mClientWidth);
		mAbsoluteY = (long)(mRelativePosition.y * mClientHeight);
		mAbsoluteSizeX = (long)(mRelativeSize.x * mClientWidth);
		mAbsoluteSizeY = (long)(mRelativeSize.y * mClientHeight);
	}
	else
	{
		//Calculate absolute coordinates for subframes.
		mAbsoluteX = (long)(mpParent->mAbsoluteX + mRelativePosition.x * mpParent->mAbsoluteSizeX);
		mAbsoluteY = (long)(mpParent->mAbsoluteY + mRelativePosition.y * mpParent->mAbsoluteSizeY);
		mAbsoluteSizeX = (long)(mRelativeSize.x * mpParent->mAbsoluteSizeX);
		mAbsoluteSizeY = (long)(mRelativeSize.y * mpParent->mAbsoluteSizeY);
	}
}

void GUIFrame::addSubFrame(GUIFrame *pSubFrame)
{
	mSubFrameList.push_back(pSubFrame);
}

void GUIFrame::removeSubFrame(GUIFrame *pSubFrame)
{
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		if((*frameIter) == pSubFrame)
		{
			delete (*frameIter);
			(*frameIter) = NULL;
			mSubFrameList.erase(frameIter);
			break;
		}
	}
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