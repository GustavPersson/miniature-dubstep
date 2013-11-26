#include "LightHandler.h"

using namespace gfx;

LightHandler::LightHandler()
{

}

LightHandler::~LightHandler()
{
}

void LightHandler::addLightSource(D3DXVECTOR3 pos, D3DXVECTOR3 dir)
{
	// Create pointer to a Light struct
	Light *pLight = myNew Light();
	pLight->setLightPos(pos);
	pLight->setLightDir(dir);

	// Save pointer in vector
	mLightSource.push_back(pLight);
}

void LightHandler::removeLightSource(unsigned int i)
{
	i;
	// Remove last element from memory
	SAFE_DELETE(mLightSource.back());

	// Remove last element from vector
	mLightSource.pop_back();
}

Light* LightHandler::getLightSource(unsigned int i)
{
	if(i < mLightSource.size())
		return mLightSource.at(i);
	else
		DEBUG_MESSAGE(LEVEL_LOW, "Position out of range in vector (LightHandler::getLightSource(i)");

	return NULL;
}