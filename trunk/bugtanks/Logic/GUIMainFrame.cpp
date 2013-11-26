#include "GUIMainFrame.h"

using namespace logic;

GUIMainFrame::GUIMainFrame(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
: gfx::GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	this->init();
}

GUIMainFrame::GUIMainFrame(gfx::GUIFrame *pParent)
: gfx::GUIImage(pParent)
{
	this->init();
}

GUIMainFrame::GUIMainFrame(const GUIMainFrame &mainframe)
: gfx::GUIImage(mainframe)
{
	this->init();
}
GUIMainFrame::~GUIMainFrame(void)
{

}

const GUIMainFrame& GUIMainFrame::operator=(const GUIMainFrame &mainframe)
{
	if(this != &mainframe)
	{
		gfx::GUIImage::operator =(mainframe);
	}
	return *this;
}
void GUIMainFrame::update(float deltaTime)
{
	gfx::GUIImage::update(deltaTime);

}
void GUIMainFrame::setFlagRender(bool render)
{
	GUIImage::setFlagRender(render);

}
void GUIMainFrame::onMouseEnter(long mX, long mY)
{
	GUIImage::onMouseEnter(mX, mY);

}
void GUIMainFrame::onMouseExit(long mX, long mY)
{
	GUIImage::onMouseExit(mX, mY);

}
void GUIMainFrame::onMouseMove(long mX, long mY)
{
	GUIImage::onMouseMove(mX, mY);

}
void GUIMainFrame::onMouseLeftDown(long mX, long mY)
{
	GUIImage::onMouseLeftDown(mX, mY);

}
void GUIMainFrame::onMouseMiddleDown(long mX, long mY)
{
	GUIImage::onMouseMiddleDown(mX, mY);

}
void GUIMainFrame::onMouseRightDown(long mX, long mY)
{
	GUIImage::onMouseRightDown(mX, mY);

}
void GUIMainFrame::onMouseLeftUp(long mX, long mY)
{
	GUIImage::onMouseLeftUp(mX, mY);

}
void GUIMainFrame::onMouseMiddleUp(long mX, long mY)
{
	GUIImage::onMouseMiddleUp(mX, mY);

}
void GUIMainFrame::onMouseRightUp(long mX, long mY)
{
	GUIImage::onMouseRightUp(mX, mY);

}
void GUIMainFrame::onKeyDown(int letterKeys, int keys)
{
	GUIImage::onKeyDown(letterKeys, keys);

}
void GUIMainFrame::addSubFrame(GUIFrame *pSubFrame)
{
	GUIImage::addSubFrame(pSubFrame);

}
void GUIMainFrame::init()
{
	this->setTextureID(-1);
	this->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	this->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	this->setRelativeSize(D3DXVECTOR2(1.0f, 1.0f));
}