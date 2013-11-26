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

#include <boost/shared_ptr.hpp>

#include "../Logic/Events.h"
#include "../Logic/GlobalTimer.h"
#include "../Logic/snooper.h"
#include "../Logic/LogicListener.h"
#include "../Logic/KeyHandler.h"
#include "../Logic/PlayerInfo.h"
#include "../Logic/Tank.h"
#include "../Logic/BlackWidow.h"


#include "../Sound/SoundListener.h"



Application::Application()
{

	actorHandler	= NULL;
	mpInputStates	= NULL;
	mSoundSystem	= NULL;
	mpEventManager  = NULL;

	mbWindowedMode	= false;
	mbIsRunning		= false;

	mdxModule		= NULL;

}

bool Application::InitInstance()
{

#ifndef _DEBUG

		if(!isOnlyInstance(getGameTitle()))
			return false;
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

	mglobalTimer = logic::GlobalTimer::getInstance();

	mpEventManager = myNew logic::EventManager("Bug Tanks Event Mgr", true);
	if (!mpEventManager)
	{
		return false;
	}

	snooper = boost::shared_ptr<logic::EventSnooper>(myNew logic::EventSnooper("snooper.log"));
	safeAddListener(snooper, logic::EventType(logic::gWildcardEventType));
	logic::safeTriggerEvent(logic::EvtData_System_EventSystemStarted());
	
	logic::Actor::init();
	actorHandler = logic::ActorHandler::getInstance();

	mPlayerInfo = logic::PlayerInfo::getInstance();
	
	mdxModule = gfx::DX10Module::getInstance();

	mdxModule->create();

	//Wait for dxModule to initialize.
	while(!mdxModule->getRunFlag())
	{
		Sleep(10);
	}
	
	logic::safeTriggerEvent(logic::EvtData_System_GFXSystemStarted());

	createLogic();

	mbIsRunning = true;
	return true;
}

void Application::createLogic()
{
	actorHandler->setDXModule();

	//If InputStates is created after SoundSystem, the memory check won't work.
	mpInputStates = myNew logic::InputStates(mdxModule->getHInstancePointer(), mdxModule->getHWNDPointer());
	
	logicListener = boost::shared_ptr<logic::LogicListener>(myNew logic::LogicListener());
	epActorHandler = boost::shared_ptr<logic::ActorHandler>(actorHandler);
	soundListener = boost::shared_ptr<sound::SoundListener>(myNew sound::SoundListener());
	registerEvents();
	
	mSoundSystem = sound::SoundSystem::getInstance();
	pQuadTree = logic::LogicQuadTree::getInstance();
	
	mpGameOptions = myNew GameOptions("../config.ini");

	mSoundSystem->setMusicVolume(mpGameOptions->mMusicVolume);
	mSoundSystem->setSoundMasterVolume(mpGameOptions->mSoundEffectsVolume);
}

void Application::registerEvents()
{
	//Add events to the LogicListener
	logic::safeAddListener(logicListener, logic::EvtData_System_EventSystemStarted::mEventType);
	logic::safeAddListener(logicListener, logic::EvtData_System_StartGame::mEventType);
	logic::safeAddListener(logicListener, logic::EvtData_From_ActorCreated::mEventType);
	logic::safeAddListener(logicListener, logic::EvtData_From_ActorDestroyed::mEventType);
	logic::safeAddListener(logicListener, logic::EvtData_To_ActorStartMove::mEventType);
	logic::safeAddListener(logicListener, logic::EvtData_From_ActorCollision::mEventType);
	logic::safeAddListener(logicListener, logic::EvtData_System_EndGame::mEventType);
	logic::safeAddListener(logicListener, logic::EvtData_To_InputChangeGameState::mEventType);
	logic::safeAddListener(logicListener, logic::EvtData_System_GameOver::mEventType);
	//Add events to the ActorHandler
	logic::safeAddListener(epActorHandler, logic::EvtData_To_EnemyUpdateHp::mEventType);
	logic::safeAddListener(epActorHandler, logic::EvtData_From_ActorCollision::mEventType);
	logic::safeAddListener(epActorHandler, logic::EvtData_To_TankStartFire::mEventType);
	logic::safeAddListener(epActorHandler, logic::EvtData_To_TankStopFire::mEventType);
	logic::safeAddListener(epActorHandler, logic::EvtData_From_ActorDestroyed::mEventType);
	logic::safeAddListener(epActorHandler, logic::EvtData_To_ActorStartMove::mEventType);
	logic::safeAddListener(epActorHandler, logic::EvtData_To_ActorStopMove::mEventType);
	logic::safeAddListener(epActorHandler, logic::EvtData_To_ActorStartRotate::mEventType);
	logic::safeAddListener(epActorHandler, logic::EvtData_To_ActorStopRotate::mEventType);
	//Add events to the SoundListener
	logic::safeAddListener(soundListener, logic::EvtData_To_ActorStartMove::mEventType);
	logic::safeAddListener(soundListener, logic::EvtData_To_ActorStopMove::mEventType);
	logic::safeAddListener(soundListener, logic::EvtData_From_ActorCreated::mEventType);
	logic::safeAddListener(soundListener, logic::EvtData_From_ActorDestroyed::mEventType);

	//Add events to the <inputName>
}

void Application::onClose()
{
	logic::KeyHandler *keyHandler = logic::KeyHandler::getInstance();

	SAFE_DELETE(keyHandler);
	SAFE_DELETE(mPlayerInfo);
	SAFE_DELETE(pQuadTree);
	SAFE_DELETE(mSoundSystem);
	SAFE_DELETE(mpInputStates);
	SAFE_DELETE(mglobalTimer);
	SAFE_DELETE(mdxModule);
	SAFE_DELETE(mpEventManager);
}

void Application::setUpGame()
{
	mpInputStates->setState(logic::InputStates::GameState_Game_Inside_Tank);

	//Create actors
	logic::Tank *pt = myNew logic::Tank(100, 100, logic::Tank::TankType_CloseCombat);
	
	mPlayerInfo->setTankActorId(pt->getKey());
	actorHandler->insertActor(pt);	
	pt->setPosition(-30.0f, 0.0f, 0.0f);
	pt->setDirection(1.0f, 0.0f, 0.0f);
	pt->setSubsetDirection(1.0f, 0.0f, 0.0f);
	
	actorHandler->setDXModule();

	mdxModule->lock(0);

	mCamera = mdxModule->getCamera();
	mCamera->setTarget(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	mCamera->setPosition(D3DXVECTOR3(-100.0f, 50.0f, -100.0f));

	srand(time(0));
	int colorCounter;
	gfx::TransformObject *tempObj;
	for(int i = 0; i < 50; i++)
	{

		colorCounter = i;
		for(int j = 0; j < 50; j++)
		{
			int rnd = rand()%10;
			tempObj = mdxModule->createObjectShape(gfx::ShapeTypes::CUBE);
			
			if(rnd == 0)
			{
				tempObj->setPosition(D3DXVECTOR3(-30.0f * j,15.0f,-30.0f * i));
				tempObj->setScale(D3DXVECTOR3(15.0f, 15.0f, 15.0f));
			}
			else
			{
				tempObj->setPosition(D3DXVECTOR3(-30.0f * j,-10.0f,-30.0f * i));
				tempObj->setScale(D3DXVECTOR3(15.0f, 0.1f, 15.0f));
			}
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

	mdxModule->loadMeshInstance("../../ModelFiles/TankGround/TankGeometry.txt", 1, "Tank", 5000);
	mMesh = mdxModule->createMeshInstanced("Tank");
	mMesh->setTextureID(1);
	mMesh->setFlagUpdateAbsolute(false);
	mMesh->setID(pt->getKey());
	mMesh->setName("player");
	mMesh->setPosition(D3DXVECTOR3(pt->getPosition().x, pt->getPosition().y, pt->getPosition().z));
	mMesh->setScale(D3DXVECTOR3(5.0f, 3.0f, 5.0f));
	mMesh->setColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));

	mdxModule->unlock(0);
}

void Application::runGame()
{
	float dt;
	while(!mdxModule->getQuit())
	{
		mCamera->setTarget(mMesh->getPositionX(), mMesh->getPositionY(), mMesh->getPositionZ());
		mCamera->setPosition(D3DXVECTOR3(mMesh->getPositionX()-100.0f, 50.0f, mMesh->getPositionZ()-100.0f));
		dt = mglobalTimer->tick();
		actorHandler->update(dt);
		mpInputStates->update();
		mpEventManager->tick(logic::EventManager::KINFINITE);
	}
}

LPCSTR Application::getGameTitle()
{
	return "BugTanks";
}