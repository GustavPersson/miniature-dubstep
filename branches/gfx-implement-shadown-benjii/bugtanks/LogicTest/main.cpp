#include "../Utilities/Macros.h"
#include "../GFX/DX10Module.h"
#include "../Logic/InputStates.h"
#include <windows.h>
#include "../Logic/FireAnt.h"
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
#include "../Network/Network.h"
#include "../Network/EventForwarder.h"
#include "../Network/EventMsgHandler.h"
#include <boost/shared_ptr.hpp>
#include <cstdlib>
#include <ctime>
#include "../Sound/SoundListener.h"
#include "../Sound/SoundSystem.h"

using namespace logic;

int main()
{

	network::InitializeNetwork();

	initMemoryCheck();
	//Init Timer
	GlobalTimer *gt = GlobalTimer::getInstance();
	//Init EventManager
	EventManager *test;
	test = myNew EventManager("test", true);
	//Init ActorHandler
	Actor::init();
	ActorHandler *actorHandler = ActorHandler::getInstance();

	network::Peer mPeer("hej2", 5002);

	network::MessageHandler *msgServer, *msgClient;
	
	
	msgClient = myNew network::MsgH_GameClient(&mPeer);
	mPeer.getServerManager()->addMessageHandler(msgClient);


	msgServer = myNew network::MsgH_GameServer("tja", mPeer.getPeerManager());
	mPeer.getPeerManager()->addMessageHandler(msgServer);
	
	mPeer.joinNetwork("localhost", 5001);


	network::EventMsgHandler netEventHandler(mPeer.getIdManager());
	mPeer.getPeerManager()->addMessageHandler(&netEventHandler);
	
	//Init Listeners
	EventListenerPtr snoop;
	snoop = boost::shared_ptr<EventSnooper>(myNew EventSnooper());
	safeAddListener(snoop, EventType(gWildcardEventType));

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

	EventListenerPtr logiclol;
	logiclol = boost::shared_ptr<LogicListener>(myNew LogicListener());
	safeAddListener(logiclol,EvtData_From_EnemyHit::mEventType);
	safeAddListener(logiclol, EvtData_System_EndGame::mEventType);

	EventListenerPtr networkForwarder;
	networkForwarder = boost::shared_ptr<network::EventForwarder>(myNew network::EventForwarder(&mPeer));
	//safeAddListener(networkForwarder, EvtData_To_ActorStartMove::mEventType);
	safeAddListener(networkForwarder, EvtData_To_ActorStopMove::mEventType);

	EventListenerPtr sound;
	sound = boost::shared_ptr<sound::SoundListener>(myNew sound::SoundListener());
	logic::safeAddListener(sound, logic::EvtData_To_ActorStartMove::mEventType);
	logic::safeAddListener(sound, logic::EvtData_To_ActorStopMove::mEventType);
	logic::safeAddListener(sound, logic::EvtData_From_ActorCreated::mEventType);
	logic::safeAddListener(sound, logic::EvtData_From_ActorDestroyed::mEventType);
	logic::safeAddListener(sound, logic::EvtData_From_WeaponFired::mEventType);
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

	
	//testing dxmodule
	module->lock(0);

	gfx::Camera *camera = module->getCamera();
	camera->setTarget(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	camera->setPosition(D3DXVECTOR3(-100.0f, 50.0f, -100.0f));

	module->loadObjectShapeInstance(gfx::ShapeTypes::CUBE, 1, "Cube", 5000);

	srand(time(0));
	int colorCounter;
	gfx::TransformObject *tempObj;
	for(int i = 0; i < 10; i++)
	{

		colorCounter = i;
		for(int j = 0; j < 10; j++)
		{
			int rnd = rand()%10;
			tempObj = module->createObjectInstanced(1);

			
			if(rnd == 0)
			{
				tempObj->setPosition(D3DXVECTOR3(-30.0f * j,0.0f,-30.0f * i));
				tempObj->setScale(D3DXVECTOR3(15.0f, 15.0f, 15.0f));
				tempObj->setID(-1);
			}
			else
			{
				tempObj->setPosition(D3DXVECTOR3(-30.0f * j, -10.0f,-30.0f * i));
				tempObj->setScale(D3DXVECTOR3(15.0f, 0.1f, 15.0f));
				tempObj->setID(-2);
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

	//module->loadObjectShapeInstance(gfx::ShapeTypes::CUBE, 1, "Cube", 5000);
	
	module->loadMeshInstance("../../ModelFiles/TankGround/TankGeometry.txt", 2, "LongRangeTank", 5000);
	//module->loadMeshInstance("../../ModelFiles/TankGround/TankGeometry.txt", 1, "Tank3", 5000);
	module->loadMeshInstance("../../ModelFiles/TankAir/TankAirGeometry.txt", 3, "CloseRangeTank", 5000);
	/*gfx::InstanceMesh *mesh1 = module->createMeshInstanced("Tank");
	mesh1->setTextureID(0);
	mesh1->setFlagUpdateAbsolute(false);
	mesh1->setID(lolAnt->getKey());
	mesh1->setName("enemy");
	mesh1->setPosition(D3DXVECTOR3(lolAnt->getPosition().x, lolAnt->getPosition().y, lolAnt->getPosition().z));
	mesh1->setScale(D3DXVECTOR3(2.5f, 1.5f, 2.5f));
	mesh1->setColor(D3DXCOLOR(0.0f, 0.25f, 1.0f, 1.0f));
	
	mesh1 = module->createMeshInstanced("Tank3");	
	mesh1->setTextureID(0);
	mesh1->setFlagUpdateAbsolute(false);
	mesh1->setID(spindelFan->getKey());
	mesh1->setName("enemy2");
	mesh1->setPosition(D3DXVECTOR3(spindelFan->getPosition().x, spindelFan->getPosition().y, spindelFan->getPosition().z));
	mesh1->setScale(D3DXVECTOR3(2.5f, 1.5f, 2.5f));
	mesh1->setColor(D3DXCOLOR(0.0f, 0.25f, 1.0f, 1.0f));

	mesh1 = module->createMeshInstanced("Tank2");
	mesh1->setTextureID(1);
	mesh1->setFlagUpdateAbsolute(false);
	mesh1->setID(pt->getKey());
	mesh1->setName("player");
	mesh1->setPosition(D3DXVECTOR3(pt->getPosition().x, pt->getPosition().y, pt->getPosition().z));
	mesh1->setScale(D3DXVECTOR3(2.5f, 1.5f, 2.5f));
	mesh1->setColor(D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f));*/

	//Create actors
	Tank *pt = myNew Tank(200, 15, Tank::TankType_CloseCombat);
	pi->setTankActorId(pt->getKey());
	//safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorCreated(pt->getKey())));

	FireAnt *lolAnt = myNew FireAnt(100,20,Enemy::Size_Small,1,150.0f);
	//safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorCreated(lolAnt->getKey())));
	actorHandler->insertActor(lolAnt);
	lolAnt->setPosition(-120.0f,0.0f,0.0f);
	lolAnt->setDirection(1.0f,0.0f,0.0f);
	
	BlackWidow *spindelFan = myNew BlackWidow(100,20,Enemy::Size_Medium,1,150.0f);
	actorHandler->insertActor(spindelFan);
	spindelFan->setPosition(-50.0f,0.0f,0.0f);
	spindelFan->setDirection(1.0f,0.0f,0.0f);

	actorHandler->insertActor(pt);	
	pt->setPosition(-10.0f, 3.0f, 0.0f);
	pt->setDirection(1.0f, 0.0f, 0.0f);
	pt->setSubsetDirection(1.0f,0.0f,0.0f);
	
	
	actorHandler->setDXModule();

	module->unlock(0);

	

	sound::SoundSystem *ss = sound::SoundSystem::getInstance();
	ss->setActor();
	LogicQuadTree *pQuadTree = LogicQuadTree::getInstance();
	float dt;  
	while(!module->getQuit())
	{
		test->tick(EventManager::KINFINITE);
		pQuadTree->updateQuadTree();
		//camera->setTarget(mesh1->getPositionX(), mesh1->getPositionY(), mesh1->getPositionZ());
		//camera->setPosition(D3DXVECTOR3(mesh1->getPositionX()-100.0f, 50.0f, mesh1->getPositionZ()-100.0f));
		dt = gt->tick();
		actorHandler->update(dt);
		ss->Update();
		input->update();
		
		Sleep(10);
	}
	logic::safeTriggerEvent(logic::EvtData_System_EndGame());
	KeyHandler *tmp = KeyHandler::getInstance();
	
	
	network::ShutdownNetwork();
	SAFE_DELETE(ss);
	SAFE_DELETE(pQuadTree);
	SAFE_DELETE(tmp);
	SAFE_DELETE(input);
	SAFE_DELETE(gt);
	SAFE_DELETE(test);
	SAFE_DELETE(pi);
	SAFE_DELETE(module);
	return 0;
}