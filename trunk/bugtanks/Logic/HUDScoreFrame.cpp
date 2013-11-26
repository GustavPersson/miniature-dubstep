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

#include "HUDScoreFrame.h"
#include "ActorHandler.h"
#include <string>
#include <sstream>

using namespace logic;
using namespace gfx;
using std::string;
using std::stringstream;

HUDScoreFrame::HUDScoreFrame(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	mpTextEnemies = NULL;
	mpTextHidden = NULL;
	mpTextScore = NULL;
	mpTextAccuracy = NULL;
	init();
}

HUDScoreFrame::HUDScoreFrame(GUIFrame *pParent)
	: GUIImage(pParent)
{
	mpTextEnemies = NULL;
	mpTextHidden = NULL;
	mpTextScore = NULL;
	mpTextAccuracy = NULL;
	init();
}

HUDScoreFrame::HUDScoreFrame(const HUDScoreFrame &frame)
	: GUIImage(frame)
{
	mpTextEnemies = NULL;
	mpTextHidden = NULL;
	mpTextScore = NULL;
	mpTextAccuracy = NULL;
	init();
}

HUDScoreFrame::~HUDScoreFrame()
{

}

const HUDScoreFrame &HUDScoreFrame::operator=(const HUDScoreFrame &frame)
{
	if(this != &frame)
	{
		GUIImage::operator=(frame);
	}
	return *this;
}

void HUDScoreFrame::update(float deltaTime)
{
	GUIImage::update(deltaTime);
}

void HUDScoreFrame::onMouseEnter(long mX, long mY)
{
	GUIImage::onMouseEnter(mX, mY);
}

void HUDScoreFrame::onMouseExit(long mX, long mY)
{
	GUIImage::onMouseExit(mX, mY);
}

void HUDScoreFrame::onMouseMove(long mX, long mY)
{
	GUIImage::onMouseMove(mX, mY);
}

void HUDScoreFrame::onMouseLeftDown(long mX, long mY)
{
	GUIImage::onMouseLeftDown(mX, mY);
}

void HUDScoreFrame::onMouseMiddleDown(long mX, long mY)
{
	GUIImage::onMouseMiddleDown(mX, mY);
}

void HUDScoreFrame::onMouseRightDown(long mX, long mY)
{
	GUIImage::onMouseRightDown(mX, mY);
}

void HUDScoreFrame::onMouseLeftUp(long mX, long mY)
{
	GUIImage::onMouseLeftUp(mX, mY);
}

void HUDScoreFrame::onMouseMiddleUp(long mX, long mY)
{
	GUIImage::onMouseMiddleUp(mX, mY);
}

void HUDScoreFrame::onMouseRightUp(long mX, long mY)
{
	GUIImage::onMouseRightUp(mX, mY);
}

void HUDScoreFrame::onKeyDown(int letterKeys, int keys)
{
	GUIImage::onKeyDown(letterKeys, keys);
}

void HUDScoreFrame::updateTextEnemies(int enemies)
{
	stringstream s;
	string text;
	s << enemies;
	s >> text;
	mpTextEnemies->setText(text);
}

void HUDScoreFrame::updateTextHidden(int hidden)
{
	stringstream s;
	string text;
	s << hidden;
	s >> text;
	mpTextHidden->setText(text);
}

void HUDScoreFrame::updateTextScore(int score)
{
	stringstream s;
	string text;
	s << score;
	s >> text;
	mpTextScore->setText(text);
}

void HUDScoreFrame::updateTextAccuracy(float accuracy)
{
	stringstream s;
	string text;
	s << (int)accuracy;
	s >> text;
	mpTextAccuracy->setText(text);
}

void HUDScoreFrame::init()
{
	int arrayIndex = 0;
	int textureID = -1;
	DX10Module::getInstance()->getTextureIndex("HUD_Score", arrayIndex, textureID);

	getRenderInstance()->setTextureArrayIndex(arrayIndex);
	setTextureID(textureID);

	mpTextEnemies = myNew GUIText(this);
	mpTextEnemies->setRelativePosition(D3DXVECTOR2(HUD_ENEMIES_POS_X, HUD_ENEMIES_POS_Y));
	mpTextEnemies->setRelativeSize(D3DXVECTOR2(HUD_TEXT_SIZE_X, HUD_TEXT_SIZE_Y));
	mpTextEnemies->setTextFormat(GUIText::TextFormat_FixedLine);
	mpTextEnemies->setTextSize(D3DXVECTOR2(10.0f, 12.0f));
	mpTextEnemies->setText("");
	addSubFrame(mpTextEnemies);

	mpTextHidden = myNew GUIText(this);
	mpTextHidden->setRelativePosition(D3DXVECTOR2(HUD_HIDDEN_POS_X, HUD_HIDDEN_POS_Y));
	mpTextHidden->setRelativeSize(D3DXVECTOR2(HUD_TEXT_SIZE_X, HUD_TEXT_SIZE_Y));
	mpTextHidden->setTextFormat(GUIText::TextFormat_FixedLine);
	mpTextHidden->setTextSize(D3DXVECTOR2(10.0f, 12.0f));
	mpTextHidden->setText("");
	addSubFrame(mpTextHidden);

	mpTextScore = myNew GUIText(this);
	mpTextScore->setRelativePosition(D3DXVECTOR2(HUD_SCORE_POS_X, HUD_SCORE_POS_Y));
	mpTextScore->setRelativeSize(D3DXVECTOR2(HUD_TEXT_SIZE_X, HUD_TEXT_SIZE_Y));
	mpTextScore->setTextFormat(GUIText::TextFormat_FixedLine);
	mpTextScore->setTextSize(D3DXVECTOR2(10.0f, 12.0f));
	mpTextScore->setText("");
	addSubFrame(mpTextScore);

	mpTextAccuracy = myNew GUIText(this);
	mpTextAccuracy->setRelativePosition(D3DXVECTOR2(HUD_ACCURACY_POS_X, HUD_ACCURACY_POS_Y));
	mpTextAccuracy->setRelativeSize(D3DXVECTOR2(HUD_TEXT_SIZE_X, HUD_TEXT_SIZE_Y));
	mpTextAccuracy->setTextFormat(GUIText::TextFormat_FixedLine);
	mpTextAccuracy->setTextSize(D3DXVECTOR2(10.0f, 12.0f));
	mpTextAccuracy->setText("");
	addSubFrame(mpTextAccuracy);
}

void HUDScoreFrame::notify(int message, GUIFrame *pSender)
{

}