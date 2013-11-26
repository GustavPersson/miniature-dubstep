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

#include "HUDMainFrame.h"
#include "ActorHandler.h"

using namespace logic;
using namespace gfx;

HUDMainFrame::HUDMainFrame(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	mpHPBar = NULL;
	mpCooldownBar = NULL;
	mpSelectionCooldownBar = NULL;
	mpMainWeapon = NULL;
	mpWeapon1 = NULL;
	mpWeapon2 = NULL;
	mpWeapon3 = NULL;
	init();
}

HUDMainFrame::HUDMainFrame(GUIFrame *pParent)
	: GUIImage(pParent)
{
	mpHPBar = NULL;
	mpCooldownBar = NULL;
	mpSelectionCooldownBar = NULL;
	mpMainWeapon = NULL;
	mpWeapon1 = NULL;
	mpWeapon2 = NULL;
	mpWeapon3 = NULL;
	init();
}

HUDMainFrame::HUDMainFrame(const HUDMainFrame &frame)
	: GUIImage(frame)
{
	mpHPBar = NULL;
	mpCooldownBar = NULL;
	mpSelectionCooldownBar = NULL;
	mpMainWeapon = NULL;
	mpWeapon1 = NULL;
	mpWeapon2 = NULL;
	mpWeapon3 = NULL;
	init();
}

HUDMainFrame::~HUDMainFrame()
{

}

const HUDMainFrame &HUDMainFrame::operator=(const HUDMainFrame &frame)
{
	if(this != &frame)
	{
		GUIImage::operator=(frame);
	}
	return *this;
}

void HUDMainFrame::update(float deltaTime)
{
	GUIImage::update(deltaTime);
}

void HUDMainFrame::onMouseEnter(long mX, long mY)
{
	GUIImage::onMouseEnter(mX, mY);
}

void HUDMainFrame::onMouseExit(long mX, long mY)
{
	GUIImage::onMouseExit(mX, mY);
}

void HUDMainFrame::onMouseMove(long mX, long mY)
{
	GUIImage::onMouseMove(mX, mY);
}

void HUDMainFrame::onMouseLeftDown(long mX, long mY)
{
	GUIImage::onMouseLeftDown(mX, mY);
}

void HUDMainFrame::onMouseMiddleDown(long mX, long mY)
{
	GUIImage::onMouseMiddleDown(mX, mY);
}

void HUDMainFrame::onMouseRightDown(long mX, long mY)
{
	GUIImage::onMouseRightDown(mX, mY);
}

void HUDMainFrame::onMouseLeftUp(long mX, long mY)
{
	GUIImage::onMouseLeftUp(mX, mY);
}

void HUDMainFrame::onMouseMiddleUp(long mX, long mY)
{
	GUIImage::onMouseMiddleUp(mX, mY);
}

void HUDMainFrame::onMouseRightUp(long mX, long mY)
{
	GUIImage::onMouseRightUp(mX, mY);
}

void HUDMainFrame::onKeyDown(int letterKeys, int keys)
{
	GUIImage::onKeyDown(letterKeys, keys);
}

void HUDMainFrame::updateHPBar(float HPPercentage)
{
	float sizeY = HUD_HP_SIZE_Y * HPPercentage;
	float posY = HUD_HP_POS_Y + HUD_HP_SIZE_Y - sizeY;
	mpHPBar->setRelativePosition(D3DXVECTOR2(HUD_HP_POS_X, posY));
	mpHPBar->setRelativeSize(D3DXVECTOR2(HUD_HP_SIZE_X, sizeY));
}

void HUDMainFrame::updateCooldownBar(float cooldownPercentage)
{
	float sizeY = HUD_COOLDOWN_SIZE_Y * cooldownPercentage;
	float posY = HUD_COOLDOWN_POS_Y + HUD_COOLDOWN_SIZE_Y - sizeY;
	mpCooldownBar->setRelativePosition(D3DXVECTOR2(HUD_COOLDOWN_POS_X, posY));
	mpCooldownBar->setRelativeSize(D3DXVECTOR2(HUD_COOLDOWN_SIZE_X, sizeY));
}

void HUDMainFrame::updateSelectionCooldownBar(float cooldownPercentage)
{
	float sizeY = HUD_SELECTION_COOLDOWN_SIZE_Y * cooldownPercentage;
	float posY = HUD_SELECTION_COOLDOWN_POS_Y + HUD_SELECTION_COOLDOWN_SIZE_Y - sizeY;
	mpSelectionCooldownBar->setRelativePosition(D3DXVECTOR2(HUD_SELECTION_COOLDOWN_POS_X, posY));
	mpSelectionCooldownBar->setRelativeSize(D3DXVECTOR2(HUD_SELECTION_COOLDOWN_SIZE_X, sizeY));
}

void HUDMainFrame::updateWeapons(Tank::TankTypes tankType, Weapon::WeaponTypes weaponType)
{
	int arrayIndex = 0;
	int textureID = -1;
	switch(tankType)
	{
		case Tank::TankType_CloseCombat:
			mpMainWeapon->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			DX10Module::getInstance()->getTextureIndex("HUD_WeaponMinigun", arrayIndex, textureID);
			mpWeapon1->setTextureID(textureID);
			mpWeapon1->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			DX10Module::getInstance()->getTextureIndex("HUD_WeaponFlame", arrayIndex, textureID);
			mpWeapon2->setTextureID(textureID);
			mpWeapon2->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			DX10Module::getInstance()->getTextureIndex("HUD_WeaponPulse", arrayIndex, textureID);
			mpWeapon3->setTextureID(textureID);
			mpWeapon3->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			break;

		case Tank::TankType_LongRange:
			mpMainWeapon->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			DX10Module::getInstance()->getTextureIndex("HUD_WeaponCannon", arrayIndex, textureID);
			mpWeapon1->setTextureID(textureID);
			mpWeapon1->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			DX10Module::getInstance()->getTextureIndex("HUD_WeaponGauss", arrayIndex, textureID);
			mpWeapon2->setTextureID(textureID);
			mpWeapon2->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			DX10Module::getInstance()->getTextureIndex("HUD_WeaponBeam", arrayIndex, textureID);
			mpWeapon3->setTextureID(textureID);
			mpWeapon3->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
			break;

		//Driver.
		case Tank::TankType_Invalid:
			mpMainWeapon->setTextureID(-1);
			mpMainWeapon->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			mpWeapon1->setTextureID(-1);
			mpWeapon1->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			mpWeapon2->setTextureID(-1);
			mpWeapon2->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			mpWeapon3->setTextureID(-1);
			mpWeapon3->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f));
			break;

		default:
			break;
	}

	switch(weaponType)
	{
		case Weapon::WeaponTypes_Minigun:
			DX10Module::getInstance()->getTextureIndex("HUD_WeaponMinigun", arrayIndex, textureID);
			mpMainWeapon->setTextureID(textureID);
			break;

		case Weapon::WeaponTypes_Flamethrower:
			DX10Module::getInstance()->getTextureIndex("HUD_WeaponFlame", arrayIndex, textureID);
			mpMainWeapon->setTextureID(textureID);
			break;

		case Weapon::WeaponTypes_PulseCannon:
			DX10Module::getInstance()->getTextureIndex("HUD_WeaponPulse", arrayIndex, textureID);
			mpMainWeapon->setTextureID(textureID);
			break;

		case Weapon::WeaponTypes_Cannon:
			DX10Module::getInstance()->getTextureIndex("HUD_WeaponCannon", arrayIndex, textureID);
			mpMainWeapon->setTextureID(textureID);
			break;

		case Weapon::WeaponTypes_GaussCannon:
			DX10Module::getInstance()->getTextureIndex("HUD_WeaponGauss", arrayIndex, textureID);
			mpMainWeapon->setTextureID(textureID);
			break;

		case Weapon::WeaponTypes_BeamCannon:
			DX10Module::getInstance()->getTextureIndex("HUD_WeaponBeam", arrayIndex, textureID);
			mpMainWeapon->setTextureID(textureID);
			break;

		default:
			break;
	}
}

void HUDMainFrame::init()
{
	int arrayIndex = 0;
	int textureID = -1;
	DX10Module::getInstance()->getTextureIndex("HUD_Main", arrayIndex, textureID);

	getRenderInstance()->setTextureArrayIndex(arrayIndex);
	setTextureID(textureID);

	mpHPBar = myNew GUIImage(this);
	mpHPBar->setRelativePosition(D3DXVECTOR2(HUD_HP_POS_X, HUD_HP_POS_Y));
	mpHPBar->setRelativeSize(D3DXVECTOR2(HUD_HP_SIZE_X, HUD_HP_SIZE_Y));
	mpHPBar->setColor(D3DXCOLOR(0.8f, 0.1f, 0.8f, 0.8f));
	addSubFrame(mpHPBar);

	//Increase depth level.
	setDepthLevel(getDepthLevel() + 1);

	mpCooldownBar = myNew GUIImage(this);
	mpCooldownBar->setRelativePosition(D3DXVECTOR2(HUD_COOLDOWN_POS_X, HUD_COOLDOWN_POS_Y));
	mpCooldownBar->setRelativeSize(D3DXVECTOR2(HUD_COOLDOWN_SIZE_X, HUD_COOLDOWN_SIZE_Y));
	mpCooldownBar->setColor(D3DXCOLOR(0.8f, 0.2f, 0.2f, 0.5f));
	addSubFrame(mpCooldownBar);

	//Decrease depth level.
	setDepthLevel(getDepthLevel() - 1);

	mpSelectionCooldownBar = myNew GUIImage(this);
	mpSelectionCooldownBar->setRelativePosition(D3DXVECTOR2(HUD_SELECTION_COOLDOWN_POS_X, HUD_SELECTION_COOLDOWN_POS_Y));
	mpSelectionCooldownBar->setRelativeSize(D3DXVECTOR2(HUD_SELECTION_COOLDOWN_SIZE_X, HUD_SELECTION_COOLDOWN_SIZE_Y));
	mpSelectionCooldownBar->setColor(D3DXCOLOR(0.8f, 0.2f, 0.2f, 0.5f));
	addSubFrame(mpSelectionCooldownBar);

	DX10Module::getInstance()->getTextureIndex("HUD_WeaponMinigun", arrayIndex, textureID);

	mpMainWeapon = myNew GUIImage(this);
	mpMainWeapon->setRelativePosition(D3DXVECTOR2(HUD_COOLDOWN_POS_X, HUD_COOLDOWN_POS_Y));
	mpMainWeapon->setRelativeSize(D3DXVECTOR2(HUD_COOLDOWN_SIZE_X, HUD_COOLDOWN_SIZE_Y));
	mpMainWeapon->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	mpMainWeapon->setTextureID(textureID);
	addSubFrame(mpMainWeapon);

	mpWeapon1 = myNew GUIImage(this);
	mpWeapon1->setRelativePosition(D3DXVECTOR2(HUD_WEAPON_POS_X_1, HUD_WEAPON_POS_Y));
	mpWeapon1->setRelativeSize(D3DXVECTOR2(HUD_WEAPON_SIZE_X, HUD_WEAPON_SIZE_Y));
	mpWeapon1->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	mpWeapon1->setTextureID(textureID);
	addSubFrame(mpWeapon1);

	mpWeapon2 = myNew GUIImage(this);
	mpWeapon2->setRelativePosition(D3DXVECTOR2(HUD_WEAPON_POS_X_2, HUD_WEAPON_POS_Y));
	mpWeapon2->setRelativeSize(D3DXVECTOR2(HUD_WEAPON_SIZE_X, HUD_WEAPON_SIZE_Y));
	mpWeapon1->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	mpWeapon1->setTextureID(textureID);
	addSubFrame(mpWeapon2);

	mpWeapon3 = myNew GUIImage(this);
	mpWeapon3->setRelativePosition(D3DXVECTOR2(HUD_WEAPON_POS_X_3, HUD_WEAPON_POS_Y));
	mpWeapon3->setRelativeSize(D3DXVECTOR2(HUD_WEAPON_SIZE_X, HUD_WEAPON_SIZE_Y));
	mpWeapon1->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	mpWeapon1->setTextureID(textureID);
	addSubFrame(mpWeapon3);
}

void HUDMainFrame::notify(int message, GUIFrame *pSender)
{

}