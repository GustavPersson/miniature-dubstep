#include "GUIRadioButtons.h"

using namespace logic;
using namespace gfx;
using namespace std;

GUIRadioButtons::GUIRadioButtons(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
		: GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	init();

}

GUIRadioButtons::GUIRadioButtons(gfx::GUIFrame *pParent) : GUIImage(pParent)
{
	init();

}

GUIRadioButtons::GUIRadioButtons(const logic::GUIRadioButtons &radioButtons) : GUIImage(radioButtons)
{
	init();

}
GUIRadioButtons::~GUIRadioButtons(void)
{

}
const GUIRadioButtons& GUIRadioButtons::operator=(const GUIRadioButtons &radioButtons)
{
	if(this != &radioButtons)
	{
		GUIImage::operator =(radioButtons);
	}
	return *this;
}
void GUIRadioButtons::update(float deltaTime)
{
	GUIImage::update(deltaTime);
}

void GUIRadioButtons::addButton(GUIButton *button, std::string option)
{
	if(button != NULL)
	{
		this->mRadioButtons[button] = option;
		this->addSubFrame(button);
	}
}
void GUIRadioButtons::select(GUIButton *button)
{
	if(button != NULL)
	{
		map<GUIButton*, string>::iterator it = this->mRadioButtons.find(button);
		if(it != this->mRadioButtons.end())
		{
			if(this->mSelected != NULL)
			{
				this->mSelected->setSwitch(false);
				this->mSelected->setButtonState(GUIB_DEFAULT);
				this->mSelected = it->first;
				this->mSelected->setSwitch(true);
				this->mOption = it->second;
			}
			else
			{
				this->mSelected = it->first;
				this->mOption = it->second;
				this->mSelected->setSwitch(true);
			}
		}
	}
	else
	{
		if(this->mSelected != NULL)
		{
			this->mSelected->setSwitch(false);
			this->mSelected = NULL;
			this->mOption = " ";
		}
	}
}
string GUIRadioButtons::getOption()const
{
	return this->mOption;
}
void GUIRadioButtons::notify(int message, GUIFrame *pSender)
{
	if(message == BasicMessage_PressLeft)
	{
		this->select((GUIButton*)pSender);
	}
}
void GUIRadioButtons::init()
{
	GUIImage::init();
	this->mSelected = NULL;
	this->mOption = " ";
	this->setTextureID(-1);
	this->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f));
	this->setRelativePosition(D3DXVECTOR2(0.0f, 0.0f));
	this->setRelativeSize(D3DXVECTOR2(1.0f, 1.0f));
	this->updateRequired();
}
