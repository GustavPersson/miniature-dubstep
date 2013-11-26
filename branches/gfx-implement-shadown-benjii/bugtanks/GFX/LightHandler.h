/**
 * Note: Not used/finished. Might be removed.
 */

#ifndef LIGHTHANDLER_H
#define LIGHTHANDLER_H

#include "Light.h"
#include <vector>

namespace gfx
{

class LightHandler
{
public:
	/**
 * Constructor
 */
	LightHandler();

	/**
	 * Destructor.
	 */
	~LightHandler();

	/**
	 * Adds light source to handler
	 * @param pos   Light position
	 * @param dir   Light direction
	 */
	void addLightSource(D3DXVECTOR3 pos, D3DXVECTOR3 dir);

	/**
	 * Removes light source to handler
	 * @param i Light source index
	 */
	void removeLightSource(unsigned int i);

	/**
	 * Returns pointer to Light instance
	 * @param i Light source index
	 */
	Light* getLightSource(unsigned int i);

	
private:
	std::vector<Light*> mLightSource;
};

}
#endif