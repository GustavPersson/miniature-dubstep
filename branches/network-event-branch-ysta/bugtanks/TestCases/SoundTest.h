
#include <cxxtest/TestSuite.h>
#include "../Utilities/Macros.h"
#include "../Sound/SoundSystem.h"
#include "../Sound/Sound.h"
#include "windows.h"


using namespace sound;

class SoundTestCase : public CxxTest::TestSuite
{
public:

	void setUp()
	{
		//tSound = new SoundSystem();
	}

	void tearDown()
	{
		//SAFE_DELETE( tSound );
	}

	void testSound()
	{
		//tSound->loadGameSounds();
		
		// 3D-sound test START
		/*
		tSound->playSoundOnce(0, -2.0f, 0.0f, 0.0f);
		while(tSound->isPlaying(1))
		{
			tSound->Update(0.0f, 0.0f, 0.0f, 0.0f);
		}

		tSound->playSoundOnce(0, 2.0f, 0.0f, 0.0f);
		while(tSound->isPlaying(1))
		{
			tSound->Update(0.0f, 0.0f, 0.0f, 0.0f);
		}
		*/
		// 3D-sound test END

		// Music test START
		/*
		tSound->playMusic(1);
		while(tSound->isPlaying(2))
		{
			tSound->Update(0.0f, 0.0f, 0.0f, 0.0f);
		}

		*/
		// Music test END

		// Channel-handler test START
		//int pew = tSound->createChannel( -2.0f, 0.0f, 0.0f );
		//int pew2 = tSound->createChannel( 2.0f, 0.0f, 0.0f );
		//tSound->stopChannel( pew2 );
		//tSound->updateChannel( 2, 1.0f, 1.0f, 1.0f );




		//TS_ASSERT_EQUALS( 1, 1);
	}

private:
	//SoundSystem *tSound;
}; 