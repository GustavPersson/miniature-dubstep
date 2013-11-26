#include "GUISubFrameTest.h"

using namespace gfx;

GUISubFrameTest::GUISubFrameTest(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	init();
}

GUISubFrameTest::GUISubFrameTest(GUIFrame *pParent) : GUIImage(pParent)
{
	init();
}

GUISubFrameTest::GUISubFrameTest(const GUISubFrameTest &frame) : GUIImage(frame)
{
	init();
}

GUISubFrameTest::~GUISubFrameTest()
{

}

const GUISubFrameTest &GUISubFrameTest::operator=(const GUISubFrameTest &frame)
{
	if(this != &frame)
	{
		GUIImage::operator=(frame);
	}
	return *this;
}

void GUISubFrameTest::update(float deltaTime)
{
	GUIImage::update(deltaTime);
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
		getRenderInstance()->setColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
	}
}

void GUISubFrameTest::onMouseEnter(long mX, long mY)
{
	GUIImage::onMouseEnter(mX, mY);
}

void GUISubFrameTest::onMouseExit(long mX, long mY)
{
	GUIImage::onMouseExit(mX, mY);
}

void GUISubFrameTest::onMouseMove(long mX, long mY)
{
	GUIImage::onMouseMove(mX, mY);
}

void GUISubFrameTest::onMouseLeftDown(long mX, long mY)
{
	GUIImage::onMouseLeftDown(mX, mY);
}

void GUISubFrameTest::onMouseMiddleDown(long mX, long mY)
{
	GUIImage::onMouseMiddleDown(mX, mY);
}

void GUISubFrameTest::onMouseRightDown(long mX, long mY)
{
	GUIImage::onMouseRightDown(mX, mY);
}

void GUISubFrameTest::onMouseLeftUp(long mX, long mY)
{
	GUIImage::onMouseLeftUp(mX, mY);
}

void GUISubFrameTest::onMouseMiddleUp(long mX, long mY)
{
	GUIImage::onMouseMiddleUp(mX, mY);
}

void GUISubFrameTest::onMouseRightUp(long mX, long mY)
{
	GUIImage::onMouseRightUp(mX, mY);
}

void GUISubFrameTest::onKeyDown(int letterKeys, int keys)
{
	GUIImage::onKeyDown(letterKeys, keys);
}

void GUISubFrameTest::init()
{
	getRenderInstance()->setTextureID(-1);
	getRenderInstance()->setColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));

	GUIAnimatedImage *image = myNew GUIAnimatedImage(this);
	image->setRelativePosition(D3DXVECTOR2(0.1f, 0.1f));
	image->setRelativeSize(D3DXVECTOR2(0.8f, 0.8f));
	addSubFrame(image);
}