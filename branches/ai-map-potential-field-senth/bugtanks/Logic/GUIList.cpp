#include "../Utilities/Macros.h"
#include "GUIList.h"

using namespace logic;
using namespace gfx;
using namespace std;

GUIList::GUIList(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight, long parentDepth)
: GUIImage(pBaseInstance, clientWidth, clientHeight, parentDepth)
{
	this->init();
}
GUIList::GUIList(gfx::GUIFrame *pParent) : GUIImage(pParent)
{
	this->init();
}
GUIList::GUIList(const GUIList &list) : GUIImage(list)
{
	this->init();
}
GUIList::~GUIList(void)
{

}
const GUIList& GUIList::operator=(const GUIList &list)
{
	if(this != &list)
	{
		gfx::GUIImage::operator=(list);
	}
	return *this;
}
void GUIList::update(float deltaTime)
{
	GUIImage::update(deltaTime);

	
	if((int)this->mScrollBar->getCurrentIndexPosition() != this->mScrollPos)
	{
		this->mScrollPos = (int)this->mScrollBar->getCurrentIndexPosition();
		this->updateRequired();
	}
	

	this->mFlagRebuildListRequired = this->mNewFlagRebuildListRequired;
	this->mNewFlagRebuildListRequired = false;

	if(this->getFlagRebuildListRequired())
	{
		float buttonHeight = (float)1/(float)this->mElementsVisible;
		for(int i=0; i<MAX_VISIBLE_ELEMENTS; i++)
		{
			if(i < this->mElementsVisible)
			{
				this->mElementButtons[i]->setRelativePosition(D3DXVECTOR2(0.0f, buttonHeight*i));
				this->mElementButtons[i]->setRelativeSize(D3DXVECTOR2(0.9f, buttonHeight));
				this->mElementButtons[i]->setFlagVisible(true);
			}
			else
			{
				this->mElementButtons[i]->setFlagVisible(false);
			}
		}
		if(this->getRenderInstance()->getFlagRender()==false)
		{
			this->setFlagVisible(false);
		}
	}

	if(this->getFlagUpdateRequired())
	{
		list<string>::iterator item = this->mItemList.begin();
		advance(item, min((int)this->mItemList.size(), mScrollPos));
		
		for(int i=0; i<this->mElementsVisible; i++)
		{
			if(item != this->mItemList.end())
			{
				if(this->mSelected.compare((*item))==0)
					this->mElementButtons[i]->setSwitch(true);
				else
				{
					this->mElementButtons[i]->setSwitch(false);
					this->mElementButtons[i]->setButtonState(GUIB_DEFAULT);
				}

				this->mElementButtons[i]->getTextFrame()->setText((*item));
				item++;
			}
			else
			{
				this->mElementButtons[i]->getTextFrame()->setText(" ");
				this->mElementButtons[i]->setSwitch(false);
				this->mElementButtons[i]->setButtonState(GUIB_DEFAULT);
			}
		}
	}
}
void GUIList::setFlagRender(bool render)
{
	GUIImage::setFlagRender(render);

}
void GUIList::notify(int message, GUIFrame *pSender)
{
	if(message == BasicMessage_PressLeft)
	{
		if(((GUIButton*)pSender)->getTextFrame()->getTextString().compare(" ")!=0 && ((GUIButton*)pSender)->getTextFrame()->getTextString().compare(this->mSelected)!=0)
		{
			this->mSelected = ((GUIButton*)pSender)->getTextFrame()->getTextString();
			this->updateRequired();
		}
		else
		{
			this->mSelected = "NULL";
			this->updateRequired();
		}
	}
}
void GUIList::showNumberOfElements(int n)
{
	if(this->mElementsVisible != n && n>0)
	{
		this->mElementsVisible = min(MAX_VISIBLE_ELEMENTS, n);
		this->rebuildListRequired();
		this->updateRequired();
	}
}
void GUIList::addItem(string item)
{
	this->mItemList.push_back(item);
	int scroll = this->mItemList.size() - this->mElementsVisible;
	if(scroll > 0)
		this->mScrollBar->setNumberOfIndices(scroll);
	this->updateRequired();
}
void GUIList::removeItem(string item)
{
	this->mItemList.remove(item);
	this->updateRequired();
}
void GUIList::clear()
{
	this->mItemList.clear();
	//this->mSelected = "NULL";
	this->updateRequired();
}
string GUIList::getSelected()const
{
	return this->mSelected;
}
int GUIList::getSize()const
{
	return this->mItemList.size();
}
void GUIList::rebuildListRequired()
{
	this->mNewFlagRebuildListRequired = true;
}
bool GUIList::getFlagRebuildListRequired()
{
	return this->mFlagRebuildListRequired;
}
void GUIList::init()
{

	int arrayIndex = 0;
	int textureID = 0;
	gfx::DX10Module::getInstance()->getTextureIndex("ListBG", arrayIndex, textureID);

	this->setTextureID(textureID);
	this->getRenderInstance()->setTextureArrayIndex(arrayIndex);
	this->setColor(D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.0f));
	this->setRelativeSize(D3DXVECTOR2(0.25f,0.6f));

	this->mElementsVisible = 25;
	this->mScrollPos = 3;
	this->mSelected = "NULL";

	gfx::DX10Module::getInstance()->getTextureIndex("ListElement_Default", arrayIndex, textureID);

	// Init all element buttons.
	for(int i=0; i<MAX_VISIBLE_ELEMENTS; i++)
	{
		this->mElementButtons[i] = myNew GUIButton(this);
		this->mElementButtons[i]->setTextureIndexOffset(textureID);
		this->mElementButtons[i]->getRenderInstance()->setTextureArrayIndex(arrayIndex);
		this->mElementButtons[i]->getTextFrame()->setTextFormat(GUIText::TextFormat_ScaledLine);
		this->mElementButtons[i]->getTextFrame()->setRelativeSize(D3DXVECTOR2(0.5f, 0.5f));
		this->mElementButtons[i]->setTextAlignment(GUI_Alignment_BottomLeft);
		this->mElementButtons[i]->enableText();
		this->addSubFrame(this->mElementButtons[i]);
	}

	this->mScrollBar = myNew GUIScrollBar(this);
	this->mScrollBar->setRelativePosition(D3DXVECTOR2(0.9f, 0.0f));
	this->mScrollBar->setRelativeSize(D3DXVECTOR2(0.1f, 1.0f));
	this->mScrollBar->setNumberOfIndices(0);

	this->mScrollBar->setFlagVisible(true);
	this->addSubFrame(this->mScrollBar);
	
	this->setMouseOmniFocus(true);
	this->rebuildListRequired();
	this->updateRequired();
}
