/**
* @file
* @author Matteus Magnusson <senth.wallace@gmail.com>
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

#include "AiApplication.h"
#include "../Logic/StaticEnvironment.h"
#include "../Logic/EventManager.h"
#include "../Ai/AiEvents.h"
#include "../Ai/AiListener.h"

using namespace logic;

const int ENEMY_ROWS = 1;
const int ENEMY_COLS = 1;

const float ENEMY_SPACING = 50.0f;

const Enemy::EnemyTypes ENEMY_TYPE = Enemy::EnemyType_Jumper;

AiApplication::AiApplication()
{
	define("AgentAi");
}

AiApplication::~AiApplication()
{
}

void AiApplication::createInstance()
{
	assert(mpsInstance == NULL);

	mpsInstance = myNew AiApplication();
}

void AiApplication::createListeners()
{
	Application::createListeners();
}

void AiApplication::gameLoop(float deltaTime)
{
	Application::gameLoop(deltaTime);
}

void AiApplication::loadMeshes()
{
	Application::loadMeshes();
	//mpDxModule->loadObjectShapeInstance(gfx::RenderObject::ShapeTypes_Cube, 1, "Cube", 5000);
	//DEBUG_MESSAGE(LEVEL_LOW, "Cube Loaded");
	//mpDxModule->loadObjectShapeInstance(gfx::RenderObject::ShapeTypes_Cube, 2, "TriggerObject", 500);
	//mpDxModule->getObjectInstanceBase("TriggerObject")->setFlagShadow(false);

	//mpDxModule->loadObjectShapeInstance(gfx::RenderObject::ShapeTypes_Plane, 2, "Plane", 100);
	//DEBUG_MESSAGE(LEVEL_LOW, "Plane Loaded");

	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Tank,		Tank::TankType_CloseCombat,		"CloseCombatTank",	"../../ModelFiles/TankAir/NewTankAirGeometry.btb", "../../ModelFiles/TankAir/NewTankAirMaterial.btb", "");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Tank,		Tank::TankType_LongRange,		"LongRangeTank",	"../../ModelFiles/TankGround/NewTankGroundGeometry.btb", "../../ModelFiles/TankGround/NewTankGroundMaterial.btb", "");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Driver,	0,								"Bugman", "../../ModelFiles/Bugman/BugmanGeometry.btb", "../../ModelFiles/Bugman/BugmanMaterial.btb", "../../ModelFiles/Bugman/BugmanWeights.btb");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Jumper,		"Spider", "../../ModelFiles/Spider/SpiderGeometry.btb", "../../ModelFiles/Spider/SpiderMaterial.btb", "../../ModelFiles/Spider/SpiderWeights.btb");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_BlackWidow,	"Spider", "../../ModelFiles/Spider/SpiderGeometry.btb", "../../ModelFiles/Spider/SpiderMaterial.btb", "../../ModelFiles/Spider/SpiderWeights.btb");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Tarantula,		"Spider", "../../ModelFiles/Spider/SpiderGeometry.btb", "../../ModelFiles/Spider/SpiderMaterial.btb", "../../ModelFiles/Spider/SpiderWeights.btb");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_FireAnt,		"Ant", "../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_KamikazeAnt,	"Ant", "../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Ant,			"Ant", "../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_PlasmaBeetle,	"Beetle", "../../ModelFiles/Beetle/BeetleGeometry.btb", "../../ModelFiles/Beetle/BeetleMaterial.btb", "../../ModelFiles/Beetle/BeetleWeights.btb");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Scarab,		"Beetle", "../../ModelFiles/Beetle/BeetleGeometry.btb", "../../ModelFiles/Beetle/BeetleMaterial.btb", "../../ModelFiles/Beetle/BeetleWeights.btb");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Acid,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Beam,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Bullet,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Fire,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Flame,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Plasma,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Pulse,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_Pedestrian, 0,							"Bugman", "../../ModelFiles/Bugman/BugmanGeometry.btb", "../../ModelFiles/Bugman/BugmanMaterial.btb", "../../ModelFiles/Bugman/BugmanWeights.btb");

	//gfx::DX10Module::getInstance()->loadAnimation("../../ModelFiles/Ant/NewAnt_IdleAnimation.btb", "Ant_Idle");
	//gfx::DX10Module::getInstance()->loadAnimation("../../ModelFiles/Ant/NewAnt_MeleeAnimation.btb", "Ant_Melee");
	//gfx::DX10Module::getInstance()->loadAnimation("../../ModelFiles/Spider/SpiderWalkAnimation.btb", "Spider_Walk");
	//gfx::DX10Module::getInstance()->loadAnimation("../../ModelFiles/Beetle/BeetleWalkAnimation.btb", "Beetle_Walk");
	//gfx::DX10Module::getInstance()->loadAnimation("../../ModelFiles/Bugman/Bugman_WalkAnimation.btb", "Bugman_Walk");

	//mpModelInfo->bindMeshToActorType(Actor::ActorType_StaticEnvironment, StaticEnvironment::StaticEnvironmentType_Skyscraper,
	//	"Skyscraper", "../../ModelFiles/Buildings/Skyscraper01/Skyscraper01Geometry.btb", "../../ModelFiles/Buildings/Skyscraper01/Skyscraper01Material.btb", "");

	////mpModelInfo->bindMeshToActorType(Actor::ActorType_StaticEnvironment, StaticEnvironment::StaticEnvironmentType_WTFBuilding,
	////	"WTFBuilding", "../../ModelFiles/Buildings/WTFBuilding/WTFBuildingGeometry.btb", "../../ModelFiles/Buildings/WTFBuilding/WTFBuildingMaterial.btb", "", "");
	//mpModelInfo->bindMeshToActorType(Actor::ActorType_StaticEnvironment, StaticEnvironment::StaticEnvironmentType_TallGlassBuilding,
	//	"TallGlassBuilding", "../../ModelFiles/Buildings/TallGlassBuilding/TallGlassBuildingGeometry.btb", "../../ModelFiles/Buildings/TallGlassBuilding/TallGlassBuildingMaterial.btb", "");
}

void AiApplication::createObjects()
{
	//Application::createObjects();

	gfx::DX10Module::getInstance()->createWorldPlane(20, 20, 200.0f, 200.0f);

#ifdef USE_AI
	gfx::WorldPlane* pWorldPlane = gfx::DX10Module::getInstance()->getWorldPlane();
	utilities::Vec3Float planeSizeWorld(pWorldPlane->getTileX() * pWorldPlane->getTileSizeX(), 0.0f, pWorldPlane->getTileZ() * pWorldPlane->getTileSizeZ());
	mpAiFactory->setMapSize(planeSizeWorld.convertToMapCoordinates());
#endif

	int playerId = mpPlayerInfo->getNetworkId();

	gfx::TransformObject *pTempObj;

	// X - coordinate
	pTempObj = mpDxModule->createObjectInstanced(1);
	pTempObj->setPosition(D3DXVECTOR3(0.0f, 1.45f, 0.0f));
	pTempObj->setScale(D3DXVECTOR3(1000.0f, 1.0f, 1.0f));
	pTempObj->setID(logic::KEY_GROUND.convertToGfxId());
	pTempObj->setColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.4f));

	// Z - coordinate
	pTempObj = mpDxModule->createObjectInstanced(1);
	pTempObj->setPosition(D3DXVECTOR3(0.0f, 1.5f, 0.0f));
	pTempObj->setScale(D3DXVECTOR3(1.0f, 1.0f, 1000.0f));
	pTempObj->setID(logic::KEY_GROUND.convertToGfxId());
	pTempObj->setColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.4f));

	
	float zOffset = ENEMY_ROWS * ENEMY_SPACING;
	float playerZOffset = (playerId - 2) * zOffset;

	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateTank(Tank::TankType_LongRange, utilities::Position(60.0f, 0.0f, playerZOffset + zOffset / 2.0f), utilities::Direction(1.0f, 0.0f, 0.0f), Actor::PlayerMovingInfo())));

	// Buildings
	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateStaticEnvironment(StaticEnvironment::StaticEnvironmentType_Skyscraper, utilities::Position(100.0f, 0.0f, playerZOffset + zOffset), utilities::Direction(1.0f, 0.0f, 0.0f), utilities::Vec3Float(1.0f,1.0f,1.0f))));
	//safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateStaticEnvironment(StaticEnvironment::StaticEnvironmentType_TallGlassBuilding, utilities::Position(200.0f, 0.0f, playerZOffset + zOffset), utilities::Direction(1.0f, 0.0f, 0.0f), utilities::Vec3Float(1.0f,1.0f,1.0f))));

	// Enemies
	//safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_FireAnt, utilities::Position(-50.0f, 3.0f, 35.0f), utilities::Direction(0.0f,0.0f,0.0f))));
	//safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_FireAnt, utilities::Position(-50.0f, 3.0f, -35.0f), utilities::Direction(0.0f,0.0f,0.0f))));

	for (int x = 0; x < ENEMY_COLS; x++)
	{
		for (int z = 0; z < ENEMY_ROWS; z++)
		{
			float xPos = - static_cast<float>(x) * ENEMY_SPACING;
			float zPos = static_cast<float>(z) * ENEMY_SPACING + playerZOffset;
			safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(ENEMY_TYPE, utilities::Position(xPos, 0.0f, zPos), utilities::Direction(0.0f,0.0f,0.0f))));
		}
	}
}

void AiApplication::setCommandWindow()
{
	HWND hWnd = GetConsoleWindow();
	MoveWindow(hWnd,-1800,100,2000,800,TRUE);
}

void AiApplication::finalGameInitialization()
{

}

void AiApplication::registerEvents()
{
	Application::registerEvents();

#ifdef USE_AI
#endif
}

void AiApplication::registerNetworkEvents()
{
	Application::registerNetworkEvents();

#ifdef USE_AI
#endif
}