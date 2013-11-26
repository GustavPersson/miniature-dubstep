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

#include "EditorApplication.h"

using namespace logic;
using namespace gfx;

void EditorApplication::createInstance()
{
	assert(mpsInstance == NULL);

	mpsInstance = myNew EditorApplication();
}

EditorApplication::EditorApplication()
{

}

EditorApplication::~EditorApplication()
{

}

void EditorApplication::gameLoop(float deltaTime)
{
	Application::gameLoop(deltaTime);
}

void EditorApplication::loadMeshes()
{
	mpDxModule->loadObjectShapeInstance(gfx::ShapeTypes::CUBE, 1, "Cube", 5000);
	mpDxModule->loadObjectShapeInstance(gfx::ShapeTypes::CUBE, 2, "TriggerObject", 500);
	mpDxModule->getObjectInstanceBase("TriggerObject")->setFlagShadow(false);
	DEBUG_MESSAGE(LEVEL_LOW, "Cube Loaded");
	
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Tank,		Tank::TankType_CloseCombat,		"CloseCombatTank",	"../../ModelFiles/TankAir/NewTankAirGeometry.btb", "../../ModelFiles/TankAir/NewTankAirMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Tank,		Tank::TankType_LongRange,		"LongRangeTank",	"../../ModelFiles/TankGround/NewTankGroundGeometry.btb", "../../ModelFiles/TankGround/NewTankGroundMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Driver,	0,								"Bugman", "../../ModelFiles/Bugman/BugmanGeometry.btb", "../../ModelFiles/Bugman/BugmanMaterial.btb", "../../ModelFiles/Bugman/BugmanWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Ant,			"Ant", "../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_BlackWidow,	"Ant", "../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_FireAnt,		"Ant", "../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Jumper,		"Ant", "../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_KamikazeAnt,	"Ant", "../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_PlasmaBeetle,	"Beetle", "../../ModelFiles/Beetle/BeetleGeometry.btb", "../../ModelFiles/Beetle/BeetleMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Scarab,		"Ant", "../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Tarantula,		"Ant", "../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
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

	DX10Module::getInstance()->loadAnimation("../../ModelFiles/Ant/NewAnt_IdleAnimation.btb", "Ant_Idle");
	DX10Module::getInstance()->loadAnimation("../../ModelFiles/Ant/NewAnt_MeleeAnimation.btb", "Ant_Melee");
	DX10Module::getInstance()->loadAnimation("../../ModelFiles/Bugman/Bugman_WalkAnimation.btb", "Bugman_Walk");
}

void EditorApplication::createObjects()
{
	srand(static_cast<unsigned int>(time(0)));

	DX10Module::getInstance()->createWorldPlane(20, 20, 200.0f, 200.0f);

	safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateTank(Tank::TankType_CloseCombat, utilities::Position(0.0f, 0.0f, 0.0f),utilities::Direction(1.0f,0.0f,0.0f), Actor::PlayerMovingInfo())));
}
	
void EditorApplication::setCommandWindow()
{

}

void EditorApplication::registerEvents()
{
	Application::registerEvents();
}