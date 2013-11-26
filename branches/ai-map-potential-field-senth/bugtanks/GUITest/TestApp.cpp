#include "TestApp.h"
#include <string>
using namespace std;
using namespace guitest;
using namespace logic;


TestApp::TestApp() : logic::Application()
{
	// Setup GUI components here!
	setVerbosityLevel(LEVEL_HIGHEST, OUTPUT_CONSOLE);
	mMenuHandler = NULL;
}
void TestApp::finalGameInitialization()
{
	mpDxModule->lock(0);


	mpDxModule->loadGUISpriteInstance(6, "Menu", 50000);

	mGUIHandlerStartMenu = myNew GUIHandlerStartMenu(mpDxModule->getGUISpriteInstanceBase("Menu"), mpDxModule->getClientWidth(), mpDxModule->getClientHeight());
	/*
	//------------------------Setup RadioButtons--------------------------------------------------
	this->mRadioButtons = myNew GUIRadioButtons(mpDxModule->getGUISpriteInstanceBase("Menu"), mpDxModule->getClientWidth(), mpDxModule->getClientHeight(), 1);
	float aspectratio = (float)mpDxModule->getClientWidth()/(float)mpDxModule->getClientHeight();


	int arrayIndex = 0;
	int textureID = 0;
	gfx::DX10Module::getInstance()->getTextureIndex("RBDefault", arrayIndex, textureID);

	GUIButton **button;
	button = myNew GUIButton *[4];
	button[0] = myNew GUIButton(this->mRadioButtons);
	button[0]->setRelativeSize(D3DXVECTOR2(0.05f, 0.05f*aspectratio));
	button[0]->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	button[0]->setTextureIndexOffset(textureID);

	button[1] = myNew GUIButton(this->mRadioButtons);
	button[1]->setRelativeSize(D3DXVECTOR2(0.05f, 0.05f*aspectratio));
	button[1]->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	button[1]->setTextureIndexOffset(textureID);

	button[2] = myNew GUIButton(this->mRadioButtons);
	button[2]->setRelativeSize(D3DXVECTOR2(0.05f, 0.05f*aspectratio));
	button[2]->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	button[2]->setTextureIndexOffset(textureID);


	button[0]->setRelativePosition(D3DXVECTOR2(0.4f, 0.1f));
	button[1]->setRelativePosition(D3DXVECTOR2(0.4f, 0.2f));
	button[2]->setRelativePosition(D3DXVECTOR2(0.4f, 0.3f));
	
	this->mRadioButtons->addButton(button[0], "Easy");
	this->mRadioButtons->addButton(button[1], "Medium");
	this->mRadioButtons->addButton(button[2], "Hard");

	this->mRadioButtons->select(button[0]);

	//--------------------------------------------------------------------------------------------



	//----------------------------Setup GUIList---------------------------------------------------

	this->mGUIList = new GUIList(mpDxModule->getGUISpriteInstanceBase("Menu"), mpDxModule->getClientWidth(), mpDxModule->getClientHeight(), 1);
	this->mGUIList->showNumberOfElements(15);
	this->mGUIList->addItem("Item_01");
	this->mGUIList->addItem("Item_02");
	this->mGUIList->addItem("Item_03");
	this->mGUIList->addItem("Item_04");
	this->mGUIList->addItem("Item_05");
	this->mGUIList->addItem("Item_06");
	this->mGUIList->addItem("Item_07");
	this->mGUIList->addItem("Item_08");
	this->mGUIList->addItem("Item_09");
	this->mGUIList->addItem("Item_10");
	this->mGUIList->addItem("Item_11");
	this->mGUIList->addItem("Item_12");
	this->mGUIList->addItem("Item_13");
	this->mGUIList->addItem("Item_14");
	this->mGUIList->addItem("Item_15");
	this->mGUIList->addItem("Item_16");
	this->mGUIList->addItem("Item_17");
	this->mGUIList->addItem("Item_18");
	this->mGUIList->addItem("Item_19");
	this->mGUIList->addItem("Item_20");
	this->mGUIList->addItem("Item_21");
	this->mGUIList->addItem("Item_22");
	this->mGUIList->addItem("Item_23");

	//--------------------------------------------------------------------------------------------

	this->mScrollBar = new GUIScrollBar(mpDxModule->getGUISpriteInstanceBase("Menu"), mpDxModule->getClientWidth(), mpDxModule->getClientHeight(), 1);
	this->mScrollBar->setRelativePosition(D3DXVECTOR2(0.3f,0.3f));
	this->mScrollBar->setRelativeSize(D3DXVECTOR2(0.03f,0.5f));
	this->mScrollBar->setNumberOfIndices(3);

	mButton = myNew GUIButton(mpDxModule->getGUISpriteInstanceBase("Menu"), mpDxModule->getClientWidth(), mpDxModule->getClientHeight(), 0);
	mMenuHandler = myNew GUIMenuHandler(mpDxModule->getGUISpriteInstanceBase("Menu"), mpDxModule->getClientWidth(), mpDxModule->getClientHeight());
	//mMenuHandler->addMenuItem(mButton);
	mMenuHandler->addMenuItem(mScrollBar);
	mMenuHandler->addMenuItem(this->mRadioButtons);
	mButton->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	mButton->setRelativeSize(D3DXVECTOR2(0.5f, 0.5f));

	// Text Button
	button[3] = myNew GUIButton(mpDxModule->getGUISpriteInstanceBase("Menu"), mpDxModule->getClientWidth(), mpDxModule->getClientHeight(), 1);
	button[3]->setRelativeSize(D3DXVECTOR2(0.1f, 0.05f));
	button[3]->setRelativePosition(D3DXVECTOR2(0.5f, 0.5f));
	button[3]->getTextFrame()->setText("Item01");
	button[3]->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.5f, 0.5f));
	button[3]->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.0f, 0.5f));


	// setup TextField.
	mTextField = new GUITextField(mpDxModule->getGUISpriteInstanceBase("Menu"), mpDxModule->getClientWidth(), mpDxModule->getClientHeight(), 1);
	this->mTextField->setRelativePosition(D3DXVECTOR2(0.65f, 0.01f));
	this->mTextField->getTextFrame()->showNumberOfRows(4);
	this->mTextField->getTextFrame()->setText("Itsy bitsy spider crawled up the water spout.\n Down came the rain and washed the spider out.\n Out came the sun and dried up all the rain.\n The itsy bitsy spider crawled up the spout again.\nItsy bitsy spider crawled up the water spout.\n Down came the rain and washed the spider out.\n Out came the sun and dried up all the rain.\n The itsy bitsy spider crawled up the spout again.\nItsy bitsy spider crawled up the water spout.\n Down came the rain and washed the spider out.\n Out came the sun and dried up all the rain.\n The itsy bitsy spider crawled up the spout again. ");
	this->mMenuHandler->addMenuItem(this->mTextField);


	this->mButtonListener = new myButtonListener(this->mGUIList, this->mTextField);

	button[3]->addButtonListener(this->mButtonListener);

	//gfx::DX10Module::getInstance()->getTextureIndex("Button_Default", arrayIndex, textureID);
	//button[3]->setTextureIndexOffset(textureID);
	//button[3]->getRenderInstance()->setTextureArrayIndex(arrayIndex);

	


	this->mMenuHandler->addMenuItem(button[3]);

	this->mMenuHandler->addMenuItem(this->mGUIList);

	mMenuHandler->setFlagRender(true);
	mButton->setFlagRender(true);
	*/

	mpDxModule->unlock(0);
}
void TestApp::loadMeshes()
{
	mpDxModule->loadObjectShapeInstance(gfx::ShapeTypes::CUBE, 2, "TriggerObject", 500);
	mpDxModule->getObjectInstanceBase("TriggerObject")->setFlagShadow(false);

	mpModelInfo->bindMeshToActorType(Actor::ActorType_Tank,	Tank::TankType_CloseCombat,	"CloseCombatTank",	"../../ModelFiles/TankAir/NewTankAirGeometry.btb",	"../../ModelFiles/TankAir/NewTankAirMaterial.btb", "");
}
void TestApp::createObjects()
{
	safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateTank(Tank::TankType_CloseCombat, utilities::Position(50.0f, 3.0f, 0.0f + (40.0f * mpPlayerInfo->getNetworkId())),utilities::Direction(1.0f,0.0f,0.0f), Actor::PlayerMovingInfo())));
}
void TestApp::createInstance()
{
	assert(mpsInstance == NULL);

	mpsInstance = myNew TestApp();
}

void TestApp::gameLoop(float deltaTime)
{
	mpEventManager->tick(static_cast<unsigned long>(logic::EventManager::KINFINITE));
	
	mpQuadTree->updateQuadTree();

#ifdef USE_EDITOR
	mpActorHandler->updateAll(deltaTime);
	mpEditor->update(deltaTime);
#else
	mpActorHandler->update(deltaTime);
#endif

	mpDxModule->lock(0);

#ifdef USE_EDITOR
	mpActorHandler->updateAllGFX();
	mpEditor->updateGFX(deltaTime);
#else
	mpActorHandler->updateGFX();
#endif


#ifdef USE_GUI

	mGUIHandlerStartMenu->updateFrames(deltaTime);
	/*
	mMenuHandler->updateFrames(deltaTime);
	static string option(" ");
	static string listSelection(" ");
	if(option.compare(this->mRadioButtons->getOption())!=0)
	{
		option = this->mRadioButtons->getOption();
		DEBUG_MESSAGE(1, "The current option is " << this->mRadioButtons->getOption());
	}
	if(listSelection.compare(this->mGUIList->getSelected())!=0)
	{
		listSelection = this->mGUIList->getSelected();
		DEBUG_MESSAGE(1, "The current listSelection is " << listSelection);
	}
	*/
#endif
	mpDxModule->unlock(0);	

	mpSoundSystem->update();
	mpInputStates->update();

	Sleep(10);
}
