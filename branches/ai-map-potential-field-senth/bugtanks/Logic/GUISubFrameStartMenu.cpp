/**
* @file
* @author Gustav Persson <ninjamuff@gmail.com>
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

#include "GUISubFrameStartMenu.h"

using namespace logic;

GUISubFrameStartMenu::GUISubFrameStartMenu(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: gfx::GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	mpText = NULL;
	init();
}

GUISubFrameStartMenu::GUISubFrameStartMenu(gfx::GUIFrame *pParent) : gfx::GUIImage(pParent)
{
	mpText = NULL;
	init();
}

GUISubFrameStartMenu::GUISubFrameStartMenu(const GUISubFrameStartMenu &frame) : gfx::GUIImage(frame)
{
	mpText = NULL;
	init();
}

GUISubFrameStartMenu::~GUISubFrameStartMenu()
{

}

const GUISubFrameStartMenu &GUISubFrameStartMenu::operator=(const GUISubFrameStartMenu &frame)
{
	if(this != &frame)
	{
		GUIImage::operator=(frame);
	}
	return *this;
}

void GUISubFrameStartMenu::update(float deltaTime)
{
	GUIImage::update(deltaTime);
	if(getFlagMouseFocus())
	{
		if(getFlagMouseLeftDown())
		{
			increaseRelativeSize(D3DXVECTOR2(0.1f * deltaTime, 0.1f * deltaTime));
		}
		if(getFlagMouseMiddleDown())
		{
			increaseRelativePosition(D3DXVECTOR2(0.1f * deltaTime, 0.0f));
		}
		if(getFlagMouseRightDown())
		{
			increaseRelativeSize(D3DXVECTOR2(-0.1f * deltaTime, -0.1f * deltaTime));
		}
		getRenderInstance()->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		getRenderInstance()->setColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

void GUISubFrameStartMenu::onMouseEnter(long mX, long mY)
{
	GUIImage::onMouseEnter(mX, mY);
}

void GUISubFrameStartMenu::onMouseExit(long mX, long mY)
{
	GUIImage::onMouseExit(mX, mY);
}

void GUISubFrameStartMenu::onMouseMove(long mX, long mY)
{
	GUIImage::onMouseMove(mX, mY);
}

void GUISubFrameStartMenu::onMouseLeftDown(long mX, long mY)
{
	GUIImage::onMouseLeftDown(mX, mY);
}

void GUISubFrameStartMenu::onMouseMiddleDown(long mX, long mY)
{
	GUIImage::onMouseMiddleDown(mX, mY);
}

void GUISubFrameStartMenu::onMouseRightDown(long mX, long mY)
{
	GUIImage::onMouseRightDown(mX, mY);
}

void GUISubFrameStartMenu::onMouseLeftUp(long mX, long mY)
{
	GUIImage::onMouseLeftUp(mX, mY);
}

void GUISubFrameStartMenu::onMouseMiddleUp(long mX, long mY)
{
	GUIImage::onMouseMiddleUp(mX, mY);
}

void GUISubFrameStartMenu::onMouseRightUp(long mX, long mY)
{
	GUIImage::onMouseRightUp(mX, mY);
}

void GUISubFrameStartMenu::onKeyDown(int letterKeys, int keys)
{
	GUIImage::onKeyDown(letterKeys, keys);
}

void GUISubFrameStartMenu::init()
{
	getRenderInstance()->setTextureID(-1);
	getRenderInstance()->setColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	mpText = myNew gfx::GUIText(this);
	mpText->setRelativePosition(D3DXVECTOR2(0.1f, 0.2f));
	mpText->setRelativeSize(D3DXVECTOR2(0.8f, 0.5f));
	mpText->setText("Waiting for connection...");
	mpText->setTextFormat(gfx::GUIText::TextFormat_FixedRows);
	mpText->setTextSize(D3DXVECTOR2(12.0f, 20.0f));
	addSubFrame(mpText);
}

void GUISubFrameStartMenu::setFlagRender(bool render)
{
	if (render == true)
	{
		addSubFrame(mpText);
	}
	else
	{
		removeSubFrame(mpText);
	}
}