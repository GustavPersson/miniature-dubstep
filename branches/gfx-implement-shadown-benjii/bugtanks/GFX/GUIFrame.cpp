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

GUIFrame::GUIFrame(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight)
		 : mSubFrameList(), mRelativePosition(0.0f, 0.0f), mRelativeSize(0.0f, 0.0f)
{
	mFrameType = FrameTypes::FRAME;
	mpBaseInstance = pBaseInstance;
	mpRenderInstance = (InstanceSprite *)mpBaseInstance->createInstance();
	mClientWidth = clientWidth;
	mClientHeight = clientHeight;
	mAbsoluteX = 0;
	mAbsoluteY = 0;
	mAbsoluteSizeX = 0;
	mAbsoluteSizeY = 0;
	mFlagMouseFocus = false;
	mFlagMouseLeftDown = false;
	mFlagMouseMiddleDown = false;
	mFlagMouseRightDown = false;
}

GUIFrame::GUIFrame(const GUIFrame &frame) : mSubFrameList(), mRelativePosition(frame.mRelativePosition),
		mRelativeSize(frame.mRelativeSize)
{
	mFrameType = frame.mFrameType;
	mpBaseInstance = frame.mpBaseInstance;
	mpRenderInstance = (InstanceSprite *)mpBaseInstance->createInstance();
	mClientWidth = frame.mClientWidth;
	mClientHeight = frame.mClientHeight;
	mAbsoluteX = frame.mAbsoluteX;
	mAbsoluteY = frame.mAbsoluteY;
	mAbsoluteSizeX = frame.mAbsoluteSizeX;
	mAbsoluteSizeY = frame.mAbsoluteSizeY;
	mFlagMouseFocus = false;
	mFlagMouseLeftDown = false;
	mFlagMouseMiddleDown = false;
	mFlagMouseRightDown = false;
}

GUIFrame::~GUIFrame()
{
	mpBaseInstance->removeInstance(mpRenderInstance);
	removeAllSubFrames();
}

const GUIFrame &GUIFrame::operator=(const GUIFrame &frame)
{
	if(this != &frame)
	{
		mFrameType = frame.mFrameType;
		mpBaseInstance = frame.mpBaseInstance;
		mpRenderInstance = (InstanceSprite *)mpBaseInstance->createInstance();
		mClientWidth = frame.mClientWidth;
		mClientHeight = frame.mClientHeight;
		mAbsoluteX = frame.mAbsoluteX;
		mAbsoluteY = frame.mAbsoluteY;
		mAbsoluteSizeX = frame.mAbsoluteSizeX;
		mAbsoluteSizeY = frame.mAbsoluteSizeY;
		mFlagMouseFocus = false;
		mFlagMouseLeftDown = false;
		mFlagMouseMiddleDown = false;
		mFlagMouseRightDown = false;
	}
	return *this;
}

void GUIFrame::update(float deltaTime)
{
	D3DXCOLOR color(1.0f, 1.0f, 1.0f, 1.0f);
	if(mFlagMouseFocus)
	{
		color = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		if(mFlagMouseLeftDown)
		{
			increaseRelativePosition(D3DXVECTOR2(0.1f * deltaTime, 0.0f));
		}
		if(mFlagMouseRightDown)
		{
			increaseRelativePosition(D3DXVECTOR2(-0.1f * deltaTime, 0.0f));
		}
	}

	D3DXVECTOR3 position;
	position.x = (float)mAbsoluteX;
	position.y = (float)-mAbsoluteY;
	position.z = 1.0f;
	D3DXVECTOR3 size;
	size.x = (float)mAbsoluteSizeX;
	size.y = (float)mAbsoluteSizeY;
	size.z = 1.0f;
	mpRenderInstance->setPosition(position);
	mpRenderInstance->setScale(size);
	mpRenderInstance->setTextureID(2);
	mpRenderInstance->setColor(color);
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		(*frameIter)->update(deltaTime);
	}
}

void GUIFrame::onMouseEnter(long mX, long mY)
{
	mFlagMouseFocus = true;
	for(vector<GUIFrame *>::iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		if((*frameIter)->pointInFrame(mX, mY))
		{
			(*frameIter)->onMouseEnter(mX, mY);
		}
	}
}

void GUIFrame::onMouseExit(long mX, long mY)
{
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

bool GUIFrame::pointInFrame(long X, long Y)
{
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

void GUIFrame::addSubFrames(std::vector<GUIFrame *> &frameList) const
{
	for(vector<GUIFrame *>::const_iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		frameList.push_back((*frameIter));
		(*frameIter)->addSubFrames(frameList);
	}
}

void GUIFrame::addVisibleSubFrames(std::vector<GUIFrame *> &frameList) const
{
	for(vector<GUIFrame *>::const_iterator frameIter = mSubFrameList.begin();
		frameIter != mSubFrameList.end(); ++frameIter)
	{
		if(mpRenderInstance->getFlagRender())
		{
			frameList.push_back((*frameIter));
			(*frameIter)->addVisibleSubFrames(frameList);
		}
	}
}

void GUIFrame::increaseRelativePosition(const D3DXVECTOR2 &offset)
{
	mRelativePosition += offset;
	updateAbsolutePosition();
}

void GUIFrame::increaseRelativeSize(const D3DXVECTOR2 &offset)
{
	mRelativeSize += offset;
	updateAbsolutePosition();
}

void GUIFrame::setRelativePosition(const D3DXVECTOR2 &position)
{
	mRelativePosition = position;
	updateAbsolutePosition();
}

void GUIFrame::setRelativeSize(const D3DXVECTOR2 &size)
{
	mRelativeSize = size;
	updateAbsolutePosition();
}

void GUIFrame::updateAbsolutePosition()
{
	mAbsoluteX = mRelativePosition.x * mClientWidth;
	mAbsoluteY = mRelativePosition.y * mClientHeight;
	mAbsoluteSizeX = mRelativeSize.x * mClientWidth;
	mAbsoluteSizeY = mRelativeSize.y * mClientHeight;
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