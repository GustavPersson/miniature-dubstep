#include "GUIButton.h"

using namespace std;
using namespace logic;


GUIButton::GUIButton(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
: GUIComboFrame(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	this->init();
}

GUIButton::GUIButton(gfx::GUIFrame *pParent) : GUIComboFrame(pParent)
{
	this->init();
}
GUIButton::GUIButton(const GUIButton &button) : GUIComboFrame(button)
{
	this->init();
	*this = button;
}
GUIButton::~GUIButton(void)
{

}
const GUIButton& GUIButton::operator=(const GUIButton &button)
{
	if(this != &button)
	{
		gfx::GUIComboFrame::operator=(button);
		this->mState = button.mState;
		this->mSwitchON = button.mSwitchON;
	}
	return *this;
}
void GUIButton::update(float deltaTime)
{
	gfx::GUIComboFrame::update(deltaTime);

	if(this->mTextEnable && this->getRenderInstance()->getFlagRender())
	{
		this->getTextFrame()->setFlagVisible(true);
	}
	else
	{
		this->getTextFrame()->setFlagVisible(false);
	}

	if(this->getFlagUpdateRequired())
	{

		if(this->mSwitchON)
		{
			if(this->mState == GUIB_MOUSE_OVER || this->mState == GUIB_MOUSE_OVER_AND_PRESS)
			{
				this->mState = GUIB_MOUSE_OVER_AND_PRESS;
			}
			else
			{
				this->mState = GUIB_PRESSED;
			}
		}

		switch(this->mState)
		{
		case GUIB_DEFAULT:

			#ifdef USE_BUTTON_COLOR
				this->setColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
			#else
				this->setTextureID(mTexIndexOffset + this->mState);
			#endif

			break;
		case GUIB_MOUSE_OVER:

			#ifdef USE_BUTTON_COLOR
				this->setColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
			#else
				this->setTextureID(mTexIndexOffset + this->mState);
			#endif

			break;
		case GUIB_MOUSE_OVER_AND_PRESS:

			#ifdef USE_BUTTON_COLOR
				this->setColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
			#else
				this->setTextureID(mTexIndexOffset + this->mState);
			#endif

			break;
		case GUIB_PRESSED:

			#ifdef USE_BUTTON_COLOR
				this->setColor(D3DXCOLOR(0.3f, 0.0f, 0.0f, 1.0f));
			#else
				this->setTextureID(mTexIndexOffset + this->mState);
			#endif

			break;
		}

		D3DXVECTOR2 textSize = this->getTextFrame()->getRelativeSize();
		
		switch(this->mTextAlignment)
		{
		case GUI_Alignment_Center:
			this->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.5f-textSize.x/2, 0.5f-textSize.y/2));
			break;

		case GUI_Alignment_CenterLeft:
			this->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.0f, 0.5f-textSize.y/2));
			break;

		case GUI_Alignment_CenterRight:
			this->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.0f-textSize.x, 0.5f-textSize.y/2));
			break;

		case GUI_Alignment_TopCenter:
			this->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.5f-textSize.x/2, 0.0f));
			break;

		case GUI_Alignment_TopLeft:
			this->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
			break;

		case GUI_Alignment_TopRight:
			this->getTextFrame()->setRelativePosition(D3DXVECTOR2(1.0f-textSize.x, 0.0f));
			break;

		case GUI_Alignment_BottomCenter:
			this->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.5f-textSize.x/2, 1.0f-textSize.y));
			break;

		case GUI_Alignment_BottomLeft:
			this->getTextFrame()->setRelativePosition(D3DXVECTOR2(0.0f, 1.0f-textSize.y));
			break;

		case GUI_Alignment_BottomRight:
			this->getTextFrame()->setRelativePosition(D3DXVECTOR2(1.0f-textSize.x, 1.0f-textSize.y));
			break;
		};

	}

}
void GUIButton::setFlagRender(bool render)
{
	GUIComboFrame::setFlagRender(render);

}
void GUIButton::onMouseEnter(long mX, long mY)
{
	GUIComboFrame::onMouseEnter(mX, mY);

	this->mState = GUIB_MOUSE_OVER;
	this->updateRequired();

	list<GUIButtonListener*>::iterator listener;
	for(listener = this->mButtonListeners.begin(); listener != this->mButtonListeners.end(); listener++)
	{
		(*listener)->onMouseEnter(this, mX, mY);
	}

}
void GUIButton::onMouseExit(long mX, long mY)
{
	GUIComboFrame::onMouseExit(mX, mY);
	this->mState = GUIB_DEFAULT;
	this->updateRequired();

	list<GUIButtonListener*>::iterator listener;
	for(listener = this->mButtonListeners.begin(); listener != this->mButtonListeners.end(); listener++)
	{
		(*listener)->onMouseExit(this, mX, mY);
	}

}
void GUIButton::onMouseMove(long mX, long mY)
{
	GUIComboFrame::onMouseMove(mX, mY);

	list<GUIButtonListener*>::iterator listener;
	for(listener = this->mButtonListeners.begin(); listener != this->mButtonListeners.end(); listener++)
	{
		(*listener)->onMouseMove(this, mX, mY);
	}
}
void GUIButton::onMouseLeftDown(long mX, long mY)
{
	GUIComboFrame::onMouseLeftDown(mX, mY);
	GUIFrame::notify(BasicMessage_PressLeft, this);
	
	this->mState = GUIB_MOUSE_OVER_AND_PRESS;
	this->updateRequired();

	list<GUIButtonListener*>::iterator listener;
	for(listener = this->mButtonListeners.begin(); listener != this->mButtonListeners.end(); listener++)
	{
		(*listener)->onMouseLeftDown(this, mX, mY);
	}
}
void GUIButton::onMouseMiddleDown(long mX, long mY)
{
	GUIComboFrame::onMouseMiddleDown(mX, mY);

	list<GUIButtonListener*>::iterator listener;
	for(listener = this->mButtonListeners.begin(); listener != this->mButtonListeners.end(); listener++)
	{
		(*listener)->onMouseMiddleDown(this, mX, mY);
	}

}
void GUIButton::onMouseRightDown(long mX, long mY)
{
	GUIComboFrame::onMouseRightDown(mX, mY);

	list<GUIButtonListener*>::iterator listener;
	for(listener = this->mButtonListeners.begin(); listener != this->mButtonListeners.end(); listener++)
	{
		(*listener)->onMouseRightDown(this, mX, mY);
	}

}
void GUIButton::onMouseLeftUp(long mX, long mY)
{
	GUIComboFrame::onMouseLeftUp(mX, mY);
	this->mState = GUIB_MOUSE_OVER;
	this->updateRequired();

	list<GUIButtonListener*>::iterator listener;
	for(listener = this->mButtonListeners.begin(); listener != this->mButtonListeners.end(); listener++)
	{
		(*listener)->onMouseLeftUp(this, mX, mY);
	}
}
void GUIButton::onMouseMiddleUp(long mX, long mY)
{
	GUIComboFrame::onMouseMiddleUp(mX, mY);
	
	list<GUIButtonListener*>::iterator listener;
	for(listener = this->mButtonListeners.begin(); listener != this->mButtonListeners.end(); listener++)
	{
		(*listener)->onMouseMiddleUp(this, mX, mY);
	}

}
void GUIButton::onMouseRightUp(long mX, long mY)
{
	GUIComboFrame::onMouseRightUp(mX, mY);
	
	list<GUIButtonListener*>::iterator listener;
	for(listener = this->mButtonListeners.begin(); listener != this->mButtonListeners.end(); listener++)
	{
		(*listener)->onMouseRightUp(this, mX, mY);
	}

}
void GUIButton::onKeyDown(int letterKeys, int keys)
{
	GUIComboFrame::onKeyDown(letterKeys, keys);

	list<GUIButtonListener*>::iterator listener;
	for(listener = this->mButtonListeners.begin(); listener != this->mButtonListeners.end(); listener++)
	{
		(*listener)->onKeyDown(this, letterKeys, keys);
	}

}
GUIButtonState GUIButton::getButtonState()const
{
	return this->mState;
}
void GUIButton::setButtonState(GUIButtonState state)
{
	this->mState = state;
	this->updateRequired();
}
void GUIButton::setSwitch(bool on)
{
	this->mSwitchON = on;
	this->updateRequired();
}
void GUIButton::toggleSwitch()
{
	this->mSwitchON = !this->mSwitchON;
	this->updateRequired();
}
bool GUIButton::isSwitchON()const
{
	return this->mSwitchON;
}
void GUIButton::setTextureIndexOffset(int offset)
{
	this->mTexIndexOffset = offset;
	this->updateRequired();
}
void GUIButton::setTextAlignment(GUI_Alignments alignment)
{
	this->mTextAlignment = alignment;

	this->updateRequired();
}
void GUIButton::addButtonListener(GUIButtonListener *listener)
{
	this->mButtonListeners.push_back(listener);
}
void GUIButton::removeListener(GUIButtonListener *listener)
{
	this->mButtonListeners.remove(listener);
}
void GUIButton::enableText()
{
	this->mTextEnable = true;
}
void GUIButton::disableText()
{
	this->mTextEnable = false;
}
void GUIButton::init()
{
	int arrayIndex = 0;
	int textureID = 0;
	gfx::DX10Module::getInstance()->getTextureIndex("Button_Default", arrayIndex, textureID);
	this->mTexIndexOffset = textureID;
	//this->setTextureIndexOffset(-1);
	this->getRenderInstance()->setTextureArrayIndex(arrayIndex);

	this->setMouseOmniFocus(true);
	this->mState = GUIB_DEFAULT;
	this->mSwitchON = false;
	this->mTextEnable = false;
	this->setTextAlignment(GUI_Alignment_Center);
	this->updateRequired();
}
