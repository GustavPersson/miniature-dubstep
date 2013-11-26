#include "GUIHandlerLoadingScreen.h"
#include "GUIFrameLoadingScreen.h"

using namespace logic;

GUIHandlerLoadingScreen::GUIHandlerLoadingScreen(gfx::InstanceSprite *pBaseInstance, long clientWidth, long clientHeight) 
		: gfx::GUIHandler(pBaseInstance, clientWidth, clientHeight)
{
	init();
}

GUIHandlerLoadingScreen::GUIHandlerLoadingScreen(const GUIHandlerLoadingScreen &handler) : GUIHandler(handler)
{
	init();
}

GUIHandlerLoadingScreen::~GUIHandlerLoadingScreen(void)
{

}

const GUIHandlerLoadingScreen &GUIHandlerLoadingScreen::operator =(const logic::GUIHandlerLoadingScreen &handler)
{
	if (this != &handler)
	{
		gfx::GUIHandler::operator=(handler);
	}
	return *this;
}

const char *GUIHandlerLoadingScreen::getName()
{
	return "GUIHandler Loading SCreen";
}

void GUIHandlerLoadingScreen::init()
{
	GUIFrameLoadingScreen *frame = myNew GUIFrameLoadingScreen(getInstanceBase(), getClientWidth(), getClientHeight(), 0);
	frame->setRelativePosition(D3DXVECTOR2(0.0f, 0.8f));
	frame->setRelativeSize(D3DXVECTOR2(1.0f, 0.2f));
	addFrame(frame);
}