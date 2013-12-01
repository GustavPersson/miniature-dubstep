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
* 
* @section DESCRIPTION
*
* Defines the entry point for the application
*/

#include "Application.h"
#include "../Utilities/ErrorHandler.h"
#include "windows.h"
#include <string.h>

#include <boost/shared_ptr.hpp>


#include "../Network/Network.h"
#include "../Network/IdManager.cpp"
#include "../Network/EventForwarder.h"

#include "../Logic/Events.h"
#include "../Logic/GlobalTimer.h"
#include "../Logic/snooper.h"
#include "../Logic/LogicListener.h"
#include "../Logic/KeyHandler.h"
#include "../Logic/PlayerInfo.h"
#include "../Logic/Tank.h"
#include "../Logic/BlackWidow.h"
#include "../Logic/LogicQuadTree.h"
#include "../Logic/Statistics.h"
#include "../Logic/ActorConstants.h"

#include "../GFX/GUIFrameTest.h"

#include "../Editor/EditorHandler.h"

#include "../Sound/SoundListener.h"

#ifdef USE_AI
#include "../Ai/AiListener.h"
#include "../Ai/AiEvents.h"
#endif

using namespace logic;

Application* Application::mpsInstance = NULL;


Application::Application()
{
#ifdef USE_AI
	mpAiFactory		= NULL;
#endif

#ifdef USE_EDITOR
	mpEditor = NULL;
#endif

	mpActorHandler	 = NULL;
	mpInputStates	 = NULL;
	mpStatistics	 = NULL;

#ifdef USE_SOUND
	mpSoundSystem	 = NULL;
	mTrack = -1;
#endif
	
	mpEventManager   = NULL;
	mpGlobalTimer	 = NULL;
	mpCamera		 = NULL;
	mpMesh			 = NULL;
	mpPlayerInfo	 = NULL;
	mpNetwork		 = NULL;
	mpIDManager		 = NULL;
	mpServLookup	 = NULL;
	mpGameOptions	 = NULL;
	mpModelInfo		 = NULL;
	mpGFXListener	 = NULL;
	mpTriggerHandler = NULL;
	mpHUDHandler	 = NULL;


	mpStartMenuHandler		= NULL;
	mpLoadingScreenHandler	= NULL;

	mbWindowedMode	= false;
	mbIsRunning		= false;

	mNrOfFinishedLoading = 0;

	mpDxModule		= NULL;

	mGameState		= GameState_Lobby;
	mNetworkState	= NetworkState_Client;
}

void Application::createInstance()
{
	assert(mpsInstance == NULL);

	mpsInstance = myNew Application();
}

Application* Application::getInstance()
{
	assert(mpsInstance != NULL);
	return mpsInstance;
}

Application::~Application()
{
	mpsInstance = NULL;
}

void Application::onClose()
{
	logic::KeyHandler *mpKeyHandler = logic::KeyHandler::getInstance();

	SAFE_DELETE(mpKeyHandler);
	SAFE_DELETE(mpServLookup);
	SAFE_DELETE(mpGameOptions);

#ifdef USE_AI
	ai::AiFactory::destroy();
#endif

	SAFE_DELETE(mpNetwork);
	SAFE_DELETE(mpIDManager);
	SAFE_DELETE(mpQuadTree);

#ifdef USE_SOUND
	SAFE_DELETE(mpSoundSystem);
#endif

	SAFE_DELETE(mpInputStates);
	SAFE_DELETE(mpModelInfo);

	SAFE_DELETE(mpDxModule);
	SAFE_DELETE(mpPlayerInfo);

	SAFE_DELETE(mpHUDHandler)

#ifdef USE_NETWORK
	network::ShutdownNetwork();
#endif

	SAFE_DELETE(mpEventManager);
	SAFE_DELETE(mpGlobalTimer);


#ifdef _DEBUG
	checkForErrors();
#endif // _DEBUG

	releaseLogger();
}

bool Application::initInstance()
{
#ifndef _DEBUG

		if(!isOnlyInstance(getGameTitle()))
			return false;
#endif

#ifdef USE_AI
	ai::AiFactory::create();
	mpAiFactory = ai::AiFactory::getInstance();
#endif

	SetCursor(NULL);

	//Start checking for resources.
	bool mResourceCheck = false;

	while (!mResourceCheck)
	{
		try
		{
			const DWORD mPhysicalRAM = 1024 * MEGABYTE;
			const DWORD mVirtualRAM = 1024 * MEGABYTE;

			checkMemory(mPhysicalRAM, mVirtualRAM);

			const int diskSpace = 10 * MEGABYTE;
			checkHardDisk(diskSpace);
		}
		catch (ErrorHandler err)
		{
			if (err.handle() == ERR_RETRY)
				continue;
			else
				return false;
		}
		mResourceCheck = true;
	}

	setCommandWindow();

	mpGlobalTimer = logic::GlobalTimer::getInstance();

	mpEventManager = myNew logic::EventManager("Bug Tanks Event Mgr", true);
	if (!mpEventManager)
	{
		return false;
	}


#ifdef _DEBUG
	mpSnooper = boost::shared_ptr<logic::EventSnooper>(myNew logic::EventSnooper("snooper.log"));
	safeAddListener(mpSnooper, logic::EventType(logic::gWildcardEventType));
#endif

	logic::safeTriggerEvent(logic::EvtData_System_EventSystemStarted());
	
	logic::Actor::init();
	mpActorHandler = logic::ActorHandler::getInstance();
	mpStatistics = logic::Statistics::getInstance();
	mpGFXListener = myNew GFXListener();

#ifdef USE_NETWORK
	network::InitializeNetwork();
#endif

	mpPlayerInfo = logic::PlayerInfo::getInstance();
	
	mpDxModule = gfx::DX10Module::getInstance();
	mpDxModule->getLight()->setAmbientLight(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	mpDxModule->getLight()->setLightDir(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	mpDxModule->getLight()->setDiffuseLight(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	mpDxModule->getLight()->setSpecularLight(D3DXCOLOR(1.0f, 1.0f, 1.0f, 20.0f));
	mpDxModule->create();

	//Wait for dxModule to initialize.
	while(!mpDxModule->getRunFlag())
	{
		Sleep(10);
	}

	logic::safeTriggerEvent(logic::EvtData_System_GFXSystemStarted());
	
	ActorConstants::setConstants("../ActorConstants.ini");

	createLogic();

#ifdef USE_EDITOR
	mpDxModule->lock(0);
	mpEditor = editor::EditorHandler::getInstance();
	mpEditor->deactivate();
	mpDxModule->unlock(0);
#endif

	mbIsRunning = true;
	return true;
}

void Application::createLogic()
{	
	mpGameOptions	= myNew GameOptions("../config.ini");
	mNetworkState	= static_cast<NetworkStates>(mpGameOptions->mIsServer);

#ifdef USE_NETWORK
	mGameState		= GameState_MainMenu;
	mpServLookup = myNew network::ServerLookup();
#else
	mGameState		= GameState_Starting;
#endif

	//mpActorHandler->setDXModule();

	//If InputStates is created after SoundSystem, the memory check won't work.
	mpInputStates = logic::InputStates::getInstance();

#ifdef USE_SOUND
	mpSoundSystem = sound::SoundSystem::getInstance();	
#endif

	createListeners();
	registerEvents();
	
	mpQuadTree = logic::LogicQuadTree::getInstance();
	mpActorHandler->setLogicQuadTreeInstance(mpQuadTree);

#ifdef USE_SOUND
	mpSoundSystem->setMusicMasterVolume(mpGameOptions->mMusicVolume);
	mpSoundSystem->setSoundMasterVolume(mpGameOptions->mSoundEffectsVolume);
#endif

	mpTriggerHandler = TriggerHandler::getInstance();

#ifdef USE_NETWORK
	mpIDManager = network::IdManager::getInstance();

#endif

#ifdef USE_GUI
	mpDxModule->lock(0);
	mpDxModule->loadGUISpriteInstance(5, "Main Menu", 15000);

	mpStartMenuHandler = myNew GUIHandlerStartMenu(mpDxModule->getGUISpriteInstanceBase("Main Menu"), mpDxModule->getClientWidth(), mpDxModule->getClientHeight());

	mpDxModule->unlock(0);
#endif

	//Initialize HUD.
	mpDxModule->lock(0);
	mpHUDHandler = HUDHandler::getInstance();
	mpDxModule->unlock(0);
}

void Application::createListeners()
{
	mLogicListener	= boost::shared_ptr<logic::LogicListener>(myNew logic::LogicListener());
#ifdef USE_SOUND
	mSoundListener	= boost::shared_ptr<sound::SoundListener>(myNew sound::SoundListener());
#endif
#ifdef USE_AI
	mAiListener		= boost::shared_ptr<ai::AiListener>(myNew ai::AiListener());
#endif
	mGFXListener	= boost::shared_ptr<logic::GFXListener>(mpGFXListener);
	mStatisticsListener = boost::shared_ptr<logic::Statistics>(mpStatistics);
	
	mActorHandlerListener	= boost::shared_ptr<logic::ActorHandler>(mpActorHandler);	
}

void Application::registerEvents()
{
	
	//Add events to the LogicListener
	logic::safeAddListener(mLogicListener, logic::EvtData_System_EventSystemStarted::msEventType);
	logic::safeAddListener(mLogicListener, logic::EvtData_System_StartGame::msEventType);
	logic::safeAddListener(mLogicListener, logic::EvtData_System_StartLoading::msEventType);
	logic::safeAddListener(mLogicListener, logic::EvtData_System_FinishedLoading::msEventType);
	logic::safeAddListener(mLogicListener, logic::EvtData_System_EndGame::msEventType);
	logic::safeAddListener(mLogicListener, logic::EvtData_System_GameOver::msEventType);
	logic::safeAddListener(mLogicListener, logic::EvtData_System_PlayerDropped::msEventType);
	logic::safeAddListener(mLogicListener, logic::EvtData_From_EnemyHit::msEventType);
	logic::safeAddListener(mLogicListener, logic::EvtData_To_InputChangeGameState::msEventType);
	logic::safeAddListener(mLogicListener, logic::EvtData_System_EnterEditorMode::msEventType);
	logic::safeAddListener(mLogicListener, logic::EvtData_System_ExitEditorMode::msEventType);

	logic::safeAddListener(mStatisticsListener, logic::EvtData_From_ActorDestroyed::msEventType);
	logic::safeAddListener(mStatisticsListener, logic::EvtData_System_EndGame::msEventType);
	logic::safeAddListener(mStatisticsListener, logic::EvtData_From_EnemyHit::msEventType);
	logic::safeAddListener(mStatisticsListener, logic::EvtData_From_WeaponFired::msEventType);
	logic::safeAddListener(mStatisticsListener, logic::EvtData_From_EnemyHitNetwork::msEventType);


#ifdef USE_SOUND
	//Add events to the SoundListener
	logic::safeAddListener(mSoundListener, logic::EvtData_To_ActorStartMove::msEventType);
	logic::safeAddListener(mSoundListener, logic::EvtData_To_ActorStopMove::msEventType);

	logic::safeAddListener(mSoundListener, logic::EvtData_From_ActorCreated::msEventType);
	logic::safeAddListener(mSoundListener, logic::EvtData_From_ActorDestroyed::msEventType);
	logic::safeAddListener(mSoundListener, logic::EvtData_From_WeaponFired::msEventType);
	logic::safeAddListener(mSoundListener, logic::EvtData_From_EnemyHit::msEventType);
	logic::safeAddListener(mSoundListener, logic::EvtData_From_WeaponSecondaryFired::msEventType);
	logic::safeAddListener(mSoundListener, logic::EvtData_From_ActorCollisionObject::msEventType);
	logic::safeAddListener(mSoundListener, logic::EvtData_From_EnemyMeleeAttack::msEventType);
	logic::safeAddListener(mSoundListener, logic::EvtData_From_KamikazeAntExplode::msEventType);
#endif
	//Add events to the GFXListener

	logic::safeAddListener(mGFXListener, logic::EvtData_To_CameraStartRotate::msEventType);
	logic::safeAddListener(mGFXListener, logic::EvtData_To_CameraStopRotate::msEventType);

#ifdef USE_AI
	logic::safeAddListener(mAiListener, ai::EvtData_To_AiTogglePause::msEventType);
	logic::safeAddListener(mAiListener, ai::EvtData_To_AgentAiGoals::msEventType);
	logic::safeAddListener(mAiListener, ai::EvtData_To_AiAddToConflictMap::msEventType);
	logic::safeAddListener(mAiListener, ai::EvtData_To_AiRemoveFromConflictMap::msEventType);
	logic::safeAddListener(mAiListener, ai::EvtData_To_AgentAiTarget::msEventType);
	logic::safeAddListener(mAiListener, ai::EvtData_From_JumperAiJump::msEventType);
#endif

	//Add events to the ActorHandler
	logic::safeAddListener(mActorHandlerListener, EvtData_System_MapDefeat::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_System_MapVictory::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_EnemyUpdateHp::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_TankStartFire::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_TankStopFire::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_DriverStartFire::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_DriverStopFire::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_DriverStrafe::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_TankStartFireSecondary::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_TankStopFireSecondary::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_TankChangeWeapon::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorStartMove::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorStopMove::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorStartRotate::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorStopRotate::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorCreateTank::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorCreateEnemy::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorCreateEnemyID::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorCreateMunition::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorCreateDriver::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorCreateSpectator::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorCreateStaticEnvironment::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorCreateStaticEnvironmentID::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorReduceHp::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_PlayerToggleGodMode::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorDestroyAll::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_To_ActorDestroyAllNonPlayer::msEventType);

	logic::safeAddListener(mActorHandlerListener, EvtData_From_ActorCollisionRay::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_From_ActorCollisionObject::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_From_DriverEnterTank::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_From_ActorHandlerSyncActors::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_From_ActorDestroyed::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_From_WeaponFireMeshMunition::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_From_WeaponFireRayMunition::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_From_EnemySmallExplosion::msEventType);

	//Add events to the <inputName>
}

void Application::registerNetworkEvents()
{

	mNetworkListener = boost::shared_ptr<network::EventForwarder>(myNew network::EventForwarder(mpNetwork->getPeer()));

	logic::safeAddListener(mNetworkListener, logic::EvtData_System_StartGame::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_System_StartLoading::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_System_FinishedLoading::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_System_EnterEditorMode::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_System_ExitEditorMode::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_System_MapVictory::msEventType);
	logic::safeAddListener(mActorHandlerListener, EvtData_System_MapDefeat::msEventType);


	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorStartMove::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorStopMove::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorStartRotate::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorStopRotate::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_TankStartFire::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_TankStopFire::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorCreateTank::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorCreateEnemy::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorCreateEnemyID::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorCreateMunition::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorCreateDriver::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorCreateStaticEnvironment::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorCreateStaticEnvironmentID::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorCreateSpectator::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorReduceHp::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorDestroyAll::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_ActorDestroyAllNonPlayer::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_IncreaseMapWeights::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_To_SetMapWeights::msEventType);
	
	logic::safeAddListener(mNetworkListener, logic::EvtData_From_ActorDestroyed::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_From_ActorHandlerSyncActors::msEventType);

	logic::safeAddListener(mNetworkListener, logic::EvtData_From_WeaponFireMeshMunition::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_From_WeaponFireRayMunition::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_From_EnemySmallExplosion::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_From_WeaponFired::msEventType);

	logic::safeAddListener(mNetworkListener, logic::EvtData_To_TankChangeWeapon::msEventType);
	logic::safeAddListener(mNetworkListener, logic::EvtData_From_EnemyHitNetwork::msEventType);

#ifdef USE_AI
	logic::safeAddListener(mNetworkListener, ai::EvtData_To_AiTogglePause::msEventType);
	logic::safeAddListener(mNetworkListener, ai::EvtData_To_AgentAiGoals::msEventType);
	logic::safeAddListener(mNetworkListener, ai::EvtData_To_AiAddToConflictMap::msEventType);
	logic::safeAddListener(mNetworkListener, ai::EvtData_To_AiRemoveFromConflictMap::msEventType);
	logic::safeAddListener(mNetworkListener, ai::EvtData_To_AgentAiTarget::msEventType);
	logic::safeAddListener(mNetworkListener, ai::EvtData_From_JumperAiJump::msEventType);
#endif

	network::EventMsgHandler *mpEventMsgHandler = myNew network::EventMsgHandler();
	mpNetwork->getPeer()->getPeerManager()->addMessageHandler(mpEventMsgHandler);
}

void Application::removeNetworkEvents()
{
	logic::safeDelListener(mNetworkListener, logic::EvtData_System_StartGame::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_System_StartLoading::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_System_FinishedLoading::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_System_EnterEditorMode::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_System_ExitEditorMode::msEventType);

	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorStartMove::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorStopMove::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorStartRotate::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorStopRotate::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_TankStartFire::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_TankStopFire::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorCreateTank::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorCreateEnemy::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorCreateEnemyID::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorCreateMunition::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorCreateDriver::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorCreateStaticEnvironment::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorCreateStaticEnvironmentID::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorCreateSpectator::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorReduceHp::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorDestroyAll::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_ActorDestroyAllNonPlayer::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_IncreaseMapWeights::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_To_SetMapWeights::msEventType);
	
	logic::safeDelListener(mNetworkListener, logic::EvtData_From_ActorDestroyed::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_From_ActorHandlerSyncActors::msEventType);

	logic::safeDelListener(mNetworkListener, logic::EvtData_From_WeaponFireMeshMunition::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_From_WeaponFireRayMunition::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_From_EnemySmallExplosion::msEventType);
	logic::safeDelListener(mNetworkListener, logic::EvtData_From_WeaponFired::msEventType);

	logic::safeDelListener(mNetworkListener, logic::EvtData_To_TankChangeWeapon::msEventType);

#ifdef USE_AI
	logic::safeDelListener(mNetworkListener, ai::EvtData_To_AiTogglePause::msEventType);
	logic::safeDelListener(mNetworkListener, ai::EvtData_To_AgentAiGoals::msEventType);
	logic::safeDelListener(mNetworkListener, ai::EvtData_To_AiAddToConflictMap::msEventType);
	logic::safeDelListener(mNetworkListener, ai::EvtData_To_AiRemoveFromConflictMap::msEventType);
	logic::safeDelListener(mNetworkListener, ai::EvtData_To_AgentAiTarget::msEventType);
	logic::safeDelListener(mNetworkListener, ai::EvtData_From_JumperAiJump::msEventType);
#endif
}

void Application::loadMeshes()
{
	mpDxModule->loadObjectShapeInstance(gfx::RenderObject::ShapeTypes_Cube, 1, "Cube", 5000);
	DEBUG_MESSAGE(LEVEL_LOW, "Cube Loaded");
	mpDxModule->loadObjectShapeInstance(gfx::RenderObject::ShapeTypes_Plane, 2, "Plane", 100);
	DEBUG_MESSAGE(LEVEL_LOW, "Plane Loaded");
	mpDxModule->loadObjectShapeInstance(gfx::RenderObject::ShapeTypes_Cube, 3, "TriggerObject", 500);
	mpDxModule->getObjectInstanceBase("TriggerObject")->setFlagShadow(false);
	DEBUG_MESSAGE(LEVEL_LOW, "TriggerObject Loaded")
	

	mpModelInfo->bindMeshToActorType(Actor::ActorType_Tank,		Tank::TankType_CloseCombat,		"CloseCombatTank",	"../../ModelFiles/TankAir/NewTankAirGeometry.btb", "../../ModelFiles/TankAir/NewTankAirMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Tank,		Tank::TankType_LongRange,		"LongRangeTank",	"../../ModelFiles/TankGround/NewTankGroundGeometry.btb", "../../ModelFiles/TankGround/NewTankGroundMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Driver,	0,								"Bugman",	"../../ModelFiles/Bugman/BugmanGeometry.btb", "../../ModelFiles/Bugman/BugmanMaterial.btb", "../../ModelFiles/Bugman/BugmanWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Jumper,		"Spider",	"../../ModelFiles/Spider/SpiderGeometry.btb", "../../ModelFiles/Spider/SpiderMaterial.btb", "../../ModelFiles/Spider/SpiderWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_BlackWidow,	"Spider",	"../../ModelFiles/Spider/SpiderGeometry.btb", "../../ModelFiles/Spider/SpiderMaterial.btb", "../../ModelFiles/Spider/SpiderWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Tarantula,		"Spider",	"../../ModelFiles/Spider/SpiderGeometry.btb", "../../ModelFiles/Spider/SpiderMaterial.btb", "../../ModelFiles/Spider/SpiderWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_FireAnt,		"Ant",		"../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_KamikazeAnt,	"Ant",		"../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Ant,			"Ant",		"../../ModelFiles/Ant/NewAntGeometry.btb", "../../ModelFiles/Ant/NewAntMaterial.btb", "../../ModelFiles/Ant/NewAntWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_PlasmaBeetle,	"Beetle",	"../../ModelFiles/Beetle/BeetleGeometry.btb", "../../ModelFiles/Beetle/BeetleMaterial.btb", "../../ModelFiles/Beetle/BeetleWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Enemy,	Enemy::EnemyType_Scarab,		"Beetle",	"../../ModelFiles/Beetle/BeetleGeometry.btb", "../../ModelFiles/Beetle/BeetleMaterial.btb", "../../ModelFiles/Beetle/BeetleWeights.btb");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Acid,	"Bullet",	"../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Beam,	"Bullet",	"../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Bullet,	"Bullet",	"../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Fire,	"Bullet",	"../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Flame,	"Bullet",	"../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Plasma,	"Bullet",	"../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Munition,	Munitions::MunitionType_Pulse,	"Bullet",	"../../ModelFiles/TankShell/NewTankShellGeometry.btb", "../../ModelFiles/TankShell/NewTankShellMaterial.btb", "");
	mpModelInfo->bindMeshToActorType(Actor::ActorType_Pedestrian, 0,							"Bugman",	"../../ModelFiles/Bugman/BugmanGeometry.btb", "../../ModelFiles/Bugman/BugmanMaterial.btb", "../../ModelFiles/Bugman/BugmanWeights.btb");

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

void Application::createObjects()
{
	gfx::DX10Module::getInstance()->createWorldPlane(20, 20, 200.0f, 200.0f);

#ifdef USE_AI
	gfx::WorldPlane* pWorldPlane = gfx::DX10Module::getInstance()->getWorldPlane();
	utilities::Vec3Float planeSizeWorld(pWorldPlane->getTileX() * pWorldPlane->getTileSizeX(), 0.0f, pWorldPlane->getTileZ() * pWorldPlane->getTileSizeZ());
	mpAiFactory->setMapSize(planeSizeWorld.convertToMapCoordinates());
#endif

	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateTank(mpGameOptions->mTankType, utilities::Position(50.0f, 3.0f, 0.0f + (40.0f * mpPlayerInfo->getNetworkId())),utilities::Direction(1.0f,0.0f,0.0f), Actor::PlayerMovingInfo())));
}

void Application::setUpGame(float deltaTime)
{

	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_InputChangeGameState(logic::InputStates::GameState_Game_Inside_Tank)));

	
#ifndef USE_NETWORK
	mpPlayerInfo->setNetworkId(0);	
#endif

	mpDxModule->lock(0);

	mpActorHandler->setDXModuleInstance(mpDxModule);

#ifdef USE_SOUND
	mpActorHandler->setSoundSystemInstance(mpSoundSystem);
#endif

	mpCamera = mpDxModule->getCamera();
	mpCamera->setTarget(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	mpCamera->setPosition(D3DXVECTOR3(-100.0f, 50.0f, -100.0f));
	mpCamera->setOffset(-100.0f, 500.0f, -100.0f);
	mpCamera->setRotateSpeed(0.01f);


	mpModelInfo = ModelInfo::getInstance();
	loadMeshes();

	createObjects();

	mpDxModule->unlock(0);

#ifdef USE_AI
	if (mpGameOptions->mDisableAI == false)
	{
		mpAiFactory->resume();
	}
#endif



#ifndef USE_NETWORK
	mGameState = GameState_Game;
#else
	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_System_FinishedLoading()));
	mGameState = GameState_Waiting;
#endif

#ifdef USE_GUI
	mpStartMenuHandler->setFlagRender(false);
#endif

#ifdef USE_EDITOR
	mpEditor->deactivate();
#endif // USE_EDITOR

	finalGameInitialization();
}

void Application::runGame()
{
	float dt;

	while(!mpDxModule->getQuit())
	{
		dt = mpGlobalTimer->tick();
		switch(mGameState)
		{
		case GameState_MainMenu:
			mainMenuLoop(dt);
			break;
		case GameState_Lobby:
			lobbyLoop(dt);
			break;

		case GameState_Starting:
			setUpGame(dt);
			break;

		case GameState_Waiting:
			waitingLoop(dt);
			break;

		case GameState_Game:
			gameLoop(dt);
			break;

		default:
			//invalid gamestate
			assert(0);
			break;
		}
	}
}

void Application::waitingLoop(float deltaTime)
{
	mpEventManager->tick(static_cast<unsigned long>(logic::EventManager::KINFINITE));
	if (mpGameOptions->mIsServer)
	{
		if (mNrOfFinishedLoading > mpGameOptions->mNumberOfPlayers)
		{
			safeTriggerEvent(EvtData_System_StartGame());
			mpDxModule->lock(0);
			editor::EditorHandler::getInstance()->loadMapNonThreadSafe(mpGameOptions->mMap);
			mpDxModule->unlock(0);
		}
	}
}

void Application::gameLoop(float deltaTime)
{
	mpQuadTree->updateQuadTree();

	mpEventManager->tick(static_cast<unsigned long>(IEventManager::KINFINITE));

#ifdef USE_EDITOR

	if (mpGameOptions->mEditorMode || mpGameOptions->mEnableEditor)
	{
		if(mpEditor->isActive())
		{
			mpActorHandler->updateAll(deltaTime);
			mpEditor->update(deltaTime);
		}
		else
		{
			mpActorHandler->update(deltaTime);
		}
	} 
	else
	{
		mpActorHandler->update(deltaTime);
	}
#else
	mpActorHandler->update(deltaTime);
#endif
#ifdef USE_SOUND
	if( mTrack != 1 )
	{
		mpSoundSystem->playMusic(sound::Music_Gameplay);
		mTrack = 1;
	}
#endif

	mpHUDHandler->update(deltaTime);

	mpDxModule->lock(0);

	mpDxModule->handleEvent(mpGFXListener->getEventList());

#ifdef USE_EDITOR
	if (mpGameOptions->mEditorMode || mpGameOptions->mEnableEditor)
	{
		if(mpEditor->isActive())
		{
			mpActorHandler->updateAllGFX();
		}
		else
		{
			mpActorHandler->updateGFX();
		}
	}
	else
	{
		mpActorHandler->updateGFX();
	}
	mpEditor->updateGFX(deltaTime);

#else
	mpActorHandler->updateGFX();
#endif

#ifdef USE_AI
	mpAiFactory->update(deltaTime);
#endif

#ifdef USE_GUI
	mpStartMenuHandler->updateFrames(deltaTime);
#endif

	mpTriggerHandler->updateGFX();

	mpHUDHandler->updateGFX(deltaTime);

	mpDxModule->unlock(0);	

#ifdef USE_SOUND
	mpSoundSystem->update();
#endif // USE_SOUND

	mpInputStates->update();

	//Check for map completion.
	if(mpActorHandler->getFlagMapCompleted())
	{
		resetGame();
	}
	if(mpActorHandler->getFlagMapDefeated())
	{
		resetGame();
	}

	Sleep(10);
}

void Application::mainMenuLoop(float deltaTime)
{
	mpEventManager->tick(static_cast<unsigned long>(logic::EventManager::KINFINITE));
#ifdef USE_GUI
	mpDxModule->lock(0);
	mpActorHandler->updateGFX();
	mpTriggerHandler->updateGFX();
	mpHUDHandler->updateGFX(deltaTime);
	mpStartMenuHandler->updateFrames(deltaTime);
	mpDxModule->unlock(0);
#endif
#ifdef USE_SOUND
	if( mTrack != 0 )
	{
		mpSoundSystem->playMusic(sound::Music_Menu);
		mTrack = 0;
	}
#endif

	mpInputStates->update();
}

void Application::lobbyLoop(float deltaTime)
{
	mpEventManager->tick(static_cast<unsigned long>(logic::EventManager::KINFINITE));
#ifdef USE_GUI
	mpDxModule->lock(0);
	mpActorHandler->updateGFX();
	mpTriggerHandler->updateGFX();
	mpHUDHandler->updateGFX(deltaTime);
	mpStartMenuHandler->updateFrames(deltaTime);
	mpDxModule->unlock(0);	
#endif
	mpInputStates->update();
}

LPCSTR Application::getGameTitle()
{
	return "BugTanks";
}

void Application::setGameState(GameStates gameState)
{
	mGameState = gameState;
}

void Application::increaseFinishedLoading()
{
	mNrOfFinishedLoading++;
}

void Application::resetGame()
{
	//Close down the network.
	mpNetwork->resetNetwork();
	removeNetworkEvents();

	//Handle the last network events.
	mpEventManager->tick(static_cast<unsigned long>(logic::EventManager::KINFINITE));

	//Remove logic objects.
	mpDxModule->lock(0);

	mpActorHandler->updateGFX();
	mpTriggerHandler->updateGFX();

	mpDxModule->unlock(0);

	//Reset game logic.
	mpActorHandler->resetActorHandler();
	mpTriggerHandler->removeAllTriggers();

	//Handle game logic events.
	mpEventManager->tick(static_cast<unsigned long>(logic::EventManager::KINFINITE));

#ifdef USE_EDITOR
	//Reset editor.
	mpEditor->deactivate();
#endif
	//Remove logic objects.
	mpDxModule->lock(0);

	mpActorHandler->removeActorsGlobal();
#ifdef USE_EDITOR
	mpEditor->updateGFX(0.0f);
#endif
	mpTriggerHandler->updateGFX();

	mpDxModule->unlock(0);

	KeyHandler::getInstance()->resetKeyHandler();

	//Reset statistics.
	Statistics::getInstance()->resetStatistics();

	//Change game state.
	mpInputStates->setState(InputStates::GameState_Menu);
	setGameState(GameState_MainMenu);
	mpStartMenuHandler->resetFields();
	mpStartMenuHandler->goToMenu("SignIn");
}

GUIHandlerStartMenu* Application::getStartMenu()const
{
	return mpStartMenuHandler;
}

void Application::setCommandWindow()
{
	HWND hWnd = GetConsoleWindow();

#ifdef _DEBUG
	MoveWindow(hWnd, 2000, 100, 1200, 900, TRUE);
#else
	ShowWindow(hWnd, SW_HIDE);
#endif // _DEBUG

}