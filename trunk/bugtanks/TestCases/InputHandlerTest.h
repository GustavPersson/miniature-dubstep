#include <cxxtest/TestSuite.h>
#include "..\Logic\InputHandler.h"
#include "..\GFX\DX10Module.h"

using namespace logic;

class InputHandlerTest : public CxxTest::TestSuite
{
public:
	void setUp()
	{
		/*module = myNew DX10Module();
		module->Init(100,100);
		handler = myNew InputHandler(module->getInstance(), module->getHwnd());*/
	}

	void tearDown()
	{		
		/*SAFE_DELETE(handler);
		SAFE_DELETE(module);*/
	}

	void testUpdate()
	{
	}
private:
	/*InputHandler *handler;
	DX10Module *module;*/

};
