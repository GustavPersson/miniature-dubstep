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

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "../Utilities/Macros.h"

#include "Initialization.h"
#include "EventManager.h"
#include "ActorHandler.h"
#include "InputStates.h"
#include "Statistics.h"

#include "GFXListener.h"

#include "GUIHandlerStartMenu.h"
#include "GUIFrameStartMenu.h"
#include "GUIHandlerLoadingScreen.h"
#include "GUIFrameLoadingScreen.h"

#include "ModelInfo.h"

#include "../GFX/DX10Module.h"

#include "../Sound/SoundSystem.h"

#ifdef USE_AI
#include "../Ai/AiFactory.h"
#endif

#ifdef USE_EDITOR
#include "../Editor/EditorHandler.h"
#endif

#include "TriggerHandler.h"
#include "HUDHandler.h"


namespace network
{
	class Network;
	class IdManager;
	class ServerLookup;
}


namespace logic
{

/**
* The current state of the game.
*/
enum GameStates
{
	GameState_MainMenu,
	GameState_Lobby,
	GameState_Starting,
	GameState_Waiting,
	GameState_Game
};

/**
* The app's role in the client-server relationship.
* If Server, it listens for incoming connections,
* if Client, it connects to a given server adress and socket.
*/
enum NetworkStates
{
	NetworkState_Client,
	NetworkState_Server	
};


/**
* Wrapper class defining the entry point, 
* main loop and exit point for the application.
*/
class Application
{
public:
	friend class GUIHandlerStartMenu;

	/**
	* Destructor. Pang. Frees the global instance.
	*/
	virtual ~Application();

	/**
	* The Application's entry point. Tests if the user's computer meets
	* some bare-minimum requirements for running the game. Starts the global timer,
	* global event manager, actor handler, initializes network and creates the 
	* DX10-Module.
	*/
	bool initInstance();

	/**
	* creates the instance, call this in the main.
	* If you want to create an instance of a derived class call that class's
	* createInstance instead.
	*/
	static void createInstance();

	/**
	* Returns an instance of the application.
	*/
	static Application* getInstance();

	/**
	* The main game loop.
	*/
	void runGame();
	
	/**
	* Returns the title of the application window.
	* @return Window title as a LPCSTR
	*/
	LPCSTR getGameTitle();
	
	/**
	* Deletes objects used during runtime. Make sure to delete allocated objects in the
	* REVERSE order of initialization.
	*/
	void onClose();

	/**
	* Sets the Application game state
	* (running, loading, lobby) from outside
	* the application loop.
	*/
	void setGameState(GameStates gameState);

	/**
	* Increase the number of players finished loading.
	*/
	void increaseFinishedLoading();

	void resetGame();
	
	
	struct GameOptions * mpGameOptions;

	GUIHandlerStartMenu* getStartMenu()const;


protected:
	/**
	* Constructor. Initializes local pointers and creates a new instance.
	*/
	Application();

	/**
	* Creates the game logic, setting the stepping-stones for
	* the application to run as intended.
	*/
	void createLogic();

	/**
	* Virtual function to create listeners
	* Call this function if you overload it in your sub-class.
	*/
	virtual void createListeners();
	
	/**
	* Registers events with event listeners.
	* Be sure to call this if you override this function.
	*/
	virtual void registerEvents();
	
	/**
	* Registers events to be shared over the network.
	* Only triggers if mpGameOptions->ignoreNetwork is false
	* Be sure to call this if you override this function.
	*/
	virtual void registerNetworkEvents();

	virtual void removeNetworkEvents();

	/**
	* The main loop that is run if we're in the main menu.
	* @param deltaTime time elapsed since the last frame
	*/
	virtual void mainMenuLoop(float deltaTime);
	
	/**
	* The main loop that is run if we're in the lobby.
	* @param deltaTime time elapsed since the last frame
	*/
	virtual void lobbyLoop(float deltaTime);

	/**
	* The loop that runs while we're in the main game loop.
	* @param deltaTime time elapsed since the last frame
	*/
	virtual void gameLoop(float deltaTime);

	/**
	* The loop that runs when you're waiting for other players.
	* @param deltaTime time elapsed since last frame.
	*/
	virtual void waitingLoop(float deltaTime);

	/**
	* Loads and instances meshes to be made.
	* Override this to only load the meshes you need.
	* @note Does NOT make mashed potatoes.
	*/
	virtual void loadMeshes();

	/**
	* Creates all objects we want to have in the game.
	* Override this if you want to create your own functions
	*/
	virtual void createObjects();

	/**
	* Sets the command window
	* Override this to set it to your own position
	*/
	virtual void setCommandWindow();

	/**
	* Final Game initialization. Gets called just before we enter the game state
	* @note this function gets called after all objects have been created, i.e. createObjects has been called.
	* Use this function if you want to test something before you add it to the Application's source code.
	*/
	virtual inline void finalGameInitialization()
	{
		// Don't add things here, they should be placed in setUpGame just before this function gets called!
	}

	bool mbWindowedMode;
	bool mbIsRunning;
	int	 mNrOfFinishedLoading;

	gfx::DX10Module			*mpDxModule;

	logic::EventManager*	mpEventManager;
	logic::EventListenerPtr mpSnooper;
	logic::EventListenerPtr mLogicListener;
	logic::EventListenerPtr mActorHandlerListener;
	logic::EventListenerPtr mGFXListener;
	logic::EventListenerPtr mSoundListener;
	logic::EventListenerPtr mNetworkListener;
	logic::EventListenerPtr mStatisticsListener;
	logic::ActorHandler		*mpActorHandler;
	logic::Statistics		*mpStatistics;
	logic::GFXListener		*mpGFXListener;
	TriggerHandler *mpTriggerHandler;
	HUDHandler *mpHUDHandler;

	logic::GUIHandlerStartMenu		*mpStartMenuHandler;
	logic::GUIHandlerLoadingScreen	*mpLoadingScreenHandler;

	logic::ModelInfo	*mpModelInfo;

	gfx::Camera			*mpCamera;
	gfx::InstanceMesh	*mpMesh;

	network::Network	*mpNetwork;
	network::IdManager	*mpIDManager;
	network::ServerLookup *mpServLookup;


	logic::InputStates	*mpInputStates;

	sound::SoundSystem	*mpSoundSystem;
	
	logic::GlobalTimer	*mpGlobalTimer;
	logic::LogicQuadTree *mpQuadTree;

	logic::PlayerInfo	*mpPlayerInfo;

	GameStates			mGameState;
	NetworkStates		mNetworkState;

#ifdef USE_AI
	ai::AiFactory*		mpAiFactory;
	logic::EventListenerPtr mAiListener;
#endif USE_AI

#ifdef USE_EDITOR
	editor::EditorHandler *mpEditor;
#endif
#ifdef USE_SOUND
	int mTrack;
#endif
	
	static Application * mpsInstance;


private:
	/**
	* Creates a game world.
	* @param deltaTime time elapsed since the last frame
	*/
	virtual void setUpGame(float deltaTime);
};
}
#endif