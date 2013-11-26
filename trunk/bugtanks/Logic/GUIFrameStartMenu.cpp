#include "GUIFrameStartMenu.h"

using namespace logic;

GUIFrameStartMenu::GUIFrameStartMenu(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
		: gfx::GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	init();
}

GUIFrameStartMenu::GUIFrameStartMenu(gfx::GUIFrame *pParent) : gfx::GUIImage(pParent)
{
	init();
}

GUIFrameStartMenu::GUIFrameStartMenu(const logic::GUIFrameStartMenu &frame) : gfx::GUIImage(frame)
{
	init();
}

GUIFrameStartMenu::~GUIFrameStartMenu(void)
{

}

const GUIFrameStartMenu& GUIFrameStartMenu::operator =(const logic::GUIFrameStartMenu &frame)
{
	if(this != &frame)
	{
		gfx::GUIImage::operator =(frame);
	}
	return *this;
}

void GUIFrameStartMenu::update(float deltaTime)
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
		//getRenderInstance()->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	}
	else
	{
		//getRenderInstance()->setColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}
}

void GUIFrameStartMenu::onMouseEnter(long mX, long mY)
{
	gfx::GUIImage::onMouseEnter(mX, mY);
	getRenderInstance()->setColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 0.0f));
}

void GUIFrameStartMenu::onMouseExit(long mX, long mY)
{
	gfx::GUIImage::onMouseExit(mX, mY);
	getRenderInstance()->setColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
}

void GUIFrameStartMenu::onMouseMove(long mX, long mY)
{
	//gfx::GUIImage::onMouseMove(mX, mY);
}

void GUIFrameStartMenu::onMouseLeftDown(long mX, long mY)
{
	gfx::GUIImage::onMouseLeftDown(mX, mY);
}

void GUIFrameStartMenu::onMouseMiddleDown(long mX, long mY)
{
	gfx::GUIImage::onMouseMiddleDown(mX, mY);
}

void GUIFrameStartMenu::onMouseRightDown(long mX, long mY)
{
	gfx::GUIImage::onMouseRightDown(mX, mY);
}

void GUIFrameStartMenu::onMouseLeftUp(long mX, long mY)
{
	gfx::GUIImage::onMouseLeftUp(mX, mY);
}

void GUIFrameStartMenu::onMouseMiddleUp(long mX, long mY)
{
	gfx::GUIImage::onMouseMiddleUp(mX, mY);
}

void GUIFrameStartMenu::onMouseRightUp(long mX, long mY)
{
	gfx::GUIImage::onMouseRightUp(mX, mY);
}

void GUIFrameStartMenu::onKeyDown(int letterKeys, int keys)
{
	gfx::GUIImage::onKeyDown(letterKeys, keys);
}

void GUIFrameStartMenu::init()
{
	getRenderInstance()->setTextureID(-1);
	getRenderInstance()->setColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));

	mpSubFrame = myNew GUISubFrameStartMenu(this);
	mpSubFrame->setRelativePosition(D3DXVECTOR2(0.5f, 0.5f));
	mpSubFrame->setRelativeSize(D3DXVECTOR2(0.1f, 0.5f));
	addSubFrame(mpSubFrame);
}

void GUIFrameStartMenu::setFlagRender(bool render)
{
	GUIImage::setFlagRender(render);

	mpSubFrame->setFlagRender(render);
}