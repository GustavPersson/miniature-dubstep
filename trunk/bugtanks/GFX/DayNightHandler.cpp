#include "DayNightHandler.h"

using namespace gfx;

DayNightHandler::DayNightHandler(float time)
{
	mTime = time;
	mNight = false;
}

DayNightHandler::~DayNightHandler()
{

}

void DayNightHandler::setTime(float dt)
{
	mTime += 0.1f * dt;

	if(mTime > (float)D3DX_PI)
	{
		mTime -= (float)D3DX_PI;

		if(mNight)
		{
			mNight = false;
		}
		else
		{
			mNight = true;
		}
	}
}

D3DXVECTOR3 DayNightHandler::getLightDirection()
{
	float x, z;

	if(mNight)
	{
		z = -sinf(mTime);
		x = -cosf(mTime);
	}
	else
	{
		z = -sinf(mTime + (float)D3DX_PI);
		x = -cosf(mTime + (float)D3DX_PI);
	}

	return D3DXVECTOR3(0.5f * x, -1.0f, 0.5f * z);
}

D3DXVECTOR3 DayNightHandler::getLightPosition()
{
	float x, z;

	if(mNight)
	{
		z = sinf(mTime);
		x = cosf(mTime);
	}
	else
	{
		z = sinf(mTime + (float)D3DX_PI);
		x = cosf(mTime + (float)D3DX_PI);
	}

	return D3DXVECTOR3(100.0f * x - 50.0f, 450.0f, 150.0f * z - 75.0f);
}

D3DXCOLOR DayNightHandler::getAmbientColor()
{
	if(mNight)
	{
		float rg = 0.0f;

		if(mTime <= 0.5f * D3DX_PI)
		{
			rg = 0.5f * ((float)D3DX_PI - mTime) / (0.5f * (float)D3DX_PI) - 0.5f;
		}
		else
		{
			rg = 0.5f * mTime / (0.5f * (float)D3DX_PI) - 0.5f;
		}

		return D3DXCOLOR(0.7f * rg, 0.7f * rg, 0.7f, 1.0f);
	}
	else
	{
		float rg = 0.0f;

		if(mTime <= 0.5f * D3DX_PI)
		{
			rg = 0.5f * mTime / (0.5f * (float)D3DX_PI) + 0.5f;
		}
		else
		{
			rg = 0.5f * ((float)D3DX_PI - mTime) / (0.5f * (float)D3DX_PI) + 0.5f;
		}

		return D3DXCOLOR(0.7f * rg, 0.7f * rg, 0.7f, 1.0f);
	}
}