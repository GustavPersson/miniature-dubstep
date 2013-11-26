/**
* @file
* @author Jonathan Udd <jonis.x@gmail.com>
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

#include "LogicApplication.h"
#include "../logic/IEventManager.h"
#include "../Logic/Events.h"
#include "../Logic/GFXListener.h"
#include <cmath>


using namespace logic;

LogicApplication::LogicApplication()
{
	// TODO - Constructor
}

LogicApplication::~LogicApplication()
{
	// TODO - Destructor
}

void LogicApplication::createInstance()
{
	assert(mpsInstance == NULL);

	mpsInstance = myNew LogicApplication();
}

void LogicApplication::createListeners()
{
	Application::createListeners();


}

void LogicApplication::gameLoop(float deltaTime)
{
	Application::gameLoop(deltaTime);
}

void LogicApplication::loadMeshes()
{
	//Application::loadMeshes();
	mpDxModule->loadObjectShapeInstance(gfx::RenderObject::ShapeTypes_Cube, 1, "Cube", 5000);
	DEBUG_MESSAGE(LEVEL_LOW, "Cube Loaded");
	mpDxModule->loadObjectShapeInstance(gfx::RenderObject::ShapeTypes_Plane, 2, "Plane", 100);
	DEBUG_MESSAGE(LEVEL_LOW, "Plane Loaded");
	mpDxModule->loadObjectShapeInstance(gfx::RenderObject::ShapeTypes_Cube, 3, "TriggerObject", 500);
	mpDxModule->getObjectInstanceBase("TriggerObject")->setFlagShadow(false);

	mpModelInfo->bindMeshToActorType(Actor::ActorType_Tank,		Tank::TankType_CloseCombat,		"CloseCombatTank",	"../../ModelFiles/TankAir/NewTankAirGeometry.btb", "../../ModelFiles/TankAir/NewTankAirMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Tank,		Tank::TankType_LongRange,		"LongRangeTank",	"../../ModelFiles/TankGround/NewTankGroundGeometry.btb", "../../ModelFiles/TankGround/NewTankGroundMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Driver,	0,								"Bugman", "../../ModelFiles/Bugman/BugmanGeometry.btb", "../../ModelFiles/Bugman/BugmanMaterial.btb", "../../ModelFiles/Bugman/BugmanWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Jumper,		"Spider", "../../ModelFiles/Spider/SpiderGeometry.btb", "../../ModelFiles/Spider/SpiderMaterial.btb", "../../ModelFiles/Spider/SpiderWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_BlackWidow,	"Spider", "../../ModelFiles/Spider/SpiderGeometry.btb", "../../ModelFiles/Spider/SpiderMaterial.btb", "../../ModelFiles/Spider/SpiderWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Tarantula,		"Spider", "../../ModelFiles/Spider/SpiderGeometry.btb", "../../ModelFiles/Spider/SpiderMaterial.btb", "../../ModelFiles/Spider/SpiderWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_FireAnt,		"Ant", "../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_KamikazeAnt,	"Ant", "../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Ant,			"Ant", "../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_PlasmaBeetle,	"Beetle", "../../ModelFiles/Beetle/BeetleGeometry.btb", "../../ModelFiles/Beetle/BeetleMaterial.btb", "../../ModelFiles/Beetle/BeetleWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Scarab,		"Beetle", "../../ModelFiles/Beetle/BeetleGeometry.btb", "../../ModelFiles/Beetle/BeetleMaterial.btb", "../../ModelFiles/Beetle/BeetleWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Acid,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Beam,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Bullet,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Fire,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Flame,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Plasma,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Pulse,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Pedestrian, 0,							"Bugman", "../../ModelFiles/Bugman/BugmanGeometry.btb", "../../ModelFiles/Bugman/BugmanMaterial.btb", "../../ModelFiles/Bugman/BugmanWeights.btb");

	mpModelInfo->bindMeshToActorType(Actor::ActorType_StaticEnvironment, StaticEnvironment::StaticEnvironmentType_StandAloneBuilding,
		"StandAloneBuilding", "../../ModelFiles/Buildings/StandAloneBuilding/StandAloneBuildingGeometry.btb", "../../ModelFiles/Buildings/StandAloneBuilding/StandAloneBuildingMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_StaticEnvironment, StaticEnvironment::StaticEnvironmentType_CityMall,
		"CityMall",	"../../ModelFiles/Buildings/CityMall/CityMallGeometry.btb",	"../../ModelFiles/Buildings/CityMall/CityMallMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_StaticEnvironment, StaticEnvironment::StaticEnvironmentType_CornerStore,
		"CornerStore", "../../ModelFiles/Buildings/CornerStore/CornerStoreGeometry.btb", "../../ModelFiles/Buildings/CornerStore/CornerStoreMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_StaticEnvironment, StaticEnvironment::StaticEnvironmentType_GreenOffice,
		"GreenOffice", "../../ModelFiles/Buildings/GreenOffice/GreenOfficeGeometry.btb", "../../ModelFiles/Buildings/GreenOffice/GreenOfficeMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_StaticEnvironment, StaticEnvironment::StaticEnvironmentType_LBuidling,
		"LBuidling", "../../ModelFiles/Buildings/L-Building01/L-Building01Geometry.btb", "../../ModelFiles/Buildings/L-Building01/L-Building01Material.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_StaticEnvironment, StaticEnvironment::StaticEnvironmentType_Skyscraper,
		"Skyscraper", "../../ModelFiles/Buildings/Skyscraper01/Skyscraper01Geometry.btb", "../../ModelFiles/Buildings/Skyscraper01/Skyscraper01Material.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_StaticEnvironment, StaticEnvironment::StaticEnvironmentType_Taipei,
		"Taipei", "../../ModelFiles/Buildings/Taipei/TaipeiGeometry.btb", "../../ModelFiles/Buildings/Taipei/TaipeiMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_StaticEnvironment, StaticEnvironment::StaticEnvironmentType_TallGlassBuilding,
		"TallGlassBuilding", "../../ModelFiles/Buildings/TallGlassBuilding/TallGlassBuildingGeometry.btb", "../../ModelFiles/Buildings/TallGlassBuilding/TallGlassBuildingMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_StaticEnvironment, StaticEnvironment::StaticEnvironmentType_WindowBuilding,
		"WindowBuilding", "../../ModelFiles/Buildings/WindowBuilding/WindowBuildingGeometry.btb", "../../ModelFiles/Buildings/WindowBuilding/WindowBuildingMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_StaticEnvironment, StaticEnvironment::StaticEnvironmentType_WTFBuilding,
		"WTFBuilding", "../../ModelFiles/Buildings/WTFBuilding/WTFBuildingGeometry.btb", "../../ModelFiles/Buildings/WTFBuilding/WTFBuildingMaterial.btb", "");

	gfx::DX10Module::getInstance()->loadAnimation("../../ModelFiles/Ant/NewAnt_IdleAnimation.btb", "Ant_Idle");
	gfx::DX10Module::getInstance()->loadAnimation("../../ModelFiles/Ant/NewAnt_MeleeAnimation.btb", "Ant_Melee");
	gfx::DX10Module::getInstance()->loadAnimation("../../ModelFiles/Spider/SpiderWalkAnimation.btb", "Spider_Walk");
	gfx::DX10Module::getInstance()->loadAnimation("../../ModelFiles/Beetle/BeetleWalkAnimation.btb", "Beetle_Walk");
	gfx::DX10Module::getInstance()->loadAnimation("../../ModelFiles/Bugman/Bugman_WalkAnimation.btb", "Bugman_Walk");
}

void LogicApplication::createObjects()
{
	Application::createObjects();

	/*gfx::TransformObject *tempObj = NULL;

	tempObj = mpDxModule->createObjectInstanced("Plane");
	tempObj->setName("WorldPlane");
	tempObj->setPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	tempObj->setScale(D3DXVECTOR3(40.0f, 1.0f, 40.0f));
	tempObj->setID(KEY_GROUND.convertToGfxId());
	tempObj->setColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_PlasmaBeetle, utilities::Position(-50.0f, 0.0f, 40.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	//safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_Ant, utilities::Position(-90.0f, 0.0f, 40.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	//safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_Jumper, utilities::Position(-90.0f, 0.0f, 20.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateTank(mpGameOptions->mTankType, utilities::Position(0.0f, 0.0f, 20.0f), utilities::Direction(1.0f, 0.0f, 0.0f), Actor::PlayerMovingInfo())));
	//safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateStaticEnvironment(StaticEnvironment::StaticEnvironmentType_Skyscraper, utilities::Position(0.0f, 0.0f, 0.0f), utilities::Direction(1.0f, 0.0f, 0.0f), utilities::Vec3Float(1.0f,1.0f,1.0f))));
	//safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateStaticEnvironment(StaticEnvironment::StaticEnvironmentType_CityMall, utilities::Position(80.0f, 0.0f, 20.0f), utilities::Direction(1.0f, 0.0f, 0.0f), utilities::Vec3Float(1.0f,1.0f,1.0f))));

	/*for(float i = 0; i < 20; i++)
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_FireAnt, utilities::Position(100.0f, 0.0f, i*20.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	}*/

	//gfx::TransformObject *pTempObj;

	// Generate the grid
/*	for (int i = -100; i < 100; i++)
	{
		utilities::MapCoordinate mapCoord(i, 0);
		utilities::Vec3Float pos = mapCoord.convertToWorldCoordinates();
		
		pos -= utilities::MAP_IN_WORLD_COORDS_HALF_VEC;

		// X
		pTempObj = mpDxModule->createObjectInstanced(1);
		pTempObj->setPosition(D3DXVECTOR3(pos.x, 1.40f, 0.0f));
		pTempObj->setScale(D3DXVECTOR3(0.1f, 0.1f, 1000.0f));
		pTempObj->setID(logic::KEY_GROUND.convertToGfxId());
		pTempObj->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));

		// Y
		pTempObj = mpDxModule->createObjectInstanced(1);
		pTempObj->setPosition(D3DXVECTOR3(0.0f, 1.35f, pos.x));
		pTempObj->setScale(D3DXVECTOR3(1000.0f, 0.1f, 0.1f));
		pTempObj->setID(logic::KEY_GROUND.convertToGfxId());
		pTempObj->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.5f));
	}*/

	/*srand(static_cast<unsigned int>(time(0)));
	int colorCounter;
	gfx::TransformObject *tempObj;
	for(int i = 0; i < 10; i++)
	{

		colorCounter = i;
		for(int j = 0; j < 100; j++)
		{
			int rnd = rand()%10;
			
			if(rnd == 0)
			{
			#ifdef USE_NETWORK
				if (mpGameOptions->mIsServer)
				{
					safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateStaticEnvironment(StaticEnvironment::StaticEnvironmentType_StandAloneBuilding, utilities::Position(-30.0f * j, 0.0f, -30.0f * i),utilities::Direction(1.0f,0.0f,0.0f), utilities::Vec3Float(5.0f,5.0f,5.0f))));
				}
			#endif
			}
			else
			{
				tempObj = mpDxModule->createObjectInstanced(1);
				tempObj->setPosition(D3DXVECTOR3(-30.0f * j, 0.0f,-30.0f * i));
				tempObj->setScale(D3DXVECTOR3(15.0f, 0.1f, 15.0f));
				tempObj->setID(KEY_GROUND.convertToGfxId());

				int color = colorCounter % 4;
				if(color == 0)
					tempObj->setColor(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f));
				else if(color == 1)
					tempObj->setColor(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));
				else if(color == 2)
					tempObj->setColor(D3DXCOLOR(1.0f,1.0f,0.0f,1.0f));
				else if(color == 3)
					tempObj->setColor(D3DXCOLOR(0.0f,1.0f,1.0f,1.0f));
				colorCounter++;
			}
		}
	}*/
	/*safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_FireAnt, utilities::Position(-50.0f, 3.0f, 0.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_Ant, utilities::Position(70.0f, 3.0f, 40.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_KamikazeAnt, utilities::Position(20, 3.0f, 100.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_Tarantula, utilities::Position(45.0f, 3.0f, -70.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_BlackWidow, utilities::Position(100.0f, 3.0f, 0.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_Jumper, utilities::Position(65.0f, 3.0f, 23.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_Scarab, utilities::Position(44.0f, 3.0f, 22.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_PlasmaBeetle, utilities::Position(77.0f, 3.0f, 88.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
*/
	/*for(int i = Building::BuildingType_First; i < Building::BuildingType_Lim; i++)
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateBuilding(static_cast<Building::BuildingTypes>(i), utilities::Position(50.0f, 0.0f, i * 40.0f))));
	}*/
}

void LogicApplication::setCommandWindow()
{
	HWND hWnd = GetConsoleWindow();
	MoveWindow(hWnd, 2000, 100, 1500, 600, TRUE);
}
void LogicApplication::registerEvents()
{
	Application::registerEvents();
}