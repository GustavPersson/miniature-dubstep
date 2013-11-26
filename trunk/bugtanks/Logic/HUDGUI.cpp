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

#include "HUDGUI.h"
#include "ActorHandler.h"
#include "Statistics.h"

using namespace logic;
using namespace gfx;

HUDGUI::HUDGUI(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight)
	: GUIHandler(pBaseInstance, clientWidth, clientHeight)
{
	mpMainFrame = NULL;
	mpScoreFrame = NULL;
	init();
}

HUDGUI::HUDGUI(const HUDGUI &handler)
	: GUIHandler(handler)
{
	mpMainFrame = NULL;
	mpScoreFrame = NULL;
	init();
}

HUDGUI::~HUDGUI()
{

}

const HUDGUI &HUDGUI::operator=(const HUDGUI &handler)
{
	if(this != &handler)
	{

	}
	return *this;
}

const char *HUDGUI::getName()
{
	return "HUDGUI";
}

void HUDGUI::show()
{
	mpMainFrame->setFlagVisible(true);
	mpScoreFrame->setFlagVisible(true);
}

void HUDGUI::hide()
{
	mpMainFrame->setFlagVisible(false);
	mpScoreFrame->setFlagVisible(false);
}

void HUDGUI::update(float deltaTime)
{
	//Retrieve the player actor and update its displayed values.
	float HPPercentage = 0.0f;
	float cooldownPercentage = 0.0f;
	float selectionCooldownPercentage = 0.0f;
	Tank::TankTypes tankType = Tank::TankType_Invalid;
	Weapon::WeaponTypes weaponType = Weapon::WeaponTypes_Invalid;
	KeyType playerKey = PlayerInfo::getInstance()->getPlayerId();
	Actor *pPlayer = ActorHandler::getInstance()->getActor(playerKey);
	if(pPlayer)
	{
		//Update HP percentage.
		if(pPlayer->getActorType() == Actor::ActorType_Tank || pPlayer->getActorType() == Actor::ActorType_Driver)
		{
			float HP = max(((Unit *)pPlayer)->getHealthPoints() , 0.0f);
			float HPMax = ((Unit *)pPlayer)->getHealthPointsMax();
			HPPercentage = HP / HPMax;
		}

		//Update cooldown percentages.
		if(pPlayer->getActorType() == Actor::ActorType_Tank)
		{
			cooldownPercentage = max(((Tank *)pPlayer)->getWeapon()->getCooldownPercentage(), 0.0f);
			selectionCooldownPercentage = max(((Tank *)pPlayer)->getSelectionCooldownPercentage(), 0.0f);
			tankType = ((Tank *)pPlayer)->getTankType();
			weaponType = ((Tank *)pPlayer)->getWeapon()->getWeaponType();
		}
	}
	mpMainFrame->updateHPBar(HPPercentage);
	mpMainFrame->updateCooldownBar(cooldownPercentage);
	mpMainFrame->updateSelectionCooldownBar(selectionCooldownPercentage);
	mpMainFrame->updateWeapons(tankType, weaponType);

	//Update score frame.
	int enemyCount = ActorHandler::getInstance()->getActiveEnemyCount();
	mpScoreFrame->updateTextEnemies(enemyCount);

	int hiddenCount = ActorHandler::getInstance()->getHiddenEnemyCount();
	mpScoreFrame->updateTextHidden(hiddenCount);

	int score = Statistics::getInstance()->getScore();
	mpScoreFrame->updateTextScore(score);

	float accuracy = Statistics::getInstance()->getAccuracy() * 100.0f;
	mpScoreFrame->updateTextAccuracy(accuracy);
}

void HUDGUI::init()
{
	mpMainFrame = myNew HUDMainFrame(getInstanceBase(), getClientWidth(), getClientHeight(), 0);
	mpMainFrame->setRelativePosition(D3DXVECTOR2(0.0f, 0.5f));
	mpMainFrame->setRelativeSize(D3DXVECTOR2(0.22f, 0.5f));
	mpMainFrame->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	addFrame(mpMainFrame);

	mpScoreFrame = myNew HUDScoreFrame(getInstanceBase(), getClientWidth(), getClientHeight(), 0);
	mpScoreFrame->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	mpScoreFrame->setRelativeSize(D3DXVECTOR2(0.22f, 0.22f));
	mpScoreFrame->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	addFrame(mpScoreFrame);
}