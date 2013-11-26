/**
 * Note: Not used/finished. Might be removed.
 */

#ifndef LIGHT_H
#define LIGHT_H

#include "Common.h"

namespace gfx
{

struct LightStruct
{
	LightStruct()
	{
		ZeroMemory(this, sizeof(LightStruct));
	}

	D3DXVECTOR3 pos;
	float		pad1;

	D3DXVECTOR3 dir;
	float		pad2;

	D3DXCOLOR ambient;
	D3DXCOLOR diffuse;
	D3DXCOLOR specular;

	float power;  // Specular power
};

class Light
{
public:

	/**
	 * Default constructor
	 */
	Light();

	/**
	 * Constructor
	 * @param pos   Light position
	 * @param dir   Light direction
	 * @param amb   Ambient
	 * @param diff  Diffuse
	 * @param spec  Specular
	 * @param power Power of ambient, diffuse and specular lighting
	 */
	Light(D3DXVECTOR3 pos, D3DXVECTOR3 dir, D3DXCOLOR amb, D3DXCOLOR diff, D3DXCOLOR spec, float power);

	/**
	 * Destructor.
	 */
	~Light();

	/**
	 * Sets light position
	 * @param pos Position
	 */
	void setLightPos(D3DXVECTOR3 pos);

	/**
	 * Sets light direction
	 * @param dir Direction
	 */
	void setLightDir(D3DXVECTOR3 dir);

	/**
	 * Sets ambient lighting
	 * @param amb Ambient color
	 */
	void setAmbientLight(D3DXCOLOR amb);

	/**
	 * Sets diffuse lighting
	 * @param diff Diffuse color
	 */
	void setDiffuseLight(D3DXCOLOR diff);

	/**
	 * Sets specular lighting
	 * @param spec Specular color
	 */
	void setSpecularLight(D3DXCOLOR spec);

	/**
	 * Sets lighting intensity
	 * @param power Value representing ambient, diffuse and specular lightpower
	 */
	void setLightPower(float power);

	/**
	 * Returns light position
	 */
	D3DXVECTOR3 getLightPos();

	/**
	 * Returns light direction
	 */
	D3DXVECTOR3 getLightDir();

	/**
	 * Returns ambient lighting
	 */
	D3DXCOLOR getAmbientLight();

	/**
	 * Returns diffuse lighting
	 */
	D3DXCOLOR getDiffuseLight();

	/**
	 * Returns specular lighting
	 */
	D3DXCOLOR getSpecularLight();

	/**
	 * Returns lighting intensity
	 */
	float getLightPower();

private:
	LightStruct mLight;
};

}
#endif