#include "GUI.h"

GUI::GUI(ID3D10Device* device)
{
	HRESULT hr;

	hr = D3DX10CreateSprite(device, 512, &mFontSprite);
	if(FAILED(hr))
	{
		MessageBox(0, "Failed to create Font sprite", "Sprite Creation error", MB_OK);
		PostQuitMessage( 0 );
	}

	mFPS		= 0.0f;

	hr = D3DX10CreateFont(
		device,
		20,
		0,
		FW_NORMAL,
		1,
		false,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		"Rockwell",
		&mFont);

	if(FAILED(hr))
	{
		MessageBox(0, "Failed to create font..", 0, 0);
		PostQuitMessage(0);
	}
}

GUI::~GUI()
{
	ReleaseCOM(mFontSprite);
}

void GUI::Update(const float dt)
{
	//////// Update FPS ////////
	static float numFrames		= 0.0f;
	static float timeElapsed	= 0.0f;

	numFrames   += 1.0f;
	timeElapsed += dt;

	if( timeElapsed >= 1.0f )
	{
		mFPS = ((float)numFrames / (float)timeElapsed);

		timeElapsed = 0.0f;
		numFrames	= 0.0f;
	}
	////////////////////////////
}

void GUI::showFPS()
{
	const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
	RECT r = {5, 5, 0, 0};

	char str[100];
	sprintf_s(str, "FPS: %0.2f", mFPS);

	mFontSprite->Begin(D3DX10_SPRITE_SORT_TEXTURE);
	mFont->DrawText(mFontSprite, str, -1, &r, DT_NOCLIP, WHITE);
	mFontSprite->End();
}

void GUI::print(char* text, int x, int y)
{
	const D3DXCOLOR WHITE(1.0f, 1.0f, 1.0f, 1.0f);
	RECT r = {x, y, 0, 0};

	mFontSprite->Begin(D3DX10_SPRITE_SORT_TEXTURE);
	mFont->DrawText(mFontSprite, (LPCSTR)text, -1, &r, DT_NOCLIP, WHITE);
	mFontSprite->End();
}