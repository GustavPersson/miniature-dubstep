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

#include "GUIImage.h"

using namespace gfx;

GUIImage::GUIImage(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: GUIFrame(pBaseInstance, clientWidth, clientHeight, parentDepth), mColor(1.0f, 1.0f, 1.0f, 1.0f)
{
	setFrameType(GUIFrame::FrameTypes_Image);
	mpRenderInstance = (InstanceSprite *)getBaseInstance()->createInstanceSorted((float)getDepthLevel());
	mTextureID = -1;
	init();
}

GUIImage::GUIImage(GUIFrame *pParent) : GUIFrame(pParent), mColor(1.0f, 1.0f, 1.0f, 1.0f)
{
	setFrameType(GUIFrame::FrameTypes_SubImage);
	mpRenderInstance = (InstanceSprite *)getBaseInstance()->createInstanceSorted((float)getDepthLevel());
	mTextureID = -1;
	init();
}

GUIImage::GUIImage(const GUIImage &frame) : GUIFrame(frame)
{
	mpRenderInstance = (InstanceSprite *)getBaseInstance()->createInstanceSorted((float)getDepthLevel());
	mColor = frame.mColor;
	mTextureID = frame.mTextureID;
	init();
}

GUIImage::~GUIImage()
{

}

const GUIImage &GUIImage::operator=(const GUIImage &frame)
{
	if(this != &frame)
	{
		GUIFrame::operator=(frame);
	}
	return *this;
}

void GUIImage::update(float deltaTime)
{
	GUIFrame::update(deltaTime);
	if(getFlagUpdateRequired())
	{
		//Update position and scale for the sprite instance.
		D3DXVECTOR3 position;
		position.x = (float)getAbsoluteX();
		position.y = (float)-getAbsoluteY();
		position.z = mpRenderInstance->getPositionZ();
		D3DXVECTOR3 size;
		size.x = (float)getAbsoluteSizeX();
		size.y = (float)getAbsoluteSizeY();
		size.z = 1.0f;
		mpRenderInstance->setPosition(position);
		mpRenderInstance->setScale(size);

		//Update color and texture ID for the sprite instance.
		mpRenderInstance->setColor(mColor);
		mpRenderInstance->setTextureID(mTextureID);
	}
}

void GUIImage::onMouseEnter(long mX, long mY)
{
	GUIFrame::onMouseEnter(mX, mY);
}

void GUIImage::onMouseExit(long mX, long mY)
{
	GUIFrame::onMouseExit(mX, mY);
}

void GUIImage::onMouseMove(long mX, long mY)
{
	GUIFrame::onMouseMove(mX, mY);
}

void GUIImage::onMouseLeftDown(long mX, long mY)
{
	GUIFrame::onMouseLeftDown(mX, mY);
}

void GUIImage::onMouseMiddleDown(long mX, long mY)
{
	GUIFrame::onMouseMiddleDown(mX, mY);
}

void GUIImage::onMouseRightDown(long mX, long mY)
{
	GUIFrame::onMouseRightDown(mX, mY);
}

void GUIImage::onMouseLeftUp(long mX, long mY)
{
	GUIFrame::onMouseLeftUp(mX, mY);
}

void GUIImage::onMouseMiddleUp(long mX, long mY)
{
	GUIFrame::onMouseMiddleUp(mX, mY);
}

void GUIImage::onMouseRightUp(long mX, long mY)
{
	GUIFrame::onMouseRightUp(mX, mY);
}

void GUIImage::onKeyDown(int letterKeys, int keys)
{
	GUIFrame::onKeyDown(letterKeys, keys);
}

void GUIImage::setFlagRender(bool render)
{
	GUIFrame::setFlagRender(render);
	mpRenderInstance->setFlagRender(render);
}

void GUIImage::setColor(const D3DXCOLOR &color)
{
	mColor = color;
	updateRequired();
}

void GUIImage::setTextureID(int textureID)
{
	mTextureID = textureID;
	updateRequired();
}

void GUIImage::init()
{

}

void GUIImage::notify(int message, GUIFrame *pSender)
{
	//Unrefer parameters.
	message;
	pSender;
}