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
 * like sound-devices and load the sounds into memory. 
 * After that it will respond to events from the event handler.
 *
 * The normal, small sounds will be loaded into memory, 
 * which will decrease the CPU-usage when the same sound plays several times on top of each other.
 * Because of the size of music and dialog-files, they will be streamed into memory during 
 * runtime to decrease memory-usage. The process does not cost a lot of CPU-performance.
 *
 * The class will also handle 3D sounds. For 3D sounds to work, the Eventhandler will have to give 
 * coordinates in 3D-space as to where the source of the sound in relation to the player/listener.
 *
 * The function update(); have to be run once every frame. . 
 */

#include "SoundSystem.h"
#include "../Logic/InputStates.h"
#include "../Logic/Events.h"
#include "../Logic/Enemy.h"
#include "../Logic/tank.h"
#include "../Logic/PlayerInfo.h"

using namespace sound;
SoundSystem* SoundSystem::mpsInstance = NULL;

SoundSystem* SoundSystem::getInstance()
{
	// If there is no instance of this object
	if(mpsInstance == NULL)
	{
		// Create an instance of this object
		mpsInstance = myNew SoundSystem();
	}
	// return GlobalTimer
	return mpsInstance;
}


SoundSystem::SoundSystem() 
{

	mpSoundSystem = NULL;
	mpChannel = NULL;
	mpMusicChannel = NULL;
	mpDialogueChannel = NULL;
	mpPlayerInfo = NULL;
	mpActorHandler = NULL;

	mMasterSoundVolume = 1.0f;
	mMasterMusicVolume = 1.0f;

	mChannelIndexCounter = 0;

	mMusicLastPlayed = -1;


	//Follow FMOD Recommended Startup Sequence for Windows. Basically just copy-paste
	//FMOD Recommended Startup Sequence for Windows
	unsigned int     version;
	int              numdrivers;
	FMOD_SPEAKERMODE speakermode;
	FMOD_CAPS        caps;
	char             name[256];

	/*
        Create a System object and initialize.
    */
	mResult = FMOD::System_Create(&mpSoundSystem);
	HandleError(mResult);

	mResult = mpSoundSystem->getVersion(&version);
    HandleError(mResult);

    if (version < FMOD_VERSION)
    {
        //printf("Error!  You are using an old version of FMOD %08x.  This program requires %08x\n", version, FMOD_VERSION);
		ERROR_MESSAGE( "Error!  You are using an old version of FMOD (" << version << ").  This program requires " << FMOD_VERSION);
        exit(-1);
    }
    
    mResult = mpSoundSystem->getNumDrivers(&numdrivers);
    HandleError(mResult);

    if (numdrivers == 0)
    {
        mResult = mpSoundSystem->setOutput(FMOD_OUTPUTTYPE_NOSOUND);
        HandleError(mResult);
    }
    else
    {
        mResult = mpSoundSystem->getDriverCaps(0, &caps, 0, 0, &speakermode);
        HandleError(mResult);

        mResult = mpSoundSystem->setSpeakerMode(speakermode);       /* Set the user selected speaker mode. */
        HandleError(mResult);

        if (caps & FMOD_CAPS_HARDWARE_EMULATED)             /* The user has the 'Acceleration' slider set to off!  This is really bad for latency!. */
        {                                                   /* You might want to warn the user about this. */
			DEBUG_MESSAGE(LEVEL_HIGHER, "Sound Acceleration is set to off in your OS! This will hurt latency!");
            mResult = mpSoundSystem->setDSPBufferSize(1024, 10);
            HandleError(mResult);
        }

        mResult = mpSoundSystem->getDriverInfo(0, name, 256, 0);
        HandleError(mResult);

        if (strstr(name, "SigmaTel"))   /* Sigmatel sound devices crackle for some reason if the format is PCM 16bit.  PCM floating point output seems to solve it. */
        {
            mResult = mpSoundSystem->setSoftwareFormat(48000, FMOD_SOUND_FORMAT_PCMFLOAT, 0,0, FMOD_DSP_RESAMPLER_LINEAR);
            HandleError(mResult);
        }
    }

    mResult = mpSoundSystem->init(2000, FMOD_INIT_NORMAL, 0);
    if (mResult == FMOD_ERR_OUTPUT_CREATEBUFFER)         /* Ok, the speaker mode selected isn't supported by this soundcard.  Switch it back to stereo... */
    {
        mResult = mpSoundSystem->setSpeakerMode(FMOD_SPEAKERMODE_STEREO);
        HandleError(mResult);
            
        mResult = mpSoundSystem->init(2000, FMOD_INIT_NORMAL, 0);/* ... and re-init. */
        HandleError(mResult);
    }
	//FMOD Recommended Startup Sequence for Windows end

	/*
        Set the distance units. (meters/feet etc).
    */
    mResult = mpSoundSystem->set3DSettings(1.0f, 1.0f, 0.5f); // TODO: Tweak
    HandleError(mResult);

	initVectors();

	mpActorHandler = logic::ActorHandler::getInstance();
	mpPlayerInfo = logic::PlayerInfo::getInstance();


	//Call loadSounds, it handles its own errors
	loadSounds();
	//testLoadMenuSounds();

	/*mResult = mpSoundSystem->playSound((FMOD_CHANNELINDEX)-1, 0, true, &mpChannel);
	HandleError( mResult );
	mResult = mpSoundSystem->playSound((FMOD_CHANNELINDEX)-1, 0, true, &mpMusicChannel);
	HandleError( mResult );
	mResult = mpSoundSystem->playSound((FMOD_CHANNELINDEX)-1, 0, true, &mpDialogueChannel);
	HandleError( mResult );*/

	/*for(int i = 0; i < 100; i++)
	{
		FMOD::Channel* newChannel;

	}*/

	this->playSoundOnce(sound::Sound_MenuButtonPressedDefault, 0.0f, 0.0f, 0.0f);


	// 010011000100111101001100
}

SoundSystem::~SoundSystem() 
{
	

	//stop channels
	for(unsigned int i = 0; i < mChannels.size(); i++)
	{
		mChannels[i].channels[0]->stop();
		mChannels[i].channels[1]->stop();
		mChannels[i].channels[2]->stop();
	}
	for(unsigned int i = 0; i < mSingleChannels.size(); i++)
	{
		mSingleChannels[i].mChannel->stop();
	}


	// for every sound in the vector mSounds
	for(unsigned int i = 0; i < mSounds.size(); i++)
	{
		//Release sound
		mResult = mSounds[i]->release();
		HandleError(mResult);
	}

	//Free sounds
	//Call FMOD::System system->release() to close output device, free memory and stop all channels. 
	mpSoundSystem->close();
	mpSoundSystem->release();
	mpChannel->stop();
	mpMusicChannel->stop();
	

	//clear vectors
	mSounds.clear();
	mGameSounds.clear();
	mGameMusic.clear();
	mChannels.clear();
	mSingleChannels.clear();

	mpsInstance = NULL;
}

int SoundSystem::playSound( int soundID, logic::KeyType actorID, int channelID ) //**
{

	int channel = findActor(actorID);

	
	/*for(unsigned int i = 0; i < mChannels.size(); i++)
	{
		if(actorID == mChannels[i].actorID)
		{
			channel = i;
			break;
		}
	}*/

	if(channel == -1)
		{
			ERROR_MESSAGE( "Sound Error - 60002: Actor " << actorID << " have no channel for its sound.\n" );
			return -1;
		}

	if( mChannels[channel].inUse == true )
	{

		//Call FMOD::System::playSound with FMOD_CHANNEL_FREE, sound-object from ClassSoundSet, false and the channel-reference. Store return in mResult. 
		mResult = mpSoundSystem->playSound((FMOD_CHANNELINDEX)((channel*3)+channelID), mSounds[soundID], false, &mChannels[channel].channels[channelID]);
		//(FMOD_CHANNELINDEX)channel

		//Call HandleError with mResult
		HandleError(mResult);

		mChannels[channel].channels[channelID]->setVolume( mMasterSoundVolume );

	/*if(mChannels.size() != 0)
	{
		float vol = 0.0f;
		mChannels[channel].channels[channelID]->getVolume(&vol);
		DEBUG_MESSAGE(LEVEL_HIGHEST, "Volume: " << vol);
	}*/

		return channel;

	}
	
 	return -1;
}

void SoundSystem::playSoundOnce( int soundID, float x, float y, float z ) 
{

	//Create position vector
	FMOD_VECTOR pos = {x, y, z};

	//Create a new channel, push it to mSingleChannels and remove it later in update
	FMOD::Channel *channel = 0;
	
	

	FMOD_CHANNELINDEX index = (FMOD_CHANNELINDEX)(2000 - mChannelIndexCounter);
	//(FMOD_CHANNELINDEX)(2000 - mChannelIndexCounter)

	//Play sound
	mResult = mpSoundSystem->playSound( index, mSounds[soundID], false, &channel ); 
	HandleError( mResult );

	singleChannelInfo info = { channel, pos };
	mSingleChannels.push_back(info);

	//set volume
	mResult = channel->setVolume( mMasterSoundVolume ); 
	HandleError( mResult );

	//Set 3D-attributes
	mResult = channel->set3DAttributes( &pos, NULL );
	HandleError( mResult );
	
	mChannelIndexCounter++;
	if(mChannelIndexCounter >= 100)
	{
		mChannelIndexCounter = 0;
	}

}

bool SoundSystem::isPlaying( logic::KeyType actorID, int channelID ) 
{
	int iActorID = findActor( actorID );

	bool temp;
	mChannels[iActorID].channels[channelID]->isPlaying(&temp);
	return temp;
}

void SoundSystem::toggleSound( logic::KeyType actorID, int channelID )
{
	int iActorID = findActor( actorID );

	bool paused;

	mChannels[iActorID].channels[channelID]->getPaused(&paused);
	mResult = mChannels[channelID].channels[channelID]->setPaused(!paused);
	HandleError(mResult);
}
void SoundSystem::SoundOff( logic::KeyType actorID, int channelID )
{
	int iActorID = findActor( actorID );

	mResult = mChannels[iActorID].channels[channelID]->setPaused(true);
	HandleError(mResult);
}

void SoundSystem::SoundOn( logic::KeyType actorID, int channelID )
{
	int iActorID = findActor( actorID );

	mResult = mChannels[iActorID].channels[channelID]->setPaused(true);
	HandleError(mResult);
}

void SoundSystem::playMusic( int soundID ) 
{

	//Call FMOD::System::playSound with FMOD_CHANNEL_FREE, sound-object from ClassSoundSet, false and the channel-reference. 
	mResult = mpSoundSystem->playSound( (FMOD_CHANNELINDEX)1899, mSounds[soundID], false, &mpMusicChannel );
	mpMusicChannel->setVolume( mMasterMusicVolume );

	//Call HandleError with mResult
	HandleError( mResult );

}

void SoundSystem::toggleMusic() 
{

	//Create a bool
	bool paused;

	//Call channel::getPaused with bool reference
	mpMusicChannel->getPaused(&paused);

	//Call channel::setPaused with !bool, returns to result;
	mResult = mpMusicChannel->setPaused(!paused);

	//Call HandleError with mResult
	HandleError(mResult);
}

void SoundSystem::setSoundVolume( float volume, logic::KeyType actorID, int channelID ) 
{
	int iActorID = findActor(actorID);

	if(volume > mMasterSoundVolume)
		volume = mMasterSoundVolume;

	if( iActorID != -1 )
	{

		//Call Channel::setVolume( float volume ) on the FMOD::Channel in that element
		mResult = mChannels[iActorID].channels[channelID]->setVolume( volume );
		HandleError(mResult);

	}

}

void SoundSystem::setMusicVolume( float volume ) 
{

	//Call Channel::setVolume( float volume ) the FMOD::Channel that handles music 
	mResult = mpMusicChannel->setVolume( volume );
	HandleError( mResult );

}

void SoundSystem::setSoundMasterVolume( float volume )
{
	if( volume <= 1.0f && volume >= 0.0f )
	{
		mMasterSoundVolume = volume;

		mResult = mpChannel->setVolume( volume );
		HandleError(mResult);
		mResult = mpMusicChannel->setVolume( volume );
		HandleError(mResult);
		mResult = mpDialogueChannel->setVolume( volume );
		HandleError(mResult);

		for(unsigned int i = 0; i < mChannels.size(); i++)
		{
			mResult = mChannels[i].channels[0]->setVolume( volume );
			HandleError(mResult);
			mResult = mChannels[i].channels[1]->setVolume( volume );
			HandleError(mResult);
			mResult = mChannels[i].channels[2]->setVolume( volume );
			HandleError(mResult);
		}

		for(unsigned int i = 0; i < mSingleChannels.size(); i++)
		{
			mResult = mSingleChannels[i].mChannel->setVolume( volume );
			HandleError(mResult);
		}
	}
}
void SoundSystem::setMusicMasterVolume( float volume )
{
	mMasterMusicVolume = volume;
	mpMusicChannel->setVolume( volume );
}

void SoundSystem::update( ) 
{
	//Update the listener
	//FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
	//FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };
	
	/*if(mChannels.size() != 0)
	{
		float vol = 0.0f;
		mChannels[0].channels[2]->getVolume(&vol);
		DEBUG_MESSAGE(LEVEL_HIGHEST, "Volume: " << vol);
	}*/

	for(unsigned int i = 0; i < mSingleChannels.size(); i++)
	{
		bool playing;
		mSingleChannels[i].mChannel->isPlaying(&playing);
		if( !playing )
		{
			mSingleChannels[i].mChannel->stop();
			//FMOD::Channel* temp = mSingleChannels[i];
			//delete temp;
			mSingleChannels.erase(mSingleChannels.begin()+i);
		}
		else 
		{
			mSingleChannels[i].mChannel->set3DAttributes(&mSingleChannels[i].mPos, NULL);
		}
	}


	mPlayerId = mpPlayerInfo->getPlayerId();
	logic::Actor* pActor = NULL; 

	if( mPlayerId != logic::INVALID_KEY )
	{
		pActor = mpActorHandler->getActor(mPlayerId);

		if( pActor != NULL )
		{
			
			utilities::Position acPos = pActor->getPosition();
			utilities::Direction acDir = pActor->getDirection();
			

			mLastpos = mListenerPos;
			mListenerPos.x = acPos.x;
			mListenerPos.y = acPos.y;
			mListenerPos.z = acPos.z;

			mListenerDir.x = acDir.x;
			mListenerDir.y = acDir.y;
			mListenerDir.z = acDir.z;

			//mChannels[0].channel->set3DAttributes(&mListenerPos, NULL);


			//mResult = mpSoundSystem->set3DListenerAttributes(0, &mListenerPos, &mVel, &mForward, &mUp);
			mResult = mpSoundSystem->set3DListenerAttributes(0, &mListenerPos, NULL, &mListenerDir, NULL);
			HandleError(mResult);

			//Updates channels
			/*for(unsigned int i = 0; i < mChannels.size(); i++)
			{
				if(mChannels[i].inUse == true)
					updateChannel(mChannels[i].actorID);
			}*/
		}
	}
	//Call FMOD::System::update
	mResult = mpSoundSystem->update();
	HandleError( mResult );

}

void SoundSystem::HandleError( FMOD_RESULT mResult )
{
	//if mResult is not equal to FMOD_OK
	if( mResult != FMOD_OK && mResult != 36 && mResult != 37 )
	{

		//print error - send debug information
  		ERROR_MESSAGE( "FMOD error! " << mResult << " - " << FMOD_ErrorString( mResult ) );
		
	}
}

void SoundSystem::initVectors(){
	
	//Menu
	mGameSounds.push_back("../../Files/Sound/Button_Rollover.wav");
	mGameSounds.push_back("../../Files/Sound/Button_Pressed_Default.wav");
	mGameSounds.push_back("../../Files/Sound/Error.wav"); 
	mGameSounds.push_back("../../Files/Sound/Connect.wav");

	//Player
	mGameSounds.push_back("../../Files/Sound/Tank_LR_Movement_Stop.wav");
	mGameSounds.push_back("../../Files/Sound/Tank_CC_Movement_Stop.wav");
	mGameSounds.push_back("../../Files/Sound/Tank_Death.wav");
	mGameSounds.push_back("../../Files/Sound/Tank_Eject.wav");
	mGameSounds.push_back("../../Files/Sound/Tank_Collision_House_1.wav");
	mGameSounds.push_back("../../Files/Sound/Tank_Collision_House_2.wav");
	mGameSounds.push_back("../../Files/Sound/Tank_Collision_LargeEnemy.wav");
	mGameSounds.push_back("../../Files/Sound/Tank_Collision_Tank.wav");
	mGameSounds.push_back("../../Files/Sound/Tank_Hit_Melee.wav");
	mGameSounds.push_back("../../Files/Sound/Tank_Hit_Ranged_Fire.wav");
	mGameSounds.push_back("../../Files/Sound/Tank_Hit_Ranged_Acid.wav");
	mGameSounds.push_back("../../Files/Sound/Tank_Hit_Ranged_Plasma.wav");
	mGameSounds.push_back("../../Files/Sound/Foot_Death.wav");
	mGameSounds.push_back("../../Files/Sound/Foot_EnterTank.wav");
	mGameSounds.push_back("../../Files/Sound/Foot_Hit.wav");

	//Ant
	mGameSounds.push_back("../../Files/Sound/Ant_Death.wav");
	mGameSounds.push_back("../../Files/Sound/Ant_Spawn.wav");
	mGameSounds.push_back("../../Files/Sound/Ant_Attack_Ranged.wav");
	mGameSounds.push_back("../../Files/Sound/Ant_Attack_Ranged_Impact.wav");
	mGameSounds.push_back("../../Files/Sound/Ant_Attack_Melee.wav");
	mGameSounds.push_back("../../Files/Sound/Ant_Attack_Explosion.wav");
	mGameSounds.push_back("../../Files/Sound/Ant_Hit.wav");

	//Spider
	mGameSounds.push_back("../../Files/Sound/Spider_Death.wav");
	mGameSounds.push_back("../../Files/Sound/Spider_Spawn.wav");
	mGameSounds.push_back("../../Files/Sound/Spider_Attack_Ranged.wav");
	mGameSounds.push_back("../../Files/Sound/Spider_Attack_Ranged_Impact.wav");
	mGameSounds.push_back("../../Files/Sound/Spider_Attack_Melee.wav");
	mGameSounds.push_back("../../Files/Sound/Spider_Jump_Attack.wav");
	mGameSounds.push_back("../../Files/Sound/Spider_Hit.wav");

	//Beetle
	mGameSounds.push_back("../../Files/Sound/Beetle_Death.wav");
	mGameSounds.push_back("../../Files/Sound/Beetle_Spawn.wav");
	mGameSounds.push_back("../../Files/Sound/Beetle_Attack_Ranged.wav");
	mGameSounds.push_back("../../Files/Sound/Beetle_Attack_Ranged_Impact.wav");
	mGameSounds.push_back("../../Files/Sound/Beetle_Attack_Melee.wav");
	mGameSounds.push_back("../../Files/Sound/Beetle_Hit.wav");

	//Civilian
	mGameSounds.push_back("../../Files/Sound/Civilian_Death.wav");
	mGameSounds.push_back("../../Files/Sound/Civilian_Stunned.wav");
	mGameSounds.push_back("../../Files/Sound/Civilian_Scream_Fleeing.wav");
	mGameSounds.push_back("../../Files/Sound/Civilian_Scream_Hijack.wav");
	mGameSounds.push_back("../../Files/Sound/Civilian_Enter_Tank.wav");
	mGameSounds.push_back("../../Files/Sound/Civilian_Hit.wav");

	//Weapons
	mGameSounds.push_back("../../Files/Sound/Minigun_Impact.wav");
	mGameSounds.push_back("../../Files/Sound/Minigun_Start.wav");
	mGameSounds.push_back("../../Files/Sound/Minigun_End.wav");

	mGameSounds.push_back("../../Files/Sound/FlameThrower_Fire.wav");
	mGameSounds.push_back("../../Files/Sound/FlameThrower_Impact.wav");
	mGameSounds.push_back("../../Files/Sound/FlameThrower_Start.wav");
	mGameSounds.push_back("../../Files/Sound/FlameThrower_End.wav");

	mGameSounds.push_back("../../Files/Sound/PulseCannon_Fire.wav");
	mGameSounds.push_back("../../Files/Sound/PulseCannon_Impact.wav");
	mGameSounds.push_back("../../Files/Sound/PulseCannon_Start.wav");
	mGameSounds.push_back("../../Files/Sound/PulseCannon_End.wav");

	mGameSounds.push_back("../../Files/Sound/Cannon_Fire.wav");
	mGameSounds.push_back("../../Files/Sound/Cannon_Impact.wav");

	mGameSounds.push_back("../../Files/Sound/GaussCannon_Fire.wav");
	mGameSounds.push_back("../../Files/Sound/GaussCannon_Impact.wav");
	mGameSounds.push_back("../../Files/Sound/GaussCannon_Start.wav");

	mGameSounds.push_back("../../Files/Sound/BeamCannon_Fire.wav");
	mGameSounds.push_back("../../Files/Sound/BeamCannon_Impact.wav");

	mGameSounds.push_back("../../Files/Sound/Shotgun_Fire.wav");
	mGameSounds.push_back("../../Files/Sound/Shotgun_Impact.wav");

	mGameSounds.push_back("../../Files/Sound/PedestrianGun_Fire.wav");
	mGameSounds.push_back("../../Files/Sound/PedestrianGun_Impact.wav");

	//Other
	mGameSounds.push_back("../../Files/Sound/Explosion_Default.wav");
	mGameSounds.push_back("../../Files/Sound/Enemy_Enter_Building.wav");

	//Loops
	mLoop.push_back("../../Files/Sound/Tank_LR_Movement_Moving.wav");
	mLoop.push_back("../../Files/Sound/Tank_CC_Movement_Moving.wav");
	mLoop.push_back("../../Files/Sound/Tank_LR_Movement_Rotation.wav");
	mLoop.push_back("../../Files/Sound/Tank_CC_Movement_Rotation.wav");
	mLoop.push_back("../../Files/Sound/Pedestrian_Movement.wav");
	mLoop.push_back("../../Files/Sound/Ant_Movement.wav");
	mLoop.push_back("../../Files/Sound/Spider_Movement.wav");
	mLoop.push_back("../../Files/Sound/Beetle_Movement.wav");
	mLoop.push_back("../../Files/Sound/Tank_TurretRotation.wav");
	mLoop.push_back("../../Files/Sound/Minigun_Fire.wav");

	
	// Music
	mGameMusic.push_back("../../Files/Music/BugTanks.mp3");
	mGameMusic.push_back("../../Files/Music/Gameplay.wav");
	mGameMusic.push_back("../../Files/Music/For_Liberty.mp3");
	mGameMusic.push_back("../../Files/Music/MoonBase.mp3");


	// Dialogue
	mDialogue.push_back("../../Files/Dialogue/F_Welcome.wav");

}

void SoundSystem::loadSounds()
{

	//for 6 (the number of menu sounds) loops
	for(unsigned int i = 0; i < 4; i++)
	{

		//Create a FMOD::Sound* object
		FMOD::Sound *sound;

		DEBUG_MESSAGE(LEVEL_LOW, "Loading: " << mGameSounds[i]);

		//Call FMOD::System::createSound using filename from the vector mGameSounds, FMOD_DEFAULT, NULL, FMOD::Sound-object and return it to mResult
		mResult = mpSoundSystem->createSound(mGameSounds[i], FMOD_DEFAULT, NULL, &sound);

		//Call HandleError with mResult
		HandleError(mResult);

		//push_back the FMOD::Sound object to mSounds
		mSounds.push_back(sound);

	}



	//for every element in the mGameSounds vector, starting with 6 (or the number of menu sounds)
	for(unsigned int i = 4; i < mGameSounds.size(); i++) //hax
	{

		//Create a FMOD::Sound* object
		FMOD::Sound *sound;

		DEBUG_MESSAGE(LEVEL_LOW, "Loading: " << mGameSounds[i]);

		//Call FMOD::System::createSound using filename from the vector mGameSounds, (FMOD_MODE) (FMOD_LOOP_OFF | FMOD_3D | FMOD_HARDWARE), NULL, FMOD::Sound-object and return it to mResult
		mResult = mpSoundSystem->createSound(mGameSounds[i], (FMOD_MODE) (FMOD_LOOP_OFF | FMOD_3D | FMOD_HARDWARE), NULL, &sound);

		//Call HandleError with mResult
		HandleError(mResult);

		//push_back the FMOD::Sound object to mSounds
		mSounds.push_back(sound);

	}

	//for elements in mLoop
	for(unsigned int i = 0; i < mLoop.size(); i++)
	{
		//Create a FMOD::Sound* object
		FMOD::Sound *sound;

		DEBUG_MESSAGE(LEVEL_LOW, "Loading: " << mLoop[i]);

		//Call FMOD::System::createSound using filename from the vector mGameSounds, (FMOD_MODE) (FMOD_LOOP_NORMAL | FMOD_3D | FMOD_HARDWARE), NULL, FMOD::Sound-object and return it to mResult
		mResult = mpSoundSystem->createSound(mLoop[i], (FMOD_MODE) (FMOD_LOOP_NORMAL | FMOD_3D | FMOD_HARDWARE), NULL, &sound);

		//Call HandleError with mResult
		HandleError(mResult);

		//push_back the FMOD::Sound object to mSounds
		mSounds.push_back(sound);
	}

	//for every element in the vector mGameMusic
	for(unsigned int i = 0; i < mGameMusic.size(); i++)
	{

		//Create a FMOD::Sound object
		FMOD::Sound *sound;

		DEBUG_MESSAGE(LEVEL_LOW, "Loading: " << mGameMusic[i]);

		//Call FMOD::System::createStream using filename from the vector mGameMusic, (FMOD_MODE) (FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE), NULL, FMOD::Sound-object and return it to mResult
		mResult = mpSoundSystem->createStream(mGameMusic[i], (FMOD_MODE) (FMOD_LOOP_NORMAL | FMOD_2D | FMOD_HARDWARE), NULL, &sound);

		//Call HandleError with mResult
		HandleError(mResult);

		//push_back the FMOD::Sound object to mSounds
		mSounds.push_back(sound);

	}

}

int SoundSystem::createChannel( logic::KeyType actorID )
{

	// Find an unused ID from the channel vector
	int pos = -1;
	//FMOD_VECTOR position = {posX, posY, posZ};

	for(unsigned int i = 0; i < mChannels.size(); i++)
	{

		if(mChannels[i].inUse == false)
		{

			pos = i;
			break;

		}

	}

	// Create a channel and push it back to the channel vector, check for errors
	if(pos > -1)
	{

		mChannels[pos].inUse = true;
		mChannels[pos].actorID = actorID;
		mChannels[pos].channels[0]->setVolume( mMasterSoundVolume );
		mChannels[pos].channels[1]->setVolume( mMasterSoundVolume );
		mChannels[pos].channels[2]->setVolume( mMasterSoundVolume );

	}
	else
	{

		// Set channel 3D attributes, check for errors
		FMOD::Channel *newChannel1;
		FMOD::Channel *newChannel2;
		FMOD::Channel *newChannel3;
		mResult = mpSoundSystem->playSound((FMOD_CHANNELINDEX)-1, 0, true, &newChannel1);
		HandleError( mResult );
		mResult = mpSoundSystem->playSound((FMOD_CHANNELINDEX)-1, 0, true, &newChannel2);
		HandleError( mResult );
		mResult = mpSoundSystem->playSound((FMOD_CHANNELINDEX)-1, 0, true, &newChannel3);
		HandleError( mResult );
		newChannel1->setVolume ( mMasterSoundVolume );
		newChannel2->setVolume ( mMasterSoundVolume );
		newChannel3->setVolume ( mMasterSoundVolume );
		channelInfo ch;
		ch.actorID = actorID;
		ch.inUse = true;
		ch.mRotating = false;
		ch.mSubRot = false;
		ch.mMoving = false;
		ch.mFire = false;
		ch.channels.push_back(newChannel1);
		ch.channels.push_back(newChannel2);
		ch.channels.push_back(newChannel3);

		mChannels.push_back( ch );
		pos = mChannels.size()-1; 

	}

	// Return channel ID
	return pos;
}

void SoundSystem::stopChannel( logic::KeyType actorID )
{
	int channelID = findActor( actorID );

	// Stop / release channel
	mResult = mChannels[channelID].channels[0]->stop();
	HandleError( mResult );
	mResult = mChannels[channelID].channels[1]->stop();
	HandleError( mResult );
	mResult = mChannels[channelID].channels[2]->stop();
	HandleError( mResult );
	mChannels[channelID].inUse = false;

}


void SoundSystem::playDialogue( int soundID )
{

	FMOD::Sound *sound;
	mResult = mpSoundSystem->createStream(mDialogue[soundID], (FMOD_MODE) (FMOD_LOOP_OFF | FMOD_2D | FMOD_HARDWARE), NULL, &sound);
	HandleError(mResult);

	bool playing;
	mResult = mpDialogueChannel->isPlaying(&playing);
	HandleError(mResult);

	if( playing == true )
	{
		mpDialogueChannel->stop();
	}
	
	mResult = mpSoundSystem->playSound((FMOD_CHANNELINDEX)1900, mSounds[soundID], false, &mpDialogueChannel);
	HandleError(mResult); 

}

void SoundSystem::stopDialogue( )
{
	mpDialogueChannel->stop();
}

void SoundSystem::toggleDialogue( )
{
	bool paused;
	mpDialogueChannel->getPaused(&paused);
	mpDialogueChannel->setPaused(!paused);
}

int SoundSystem::findActor( logic::KeyType actorID )
{
	for(unsigned int i = 0; i < mChannels.size(); i++)
	{
		if(mChannels[i].actorID == actorID)
			return i;
	}

	ERROR_MESSAGE( "Sound Error: 60003 - No actor found with the provided actor ID\n");
	return -1;
}

void SoundSystem::updateChannel( const ActorInfo& actorInfo )
{
	logic::Actor* actor = mpActorHandler->getActor(actorInfo.mActorID);
	if( actor->getActorType() != logic::Actor::ActorType_Munition )
	{
		int iActorID = findActor( actorInfo.mActorID );

		FMOD_VECTOR iPos = {actorInfo.mPosition.x, actorInfo.mPosition.y, actorInfo.mPosition.z};
		
		if( iActorID != -1 )
		{
			// Set new 3D-attributes on the channel that match the channel ID
			mResult = mChannels[iActorID].channels[0]->set3DAttributes(&iPos, NULL);
			HandleError( mResult );
			mResult = mChannels[iActorID].channels[1]->set3DAttributes(&iPos, NULL);
			HandleError( mResult );
			mResult = mChannels[iActorID].channels[2]->set3DAttributes(&iPos, NULL);
			HandleError( mResult );

			/*
			mResult = mChannels[iActorID].channels[0]->setVolume( mMasterSoundVolume );
			HandleError( mResult );
			mResult = mChannels[iActorID].channels[1]->setVolume( mMasterSoundVolume );
			HandleError( mResult );
			mResult = mChannels[iActorID].channels[2]->setVolume( mMasterSoundVolume );
			HandleError( mResult );
			*/
	 
			if(actorInfo.mRotating != mChannels[iActorID].mRotating)
			{
				if(actorInfo.mRotating == true)
				{
					int soundID = findRotationSound(actorInfo.mActorID, false);
					if(soundID != -1)
					{
						this->playSound( soundID, actorInfo.mActorID, Channel_Rotation);
						//mResult = mpSoundSystem->playSound((FMOD_CHANNELINDEX)((iActorID*3)+1), mSounds[soundID], false, &mChannels[iActorID].channels[Channel_Rotation]);
						//HandleError(mResult);
						//mResult = mChannels[iActorID].channels[1]->setVolume( mMasterSoundVolume );
						//HandleError( mResult );
					}
				}
				//else 
				//{
					//SoundOff(actorInfo.mActorID, Channel_Rotation);
				//}
				mChannels[iActorID].mRotating = !mChannels[iActorID].mRotating;

			}


			if(actorInfo.mSubsetRotating != mChannels[iActorID].mSubRot)
			{
				if(actorInfo.mSubsetRotating == true)
				{
					int soundID = findRotationSound(actorInfo.mActorID, true);
					if(soundID != -1)
					{
						this->playSound( soundID, actorInfo.mActorID, Channel_Rotation);
						//mResult = mpSoundSystem->playSound((FMOD_CHANNELINDEX)((iActorID*3)+1), mSounds[soundID], false, &mChannels[iActorID].channels[Channel_Rotation]);
						//HandleError(mResult);
						//mResult = mChannels[iActorID].channels[1]->setVolume( mMasterSoundVolume );
						//HandleError( mResult );
						// FMOD_CHANNEL_FREE
					}
				}
				//else 
				//{
					//SoundOff(actorInfo.mActorID, Channel_Rotation);
				//}
				mChannels[iActorID].mSubRot = !mChannels[iActorID].mSubRot;

			}
			if(mChannels[iActorID].mRotating == false) //TODO: Find stop-rotation sound
			{
				
			}

			if(mChannels[iActorID].mRotating == false && mChannels[iActorID].mSubRot == false)
			{
				SoundOff(actorInfo.mActorID, Channel_Rotation);
			}


			if(actorInfo.mMoving != mChannels[iActorID].mMoving)
			{
				if(actorInfo.mMoving == true)
				{
					int soundID = findMovementSound(actorInfo.mActorID);
					if(soundID != -1)
					{
						this->playSound( soundID, actorInfo.mActorID, Channel_Movement);
						//mResult = mpSoundSystem->playSound((FMOD_CHANNELINDEX)((iActorID*3)), mSounds[soundID], false, &mChannels[iActorID].channels[Channel_Movement]);
						//HandleError(mResult);
					}
				}
				else 
				{
					if( actor->getActorType() == logic::Actor::ActorType_Tank)
					{
						if( dynamic_cast<logic::Tank*>(actor)->getTankType() == logic::Tank::TankType_LongRange ){
							//SoundOff(actorInfo.mActorID, 0);
							//mResult = mpSoundSystem->playSound((FMOD_CHANNELINDEX)((iActorID*3)), mSounds[sound::Sound_PlayerTankLongRangeMovementStop], false, &mChannels[iActorID].channels[Channel_Movement]);
							//HandleError(mResult);
							this->playSound(sound::Sound_PlayerTankLongRangeMovementStop, actorInfo.mActorID, Channel_Movement);
						}
						else if( dynamic_cast<logic::Tank*>(actor)->getTankType() == logic::Tank::TankType_CloseCombat ){
							//SoundOff(actorInfo.mActorID, Channel_Movement);
							//mResult = mpSoundSystem->playSound((FMOD_CHANNELINDEX)((iActorID*3)), mSounds[sound::Sound_PlayerTankCloseCombatMovementStop], false, &mChannels[iActorID].channels[Channel_Movement]);
							//HandleError(mResult);
							this->playSound(sound::Sound_PlayerTankCloseCombatMovementStop, actorInfo.mActorID, Channel_Movement);
						}
					}
					else
					{
						this->SoundOff(actorInfo.mActorID, Channel_Movement);
					}


				}

				mChannels[iActorID].mMoving = !mChannels[iActorID].mMoving;

			}

			if( actor->getActorType() == logic::Actor::ActorType_Tank && actorInfo.mFire != mChannels[iActorID].mFire )
			{
				if( dynamic_cast<logic::Tank*>(actor)->getWeapon()->getWeaponType() == logic::Weapon::WeaponTypes_Minigun )
				{
					if( actorInfo.mFire )
					{
						this->playSound(sound::Sound_WeaponMinigunFire, actorInfo.mActorID, Channel_Attack);
					}
					else
					{
						this->playSound(sound::Sound_WeaponMinigunEnd, actorInfo.mActorID, Channel_Attack);
					}
				}
				else if ( dynamic_cast<logic::Tank*>(actor)->getWeapon()->getWeaponType() == logic::Weapon::WeaponTypes_Flamethrower )
				{
					if( actorInfo.mFire )
					{
						this->playSound(sound::Sound_WeaponFlamethrowerFire, actorInfo.mActorID, Channel_Attack);
					}
					else
					{
						this->playSound(sound::Sound_WeaponFlamethrowerEnd, actorInfo.mActorID, Channel_Attack);
					}
				}

				mChannels[iActorID].mFire = !mChannels[iActorID].mFire;
			}
		}
	}
}

int SoundSystem::findMovementSound( logic::KeyType actorID )
{
	logic::Actor* pActor = mpActorHandler->getActor(actorID);

	if( pActor->getActorType() == logic::Tank::ActorType_Tank )  
	{
		logic::Tank::TankTypes type = dynamic_cast<logic::Tank*>(mpActorHandler->getActor(actorID))->getTankType();

		if (type == logic::Tank::TankType_LongRange)
		{
			return sound::Sound_PlayerTankLongRangeMovementMoving;
		}
		else //Close Range
		{
			return sound::Sound_PlayerTankCloseCombatMovementMoving;
		}
	}
	else if (pActor->getActorType() == logic::Enemy::ActorType_Enemy)
	{
		logic::Enemy* pEnemy = dynamic_cast<logic::Enemy*>(pActor);
		assert (pEnemy != NULL);

		if(pEnemy->getEnemySize() == logic::Enemy::Size_Small)
		{
			return sound::Sound_EnemyAntMovement;
		}
		if(pEnemy->getEnemySize() == logic::Enemy::Size_Medium)
		{
			return sound::Sound_EnemySpiderMovement;
		}
		if(pEnemy->getEnemySize() == logic::Enemy::Size_Large)
		{
			return sound::Sound_EnemyBeetleMovement;
		}
	}
	else
	{
		return sound::Sound_PedestrianMovement;
	}
	
	return -1;
}
int SoundSystem::findRotationSound( logic::KeyType actorID, bool turret )
{
	logic::Actor* pActor = mpActorHandler->getActor(actorID);

	if( pActor->getActorType() == logic::Tank::ActorType_Tank || turret == true ) 
	{
		logic::Tank::TankTypes type = dynamic_cast<logic::Tank*>(mpActorHandler->getActor(actorID))->getTankType();

		if (turret == true)
		{
			return sound::Sound_PlayerTankTurretRotation;
		}
		else if (type == logic::Tank::TankType_LongRange)
		{
			return sound::Sound_PlayerTankLongRangeMovementRotation;
		}
		else if(type == logic::Tank::TankType_CloseCombat) 
		{
			return sound::Sound_PlayerTankCloseCombatMovementRotation;
		}
		return sound::Sound_MenuError;
	}
	else if (pActor->getActorType() == logic::Enemy::ActorType_Enemy)
	{
		logic::Enemy* pEnemy = dynamic_cast<logic::Enemy*>(pActor);
		assert(pEnemy != NULL);

		if(pEnemy->getEnemySize() == logic::Enemy::Size_Small)
		{
			return sound::Sound_EnemyAntMovement;
		}
		if(pEnemy->getEnemySize() == logic::Enemy::Size_Medium)
		{
			return sound::Sound_EnemySpiderMovement;
		}
		if(pEnemy->getEnemySize() == logic::Enemy::Size_Large)
		{
			return sound::Sound_EnemyBeetleMovement;
		}
		return sound::Sound_MenuError;
	}

	return -1;
}

void SoundSystem::musicChanger(sound::Sounds sound)
{
	if( sound !=  mMusicLastPlayed )
	{
		this->playMusic( sound );
		mMusicLastPlayed = sound;
	}
}
