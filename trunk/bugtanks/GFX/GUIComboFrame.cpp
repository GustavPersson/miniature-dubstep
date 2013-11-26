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

#include "GUIComboFrame.h"

using namespace gfx;

GUIComboFrame::GUIComboFrame(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	mpTextFrame = NULL;
	init();
}

GUIComboFrame::GUIComboFrame(GUIFrame *pParent)
	: GUIImage(pParent)
{
	mpTextFrame = NULL;
	init();
}

GUIComboFrame::GUIComboFrame(const GUIImage &frame)
	: GUIImage(frame)
{
	mpTextFrame = NULL;
	init();
}

GUIComboFrame::~GUIComboFrame()
{

}

const GUIComboFrame &GUIComboFrame::operator=(const GUIComboFrame &frame)
{
	if(this != &frame)
	{
		GUIImage::operator=(frame);
	}
	return *this;
}

void GUIComboFrame::update(float deltaTime)
{
	GUIImage::update(deltaTime);
}

void GUIComboFrame::onMouseEnter(long mX, long mY)
{
	GUIImage::onMouseEnter(mX, mY);
}

void GUIComboFrame::onMouseExit(long mX, long mY)
{
	GUIImage::onMouseExit(mX, mY);
}

void GUIComboFrame::onMouseMove(long mX, long mY)
{
	GUIImage::onMouseMove(mX, mY);
}

void GUIComboFrame::onMouseLeftDown(long mX, long mY)
{
	GUIImage::onMouseLeftDown(mX, mY);
}

void GUIComboFrame::onMouseMiddleDown(long mX, long mY)
{
	GUIImage::onMouseMiddleDown(mX, mY);
}

void GUIComboFrame::onMouseRightDown(long mX, long mY)
{
	GUIImage::onMouseRightDown(mX, mY);
}

void GUIComboFrame::onMouseLeftUp(long mX, long mY)
{
	GUIImage::onMouseLeftUp(mX, mY);
}

void GUIComboFrame::onMouseMiddleUp(long mX, long mY)
{
	GUIImage::onMouseMiddleUp(mX, mY);
}

void GUIComboFrame::onMouseRightUp(long mX, long mY)
{
	GUIImage::onMouseRightUp(mX, mY);
}

void GUIComboFrame::onKeyDown(int letterKeys, int keys)
{
	GUIImage::onKeyDown(letterKeys, keys);
}

void GUIComboFrame::setFlagRender(bool render)
{
	GUIImage::setFlagRender(render);
	mpTextFrame->setFlagRender(render);
}

void GUIComboFrame::init()
{
	mpTextFrame = myNew GUIText(this);
	mpTextFrame->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	mpTextFrame->setRelativeSize(D3DXVECTOR2(1.0f, 1.0f));
	mpTextFrame->setText("");
	mpTextFrame->setTextFormat(GUIText::TextFormat_ScaledLine);
	addSubFrame(mpTextFrame);
}

void GUIComboFrame::notify(int message, GUIFrame *pSender)
{
	//Unref parameters.
	message;
	pSender;
}