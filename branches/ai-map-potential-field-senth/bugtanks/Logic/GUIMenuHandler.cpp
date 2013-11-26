#include "GUIMenuHandler.h"

using namespace logic;
using namespace gfx;
using namespace std;

GUIMenuHandler::GUIMenuHandler(InstanceSprite *pBaseInstance, long clientWidth, long clientHeight)
: GUIHandler(pBaseInstance, clientWidth, clientHeight)
{
	this->init();
}
GUIMenuHandler::GUIMenuHandler(const GUIMenuHandler &handler) : GUIHandler(handler)
{
	this->init();
}	
GUIMenuHandler::~GUIMenuHandler(void)
{
	// Delete all menu items in the vector.
}
const GUIMenuHandler& GUIMenuHandler::operator=(const GUIMenuHandler &handler)
{
	if (this != &handler)
	{
		gfx::GUIHandler::operator=(handler);
		this->mMenuItems = handler.mMenuItems;
	}
	return *this;
}
const char* GUIMenuHandler::getName()
{
	return "GUIMenuHandler";
}
void GUIMenuHandler::setFlagRender(bool renderFlag)
{
	std::list<GUIFrame*>::iterator item;
	for(item = this->mMenuItems.begin(); item != this->mMenuItems.end(); item++)
	{
		(*item)->setFlagRender(renderFlag);
	}
}
void GUIMenuHandler::addMenuItem(GUIFrame* menuItem)
{
	this->addFrame(menuItem);
	this->mMenuItems.push_back(menuItem);
}
void GUIMenuHandler::init()
{
	gfx::GUIHandler::init();

}
