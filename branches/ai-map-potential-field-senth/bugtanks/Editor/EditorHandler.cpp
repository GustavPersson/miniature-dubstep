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

#include "EditorHandler.h"
#include "../Logic/InputStates.h"
#include "../Logic/Actor.h"
#include "../Logic/Ant.h"
#include "../Logic/BlackWidow.h"
#include "../Logic/FireAnt.h"
#include "../Logic/Jumper.h"
#include "../Logic/KamikazeAnt.h"
#include "../Logic/PlasmaBeetle.h"
#include "../Logic/Scarab.h"
#include "../Logic/Tarantula.h"
#include "../Logic/ModelInfo.h"
#include "../Logic/Application.h"


using namespace editor;
using namespace gfx;
using namespace logic;
using std::string;
using std::vector;


EditorHandler *EditorHandler::mpEditorHandlerInstance = NULL;

EditorHandler::EditorHandler() : EventListener(), mMessages(), mMouseMarker()
{
	//Initialize editor GUI handler.
	DX10Module *module = DX10Module::getInstance();
	module->loadGUISpriteInstance(1, "EditorGUI", 50000);
	long cW = module->getClientWidth();
	long cH = module->getClientHeight();
	mpEditorGUI = myNew EditorGUI(module->getGUISpriteInstanceBase("EditorGUI"), cW, cH);

	mpEditorFileHandler = myNew EditorFileHandler();

	mActive = false;
	mFlagMouseLeftDown = false;
	mFlagMouseMiddleDown = false;
	mFlagMouseRightDown = false;
	mFlagMouseClip = false;
	mMouseClipX = 0;
	mMouseClipY = 0;
	mpCreateObjectMessage = NULL;

	mMouseMarker.setSpeedObjectScale(2.0f);
	mMouseMarker.setSpeedObjectRotate((float)(2.0 * D3DX_PI));

	//Register events.
	logic::EventListenerPtr handlerPtr = boost::shared_ptr<EditorHandler>(this);
	logic::safeAddListener(handlerPtr, logic::EvtData_System_MouseMove::msEventType);
	logic::safeAddListener(handlerPtr, logic::EvtData_System_MouseKeyDown::msEventType);
	logic::safeAddListener(handlerPtr, logic::EvtData_System_MouseKeyUp::msEventType);
	logic::safeAddListener(handlerPtr, logic::EvtData_System_KeysPressed::msEventType);
}

EditorHandler::EditorHandler(const EditorHandler &handler) : EventListener(), mMessages(), mMouseMarker()
{
	//Initialize editor GUI handler.
	DX10Module *module = DX10Module::getInstance();
	module->loadGUISpriteInstance(1, "EditorGUI", 50000);
	long cW = module->getClientWidth();
	long cH = module->getClientHeight();
	mpEditorGUI = myNew EditorGUI(module->getGUISpriteInstanceBase("EditorGUI"), cW, cH);

	mpEditorFileHandler = myNew EditorFileHandler();

	mActive = false;
	mFlagMouseLeftDown = false;
	mFlagMouseMiddleDown = false;
	mFlagMouseRightDown = false;
	mFlagMouseClip = false;
	mMouseClipX = 0;
	mMouseClipY = 0;
	mpCreateObjectMessage = NULL;

	//Register events.
	logic::EventListenerPtr handlerPtr = boost::shared_ptr<EditorHandler>(this);
	logic::safeAddListener(handlerPtr, logic::EvtData_System_MouseMove::msEventType);
	logic::safeAddListener(handlerPtr, logic::EvtData_System_MouseKeyDown::msEventType);
	logic::safeAddListener(handlerPtr, logic::EvtData_System_MouseKeyUp::msEventType);
	logic::safeAddListener(handlerPtr, logic::EvtData_System_KeysPressed::msEventType);

	MapDataActor d;
	EditorMapHandler::packActor(NULL, d);
}

EditorHandler::~EditorHandler()
{
	if(mpEditorFileHandler)
	{
		delete mpEditorFileHandler;
		mpEditorFileHandler = NULL;
	}
	if(mpCreateObjectMessage)
	{
		delete mpCreateObjectMessage;
		mpCreateObjectMessage = NULL;
	}
	for(vector<EM *>::iterator messageIter = mMessages.begin();
		messageIter != mMessages.end(); ++messageIter)
	{
		delete (*messageIter);
		(*messageIter) = NULL;
	}
	mMessages.clear();
}

const EditorHandler &EditorHandler::operator=(const EditorHandler &handler)
{
	if(this != &handler)
	{

	}
	return *this;
}

EditorHandler *EditorHandler::getInstance()
{
	if(!mpEditorHandlerInstance)
	{
		mpEditorHandlerInstance = myNew EditorHandler();
	}
	return mpEditorHandlerInstance;
}

char const *EditorHandler::getName(void)
{
	return "EditorHandler";
}

bool EditorHandler::handleEvent(EventData const &eventData)
{
	//Keyboard keys.
	if(mActive)
	{
		if(eventData.getEventType() == EvtData_System_KeysPressed::msEventType)
		{
			const EvtData_System_KeysPressed &data = (const EvtData_System_KeysPressed &)eventData;
			if(data.mKeysPressed & logic::InputStates::Key_Return)
			{

			}
			if(data.mKeysPressed & logic::InputStates::Key_0)
			{
				if(!mpEditorGUI->getFlagMouseInFrame())
				{
					string mapName = "";
					mpEditorGUI->getUtilityBar()->getMapName(mapName);
					addMessage(myNew EMSaveMap(mapName.c_str(), false));
				}
			}
			if(data.mKeysPressed & logic::InputStates::Key_9)
			{
				if(!mpEditorGUI->getFlagMouseInFrame())
				{
					string mapName = "";
					mpEditorGUI->getUtilityBar()->getMapName(mapName);
					addMessage(myNew EMLoadMap(mapName.c_str()));
				}
			}
			if(data.mKeysPressed & logic::InputStates::Key_Backspace)
			{
				if(!mpEditorGUI->getFlagMouseInFrame())
				{
					//Remove the current marker object during next message handling.
					addMessage(myNew EMRemoveMarker());

					//Try to remove the selected actor.
					if(mMouseMarker.getMarkerType() == mMouseMarker.MarkerType_Actor)
					{
						// HACK - Commenting this due to compile errors - pActor not declared  // Senth
						//if (pActor->getActorType() == Actor::ActorType_StaticEnvironment)
						//{
						//	safeQueueEvent(EventDataPtr(myNew logic::EvtData_From_ActorDestroyed(pActor->getKey())));
						//} 
						//else if (pActor->getActorType() == Actor::ActorType_Enemy)
						//{
						//	if (pActor->getKey().isOwner())
						//	{
						//		safeQueueEvent(EventDataPtr(myNew logic::EvtData_From_ActorDestroyed(pActor->getKey())));
						//	}
						//}
					}
				}
			}

		}
		//Mouse button down.
		else if(eventData.getEventType() == logic::EvtData_System_MouseKeyDown::msEventType)
		{
			const logic::EvtData_System_MouseKeyDown &keyDown = (const logic::EvtData_System_MouseKeyDown &)eventData;
			if(mActive)
			{
				if(keyDown.mKeys & logic::EvtData_System_MouseKeyDown::MouseKeyType_Left)
				{
					mFlagMouseLeftDown = true;

					//The performed action depends on the current MouseMarker state.
					switch(mMouseMarker.getMarkerState())
					{
						case MouseMarker::MarkerState_None:
							if(!mpEditorGUI->getFlagMouseInFrame())
							{
								//Attempt to select an object.
								//Note: only select actors at the moment in order to not move the world plane.
								if(mMouseMarker.selectObject(MouseMarker::MarkerType_Actor | MouseMarker::MarkerType_Trigger, true))
								{
									mMouseMarker.setMarkerState(MouseMarker::MarkerState_Move);
								}
							}
							break;

						case MouseMarker::MarkerState_Select:
							break;

						case MouseMarker::MarkerState_Move:
							if(!mpEditorGUI->getFlagMouseInFrame())
							{
								//Remove the current marker object during next message handling.
								addMessage(myNew EMRemoveMarker());
							}
							break;

						case MouseMarker::MarkerState_Create:
							if(!mpEditorGUI->getFlagMouseInFrame())
							{
								//Create a new object based on the stored CreateObjectMessage.
								if(mMouseMarker.getObject())
								{
									addMessage(mpCreateObjectMessage->clone());
								}
							}
							break;

						default:
							break;
					}
				}
				if(keyDown.mKeys & logic::EvtData_System_MouseKeyDown::MouseKeyType_Middle)
				{
					mFlagMouseMiddleDown = true;
				}
				if(keyDown.mKeys & logic::EvtData_System_MouseKeyDown::MouseKeyType_Right)
				{
					mFlagMouseRightDown = true;

					if(!mpEditorGUI->getFlagMouseInFrame())
					{
						//Remove the current marker object during next message handling.
						addMessage(myNew EMRemoveMarker());
					}
				}
				if(keyDown.mKeys & logic::EvtData_System_MouseKeyDown::MouseKeyType_LeftDouble)
				{

				}
				if(keyDown.mKeys & logic::EvtData_System_MouseKeyDown::MouseKeyType_MiddleDouble)
				{

				}
				if(keyDown.mKeys & logic::EvtData_System_MouseKeyDown::MouseKeyType_RightDouble)
				{

				}
			}
		}
		//Mouse button up.
		else if(eventData.getEventType() == logic::EvtData_System_MouseKeyUp::msEventType)
		{
			const logic::EvtData_System_MouseKeyUp &keyUp = (const logic::EvtData_System_MouseKeyUp &)eventData;
			if(mActive)
			{
				if(keyUp.mKeys & logic::EvtData_System_MouseKeyUp::MouseKeyType_Left)
				{
					mFlagMouseLeftDown = false;
				}
				if(keyUp.mKeys & logic::EvtData_System_MouseKeyUp::MouseKeyType_Middle)
				{
					mFlagMouseMiddleDown = false;
				}
				if(keyUp.mKeys & logic::EvtData_System_MouseKeyUp::MouseKeyType_Right)
				{
					mFlagMouseRightDown = false;
				}
			}
		}
	}
	return false;
}

void EditorHandler::handleMessage(const EM &message)
{
	//Handle messages from editor components.
	//Note: some messages are not threadsafe!

	//The editor should activate.
	if(message.getType() == EM::MessageType_Activate)
	{
		if (Application::getInstance()->mpGameOptions->mEditorMode || Application::getInstance()->mpGameOptions->mEnableEditor)
		{
			activate();
		}
	}
	//The editor should deactivate.
	else if(message.getType() == EM::MessageType_Deactivate)
	{
		deactivate();
	}
	//The user has selected a model from the SelectionList.
	else if(message.getType() == EM::MessageType_ListSelectModel)
	{
		const EMListSelectModel &msg = (const EMListSelectModel &)message;

		//Remove the currently selected object marker.
		handleMessage(EMRemoveMarker());

		//Static actors (models) use the StaticEnvironment enumeration.
		const char *model = ModelInfo::getInstance()->getModelName(Actor::ActorType_StaticEnvironment, msg.getID());
		mMouseMarker.resetTransform();
		mMouseMarker.setObject(DX10Module::getInstance()->createMeshInstanced(model));

		//Create a temporary cube.
		if(!mMouseMarker.getObject())
		{
			mMouseMarker.setObject(DX10Module::getInstance()->createObjectInstanced("Cube"));
		}

		if(mMouseMarker.getObject())
		{
			mMouseMarker.setMarkerState(MouseMarker::MarkerState_Create);
			mMouseMarker.setMarkerType(MouseMarker::MarkerType_Object);
			TransformObject *pObjectMarker = (TransformObject *)mMouseMarker.getObject();
			pObjectMarker->setID(-2);

			mpCreateObjectMessage = myNew EMCreateModel(msg.getID(), msg.getName());
		}
	}
	//The user har selected an enemy from the SelectionList.
	else if(message.getType() == EM::MessageType_ListSelectEnemy)
	{
		const EMListSelectEnemy &msg = (const EMListSelectEnemy &)message;
		
		//Remove the currently selected object marker.
		handleMessage(EMRemoveMarker());

		const char *model = ModelInfo::getInstance()->getModelName(Actor::ActorType_Enemy, msg.getID());
		mMouseMarker.resetTransform();
		mMouseMarker.setObject(DX10Module::getInstance()->createMeshInstanced(model));

		//Create a temporary cube.
		if(!mMouseMarker.getObject())
		{
			mMouseMarker.setObject(DX10Module::getInstance()->createObjectInstanced("Cube"));
		}

		if(mMouseMarker.getObject())
		{
			mMouseMarker.setMarkerState(MouseMarker::MarkerState_Create);
			mMouseMarker.setMarkerType(MouseMarker::MarkerType_Object);
			TransformObject *pObjectMarker = (TransformObject *)mMouseMarker.getObject();
			pObjectMarker->setID(-2);

			mpCreateObjectMessage = myNew EMCreateEnemy(msg.getID(), msg.getName());
		}
	}
	//A new model should be created where the object marker is located.
	else if(message.getType() == EM::MessageType_CreateModel)
	{
		const EMCreateModel &msg = (const EMCreateModel &)message;

		TransformObject *pObjectMarker = (TransformObject *)mMouseMarker.getObject();
		if(pObjectMarker)
		{
			utilities::Position modelPos;
			modelPos.x = pObjectMarker->getPosition().x;
			modelPos.y = pObjectMarker->getPosition().y;
			modelPos.z = pObjectMarker->getPosition().z;

			D3DXVECTOR3 direction;
			mMouseMarker.getDirection(direction, pObjectMarker->getRotationY() + (float)(0.5 * D3DX_PI));
			utilities::Direction modelDir;
			modelDir.x = direction.x;
			modelDir.y = direction.y;
			modelDir.z = direction.z;

			utilities::Vec3Float modelScale;
			modelScale.x = pObjectMarker->getScale().x;
			modelScale.y = pObjectMarker->getScale().y;
			modelScale.z = pObjectMarker->getScale().z;

			safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateStaticEnvironment((StaticEnvironment::StaticEnvironmentTypes)msg.getID(),
				modelPos, modelDir, modelScale)));
		}
	}
	//A new enemy should be created where the object marker is located.
	else if(message.getType() == EM::MessageType_CreateEnemy)
	{
		const EMCreateEnemy &msg = (const EMCreateEnemy &)message;

		TransformObject *pObjectMarker = (TransformObject *)mMouseMarker.getObject();
		if(pObjectMarker)
		{
			utilities::Position enemyPos;
			enemyPos.x = pObjectMarker->getPosition().x;
			enemyPos.y = pObjectMarker->getPosition().y;
			enemyPos.z = pObjectMarker->getPosition().z;

			D3DXVECTOR3 direction;
			mMouseMarker.getDirection(direction);
			utilities::Direction enemyDir;
			enemyDir.x = direction.x;
			enemyDir.y = direction.y;
			enemyDir.z = direction.z;

			safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy((Enemy::EnemyTypes)msg.getID(), enemyPos, enemyDir)));
		}
	}
	//The current temporary marker model should be cleared removed.
	else if(message.getType() == EM::MessageType_RemoveMarker)
	{
		switch(mMouseMarker.getMarkerState())
		{
			case MouseMarker::MarkerState_None:
				break;

			case MouseMarker::MarkerState_Select:
				break;

			case MouseMarker::MarkerState_Move:
				//The object is not temporary; just NULL the marker pointer.
				mMouseMarker.clearObject();
				break;

			case MouseMarker::MarkerState_Create:
				//The object is temporary; remove it from the graphics module.
				mMouseMarker.removeObject();
				if(mpCreateObjectMessage)
				{
					delete mpCreateObjectMessage;
					mpCreateObjectMessage = NULL;
				}
				break;

			default:
				break;
		}
		mMouseMarker.setMarkerState(MouseMarker::MarkerState_None);
	}
	else if(message.getType() == EM::MessageType_SaveMap)
	{
		const EMSaveMap &msg = (const EMSaveMap &)message;
		mpEditorFileHandler->saveMap(msg.getMapName().c_str(), msg.getOverwrite());
	}
	else if(message.getType() == EM::MessageType_LoadMap)
	{
		const EMLoadMap &msg = (const EMLoadMap &)message;
		mpEditorFileHandler->loadMap(msg.getMapName().c_str());
	}
}

void EditorHandler::activate()
{
	mActive = true;

	//Update GUI.
	mpEditorGUI->showAllFrames();
	mpEditorGUI->updateSelectionList();
}

void EditorHandler::deactivate()
{
	mActive = false;

	handleMessage(EMRemoveMarker());

	//Update GUI.
	mpEditorGUI->hideAllFrames();
	resetMouseFlags();
}

void EditorHandler::update(float deltaTime)
{
	if(mActive)
	{
		//Update the MouseMarker.
		//Only move the mouse marker if no mouse buttons are pressed.
		if(!checkMouseFlags())
		{
			mMouseMarker.setMouseCoordinates(mpEditorGUI->getMouseX(), mpEditorGUI->getMouseY());
			mMouseMarker.updateMouseRay();
			mMouseMarker.updateMousePlanePosition();
		}
		//Check buttons that control scale of the mouse marker.
		if(InputStates::getInstance()->isKeyDown(DIK_Z))
		{
			if(InputStates::getInstance()->isKeyDown(DIK_LSHIFT))
			{
				mMouseMarker.setFlagObjectRotate(true);
				mMouseMarker.setSpeedObjectRotate((float)(1.0 * D3DX_PI));
			}
			else
			{
				mMouseMarker.setFlagObjectScale(true);
				mMouseMarker.setSpeedObjectScale(0.3f);
			}
		}
		if(InputStates::getInstance()->isKeyDown(DIK_X))
		{
			if(InputStates::getInstance()->isKeyDown(DIK_LSHIFT))
			{
				mMouseMarker.setFlagObjectRotate(true);
				mMouseMarker.setSpeedObjectRotate(-(float)(1.0 * D3DX_PI));
			}
			else
			{
				mMouseMarker.setFlagObjectScale(true);
				mMouseMarker.setSpeedObjectScale(-0.3f);
			}
		}
		if(InputStates::getInstance()->isKeyDown(DIK_F1))
		{
			DX10Module::getInstance()->getWorldPlane()->increaseWeights(mMouseMarker.getPlanePosition(), 30.0f, D3DXVECTOR4(1.0f * deltaTime, 0.0f, 0.0f, 0.0f));
		}
		if(InputStates::getInstance()->isKeyDown(DIK_F2))
		{
			DX10Module::getInstance()->getWorldPlane()->increaseWeights(mMouseMarker.getPlanePosition(), 30.0f, D3DXVECTOR4(0.0f, 1.0f * deltaTime, 0.0f, 0.0f));
		}
		if(InputStates::getInstance()->isKeyDown(DIK_F3))
		{
			DX10Module::getInstance()->getWorldPlane()->increaseWeights(mMouseMarker.getPlanePosition(), 30.0f, D3DXVECTOR4(0.0f, 0.0f, 1.0f * deltaTime, 0.0f));
		}
		if(InputStates::getInstance()->isKeyDown(DIK_F4))
		{
			DX10Module::getInstance()->getWorldPlane()->increaseWeights(mMouseMarker.getPlanePosition(), 30.0f, D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f * deltaTime));
		}
		mMouseMarker.updateTransform(deltaTime);
	}
}

void EditorHandler::updateGFX(float deltaTime)
{
	if(mActive)
	{
		//Update the render instances of the GUI.
		mpEditorGUI->updateFrames(deltaTime);

		//Update the current object marker.
		mMouseMarker.updateObject();
	}
	//Handle all messages.
	for(vector<EM *>::iterator messageIter = mMessages.begin();
		messageIter != mMessages.end(); ++messageIter)
	{
		handleMessage(*(*messageIter));
		delete (*messageIter);
		(*messageIter) = NULL;
	}
	mMessages.clear();
}

void EditorHandler::addMessage(EM *pMessage)
{
	mMessages.push_back(pMessage);
}

bool EditorHandler::checkMouseFlags()
{
	if(!mFlagMouseLeftDown && !mFlagMouseMiddleDown
	   && !mFlagMouseRightDown)
	{
		return false;
	}
	return true;
}

void EditorHandler::resetMouseFlags()
{
	mFlagMouseLeftDown = false;
	mFlagMouseMiddleDown = false;
	mFlagMouseRightDown = false;
}