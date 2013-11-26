#include "../Utilities/Macros.h"
#include "GUITextField.h"

using namespace std;
using namespace logic;
using namespace gfx;

GUITextField::GUITextField(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth):
GUIComboFrame(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	init();
}
GUITextField::GUITextField(gfx::GUIFrame *pParent):
GUIComboFrame(pParent)
{
	init();
}
GUITextField::GUITextField(const GUITextField &textField):
GUIComboFrame(textField)
{
	*this = textField;
}
GUITextField::~GUITextField(void)
{

}
const GUITextField& GUITextField::operator=(const GUITextField &textField)
{
	if(this != &textField)
	{
		GUIComboFrame::operator=(textField);
	}
	return *this;
}
void GUITextField::update(float deltaTime)
{
	GUIComboFrame::update(deltaTime);

	if(this->mShowScrollBar)
	{
		this->mScrollBar->setFlagVisible(true);
		this->getTextFrame()->setRowOffset((int)this->mScrollBar->getCurrentIndexPosition());
		int scroll = this->getTextFrame()->getTotalNumberOfRows() - this->getTextFrame()->getNumberOfRowsShown();
		this->mScrollBar->setNumberOfIndices(scroll);
	}
	else
	{
		this->mScrollBar->setFlagVisible(false);
	}

	if(this->getFlagUpdateRequired())
	{
				
	}
}
void GUITextField::onKeyDown(int letterKeys, int keys)
{
	GUIComboFrame::onKeyDown(letterKeys, keys);
	
	if((this->mMode & input_key)>0 && this->getRenderInstance()->getFlagRender())
	{
		if((this->mMode & input_ignore_enter)>0)
		{
			if(keys & logic::InputStates::Key_Return)
				return;
		}
		if((this->mMode & input_ignore_space)>0)
		{
			if(keys & logic::InputStates::Key_Space)
				return;
		}
		this->getTextFrame()->addCharFromKey(letterKeys, keys);
		this->updateRequired();
	}
}
void GUITextField::enableScrollBar()
{
	this->mShowScrollBar = true;
	this->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.94f, 1.0f));
	this->updateRequired();
}
void GUITextField::disableScrollBar()
{
	this->mShowScrollBar = false;
	this->getTextFrame()->setRelativeSize(D3DXVECTOR2(1.0f, 1.0f));
	this->updateRequired();
}
bool GUITextField::isScrollBarEnabled()const
{
	return this->mShowScrollBar;
}
void GUITextField::setInputMode(int mode)
{
	this->mMode = mode;
}
void GUITextField::init()
{
	this->setMouseOmniFocus(true);
	mShowScrollBar = false;
	mMode = input_none;
	this->mScrollBar = myNew GUIScrollBar(this);
	this->mScrollBar->setRelativeSize(D3DXVECTOR2(0.06f, 1.0f));
	this->mScrollBar->setRelativePosition(D3DXVECTOR2(0.94f, 0.0f));
	this->addSubFrame(mScrollBar);
	disableScrollBar();

	this->getTextFrame()->setTextFormat(GUIText::TextFormat_FixedRows);
	this->getTextFrame()->setTextSize(D3DXVECTOR2(15.0f, 15.0f));


	this->setRelativeSize(D3DXVECTOR2(0.3f, 0.2f));
	this->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	this->updateRequired();
}
