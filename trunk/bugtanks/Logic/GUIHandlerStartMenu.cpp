#include "GUIHandlerStartMenu.h"
#include "../Logic/GUIMainFrame.h"
#include "../Logic/IEventManager.h"
#include "../Logic/Events.h"
#include "../Logic/Application.h"
#include "../Network/Network.h"

using namespace std;
using namespace logic;
using namespace gfx;


void replaceChar(string &str, char c, char replacement)
{
	for(size_t i=0; i<str.size(); i++)
	{
		if(str[i]==c)
			str[i]=replacement;
	}
}

GUIHandlerStartMenu::myButtonListener::myButtonListener(GUIHandlerStartMenu *startMenu)
{
	mStartMenu = startMenu;
}
GUIHandlerStartMenu::myButtonListener::~myButtonListener()
{

}
void GUIHandlerStartMenu::myButtonListener::onMouseLeftUp(logic::GUIButton* button, long mX, long mY)
{
	if(this->mStartMenu->mCurrentMenu == this->mStartMenu->mMenus["SignIn"])
	{
		if(button->getTextFrame()->getTextString().compare("Sign In")==0)
		{
			if(this->mStartMenu->mSignInTextField->getTextFrame()->getTextString().empty()==false)
			{
				// Goto Main Menu
				Application::getInstance()->mpGameOptions->mPlayerName = this->mStartMenu->mSignInTextField->getTextFrame()->getTextString();
				replaceChar(Application::getInstance()->mpGameOptions->mPlayerName, ' ', '_');
				//Application::getInstance()->mpServLookup->startLookup(5546);

				this->mStartMenu->mPlayer1->getTextFrame()->setText(Application::getInstance()->mpGameOptions->mPlayerName);

				this->mStartMenu->goToMenu("Main");
			}
		}
	}
	if(this->mStartMenu->mCurrentMenu == this->mStartMenu->mMenus["Main"])
	{
		if(button->getTextFrame()->getTextString().compare("Create Game")==0)
		{
			//this->mStartMenu->mServerLookup->stopLookup();
			Application::getInstance()->mpGameOptions->mIsServer=true;

			string serverName = Application::getInstance()->mpGameOptions->mPlayerName;

			Application::getInstance()->mpGameOptions->mEditorMode = false;

			// Create network for Server.
			if(Application::getInstance()->mpNetwork)
			{
				Application::getInstance()->removeNetworkEvents();
				SAFE_DELETE(Application::getInstance()->mpNetwork);
				network::ShutdownNetwork();
				this->mStartMenu->mNetwork = NULL;
			}
			network::InitializeNetwork();
			Application::getInstance()->mpNetwork = myNew network::Network(3000);
			this->mStartMenu->mNetwork = Application::getInstance()->mpNetwork;
			this->mStartMenu->mNetwork->setProfile(Application::getInstance()->mpGameOptions->mPlayerName);
			this->mStartMenu->mNetwork->getPeer()->getPeerManager()->addMessageHandler(myNew network::MsgH_TextMessage());
			Application::getInstance()->registerNetworkEvents();

			this->mStartMenu->mNetwork->startServer(serverName, 4);
			this->mStartMenu->mNetwork->join("127.0.0.1", 3000); // Join my own server.


			//Application::getInstance()->mpServLookup->stopLookup();
			Application::getInstance()->setGameState(GameState_Lobby);
			//this->mStartMenu->mServerLookup->registerServer(serverName, 3000);

			this->mStartMenu->goToMenu("Lobby");
		}
		else if(button->getTextFrame()->getTextString().compare("Join Game")==0)
		{
			string gameName = this->mStartMenu->mGameList->getSelected();
			if(gameName.compare("NULL")==0)
				return;

			//this->mStartMenu->mServerLookup->stopLookup();
			this->mStartMenu->mApplication->mpGameOptions->mIsServer=false;

			// Create network for Client.
			if(Application::getInstance()->mpNetwork)
			{
				Application::getInstance()->removeNetworkEvents();
				SAFE_DELETE(Application::getInstance()->mpNetwork);
				network::ShutdownNetwork();
				this->mStartMenu->mNetwork = NULL;
			}
			network::InitializeNetwork();
			Application::getInstance()->mpNetwork =  myNew network::Network(3050);
			this->mStartMenu->mNetwork = Application::getInstance()->mpNetwork;
			this->mStartMenu->mNetwork->setProfile(Application::getInstance()->mpGameOptions->mPlayerName);
			this->mStartMenu->mNetwork->getPeer()->getPeerManager()->addMessageHandler(myNew network::MsgH_TextMessage());
			Application::getInstance()->registerNetworkEvents();
			
			DEBUG_MESSAGE(LEVEL_MEDIUM, this->mStartMenu->mGameServers[gameName].addr << " " << this->mStartMenu->mGameServers[gameName].port);
			this->mStartMenu->mNetwork->join(this->mStartMenu->mGameServers[gameName].addr, this->mStartMenu->mGameServers[gameName].port);
			//this->mStartMenu->mNetwork->join("127.0.0.1", 3000);
			//Application::getInstance()->mpNetwork->
			//this->mStartMenu->mGameServers[gameName].


			//Application::getInstance()->mpServLookup->stopLookup();
			Application::getInstance()->setGameState(GameState_Lobby);


			this->mStartMenu->goToMenu("Lobby");
			//this->mStartMenu->mCurrentMenu->removeSubFrame(this->mStartMenu->mReadyButton);
			this->mStartMenu->mReadyButton->setFlagVisible(false);
			this->mStartMenu->mLevelSelect->setFlagVisible(false);
			this->mStartMenu->mLevelSelectText->setFlagVisible(false);
		}
		else if(button->getTextFrame()->getTextString().compare("MapEdit")==0)
		{
			// This is the same as create game but we also enables the Edit mode.
			
			//this->mStartMenu->mServerLookup->stopLookup();
			Application::getInstance()->mpGameOptions->mIsServer=true;

			string serverName = Application::getInstance()->mpGameOptions->mPlayerName;
			this->mStartMenu->mApplication->mpGameOptions->mEditorMode=true;

			// Create network for Server.
			Application::getInstance()->mpNetwork =  myNew network::Network(3000);
			this->mStartMenu->mNetwork = Application::getInstance()->mpNetwork;
			this->mStartMenu->mNetwork->setProfile(Application::getInstance()->mpGameOptions->mPlayerName);
			this->mStartMenu->mNetwork->getPeer()->getPeerManager()->addMessageHandler(myNew network::MsgH_TextMessage());
			Application::getInstance()->registerNetworkEvents();

			this->mStartMenu->mNetwork->startServer(serverName, 4);
			this->mStartMenu->mNetwork->join("127.0.0.1", 3000); // Join my own server.

			//Application::getInstance()->mpServLookup->stopLookup();
			this->mStartMenu->mServerLookup->registerServer(serverName, 3000);

			this->mStartMenu->goToMenu("Lobby");
			
		}
		else if(button->getTextFrame()->getTextString().compare("Quit")==0)
		{
			// Quit Game
			safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_System_EndGame()));
		}
	}
	if(this->mStartMenu->mCurrentMenu == this->mStartMenu->mMenus["Lobby"])
	{
		if(button->getTextFrame()->getTextString().compare("Send")==0)
		{
			string newText(this->mStartMenu->mChatBox->getTextFrame()->getTextString());
			string textSend(this->mStartMenu->mTextToSend->getTextFrame()->getTextString());
			newText.append("\n" + Application::getInstance()->mpGameOptions->mPlayerName + " says   " + textSend);
			this->mStartMenu->mNetwork->getPeer()->sendToAll(textSend.size(), network::MessageType_TextMessage, textSend.c_str());
			this->mStartMenu->mTextToSend->getTextFrame()->setText("");
			this->mStartMenu->mChatBox->getTextFrame()->setText(newText);
		}
		else if(button->getTextFrame()->getTextString().compare("Start")==0)
		{
			this->mStartMenu->mNetwork->stopServer();
			Application::getInstance()->mpGameOptions->mMap = this->mStartMenu->mLevelSelect->getTextFrame()->getTextString();
			Application::getInstance()->mpGameOptions->mNumberOfPlayers=network::IdManager::getInstance()->size();
			logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_System_StartLoading(Application::getInstance()->mpGameOptions->mEditorMode)));			

			//this->mStartMenu->setFlagVisible(false);
		}
	}
}
void GUIHandlerStartMenu::myButtonListener::onKeyDown(logic::GUIButton* button, int letterKeys, int keys)
{
	if(this->mStartMenu->mCurrentMenu == this->mStartMenu->mMenus["SignIn"])
	{
		if(button->getTextFrame()->getTextString().compare("Sign In")==0)
		{
			if(keys & logic::InputStates::Key_Return)
			{
				if(this->mStartMenu->mSignInTextField->getTextFrame()->getTextString().empty()==false)
				{
					// Goto Main Menu
					Application::getInstance()->mpGameOptions->mPlayerName = this->mStartMenu->mSignInTextField->getTextFrame()->getTextString();
					replaceChar(Application::getInstance()->mpGameOptions->mPlayerName, ' ', '_');
					//Application::getInstance()->mpServLookup->startLookup(5546);

					this->mStartMenu->mPlayer1->getTextFrame()->setText(Application::getInstance()->mpGameOptions->mPlayerName);

					this->mStartMenu->goToMenu("Main");
				}
			}
		}
	}

	if(this->mStartMenu->mCurrentMenu == this->mStartMenu->mMenus["Lobby"])
	{
		if(button->getTextFrame()->getTextString().compare("Send")==0)
		{
			if(keys & logic::InputStates::Key_Return)
			{
				if(this->mStartMenu->mSignInTextField->getTextFrame()->getTextString().empty()==false)
				{
					string newText(this->mStartMenu->mChatBox->getTextFrame()->getTextString());
					string textSend(this->mStartMenu->mTextToSend->getTextFrame()->getTextString());
					newText.append("\n" + Application::getInstance()->mpGameOptions->mPlayerName + " says   " + textSend);
					this->mStartMenu->mNetwork->getPeer()->sendToAll(textSend.size(), network::MessageType_TextMessage, textSend.c_str());
					this->mStartMenu->mTextToSend->getTextFrame()->setText("");
					this->mStartMenu->mChatBox->getTextFrame()->setText(newText);
				}
			}
		}
	}
}

GUIHandlerStartMenu::GUIHandlerStartMenu(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight) 
		: GUIHandler(pBaseInstance, clientWidth, clientHeight)
{
	init();
}
GUIHandlerStartMenu::GUIHandlerStartMenu(const GUIHandlerStartMenu &handler) : GUIHandler(handler)
{
	init();
}

GUIHandlerStartMenu::~GUIHandlerStartMenu(void)
{
	SAFE_DELETE(mButtonListener);
}
const GUIHandlerStartMenu &GUIHandlerStartMenu::operator =(const logic::GUIHandlerStartMenu &handler)
{
	if (this != &handler)
	{
		gfx::GUIHandler::operator=(handler);
	}
	return *this;
}
void GUIHandlerStartMenu::updateFrames(float deltaTime)
{
	GUIHandler::updateFrames(deltaTime);

	static float refreshTime = 2.0f;
	if(this->mCurrentMenu == this->mMenus["Main"])
	{

		refreshTime-=deltaTime;
		//if(refreshTime<=0)
		//{
			this->mServerLookup->refresh(deltaTime);
			vector<network::ServerInfo> serverList = this->mServerLookup->getServerList();
			

			// update game list.
			this->mGameList->clear();
			this->mGameServers.clear();


			//cout << "-------------ServerList----------------------------" << endl;
			for(unsigned int i=0; i<serverList.size(); i++)
			{
				//cout << serverList.at(i).name << " " << serverList.at(i).addr << " " << serverList.at(i).port << endl;
				this->mGameServers[serverList.at(i).name] = serverList.at(i);
				this->mGameList->addItem(serverList.at(i).name);

			}
			refreshTime=2.0f;
		//}
	}

	if(this->mCurrentMenu == this->mMenus["Lobby"])
	{
		// update the players.
		vector<network::IDstructure> vIDs = network::IdManager::getInstance()->getIds();
		if(0 < vIDs.size())
			this->mPlayer2->getTextFrame()->setText(vIDs.at(0).name);
		else
			this->mPlayer2->getTextFrame()->setText("............");

		if(1 < vIDs.size())
			this->mPlayer3->getTextFrame()->setText(vIDs.at(1).name);
		else
			this->mPlayer3->getTextFrame()->setText("............");

		if(2 < vIDs.size())
			this->mPlayer4->getTextFrame()->setText(vIDs.at(2).name);
		else
			this->mPlayer4->getTextFrame()->setText("............");


		// update tank select.
		if(this->mTankSelect->getOption().compare("BattleTank")==0)
		{
			Application::getInstance()->mpGameOptions->mTankType = Tank::TankType_LongRange;
		}
		else if(this->mTankSelect->getOption().compare("HoverTank")==0)
		{
			Application::getInstance()->mpGameOptions->mTankType = Tank::TankType_CloseCombat;
		}

		// update the chatbox.
		int scroll=0;
		scroll = this->mChatBox->getTextFrame()->getTotalNumberOfRows()-this->mChatBox->getTextFrame()->getNumberOfRowsShown();
		this->mChatBox->getTextFrame()->setRowOffset(scroll);


		// update levelSelect
		if(this->mLevelSelect->getFlagMouseFocus())
		{
			this->mTextToSend->setInputMode(GUITextField::input_none);
			this->mLevelSelect->setInputMode(GUITextField::input_key | GUITextField::input_ignore_enter);
		}
		else
		{
			this->mLevelSelect->setInputMode(GUITextField::input_none);
			this->mTextToSend->setInputMode(GUITextField::input_key | GUITextField::input_ignore_enter);
		}

	}

}
void GUIHandlerStartMenu::setFlagRender(bool renderFlag)
{
	map<std::string, gfx::GUIFrame*>::iterator menu;
	for(menu = mMenus.begin(); menu != mMenus.end(); menu++)
	{
		(*menu).second->setFlagVisible(false);
	}
}

GUITextField* GUIHandlerStartMenu::getChatBox()const
{
	return this->mChatBox;
}

const char *GUIHandlerStartMenu::getName()
{
	return "GUIHandler Start Menu";
}

void GUIHandlerStartMenu::init()
{
	gfx::GUIHandler::init();

	int arrayIndex = 0;
	int textureID = 0;

	this->mButtonListener = myNew myButtonListener(this);

	// Creating the SignIn menu.
	GUIMainFrame *menu = myNew GUIMainFrame(getInstanceBase(), getClientWidth(), getClientHeight(), 0);
	menu->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	menu->setRelativeSize(D3DXVECTOR2(1.0f, 1.0f));
	menu->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));

	this->mSignInButton = myNew GUIButton(menu);
	menu->addSubFrame(this->mSignInButton);
	this->mSignInButton->setRelativeSize(D3DXVECTOR2(0.1f, 0.025f));
	this->mSignInButton->setRelativePosition(D3DXVECTOR2(0.5f, 0.45f));
	this->mSignInButton->getTextFrame()->setText("Sign In");
	gfx::DX10Module::getInstance()->getTextureIndex("SignIn_Default", arrayIndex, textureID);
	this->mSignInButton->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	this->mSignInButton->setTextureIndexOffset(textureID);
	this->mSignInButton->addButtonListener(this->mButtonListener);

	this->mSignInTextField = myNew GUITextField(menu);
	menu->addSubFrame(this->mSignInTextField);
	this->mSignInTextField->setRelativeSize(D3DXVECTOR2(0.2f, 0.025f));
	this->mSignInTextField->setRelativePosition(D3DXVECTOR2(0.3f, 0.45f));
	this->mSignInTextField->setInputMode(GUITextField::input_key | GUITextField::input_ignore_enter);

	menu->setFlagVisible(false);
	this->addFrame(menu);
	this->mCurrentMenu = menu;
	this->mMenus["SignIn"] = menu;


	// Creating the Main menu.
	menu = myNew GUIMainFrame(getInstanceBase(), getClientWidth(), getClientHeight(), 0);
	menu->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	menu->setRelativeSize(D3DXVECTOR2(1.0f, 1.0f));
	menu->setColor(D3DXCOLOR(0.3f, 0.0f, 0.0f, 1.0f));

	
	this->mGameList = myNew GUIList(menu);
	menu->addSubFrame(this->mGameList);
	this->mGameList->showNumberOfElements(15);
	this->mGameList->setRelativeSize(D3DXVECTOR2(0.5f, 0.4f));
	this->mGameList->setRelativePosition(D3DXVECTOR2(0.25f, 0.3f));


	this->mCreateButton = myNew GUIButton(menu);
	menu->addSubFrame(this->mCreateButton);
	this->mCreateButton->setRelativeSize(D3DXVECTOR2(0.1f, 0.04f));
	this->mCreateButton->setRelativePosition(D3DXVECTOR2(0.25f, 0.75f));
	this->mCreateButton->getTextFrame()->setText("Create Game");
	gfx::DX10Module::getInstance()->getTextureIndex("CreateGame_Default", arrayIndex, textureID);
	this->mCreateButton->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	this->mCreateButton->setTextureIndexOffset(textureID);
	this->mCreateButton->addButtonListener(this->mButtonListener);

	this->mJoinButton = myNew GUIButton(menu);
	menu->addSubFrame(this->mJoinButton);
	this->mJoinButton->setRelativeSize(D3DXVECTOR2(0.1f, 0.04f));
	this->mJoinButton->setRelativePosition(D3DXVECTOR2(0.40f, 0.75f));
	this->mJoinButton->getTextFrame()->setText("Join Game");
	gfx::DX10Module::getInstance()->getTextureIndex("JoinGame_Default", arrayIndex, textureID);
	this->mJoinButton->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	this->mJoinButton->setTextureIndexOffset(textureID);
	this->mJoinButton->addButtonListener(this->mButtonListener);

	this->mMapEditButton = myNew GUIButton(menu);
	menu->addSubFrame(this->mMapEditButton);
	this->mMapEditButton->setRelativeSize(D3DXVECTOR2(0.1f, 0.04f));
	this->mMapEditButton->setRelativePosition(D3DXVECTOR2(0.55f, 0.75f));
	this->mMapEditButton->getTextFrame()->setText("MapEdit");
	gfx::DX10Module::getInstance()->getTextureIndex("MapEdit_Default", arrayIndex, textureID);
	this->mMapEditButton->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	this->mMapEditButton->setTextureIndexOffset(textureID);
	this->mMapEditButton->addButtonListener(this->mButtonListener);

	this->mQuitButton = myNew GUIButton(menu);
	menu->addSubFrame(this->mQuitButton);
	this->mQuitButton->setRelativeSize(D3DXVECTOR2(0.1f, 0.04f));
	this->mQuitButton->setRelativePosition(D3DXVECTOR2(0.70f, 0.75f));
	this->mQuitButton->getTextFrame()->setText("Quit");
	gfx::DX10Module::getInstance()->getTextureIndex("Quit_Default", arrayIndex, textureID);
	this->mQuitButton->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	this->mQuitButton->setTextureIndexOffset(textureID);
	this->mQuitButton->addButtonListener(this->mButtonListener);
	

	menu->setFlagVisible(false);
	this->addFrame(menu);
	this->mMenus["Main"] = menu;


	// Creating the Lobby menu.
	menu = myNew GUIMainFrame(getInstanceBase(), getClientWidth(), getClientHeight(), 0);
	menu->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	menu->setRelativeSize(D3DXVECTOR2(1.0f, 1.0f));
	menu->setColor(D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.0f));


	this->mPlayer1 = myNew GUITextField(menu);
	menu->addSubFrame(this->mPlayer1);
	this->mPlayer1->setRelativeSize(D3DXVECTOR2(0.4f, 0.03f));
	this->mPlayer1->setRelativePosition(D3DXVECTOR2(0.02f, 0.2f));
	this->mPlayer1->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	this->mPlayer1->getTextFrame()->setText("............");

	this->mPlayer2 = myNew GUITextField(menu);
	menu->addSubFrame(this->mPlayer2);
	this->mPlayer2->setRelativeSize(D3DXVECTOR2(0.4f, 0.03f));
	this->mPlayer2->setRelativePosition(D3DXVECTOR2(0.02f, 0.26f));
	this->mPlayer2->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	this->mPlayer2->getTextFrame()->setText("............");

	this->mPlayer3 = myNew GUITextField(menu);
	menu->addSubFrame(this->mPlayer3);
	this->mPlayer3->setRelativeSize(D3DXVECTOR2(0.4f, 0.03f));
	this->mPlayer3->setRelativePosition(D3DXVECTOR2(0.02f, 0.32f));
	this->mPlayer3->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	this->mPlayer3->getTextFrame()->setText("............");

	this->mPlayer4 = myNew GUITextField(menu);
	menu->addSubFrame(this->mPlayer4);
	this->mPlayer4->setRelativeSize(D3DXVECTOR2(0.4f, 0.03f));
	this->mPlayer4->setRelativePosition(D3DXVECTOR2(0.02f, 0.38f));
	this->mPlayer4->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	this->mPlayer4->getTextFrame()->setText("............");


	this->mTextBattleTank = myNew GUITextField(menu);
	menu->addSubFrame(this->mTextBattleTank);
	this->mTextBattleTank->setRelativeSize(D3DXVECTOR2(0.13f, 0.03f));
	this->mTextBattleTank->setRelativePosition(D3DXVECTOR2(0.4f, 0.2f));
	this->mTextBattleTank->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	this->mTextBattleTank->getTextFrame()->setText("BattleTank");

	this->mTextHoverTank = myNew GUITextField(menu);
	menu->addSubFrame(this->mTextHoverTank);
	this->mTextHoverTank->setRelativeSize(D3DXVECTOR2(0.13f, 0.03f));
	this->mTextHoverTank->setRelativePosition(D3DXVECTOR2(0.7f, 0.2f));
	this->mTextHoverTank->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	this->mTextHoverTank->getTextFrame()->setText("HoverTank");

	// Create the radiobuttons
	this->mTankSelect = myNew GUIRadioButtons(menu);
	menu->addSubFrame(this->mTankSelect);
	gfx::DX10Module *pDxModule = gfx::DX10Module::getInstance();
	float aspectratio = (float)pDxModule->getClientWidth()/(float)pDxModule->getClientHeight();

	gfx::DX10Module::getInstance()->getTextureIndex("RBDefault", arrayIndex, textureID);

	GUIButton **button;
	button = myNew GUIButton *[2];
	button[0] = myNew GUIButton(this->mTankSelect);
	button[0]->setRelativeSize(D3DXVECTOR2(0.05f, 0.05f*aspectratio));
	button[0]->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	button[0]->setTextureIndexOffset(textureID);

	button[1] = myNew GUIButton(this->mTankSelect);
	button[1]->setRelativeSize(D3DXVECTOR2(0.05f, 0.05f*aspectratio));
	button[1]->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	button[1]->setTextureIndexOffset(textureID);

	button[0]->setRelativePosition(D3DXVECTOR2(this->mTextBattleTank->getRelativePosition().x + this->mTextBattleTank->getRelativeSize().x, 0.172f));
	button[1]->setRelativePosition(D3DXVECTOR2(this->mTextHoverTank->getRelativePosition().x + this->mTextHoverTank->getRelativeSize().x, 0.172f));
	
	this->mTankSelect->addButton(button[0], "BattleTank");
	this->mTankSelect->addButton(button[1], "HoverTank");

	this->mTankSelect->select(button[0]);
	SAFE_DELETE_ARRAY(button);


	// Setting upp the chat box
	this->mChatBox = myNew GUITextField(menu);
	menu->addSubFrame(this->mChatBox);
	this->mChatBox->setRelativeSize(D3DXVECTOR2(0.7f, 0.4f));
	this->mChatBox->setRelativePosition(D3DXVECTOR2(0.02f, 0.5f));
	this->mChatBox->setColor(D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f));
	this->mChatBox->getTextFrame()->setTextSize(D3DXVECTOR2(10.0f, 10.0f));
	this->mChatBox->getTextFrame()->setText("");
	this->mChatBox->getTextFrame()->showNumberOfRows(25);


	this->mTextToSend = myNew GUITextField(menu);
	menu->addSubFrame(this->mTextToSend);
	this->mTextToSend->setRelativeSize(D3DXVECTOR2(0.6f, 0.02f));
	this->mTextToSend->setRelativePosition(D3DXVECTOR2(0.02f, 0.95f));
	this->mTextToSend->setColor(D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f));
	this->mTextToSend->getTextFrame()->setTextSize(D3DXVECTOR2(10.0f, 10.0f));
	this->mTextToSend->getTextFrame()->setText("");
	this->mTextToSend->setInputMode(GUITextField::input_key | GUITextField::input_ignore_enter);


	this->mLevelSelectText = myNew GUITextField(menu);
	menu->addSubFrame(this->mLevelSelectText);
	this->mLevelSelectText->setRelativeSize(D3DXVECTOR2(0.2f, 0.02f));
	this->mLevelSelectText->setRelativePosition(D3DXVECTOR2(0.75f, 0.63f));
	this->mLevelSelectText->setColor(menu->getColor());
	this->mLevelSelectText->getTextFrame()->setTextSize(D3DXVECTOR2(10.0f, 10.0f));
	this->mLevelSelectText->getTextFrame()->setText("Enter Map-name");
	this->mLevelSelectText->setInputMode(GUITextField::input_none);


	this->mLevelSelect = myNew GUITextField(menu);
	menu->addSubFrame(this->mLevelSelect);
	this->mLevelSelect->setRelativeSize(D3DXVECTOR2(0.2f, 0.05f));
	this->mLevelSelect->setRelativePosition(D3DXVECTOR2(0.75f, 0.65f));
	this->mLevelSelect->setColor(D3DXCOLOR(0.9f, 0.9f, 0.9f, 1.0f));
	this->mLevelSelect->getTextFrame()->setTextSize(D3DXVECTOR2(10.0f, 10.0f));
	this->mLevelSelect->getTextFrame()->setText("default");
	this->mLevelSelect->setInputMode(GUITextField::input_key | GUITextField::input_ignore_enter);


	this->mSendButton = myNew GUIButton(menu);
	menu->addSubFrame(this->mSendButton);
	this->mSendButton->setRelativeSize(D3DXVECTOR2(0.1f, 0.02f));
	this->mSendButton->setRelativePosition(D3DXVECTOR2(0.62f, 0.95f));
	this->mSendButton->getTextFrame()->setText("Send");
	gfx::DX10Module::getInstance()->getTextureIndex("Send_Default", arrayIndex, textureID);
	this->mSendButton->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	this->mSendButton->setTextureIndexOffset(textureID);
	this->mSendButton->addButtonListener(this->mButtonListener);


	this->mReadyButton = myNew GUIButton(menu);
	menu->addSubFrame(this->mReadyButton);
	this->mReadyButton->setRelativeSize(D3DXVECTOR2(0.2f, 0.2f));
	this->mReadyButton->getTextFrame()->setRelativeSize(D3DXVECTOR2(1.0f, 0.5f));
	this->mReadyButton->setRelativePosition(D3DXVECTOR2(0.75f, 0.75f));
	this->mReadyButton->getTextFrame()->setText("Start");
	gfx::DX10Module::getInstance()->getTextureIndex("StartGame_Default", arrayIndex, textureID);
	this->mReadyButton->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	this->mReadyButton->setTextureIndexOffset(textureID);
	this->mReadyButton->addButtonListener(this->mButtonListener);

	menu->setFlagVisible(false);
	this->addFrame(menu);
	this->mMenus["Lobby"] = menu;

	goToMenu("SignIn");



	this->mApplication = Application::getInstance();
	//this->mNetwork = this->mApplication->mpNetwork;
	this->mServerLookup = this->mApplication->mpServLookup;
}

void GUIHandlerStartMenu::goToMenu(string menuName)
{
	this->mCurrentMenu->setFlagVisible(false);
	this->mCurrentMenu = this->mMenus[menuName];
	this->mCurrentMenu->setFlagVisible(true);
}

void GUIHandlerStartMenu::resetFields()
{
	mSignInTextField->getTextFrame()->setText("");
	mPlayer1->getTextFrame()->setText("");
	mPlayer2->getTextFrame()->setText("");
	mPlayer3->getTextFrame()->setText("");
	mPlayer4->getTextFrame()->setText("");
	mChatBox->getTextFrame()->setText("");
	mTextToSend->getTextFrame()->setText("");
	mLevelSelect->getTextFrame()->setText("DEFAULT");
}