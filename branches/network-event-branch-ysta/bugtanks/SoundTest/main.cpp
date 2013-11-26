#include "../Sound/SoundSystem.h"
#include "../Sound/Sound.h"
#include "../Sound/SoundListener.h"

#include "../Utilities/Macros.h"
#include "../GFX/DX10Module.h"
#include "../Logic/InputStates.h"
#include <windows.h>
#include "../Logic/Driver.h"
#include "../Logic/Actor.h"
#include "../Logic/ActorHandler.h"
#include "../Logic/Tank.h"
#include "../Logic/Enemy.h"
#include "../Logic/Ant.h"
#include "../Logic/BlackWidow.h"
#include "../Logic/EventManager.h"
#include "../Logic/snooper.h"
#include "../Logic/LogicListener.h"
#include "../Logic/PlayerInfo.h"
#include "../Logic/GlobalTimer.h"
#include "../Logic/Bullet.h"
#include "../Logic/Munitions.h"
#include "../Logic/KeyHandler.h"
#include "../Logic/Statistics.h"
#include <boost/shared_ptr.hpp>
#include <cstdlib>
#include <ctime>

using namespace logic;

int main()
{	
	initMemoryCheck();
	//Init Timer
	GlobalTimer *gt = GlobalTimer::getInstance();
	//Init EventManager
	EventManager *test;
	test = myNew EventManager("test", true);
	//Init ActorHandler
	Actor::init();
	ActorHandler *actorHandler = ActorHandler::getInstance();
	//Init Listeners
	EventListenerPtr snoop;
	snoop = boost::shared_ptr<EventSnooper>(myNew EventSnooper());
	safeAddListener(snoop, EventType(gWildcardEventType));

	//Sound
	sound::SoundSystem *mSoundSystem	= NULL;
	mSoundSystem = sound::SoundSystem::getInstance();
	logic::EventListenerPtr soundListener;
	soundListener = boost::shared_ptr<sound::SoundListener>(myNew sound::SoundListener());

	EventListenerPtr stats;
	stats = boost::shared_ptr<Statistics>(myNew Statistics());
	safeAddListener(stats, EvtData_From_ActorDestroyed::mEventType);
	safeAddListener(stats, EvtData_System_EndGame::mEventType);

	EventListenerPtr epActorHandler;
	epActorHandler = boost::shared_ptr<ActorHandler>(actorHandler);
	safeAddListener(epActorHandler,EvtData_To_EnemyUpdateHp::mEventType);
	safeAddListener(epActorHandler, EvtData_From_ActorCollision::mEventType);
	safeAddListener(epActorHandler, EvtData_To_TankStartFire::mEventType);
	safeAddListener(epActorHandler, EvtData_To_TankStopFire::mEventType);
	safeAddListener(epActorHandler, EvtData_From_ActorDestroyed::mEventType);
	safeAddListener(epActorHandler, EvtData_To_ActorStartMove::mEventType);
	safeAddListener(epActorHandler, EvtData_To_ActorStopMove::mEventType);
	safeAddListener(epActorHandler, EvtData_To_ActorStartRotate::mEventType);
	safeAddListener(epActorHandler, EvtData_To_ActorStopRotate::mEventType);

	logic::safeAddListener(soundListener, logic::EvtData_To_ActorStartMove::mEventType);
	logic::safeAddListener(soundListener, logic::EvtData_To_ActorStopMove::mEventType);
	logic::safeAddListener(soundListener, logic::EvtData_From_ActorCreated::mEventType);
	logic::safeAddListener(soundListener, logic::EvtData_From_ActorDestroyed::mEventType);
	logic::safeAddListener(soundListener, logic::EvtData_From_WeaponFired::mEventType);
	logic::safeAddListener(soundListener, logic::EvtData_To_ActorStartRotate::mEventType);
	logic::safeAddListener(soundListener, logic::EvtData_To_ActorStopRotate::mEventType);

	EventListenerPtr logiclol;
	logiclol = boost::shared_ptr<LogicListener>(myNew LogicListener());
	safeAddListener(logiclol,EvtData_From_EnemyHit::mEventType);
	//Init PLayerInfo
	PlayerInfo *pi = PlayerInfo::getInstance();
	
	//Init Dx
	gfx::DX10Module *module = gfx::DX10Module::getInstance();
	pthread_t th = module->create();
	while(!module->getRunFlag())
	{
		Sleep(10);
	}
	//Init Input
	logic::InputStates *input = myNew logic::InputStates(module->getHInstancePointer(), module->getHWNDPointer());	
	input->setState(logic::InputStates::GameState_Game_Inside_Tank);

	//Create actors
	logic::Tank *pt = myNew Tank(100, 100, logic::Tank::TankType_LongRange);
	pi->setTankActorId(pt->getKey());

	Ant *lolAnt = myNew Ant(100,10,Enemy::Size_Small,1,1);
	actorHandler->insertActor(lolAnt);
	lolAnt->setPosition(-50.0f,0.0f,0.0f);
	lolAnt->setDirection(1.0f,0.0f,0.0f);

	actorHandler->insertActor(pt);	
	pt->setPosition(-30.0f, 0.0f, 0.0f);
	pt->setDirection(1.0f, 0.0f, 0.0f);
	pt->setSubsetDirection(1.0f,0.0f,0.0f);
	
	actorHandler->setDXModule();
	//testing dxmodule
	module->lock(0);

	gfx::Camera *camera = module->getCamera();
	camera->setTarget(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	camera->setPosition(D3DXVECTOR3(-100.0f, 50.0f, -100.0f));

	srand(time(0));
	int colorCounter;
	gfx::TransformObject *tempObj;
	for(int i = 0; i < 50; i++)
	{

		colorCounter = i;
		for(int j = 0; j < 50; j++)
		{
			int rnd = rand()%10;
			tempObj = module->createObjectShape(gfx::ShapeTypes::CUBE);
			
			if(rnd == 0)
			{
				tempObj->setPosition(D3DXVECTOR3(-30.0f * j,15.0f,-30.0f * i));
				tempObj->setScale(D3DXVECTOR3(15.0f, 15.0f, 15.0f));
			}
			else
			{
				tempObj->setPosition(D3DXVECTOR3(-30.0f * j,0.0f,-30.0f * i));
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
	/*gfx2::TransformObject *testObj = module->createObjectShape(gfx2::ShapeTypes::CUBE);
	testObj->setPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	testObj->setScale(D3DXVECTOR3(15.0f, 0.0f, 15.0f));
	testObj->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	testObj->setID(lolBullet->getKey());
	testObj->setName("lolBullet");
	testObj = module->createObjectShape(gfx2::ShapeTypes::CUBE);
	testObj->setPosition(D3DXVECTOR3(30.0f, 0.0f, 30.0f));
	testObj->setScale(D3DXVECTOR3(15.0f, 0.0f, 15.0f));
	testObj->setColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	testObj->setID(lolBullet->getKey());
	testObj->setName("lolBullet");
	testObj = module->createObjectShape(gfx2::ShapeTypes::CUBE);
	testObj->setPosition(D3DXVECTOR3(0.0f, 0.0f, 30.0f));
	testObj->setScale(D3DXVECTOR3(15.0f, 0.0f, 15.0f));
	testObj->setColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	testObj->setID(lolBullet->getKey());
	testObj->setName("lolBullet");
	testObj = module->createObjectShape(gfx2::ShapeTypes::CUBE);
	testObj->setPosition(D3DXVECTOR3(30.0f, 0.0f, 0.0f));
	testObj->setScale(D3DXVECTOR3(15.0f, 0.0f, 15.0f));
	testObj->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	testObj->setID(70);
	testObj->setName("lolBullet");*/

	module->loadMeshInstance("../../ModelFiles/TankGround/TankGeometry.txt", 1, "Tank", 5000);
	gfx::InstanceMesh *mesh1 = module->createMeshInstanced("Tank");
	mesh1->setTextureID(1);
	mesh1->setFlagUpdateAbsolute(false);
	mesh1->setID(lolAnt->getKey());
	mesh1->setName("enemy");
	mesh1->setPosition(D3DXVECTOR3(lolAnt->getPosition().x, lolAnt->getPosition().y, lolAnt->getPosition().z));
	mesh1->setScale(D3DXVECTOR3(5.0f, 3.0f, 5.0f));
	mesh1->setColor(D3DXCOLOR(0.0f, 0.25f, 1.0f, 1.0f));
	
	mesh1 = module->createMeshInstanced("Tank");
	mesh1->setTextureID(1);
	mesh1->setFlagUpdateAbsolute(false);
	mesh1->setID(pt->getKey());
	mesh1->setName("player");
	mesh1->setPosition(D3DXVECTOR3(pt->getPosition().x, pt->getPosition().y, pt->getPosition().z));
	mesh1->setScale(D3DXVECTOR3(5.0f, 3.0f, 5.0f));
	mesh1->setColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));
	
	module->unlock(0);

	mSoundSystem->setActor();

	float dt;  
	while(!module->getQuit())
	{
		camera->setTarget(mesh1->getPositionX(), mesh1->getPositionY(), mesh1->getPositionZ());
		camera->setPosition(D3DXVECTOR3(mesh1->getPositionX()-100.0f, 50.0f, mesh1->getPositionZ()-100.0f));
		dt = gt->tick();
		actorHandler->update(dt);
		mSoundSystem->Update();
		input->update();
		test->tick(EventManager::KINFINITE);
	}
	logic::safeTriggerEvent(logic::EvtData_System_EndGame());
	KeyHandler *tmp = KeyHandler::getInstance();

	SAFE_DELETE(tmp);
	SAFE_DELETE(input);
	SAFE_DELETE(gt);
	SAFE_DELETE(test);
	SAFE_DELETE(pi);
	SAFE_DELETE(module);
	SAFE_DELETE(mSoundSystem);
	return 0;
}