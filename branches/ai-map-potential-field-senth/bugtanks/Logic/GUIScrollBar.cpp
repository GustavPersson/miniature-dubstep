#include "../Utilities/Macros.h"
#include "GUIScrollBar.h"



using namespace logic;


GUIScrollBar::GUIScrollBar(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
: GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	this->init();
}

GUIScrollBar::GUIScrollBar(gfx::GUIFrame *pParent) : GUIImage(pParent)
{
	this->init();
}
GUIScrollBar::GUIScrollBar(const GUIScrollBar &scrollbar) : GUIImage(scrollbar)
{
	this->init();
}
GUIScrollBar::~GUIScrollBar(void)
{

}
const GUIScrollBar& GUIScrollBar::operator=(const GUIScrollBar &scrollbar)
{
	if(this != &scrollbar)
	{
		this->mNumberOfIndices = scrollbar.mNumberOfIndices;
		this->mCurrentIndexPosition = scrollbar.mCurrentIndexPosition;
	}
	return *this;
}
void GUIScrollBar::update(float deltaTime)
{
	gfx::GUIImage::update(deltaTime);
	if(getFlagUpdateRequired())
	{
		float aspectratio = (float)this->getAbsoluteSizeX()/(float)this->getAbsoluteSizeY();
		float mButtonWidth = aspectratio;// this->getRelativePosition().y*aspectratio;
	
		// update position and size for the buttons.
		this->mButtonUp->setRelativeSize(D3DXVECTOR2(1.0f, mButtonWidth));
		this->mButtonDown->setRelativeSize(D3DXVECTOR2(1.0f, mButtonWidth));

		this->mButtonDown->setRelativePosition(D3DXVECTOR2(0.0f,1.0f-mButtonWidth));

		gfx::GUIImage::update(deltaTime);
		// update the slidebar.
		if(this->mHoldSlideBar)
		{
			float barY = (float)this->getAbsoluteY() + (float)this->mButtonUp->getAbsoluteSizeY();
			float barSizeY = (float)this->getAbsoluteSizeY() - (float)this->mButtonDown->getAbsoluteSizeY()*2;
			
			float localY = (this->mDragPoint.y - barY)/barSizeY;
			float deltaY = localY - this->mRefPoint.y;
			this->mCurrentIndexPosition = (mOldSlideBarPos.y + deltaY)*this->mNumberOfIndices;
			this->mCurrentIndexPosition = max(this->mCurrentIndexPosition, 0);
			this->mCurrentIndexPosition = min(this->mCurrentIndexPosition, this->mNumberOfIndices);
		}


		this->mLengthOfSlide = 1 - (mButtonWidth+mButtonWidth);
		float slideBarLen = this->mLengthOfSlide/(this->mNumberOfIndices+1);
		float slideBarPos = mButtonWidth + this->mCurrentIndexPosition*slideBarLen;

		//mLengthOfSlide = this->mButtonDown->getRelativePosition().y;
		//float indexSize = this->mLengthOfSlide/this->mNumberOfIndices;
		this->mSlideBar->setRelativeSize(D3DXVECTOR2(1.0f, slideBarLen));
		this->mSlideBar->setRelativePosition(D3DXVECTOR2(0.0f, slideBarPos));
	}
}
void GUIScrollBar::setFlagRender(bool render)
{
	GUIImage::setFlagRender(render);
}
void GUIScrollBar::onMouseEnter(long mX, long mY)
{
	GUIImage::onMouseEnter(mX, mY);

}
void GUIScrollBar::onMouseExit(long mX, long mY)
{
	GUIImage::onMouseExit(mX, mY);

}
void GUIScrollBar::onMouseMove(long mX, long mY)
{
	GUIImage::onMouseMove(mX, mY);


	if(this->mHoldSlideBar)
	{
		mDragPoint.x = (float)mX;
		mDragPoint.y = (float)mY;
		this->updateRequired();
	}
	this->setMouseOmniFocus(true);
}
void GUIScrollBar::onMouseLeftDown(long mX, long mY)
{
	GUIImage::onMouseLeftDown(mX, mY);


	mOldSlideBarPos = this->mSlideBar->getRelativePosition();
	// get the local position of the mouse and store it as a refference point.
	this->mRefPoint.x = (mX - (float)this->getAbsoluteX()) / (float)this->getAbsoluteSizeX();
	this->mRefPoint.y = (mY - (float)this->getAbsoluteY()) / (float)this->getAbsoluteSizeY();
}
void GUIScrollBar::onMouseMiddleDown(long mX, long mY)
{
	GUIImage::onMouseMiddleDown(mX, mY);

}
void GUIScrollBar::onMouseRightDown(long mX, long mY)
{
	GUIImage::onMouseRightDown(mX, mY);

}
void GUIScrollBar::onMouseLeftUp(long mX, long mY)
{
	GUIImage::onMouseLeftUp(mX, mY);
	this->mHoldSlideBar = false;

}
void GUIScrollBar::onMouseMiddleUp(long mX, long mY)
{
	GUIImage::onMouseMiddleUp(mX, mY);

}
void GUIScrollBar::onMouseRightUp(long mX, long mY)
{
	GUIImage::onMouseRightUp(mX, mY);

}
void GUIScrollBar::onKeyDown(int letterKeys, int keys)
{
	GUIImage::onKeyDown(letterKeys, keys);

}

void GUIScrollBar::setNumberOfIndices(int indices)
{
	this->mNumberOfIndices = indices;
	this->updateRequired();
}
int GUIScrollBar::getNumberOfIndices()const
{
	return this->mNumberOfIndices;
}
void GUIScrollBar::setCurrentIndexPosition(float indexPosition)
{
	this->mCurrentIndexPosition = indexPosition;
	this->updateRequired();
}
float GUIScrollBar::getCurrentIndexPosition()const
{
	return this->mCurrentIndexPosition;
}

void GUIScrollBar::notify(int message, GUIFrame *pSender)
{
	float val = this->mCurrentIndexPosition;
	if(message == BasicMessage_PressLeft)
	{
		if(pSender == this->mButtonDown)
		{
			this->setCurrentIndexPosition(min(this->mCurrentIndexPosition+1, this->mNumberOfIndices));
		}
		else if(pSender == this->mButtonUp)
		{
			this->setCurrentIndexPosition(max(this->mCurrentIndexPosition-1, 0));
		}
		else if(pSender == this->mSlideBar)
		{
			this->mHoldSlideBar = true;
		}
	}
	if(val != this->mCurrentIndexPosition)
	{
		DEBUG_MESSAGE(7, "current index = " << this->mCurrentIndexPosition);
	}
}
void GUIScrollBar::init()
{
	this->setTextureID(-1);
	this->setColor(D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));

	this->mNumberOfIndices=0;
	this->mCurrentIndexPosition=0.0f;

	this->setRelativeSize(D3DXVECTOR2(0.02f,0.3f));

	// Setup the subframes;
	this->mButtonUp = new GUIButton(this);
	this->mButtonDown = new GUIButton(this);
	this->mSlideBar = new GUIButton(this);

	int arrayIndex = 0;
	int textureID = 0;
	gfx::DX10Module::getInstance()->getTextureIndex("ScrollUp_Default", arrayIndex, textureID);
	this->mButtonUp->setTextureIndexOffset(textureID);
	this->mButtonUp->getRenderInstance()->setTextureArrayIndex(arrayIndex);

	gfx::DX10Module::getInstance()->getTextureIndex("ScrollDown_Default", arrayIndex, textureID);
	this->mButtonDown->setTextureIndexOffset(textureID);
	this->mButtonUp->getRenderInstance()->setTextureArrayIndex(arrayIndex);

	gfx::DX10Module::getInstance()->getTextureIndex("ScrollBar_Default", arrayIndex, textureID);
	this->mSlideBar->setTextureIndexOffset(textureID);
	this->mSlideBar->getRenderInstance()->setTextureArrayIndex(arrayIndex);

	long cW = gfx::DX10Module::getInstance()->getClientWidth(),
		cH = gfx::DX10Module::getInstance()->getClientHeight();

	float aspectratio = (float)cW/(float)cH;

	//set button sizes.
	mButtonWidth = (this->getRelativeSize().x/this->getRelativeSize().y)*aspectratio;

	mLengthOfSlide = this->getRelativeSize().y - (mButtonWidth+mButtonWidth);

	this->mButtonUp->setRelativeSize(D3DXVECTOR2(1.0f, mButtonWidth));
	this->mButtonDown->setRelativeSize(D3DXVECTOR2(1.0f, mButtonWidth));

	//set button positions.
	this->mButtonUp->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	this->mButtonDown->setRelativePosition(D3DXVECTOR2(0.0f,1.0f-mButtonWidth));

	this->addSubFrame(this->mButtonUp);
	this->addSubFrame(this->mButtonDown);
	this->addSubFrame(this->mSlideBar);
	this->updateRequired();
}
