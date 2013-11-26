#include "GUIFrameTest.h"

using namespace gfx;

GUIFrameTest::GUIFrameTest(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
	: GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	init();
}

GUIFrameTest::GUIFrameTest(GUIFrame *pParent) : GUIImage(pParent)
{
	init();
}

GUIFrameTest::GUIFrameTest(const GUIFrameTest &frame) : GUIImage(frame)
{
	init();
}


GUIFrameTest::~GUIFrameTest()
{

}

const GUIFrameTest &GUIFrameTest::operator=(const GUIFrameTest &frame)
{
	if(this != &frame)
	{
		GUIImage::operator=(frame);
	}
	return *this;
}

void GUIFrameTest::update(float deltaTime)
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
		getRenderInstance()->setColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
	}
}

void GUIFrameTest::onMouseEnter(long mX, long mY)
{
	GUIImage::onMouseEnter(mX, mY);
}

void GUIFrameTest::onMouseExit(long mX, long mY)
{
	GUIImage::onMouseExit(mX, mY);
}

void GUIFrameTest::onMouseMove(long mX, long mY)
{
	GUIImage::onMouseMove(mX, mY);
}

void GUIFrameTest::onMouseLeftDown(long mX, long mY)
{
	GUIImage::onMouseLeftDown(mX, mY);
}

void GUIFrameTest::onMouseMiddleDown(long mX, long mY)
{
	GUIImage::onMouseMiddleDown(mX, mY);
}

void GUIFrameTest::onMouseRightDown(long mX, long mY)
{
	GUIImage::onMouseRightDown(mX, mY);
}

void GUIFrameTest::onMouseLeftUp(long mX, long mY)
{
	GUIImage::onMouseLeftUp(mX, mY);
}

void GUIFrameTest::onMouseMiddleUp(long mX, long mY)
{
	GUIImage::onMouseMiddleUp(mX, mY);
}

void GUIFrameTest::onMouseRightUp(long mX, long mY)
{
	GUIImage::onMouseRightUp(mX, mY);
}

void GUIFrameTest::onKeyDown(int letterKeys, int keys)
{
	GUIImage::onKeyDown(letterKeys, keys);
}

void GUIFrameTest::init()
{
	getRenderInstance()->setTextureID(-1);
	getRenderInstance()->setColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.0f));

	GUISubFrameTest *sub = myNew GUISubFrameTest(this);
	sub->setRelativePosition(D3DXVECTOR2(0.0f, 0.1f));
	sub->setRelativeSize(D3DXVECTOR2(0.1f, 0.5f));
	addSubFrame(sub);

	GUISubFrameTest *sub2 = myNew GUISubFrameTest(this);
	sub2->setRelativePosition(D3DXVECTOR2(0.9f, 0.1f));
	sub2->setRelativeSize(D3DXVECTOR2(0.1f, 0.5f));
	addSubFrame(sub2);

	GUIText *text = myNew GUIText(this);
	text->setRelativePosition(D3DXVECTOR2(0.1f, 0.2f));
	text->setRelativeSize(D3DXVECTOR2(0.8f, 0.5f));
	text->setText("");
	text->setTextFormat(GUIText::TextFormat_FixedRows);
	text->setTextSize(D3DXVECTOR2(12.0f, 20.0f));
	addSubFrame(text);
}