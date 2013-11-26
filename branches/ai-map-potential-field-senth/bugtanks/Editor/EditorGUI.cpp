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

#include "EditorGUI.h"
#include "../Logic/Actor.h"
#include "../Logic/Ant.h"
#include "../Logic/BlackWidow.h"
#include "../Logic/FireAnt.h"
#include "../Logic/Jumper.h"
#include "../Logic/KamikazeAnt.h"
#include "../Logic/PlasmaBeetle.h"
#include "../Logic/Scarab.h"
#include "../Logic/Tarantula.h"

using namespace editor;
using namespace gfx;
using namespace logic;

using std::vector;

EditorGUI::EditorGUI(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight)
	: GUIHandler(pBaseInstance, clientWidth, clientHeight)
{
	mpUniversalList = NULL;
	mpUtilityBar = NULL;
	init();
}

EditorGUI::EditorGUI(const EditorGUI &GUI) : GUIHandler(GUI)
{
	mpUniversalList = NULL;
	mpUtilityBar = NULL;
	init();
}

const EditorGUI &EditorGUI::operator=(const EditorGUI &GUI)
{
	if(this != &GUI)
	{
		GUIHandler::operator=(GUI);
	}
	return *this;
}

EditorGUI::~EditorGUI()
{

}

const char *EditorGUI::getName()
{
	return "EditorGUI";
}

bool EditorGUI::handleEvent(const EventData &eventMessage)
{
	return GUIHandler::handleEvent(eventMessage);
}

void EditorGUI::showAllFrames()
{
	mpUniversalList->setFlagRender(true);
	mpUtilityBar->setFlagRender(true);
}

void EditorGUI::hideAllFrames()
{
	mpUniversalList->setFlagRender(false);
	mpUtilityBar->setFlagRender(false);
}

void EditorGUI::updateFrames(float deltaTime)
{
	GUIHandler::updateFrames(deltaTime);
}

void EditorGUI::updateSelectionList()
{
	//Retrieve all static actors and add them to the model content list.
	mpUniversalList->removeAllModelContent();
	mpUniversalList->addModelContent(StaticEnvironment::StaticEnvironmentType_CityMall, "City Mall");
	mpUniversalList->addModelContent(StaticEnvironment::StaticEnvironmentType_CornerStore, "Corner Store");
	mpUniversalList->addModelContent(StaticEnvironment::StaticEnvironmentType_GreenOffice, "Green Office");
	mpUniversalList->addModelContent(StaticEnvironment::StaticEnvironmentType_LBuidling, "L Building");
	mpUniversalList->addModelContent(StaticEnvironment::StaticEnvironmentType_Skyscraper, "Skyscraper");
	mpUniversalList->addModelContent(StaticEnvironment::StaticEnvironmentType_StandAloneBuilding, "Stand Alone Building");
	mpUniversalList->addModelContent(StaticEnvironment::StaticEnvironmentType_Taipei, "Taipei");
	mpUniversalList->addModelContent(StaticEnvironment::StaticEnvironmentType_TallGlassBuilding, "Tall Glass Building");
	mpUniversalList->addModelContent(StaticEnvironment::StaticEnvironmentType_WindowBuilding, "Window Building");
	mpUniversalList->addModelContent(StaticEnvironment::StaticEnvironmentType_WTFBuilding, "World Trade Building");

	//Retrieve all enemies and add them to the enemy content list.
	mpUniversalList->removeAllEnemyContent();
	mpUniversalList->addEnemyContent(Enemy::EnemyType_Ant, Ant::getEnemyName().c_str());
	mpUniversalList->addEnemyContent(Enemy::EnemyType_BlackWidow, BlackWidow::getEnemyName().c_str());
	mpUniversalList->addEnemyContent(Enemy::EnemyType_FireAnt, FireAnt::getEnemyName().c_str());
	mpUniversalList->addEnemyContent(Enemy::EnemyType_Jumper, Jumper::getEnemyName().c_str());
	mpUniversalList->addEnemyContent(Enemy::EnemyType_KamikazeAnt, KamikazeAnt::getEnemyName().c_str());
	mpUniversalList->addEnemyContent(Enemy::EnemyType_PlasmaBeetle, PlasmaBeetle::getEnemyName().c_str());
	mpUniversalList->addEnemyContent(Enemy::EnemyType_Scarab, Scarab::getEnemyName().c_str());
	mpUniversalList->addEnemyContent(Enemy::EnemyType_Tarantula, Tarantula::getEnemyName().c_str());
}

void EditorGUI::init()
{
	GUIHandler::init();

	mpUniversalList = myNew EditorGUIUniversalList(getInstanceBase(), getClientWidth(), getClientHeight(), 0);
	mpUniversalList->setRelativePosition(D3DXVECTOR2(0.8f, 0.0f));
	mpUniversalList->setRelativeSize(D3DXVECTOR2(0.2f, 1.0f));
	addFrame(mpUniversalList);

	mpUtilityBar = myNew EditorGUIUtilityBar(getInstanceBase(), getClientWidth(), getClientHeight(), 0);
	mpUtilityBar->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	mpUtilityBar->setRelativeSize(D3DXVECTOR2(0.2f, 1.0f));
	mpUtilityBar->setColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 0.9f));
	addFrame(mpUtilityBar);
}