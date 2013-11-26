/**
 * @file
 * @author Viktor Svensson <arkangel88@gmail.com>
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
 * The class will use the FMOD EX API to load and play sounds and music. 
 * At first the class runs init-functions that will set everything up,
 * like sound-devices and then initialize ClassSoundSet, which will load the sounds into memory. 
 * After that it will respond to events from the event handler.
 *
 * The normal, small sounds will be loaded into memory, 
 * which will decrease the CPU-usage when the same sound plays several times on top of each other.
 * Because of the size of music and dialog-files, they will be streamed into memory during 
 * runtime to decrease memory-usage. The process does not cost a lot of CPU-performance.
 *
 * The class will also handle 3D sounds. For 3D sounds to work, the Eventhandler will have to give 
 * coordinates in 3D-space as to where the source of the sound in relation to the driver/listener.
 *
 * The function update(); have to be run once every frame. . 
 */

#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#pragma warning(disable:4505)

#include "../Utilities/Macros.h"
#include <fmod.hpp>
#include <fmod_errors.h>
#include <vector>
#include "Sound.h"

#include "../Logic/IEventManager.h"
#include "../Logic/Events.h"
#include "../Logic/ActorHandler.h"
#include "../Logic/KeyHandler.h"

namespace sound
{

	struct ActorInfo
	{
		logic::KeyType mActorID;
		bool mMoving;
		bool mRotating;
		utilities::Position mPosition;
	};

class SoundSystem
{
public:
	static SoundSystem* getInstance();

	/**
	 * Destructor
	 */
	~SoundSystem();

	/**
	 * Play a sound
	 * Channel ID: 0 = moveing, 1 = rotation, 2 = attacking
	 */
	int playSound( int soundID, logic::KeyType actorID, int channelID );

	/**
	* Plays a sound once at the coordinates
	*/
	void playSoundOnce( int soundID, float x, float y, float z );

	/**
	* Checks if the actors channel is playing a sound
	*/
	bool isPlaying( logic::KeyType actorID, int channelID ); 

	/**
	 * Toggles a channel on and off
	 */
	void toggleSound( logic::KeyType actorID, int channelID );
	/**
	* Turn a sound off
	*/
	void SoundOff( logic::KeyType actorID, int channelID );
	/**
	* Turn a sound on
	*/
	void SoundOn( logic::KeyType actorID, int channelID );

	/**
	 * Loads a music track and plays it
	 */
	void playMusic( int id );

	/**
	 * Toggles the music on or off
	 */
	void toggleMusic(); 

	/**
	 * Sets the volume for all sound channel. 
	 * Goes from 0.0 to 1.0, 0.0 = silent, 1.0 = full volume. Default = 1.0
	 */
	void setSoundVolume( float volume, logic::KeyType actorID, int channelID );

	/**
	 * Sets the volume for the music channel. 
	 * Goes from 0.0 to 1.0, 0.0 = silent, 1.0 = full volume. Default = 1.0
	 */
	void setMusicVolume( float volume );

	/**
	 * Set Sound Master Volume
	 */
	void setSoundMasterVolume( float volume );

	/**
	 * Set Music Master Volume
	 */
	void setMusicMasterVolume( float volume );

	/**
	 * Updates FMOD, have to be called every frame from the game-loop
	 */
	void Update( ); 

	/**
	 * Loads all game sounds that will be in the game 
	 */
	void testLoadMenuSounds(); // development function, will be deleted later
	void loadSounds();

	/**
	 * Creates a sound-channel and return an ID to contact it
	 */
	int createChannel( logic::KeyType actorID );

	/**
	 * Stops and releases a channel that isn't used anymore (an enemy dies, for example)
	 */
	void stopChannel( logic::KeyType actorID );

	/**
	 * Updates a channel with a new position.
	 * @param actorInfo A struct that contains necessary information to update the actors channel.
	 */
	void updateChannel(const ActorInfo& actorInfo );
	int findMovementSound( logic::KeyType actorID );
	int findRotationSound( logic::KeyType actorID );

	/**
	 * Starts playing a dialogue-sound
	 */
	void playDialogue( int soundID );

	/**
	 * Stops playing a dialogue-sound
	 */
	void stopDialogue(  );

	/**
	 * Paus/resume a dialogue-sound
	 */
	void toggleDialogue(  );

	/**
	 * Gets the player-actor from ActorHandler
	 */
	void setActor();

	/**
	 * Find the actor accociated with the actorID
	 */
	int findActor(logic::KeyType actorID);

	struct channelInfo
	{
		std::vector <FMOD::Channel*> channels;
		bool inUse;
		bool mRotating;
		bool mMoving;
		logic::KeyType actorID;
	};


private:
	/**
	 * Constructor
	 */
	SoundSystem();

	/**
	 * Checks for FMOD-errors
	 */
	void HandleError( FMOD_RESULT mResult );

	/**
	* Initialize vectors and fills them with data
	*/
	void initVectors();

	FMOD::System *mpSoundSystem;
	FMOD::Channel *mpChannel;
	FMOD::Channel *mpMusicChannel;
	FMOD::Channel *mpDialogueChannel;
	FMOD_RESULT mResult; //for error handling 

	std::vector<FMOD::Sound*> mSounds;
	std::vector<FMOD::Sound*> mMusic;
	std::vector<FMOD::Sound*> mLoops; //*********
	std::vector<char*> mGameSounds;
	std::vector<char*> mGameMusic;
	std::vector<char*> mDialogue;
	std::vector<char*> mLoop;
	std::vector<channelInfo> mChannels;

 
	FMOD_VECTOR mListenerPos; 
	FMOD_VECTOR mLastpos;
	//FMOD_VECTOR mForward;
	//FMOD_VECTOR mUp;

	logic::Actor *mpPlayer;
	logic::ActorHandler *mpActorHandler;

	static SoundSystem *mpsInstance;

	float mMasterSoundVolume;
	float mMasterMusicVolume;
	

};
}
#endif
