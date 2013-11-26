#include "GUIFrameLoadingScreen.h"
#include "../GFX/GUIText.h"

using namespace logic;

GUIFrameLoadingScreen::GUIFrameLoadingScreen(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
		: gfx::GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	init();
}

GUIFrameLoadingScreen::GUIFrameLoadingScreen(gfx::GUIFrame *pParent) : gfx::GUIImage(pParent)
{
	init();
}

GUIFrameLoadingScreen::GUIFrameLoadingScreen(const logic::GUIFrameLoadingScreen &frame) : gfx::GUIImage(frame)
{
	init();
}

GUIFrameLoadingScreen::~GUIFrameLoadingScreen(void)
{

}

const GUIFrameLoadingScreen& GUIFrameLoadingScreen::operator =(const logic::GUIFrameLoadingScreen &frame)
{
	if(this != &frame)
	{
		gfx::GUIImage::operator =(frame);
	}
	return *this;
}

void GUIFrameLoadingScreen::update(float deltaTime)
{
	gfx::GUIImage::update(deltaTime);
	if(getFlagMouseFocus())
	{
		if(getFlagMouseLeftDown())
		{
			increaseRelativeSize(D3DXVECTOR2(0.1f * deltaTime, 0.1f * deltaTime));
		}
		if(getFlagMouseMiddleDown())
		{
			increaseRelativePosition(D3DXVECTOR2(0.1f * deltaTime, 0.0f));
		}
		if(getFlagMouseRightDown())
		{
			increaseRelativeSize(D3DXVECTOR2(-0.1f * deltaTime, -0.1f * deltaTime));
		}
		getRenderInstance()->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		getRenderInstance()->setColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}
}

void GUIFrameLoadingScreen::onMouseEnter(long mX, long mY)
{
	gfx::GUIImage::onMouseEnter(mX, mY);
}

void GUIFrameLoadingScreen::onMouseExit(long mX, long mY)
{
	gfx::GUIImage::onMouseExit(mX, mY);
}

void GUIFrameLoadingScreen::onMouseMove(long mX, long mY)
{
	gfx::GUIImage::onMouseMove(mX, mY);
}

void GUIFrameLoadingScreen::onMouseLeftDown(long mX, long mY)
{
	gfx::GUIImage::onMouseLeftDown(mX, mY);
}

void GUIFrameLoadingScreen::onMouseMiddleDown(long mX, long mY)
{
	gfx::GUIImage::onMouseMiddleDown(mX, mY);
}

void GUIFrameLoadingScreen::onMouseRightDown(long mX, long mY)
{
	gfx::GUIImage::onMouseRightDown(mX, mY);
}

void GUIFrameLoadingScreen::onMouseLeftUp(long mX, long mY)
{
	gfx::GUIImage::onMouseLeftUp(mX, mY);
}

void GUIFrameLoadingScreen::onMouseMiddleUp(long mX, long mY)
{
	gfx::GUIImage::onMouseMiddleUp(mX, mY);
}

void GUIFrameLoadingScreen::onMouseRightUp(long mX, long mY)
{
	gfx::GUIImage::onMouseRightUp(mX, mY);
}

void GUIFrameLoadingScreen::onKeyDown(int letterKeys, int keys)
{
	gfx::GUIImage::onKeyDown(letterKeys, keys);
}

void GUIFrameLoadingScreen::init()
{
	getRenderInstance()->setTextureID(-1);
	getRenderInstance()->setColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	gfx::GUIText *text = myNew gfx::GUIText(this);
	text->setRelativePosition(D3DXVECTOR2(0.1f, 0.2f));
	text->setRelativeSize(D3DXVECTOR2(0.8f, 0.5f));
	text->setText("Loading...");
	text->setTextFormat(gfx::GUIText::TextFormat_FixedRows);
	text->setTextSize(D3DXVECTOR2(10.0f, 15.0f));
	addSubFrame(text);
}