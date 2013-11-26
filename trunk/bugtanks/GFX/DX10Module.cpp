/**
 * @file
 * @author Martin Jönsson <snowman.mrgames@gmail.com>, Victor Lundberg <lundberg.victor@gmail.com>
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

#include "DX10Module.h"
#include "Settings.h"
#include "../Logic/ActorHandler.h"
#include "Particle.h"
#include "../Logic/Events.h"

using namespace gfx;

DX10Module* DX10Module::mpsThreadInstance = NULL;

DX10Module::DX10Module() : Thread(), mWindowHandler(), mRenderHandler(), mShadowRenderHandler(), mShaderHandler(),
						   mObjectHandler(), mParticleHandler()
{
	mButtonPressTimer = 0.0f;
	mpMeshLoader = NULL;
	mpAnimationHandler = NULL;
	mpWorldPlane = NULL;
	mCamera = NULL;
	mLight	= myNew Light(D3DXVECTOR3(50.0f, 100.0f, 50.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f), D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f), D3DXCOLOR(0.3f, 0.3f, 0.3f, 7.0f), 1.0f);
	mpTextureHandler = NULL;
	mShadowsEnabled = true;
	mUpdateAnimations = false;
	mTimer = NULL;
	InitVertexStructures();
}

DX10Module::DX10Module(const DX10Module &module) : mWindowHandler(), mRenderHandler(), mShadowRenderHandler(),
					   mShaderHandler(), mObjectHandler(), mParticleHandler()
{
	module;
}

DX10Module::~DX10Module()
{
	if(mpMeshLoader)
	{
		delete mpMeshLoader;
		mpMeshLoader = NULL;
	}
	if(mpAnimationHandler)
	{
		delete mpAnimationHandler;
		mpAnimationHandler = NULL;
	}
	if(mpWorldPlane)
	{
		delete mpWorldPlane;
		mpWorldPlane = NULL;
	}
	if(mCamera)
	{
		delete mCamera;
		mCamera = NULL;
	}
	if(mLight)
	{
		delete mLight;
		mLight = NULL;
	}
	if(mTimer)
	{
		delete mTimer;
		mTimer = NULL;
	}
	if(mpTextureHandler)
	{
		delete mpTextureHandler;
		mpTextureHandler = NULL;
	}

	DestroyVertexStructures();
}

const DX10Module &DX10Module::operator=(const DX10Module &module)
{
	if(this != &module)
	{

	}
	return *this;
}

HRESULT DX10Module::init(long width, long height)
{
	if(FAILED(mWindowHandler.initWindow(TITLE, WIDTH, HEIGHT)))
	{
		ERROR_MESSAGE("Could not initialize window.")
		return E_FAIL;
	}
	if(FAILED(mWindowHandler.initDevice()))
	{
		ERROR_MESSAGE("Could not initialize ID3D10Device.")
		return E_FAIL;
	}
	if(FAILED(mRenderHandler.initRenderer(mWindowHandler.getHWND(), mWindowHandler.getDevice())))
	{
		ERROR_MESSAGE("Could not initialize renderer.")
		return E_FAIL;
	}
	if(FAILED(mShadowRenderHandler.initRenderer(mWindowHandler.getHWND(), mWindowHandler.getDevice(), false)))
	{
		ERROR_MESSAGE("Could not initialize shadow renderer.")
		return E_FAIL;
	}

	mShaderHandler.init(mWindowHandler.getDevice());
	if(FAILED(loadShaders()))
	{
		ERROR_MESSAGE("Failed to load shaders.")
		return E_FAIL;
	}

	mpMeshLoader = myNew MeshLoader(mWindowHandler.getDevice(), mShaderHandler.getEffect("MainShader"));

	mpAnimationHandler = myNew AnimationHandler(mWindowHandler.getDevice());
	
	// Init timer
	mTimer = myNew utilities::Timer();

	// Init camera
	mCamera = myNew Camera(width, height, D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	
	// Init particle system
	mParticleHandler.init(mWindowHandler.getDevice(), mShaderHandler.getEffect("MainShader"));

	// Init light view and projection
	mLight->MatrixLookAtLH();
	mLight->MatrixOrthoLH(1000.0f, 1000.0f, 1.0f, 2000.0f);

	// Init texture handler
	mpTextureHandler = myNew TextureHandler(mWindowHandler.getDevice());
	loadFont();
	mpTextureHandler->addTexture("../../Textures/Ground/tile01.png",						"WorldPlane01");
	mpTextureHandler->addTexture("../../Textures/Ground/tile02.png",						"WorldPlane02");
	mpTextureHandler->addTexture("../../Textures/Ground/tile03.png",						"WorldPlane03");
	mpTextureHandler->addTexture("../../Textures/Ground/tile04.png",						"WorldPlane04");

	// Regular buttons
	mpTextureHandler->addTexture("../../Textures/Button_default.png",						"Button_Default");
	mpTextureHandler->addTexture("../../Textures/Button_mouse_over.png",					"Button_MouseOver");
	mpTextureHandler->addTexture("../../Textures/Button_mouse_over_and_pressed.png",		"Button_MouseOverPressed");
	mpTextureHandler->addTexture("../../Textures/Button_pressed.png",						"Button_Pressed");

	mpTextureHandler->addTexture("../../Textures/CreateGame_default.png",					"CreateGame_Default");
	mpTextureHandler->addTexture("../../Textures/CreateGame_mouse_over.png",				"CreateGame_MouseOver");
	mpTextureHandler->addTexture("../../Textures/CreateGame_mouse_over_and_pressed.png",	"CreateGame_MouseOverPressed");
	mpTextureHandler->addTexture("../../Textures/CreateGame_pressed.png",					"CreateGame_Pressed");

	mpTextureHandler->addTexture("../../Textures/JoinGame_default.png",						"JoinGame_Default");
	mpTextureHandler->addTexture("../../Textures/JoinGame_mouse_over.png",					"JoinGame_MouseOver");
	mpTextureHandler->addTexture("../../Textures/JoinGame_mouse_over_and_pressed.png",		"JoinGame_MouseOverPressed");
	mpTextureHandler->addTexture("../../Textures/JoinGame_pressed.png",						"JoinGame_Pressed");

	mpTextureHandler->addTexture("../../Textures/MapEdit_default.png",						"MapEdit_Default");
	mpTextureHandler->addTexture("../../Textures/MapEdit_mouse_over.png",					"MapEdit_MouseOver");
	mpTextureHandler->addTexture("../../Textures/MapEdit_mouse_over_and_pressed.png",		"MapEdit_MouseOverPressed");
	mpTextureHandler->addTexture("../../Textures/MapEdit_pressed.png",						"MapEdit_Pressed");

	mpTextureHandler->addTexture("../../Textures/Quit_default.png",							"Quit_Default");
	mpTextureHandler->addTexture("../../Textures/Quit_mouse_over.png",						"Quit_MouseOver");
	mpTextureHandler->addTexture("../../Textures/Quit_mouse_over_and_pressed.png",			"Quit_MouseOverPressed");
	mpTextureHandler->addTexture("../../Textures/Quit_pressed.png",							"Quit_Pressed");

	mpTextureHandler->addTexture("../../Textures/SignIn_default.png",						"SignIn_Default");
	mpTextureHandler->addTexture("../../Textures/SignIn_mouse_over.png",					"SignIn_MouseOver");
	mpTextureHandler->addTexture("../../Textures/SignIn_mouse_over_and_pressed.png",		"SignIn_MouseOverPressed");
	mpTextureHandler->addTexture("../../Textures/SignIn_pressed.png",						"SignIn_Pressed");

	mpTextureHandler->addTexture("../../Textures/Send_default.png",							"Send_Default");
	mpTextureHandler->addTexture("../../Textures/Send_mouse_over.png",						"Send_MouseOver");
	mpTextureHandler->addTexture("../../Textures/Send_mouse_over_and_pressed.png",			"Send_MouseOverPressed");
	mpTextureHandler->addTexture("../../Textures/Send_pressed.png",							"Send_Pressed");

	mpTextureHandler->addTexture("../../Textures/StartGame_default.png",						"StartGame_Default");
	mpTextureHandler->addTexture("../../Textures/StartGame_mouse_over.png",						"StartGame_MouseOver");
	mpTextureHandler->addTexture("../../Textures/StartGame_mouse_over_and_pressed.png",			"StartGame_MouseOverPressed");
	mpTextureHandler->addTexture("../../Textures/StartGame_pressed.png",						"StartGame_Pressed");

	// Radiobutton.
	mpTextureHandler->addTexture("../../Textures/Radiobutton_default.png",					"RBDefault");
	mpTextureHandler->addTexture("../../Textures/Radiobutton_mouse_over.png",				"RBOver");
	mpTextureHandler->addTexture("../../Textures/Radiobutton_mouse_over_and_select.png",	"RBOverSelect");
	mpTextureHandler->addTexture("../../Textures/Radiobutton_select.png",					"RBSelect");

	// Scrollbar
	mpTextureHandler->addTexture("../../Textures/Scroll_up_default.png",					"ScrollUp_Default");
	mpTextureHandler->addTexture("../../Textures/Scroll_up_mouse_over.png",					"ScrollUp_MouseOver");
	mpTextureHandler->addTexture("../../Textures/Scroll_up_mouse_over_and_pressed.png",		"ScrollUp_MouseOverPressed");
	mpTextureHandler->addTexture("../../Textures/Scroll_up_default.png",					"ScrollUp_Pressed");
	mpTextureHandler->addTexture("../../Textures/Scroll_down_default.png",					"ScrollDown_Default");
	mpTextureHandler->addTexture("../../Textures/Scroll_down_mouse_over.png",				"ScrollDown_MouseOver");
	mpTextureHandler->addTexture("../../Textures/Scroll_down_mouse_over_and_pressed.png",	"ScrollDown_MouseOverPressed");
	mpTextureHandler->addTexture("../../Textures/Scroll_down_default.png",					"ScrollDown_Pressed");
	mpTextureHandler->addTexture("../../Textures/Scrollbar_default.png",					"ScrollBar_Default");
	mpTextureHandler->addTexture("../../Textures/Scrollbar_mouse_over.png",					"ScrollBar_MouseOver");
	mpTextureHandler->addTexture("../../Textures/Scrollbar_mouse_over.png",					"ScrollBar_MouseOverPressed");
	mpTextureHandler->addTexture("../../Textures/Scrollbar_mouse_over.png",					"ScrollBar_Pressed");

	// List
	mpTextureHandler->addTexture("../../Textures/sand01.png",								"ListBG");
	mpTextureHandler->addTexture("../../Textures/ListElement_default.png",					"ListElement_Default");
	mpTextureHandler->addTexture("../../Textures/ListElement_mouse_over.png",				"ListElement_MouseOver");
	mpTextureHandler->addTexture("../../Textures/ListElement_mouse_over_and_pressed.png",	"ListElement_MouseOverPressed");
	mpTextureHandler->addTexture("../../Textures/ListElement_pressed.png",					"ListElement_Pressed");

	//HUD.
	mpTextureHandler->addTexture("../../Textures/HUD/HUDMainFrame.png",						"HUD_Main");
	mpTextureHandler->addTexture("../../Textures/HUD/HUDScoreFrame.png",					"HUD_Score");
	mpTextureHandler->addTexture("../../Textures/HUD/HUDWeaponBeam.png",					"HUD_WeaponBeam");
	mpTextureHandler->addTexture("../../Textures/HUD/HUDWeaponCannon.png",					"HUD_WeaponCannon");
	mpTextureHandler->addTexture("../../Textures/HUD/HUDWeaponFlame.png",					"HUD_WeaponFlame");
	mpTextureHandler->addTexture("../../Textures/HUD/HUDWeaponGauss.png",					"HUD_WeaponGauss");
	mpTextureHandler->addTexture("../../Textures/HUD/HUDWeaponMinigun.png",					"HUD_WeaponMinigun");
	mpTextureHandler->addTexture("../../Textures/HUD/HUDWeaponPulse.png",					"HUD_WeaponPulse");

	//Particles
	mpTextureHandler->addTexture("../../Textures/Particles/Flame.bmp",						"Flames");
	mpTextureHandler->addTexture("../../Textures/Particles/pulsee.png",						"Pulse"); // Position in array0: 85
	mpTextureHandler->addTexture("../../Textures/Particles/BigExplosion.png",				"BigExplosion");
	mpTextureHandler->addTexture("../../Textures/Particles/TankExplosion.png",				"TankExplosion");
	mpTextureHandler->addTexture("../../Textures/Particles/GunSparks.png",					"GunSparks");

	mShaderHandler.getEffect("MainShader")->updateTextureArray128(mpTextureHandler->getTextureArraySize(TextureHandler::SizeType_128));
	mShaderHandler.getEffect("MainShader")->updateTextureArray256(mpTextureHandler->getTextureArraySize(TextureHandler::SizeType_256));
	mShaderHandler.getEffect("MainShader")->updateTextureArray512(mpTextureHandler->getTextureArraySize(TextureHandler::SizeType_512));
	mShaderHandler.getEffect("MainShader")->updateTextureArray1024(mpTextureHandler->getTextureArraySize(TextureHandler::SizeType_1024));

	mShaderHandler.getEffect("MainShader")->updateParticleTextureArray(mpTextureHandler->getTextureArraySize("Flames,Pulse,BigExplosion,TankExplosion,GunSparks"));

	mObjectHandler.setCamera(mCamera);

	return S_OK;
}

DX10Module* DX10Module::getInstance()
{
	if(mpsThreadInstance == NULL)
	{
		mpsThreadInstance = myNew DX10Module();
	}

	return mpsThreadInstance;
}

void DX10Module::mainLoop()
{
	if(FAILED(init(1240, 800)))
	{
		ERROR_MESSAGE("Could not initialize DX10Module.")
	}

	setRunFlag(true);
	updateShaderOnceBuffers();

	float dt = 0.0f;
	mTimer->start();
	while(getRunFlag())
	{
		MSG msg = {0};
		while(WM_QUIT != msg.message)
		{
			if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else
			{
				dt = mTimer->tick();
				lock(0);
				update(dt);
				render();
				unlock(0);

				Sleep(10);
			}
		}
		exit();
		break;
	}
}

void DX10Module::handleEvent(utilities::VectorList<logic::EventDataPtr> &events)
{
	while(!events.empty())
	{
		logic::EventDataPtr evtData = events.getFirst();
		if(logic::EvtData_To_CameraStartRotate::msEventType == evtData->getEventType())
		{
			const logic::EvtData_To_CameraStartRotate& eventData = static_cast<const logic::EvtData_To_CameraStartRotate&>(*evtData);
			
			if(eventData.mRotateLeft == true)
			{
				mCamera->rotationDirection(-1);
			}
			else
			{
				mCamera->rotationDirection(1);
			}
		}
		if(logic::EvtData_To_CameraStopRotate::msEventType == evtData->getEventType())
		{
			const logic::EvtData_To_CameraStopRotate& eventData = static_cast<const logic::EvtData_To_CameraStopRotate&>(*evtData);
			
			if(eventData.mRotateLeft == true)
			{
				mCamera->rotationDirection(1);
			}
			else
			{
				mCamera->rotationDirection(-1);
			}
		}
		events.removeFirst();
	}
}

void DX10Module::update(float deltaTime)
{
	mButtonPressTimer += deltaTime;

	if(mpWorldPlane)
	{
		mpWorldPlane->update();
	}
	mCamera->update();
	updateShaderPerFrameBuffers(deltaTime);
	mObjectHandler.updateObjects(deltaTime);
	mObjectHandler.updateCulling();
	mParticleHandler.updateParticleSystems();
}

void DX10Module::render()
{
	mShaderHandler.getEffect("MainShader")->bindShadowMap(NULL);
	mShaderHandler.getEffect("MainShader")->getEffect()->GetTechniqueByIndex(0)->GetPassByIndex(RenderObject::ShaderPasses_Cel)->Apply(0);
	mShaderHandler.getEffect("MainShader")->getEffect()->GetTechniqueByIndex(0)->GetPassByIndex(RenderObject::ShaderPasses_MultiTex)->Apply(0);

	if(mShadowsEnabled)
	{
		mShadowRenderHandler.start();
		mShadowRenderHandler.renderShadowVector(mObjectHandler.getCulledObjects());
		mShadowRenderHandler.end();
	}

	mShaderHandler.getEffect("MainShader")->bindShadowMap(mShadowRenderHandler.getDepthMapSRV());
	mRenderHandler.start();
	mRenderHandler.renderVector(mObjectHandler.getCulledObjects());
	mParticleHandler.renderParticleSystems();
	mRenderHandler.end();
}

bool DX10Module::getTextureIndex(const char *textureName, int &arrayIndex, int &textureID)
{
	return mpTextureHandler->getTextureIndex(textureName, arrayIndex, textureID);
}

bool DX10Module::getTextureIndex(const std::string &textureName, int &arrayIndex, int &textureID)
{
	return mpTextureHandler->getTextureIndex(textureName, arrayIndex, textureID);
}

void DX10Module::loadMeshInstance(const char *filePath, const char *mtrlFilePath, int instanceID, const char *instanceName,
	unsigned int maxInstances, bool updateFromBase)
{
	mpMeshLoader->openGeometryFile(filePath);
	mpMeshLoader->openMaterialFile(mtrlFilePath);
	InstanceMesh *meshInstance = mpMeshLoader->importInstanceMesh();
	meshInstance->setID(instanceID);
	meshInstance->setName(instanceName);
	meshInstance->setInstanceCountMax(maxInstances);
	meshInstance->setFlagUpdateFromBase(updateFromBase);
	mObjectHandler.addMeshInstanceBase(meshInstance);
}

void DX10Module::loadAnimationMeshInstance(const char *filePath, const char *mtrlFilePath, const char *weightFilePath,
	int instanceID, const char *instanceName, unsigned int maxInstances, bool updateFromBase)
{
	mpMeshLoader->openGeometryFile(filePath);
	mpMeshLoader->openMaterialFile(mtrlFilePath);
	mpMeshLoader->openWeightFile(weightFilePath);
	InstanceAnimationMesh *meshInstance = mpMeshLoader->importInstanceAnimationMesh();
	meshInstance->setID(instanceID);
	meshInstance->setName(instanceName);
	meshInstance->setInstanceCountMax(maxInstances);
	meshInstance->setFlagUpdateFromBase(updateFromBase);
	mObjectHandler.addMeshInstanceBase(meshInstance);
}

void DX10Module::loadObjectShapeInstance(RenderObject::ShapeTypes type, int instanceID, const char *instanceName, unsigned int maxInstances)
{
	InstanceObject *objectInstanceBase = NULL;

	switch(type)
	{
		case RenderObject::ShapeTypes_Triangle:
			objectInstanceBase = myNew InstanceShapeTriangle(mWindowHandler.getDevice(), mShaderHandler.getEffect("MainShader"));
			objectInstanceBase->setFlagInstanceBase(true);
			objectInstanceBase->setID(instanceID);
			objectInstanceBase->setName(instanceName);
			objectInstanceBase->setInstanceCountMax(maxInstances);
			objectInstanceBase->setFlagUpdateFromBase(true);
			((InstanceShapeCube *)objectInstanceBase)->loadBuffers();
			mObjectHandler.addObjectInstanceBase(objectInstanceBase);
			break;

		case RenderObject::ShapeTypes_Cube:
			objectInstanceBase = myNew InstanceShapeCube(mWindowHandler.getDevice(), mShaderHandler.getEffect("MainShader"));
			objectInstanceBase->setFlagInstanceBase(true);
			objectInstanceBase->setID(instanceID);
			objectInstanceBase->setName(instanceName);
			objectInstanceBase->setInstanceCountMax(maxInstances);
			objectInstanceBase->setFlagUpdateFromBase(true);
			((InstanceShapeCube *)objectInstanceBase)->loadBuffers();
			mObjectHandler.addObjectInstanceBase(objectInstanceBase);
			break;

		case RenderObject::ShapeTypes_Plane:
			objectInstanceBase = myNew InstanceShapePlane(mWindowHandler.getDevice(), mShaderHandler.getEffect("MainShader"));
			objectInstanceBase->setFlagInstanceBase(true);
			objectInstanceBase->setID(instanceID);
			objectInstanceBase->setName(instanceName);
			objectInstanceBase->setInstanceCountMax(maxInstances);
			objectInstanceBase->setFlagUpdateFromBase(true);
			((InstanceShapePlane *)objectInstanceBase)->loadBuffers();
			mObjectHandler.addObjectInstanceBase(objectInstanceBase);
			break;

		case RenderObject::ShapeTypes_Sprite:
			objectInstanceBase = myNew InstanceSprite(mWindowHandler.getDevice(), mShaderHandler.getEffect("MainShader"));
			objectInstanceBase->setFlagInstanceBase(true);
			objectInstanceBase->setID(instanceID);
			objectInstanceBase->setName(instanceName);
			objectInstanceBase->setInstanceCountMax(maxInstances);
			objectInstanceBase->setFlagUpdateFromBase(true);
			((InstanceSprite *)objectInstanceBase)->loadBuffers();
			mObjectHandler.addObjectInstanceBase(objectInstanceBase);
			break;

		default:
			break;
	}
}

void DX10Module::loadGUISpriteInstance(int instanceID, const char *instanceName, unsigned int maxInstances)
{
	InstanceSprite *GUIInstanceBase = myNew InstanceSprite(mWindowHandler.getDevice(), mShaderHandler.getEffect("MainShader"));
	GUIInstanceBase->setFlagInstanceBase(true);
	GUIInstanceBase->setID(instanceID);
	GUIInstanceBase->setName(instanceName);
	GUIInstanceBase->setInstanceCountMax(maxInstances);
	GUIInstanceBase->setFlagUpdateFromBase(true);
	((InstanceSprite *)GUIInstanceBase)->loadBuffers();
	mObjectHandler.addGUIInstanceBase(GUIInstanceBase);
}

void DX10Module::loadAnimation(const char *animationFilePath, const char *animationName)
{
	mpMeshLoader->openAnimationFile(animationFilePath);
	Animation *pAnimation = mpMeshLoader->importAnimation();
	if(pAnimation)
	{
		pAnimation->setName(animationName);
		mpAnimationHandler->addAnimation(pAnimation);
		mUpdateAnimations = true;
	}
}

int DX10Module::getAnimationIndex(const char *animationName)
{
	return mpAnimationHandler->getAnimationIndex(animationName);
}

int DX10Module::getAnimationIndex(const std::string &animationName)
{
	return mpAnimationHandler->getAnimationIndex(animationName);
}

InstanceMesh *DX10Module::getMeshInstanceBase(int instanceID)
{
	return mObjectHandler.getMeshInstanceBase(instanceID);
}

InstanceMesh *DX10Module::getMeshInstanceBase(const char *instanceName)
{
	return mObjectHandler.getMeshInstanceBase(instanceName);
}

InstanceObject *DX10Module::getObjectInstanceBase(int instanceID)
{
	return mObjectHandler.getObjectInstanceBase(instanceID);
}

InstanceObject *DX10Module::getObjectInstanceBase(const char *instanceName)
{
	return mObjectHandler.getObjectInstanceBase(instanceName);
}

InstanceSprite *DX10Module::getGUISpriteInstanceBase(int instanceID)
{
	return mObjectHandler.getGUIInstanceBase(instanceID);
}

InstanceSprite *DX10Module::getGUISpriteInstanceBase(const char *instanceName)
{
	return mObjectHandler.getGUIInstanceBase(instanceName);
}

TransformObject *DX10Module::createObjectInstanced(int instanceID)
{
	return mObjectHandler.addObjectFromInstance(instanceID);
}

TransformObject *DX10Module::createObjectInstanced(const char *instanceName)
{
	return mObjectHandler.addObjectFromInstance(instanceName);
}

InstanceMesh *DX10Module::createMeshInstanced(int instanceID)
{
	return mObjectHandler.addMeshFromInstance(instanceID);
}

InstanceMesh *DX10Module::createMeshInstanced(const char *instanceName)
{
	return mObjectHandler.addMeshFromInstance(instanceName);
}

RenderObject *DX10Module::getRenderObject(int objectID)
{
	return mObjectHandler.getObject(objectID);
}

RenderObject *DX10Module::getRenderObject(const char *objectName)
{
	return mObjectHandler.getObject(objectName);
}

void DX10Module::removeRenderObject(RenderObject *object)
{
	mObjectHandler.removeObject(object);
}

void DX10Module::removeRenderObject(int objectID)
{
	mObjectHandler.removeObject(objectID);
}

void DX10Module::removeRenderObject(const char *objectName)
{
	mObjectHandler.removeObject(objectName);
}

void DX10Module::removeAllRenderObjects()
{
	mObjectHandler.removeAllObjects();
}

void DX10Module::removeAllRenderInstances()
{
	mObjectHandler.removeAllObjectInstances();
	mObjectHandler.removeAllMeshInstances();
}

void DX10Module::createWorldPlane(int tileX, int tileZ, float tileSizeX, float tileSizeZ)
{
	if(mpWorldPlane)
	{
		delete mpWorldPlane;
		mpWorldPlane = NULL;
	}
	mpWorldPlane = myNew WorldPlane(tileX, tileZ, tileSizeX, tileSizeZ);
}

void DX10Module::copyBoundingVolumes(std::vector<CollisionObject *> *pBVs)
{
	mObjectHandler.copyBoundingVolumes(pBVs);
}

HRESULT DX10Module::loadShaders()
{
	if(FAILED(mShaderHandler.addEffect("../../Shaders/MainShader.fx", "MainShader")))
	{
		return E_FAIL;
	}
	mShaderHandler.getEffect("MainShader")->createInputLayout(0, RenderObject::ShaderPasses_GUI, VertexTypes::VERTEX_GUI);
	mShaderHandler.getEffect("MainShader")->createInputLayout(0, RenderObject::ShaderPasses_Cel, VertexTypes::VERTEX_TRANSFORM);
	mShaderHandler.getEffect("MainShader")->createInputLayout(0, RenderObject::ShaderPasses_Animation, VertexTypes::VERTEX_ANIMATION);
	mShaderHandler.getEffect("MainShader")->createInputLayout(0, RenderObject::ShaderPasses_MultiTex, VertexTypes::VERTEX_MULTI_TEXTURE);
	mShaderHandler.getEffect("MainShader")->createInputLayout(0, RenderObject::ShaderPasses_Particle, VertexTypes::VERTEX_PARTICLES);

	return S_OK;
}

HRESULT DX10Module::loadFont()
{
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_A.png", "FontA");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_B.png", "FontB");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_C.png", "FontC");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_D.png", "FontD");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_E.png", "FontE");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_F.png", "FontF");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_G.png", "FontG");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_H.png", "FontH");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_I.png", "FontI");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_J.png", "FontJ");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_K.png", "FontK");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_L.png", "FontL");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_M.png", "FontM");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_N.png", "FontN");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_O.png", "FontO");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_P.png", "FontP");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_Q.png", "FontQ");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_R.png", "FontR");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_S.png", "FontS");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_T.png", "FontT");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_U.png", "FontU");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_V.png", "FontV");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_W.png", "FontW");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_X.png", "FontX");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_Y.png", "FontY");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_Z.png", "FontZ");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_0.png", "Font0");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_1.png", "Font1");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_2.png", "Font2");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_3.png", "Font3");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_4.png", "Font4");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_5.png", "Font5");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_6.png", "Font6");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_7.png", "Font7");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_8.png", "Font8");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_9.png", "Font9");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_Dot.png", "FontDot");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_Comma.png", "FontComma");
	mpTextureHandler->addTexture("../../Textures/BTFont/BTF_Space.png", "FontSpace");
	ID3D10ShaderResourceView *pFontArray = mpTextureHandler->getTextureArray(
		std::string("FontA,FontB,FontC,FontD,FontE,FontF,FontG,FontH,FontI,FontJ,FontK,")
		+ std::string("FontL,FontM,FontN,FontO,FontP,FontQ,FontR,FontS,FontT,FontU,FontV,FontW,FontX,FontY,FontZ,")
		+ std::string("Font0,Font1,Font2,Font3,Font4,Font5,Font6,Font7,Font8,Font9,")
		+ std::string("FontDot,FontComma,FontSpace"));
	mShaderHandler.getEffect("MainShader")->updateFontArray(pFontArray);
	return S_OK;
}

void DX10Module::updateShaderOnceBuffers()
{

}

void DX10Module::updateShaderPerFrameBuffers(float deltaTime)
{
	// Update lights view matrix
	mLight->MatrixLookAtLH();

	//Update animation texture.
	if(mUpdateAnimations)
	{
		mShaderHandler.getEffect("MainShader")->updateAnimations(mpAnimationHandler->getTextureArrayVector());
		mUpdateAnimations = false;
	}

	//Update world plane weight texture.
	if(mpWorldPlane)
	{
		if(mpWorldPlane->getFlagSRVUpdateRequired())
		{
			mShaderHandler.getEffect("MainShader")->updateWorldPlaneArray(mpWorldPlane->getWeightMapSRV());
			mpWorldPlane->SRVUpdated();
		}
	}

	// Camera related
	D3DXMATRIX MV;
	D3DXMATRIX MP;
	D3DXMATRIX MOrtP;
	mCamera->GetVPMatrices(MV, MP, MOrtP);
	mShaderHandler.getEffect("MainShader")->updateCameraMatrices(MV, MP, MOrtP);
	mShaderHandler.getEffect("MainShader")->updateVCameraPosition(mCamera->getPosition());
	mShaderHandler.getEffect("MainShader")->updateVCameraDirection(mCamera->getTarget() - mCamera->getPosition());

	// Timer
	mShaderHandler.getEffect("MainShader")->updateGameTime(mTimer->getTime(utilities::Timer::ReturnType_MilliSeconds), deltaTime);
	
	// Light
	mShaderHandler.getEffect("MainShader")->updateLightMatrices(mLight->getViewMatrix(), mLight->getProjMatrix());
	mShaderHandler.getEffect("MainShader")->updateVLight(mLight->getLightStruct());	
	
	D3DXVECTOR2 clientArea;
	clientArea.x = (float)mWindowHandler.getClientWidth();
	clientArea.y = (float)mWindowHandler.getClientHeight();
	mShaderHandler.getEffect("MainShader")->updateViewportSize(clientArea);
}