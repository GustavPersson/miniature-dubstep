#pragma once

#include <string>
#include "DX10Util.h"

class GUI
{
private:
	float			mFPS;			// FPS
	ID3DX10Font*	mFont;			// Font used
	ID3DX10Sprite*  mFontSprite;	// Pointer for rendering

public:
	GUI(ID3D10Device* device);
	~GUI();

	void Update(const float dt);

	void showFPS();
	void print(char* text, int x, int y);
};