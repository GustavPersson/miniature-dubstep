//#include <iostream>
//
//#include "../Macros/Macros.h"
////
//#include "../GFX/DX10Module.h"
//#include "../GFX/GameTimer.h"
//#include "../GFX/Camera.h"
//
//#include "../Logic/InputHandler.h"
//
//using logic::InputHandler;
//using namespace std;
//using namespace gfx;
//
//
int main()
{
//	DX10Module	*engine = new DX10Module();
//	GameTimer	*timer	= new GameTimer();
//	Camera		*camera = new Camera(D3DXVECTOR3(50.0f, 50.0f, 0.0f));
//
//	engine->init(800, 600);
//	InputHandler *input = new InputHandler(engine->getInstance(), engine->getHwnd());
//
//	bool move;
//	MSG msg = {0};
//	timer->start();
//	while(WM_QUIT != msg.message)
//	{
//		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
//		{
//			TranslateMessage(&msg);
//			DispatchMessage(&msg);
//		}
//		else
//		{
//			timer->tick();
//
//			camera->GetVPMatrices(engine->getViewMatrix(), engine->getProjMatrix());
//
//			move		= false;
//			D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);
//			if(GetAsyncKeyState('W') & 0x8000)
//			{
//				dir.z	= 1.0f;
//				move	= true;
//			}
//			if(GetAsyncKeyState('S') & 0x8000)
//			{
//				dir.z	= -1.0f;
//				move	= true;
//			}
//			if(GetAsyncKeyState('A') & 0x8000)
//			{
//				dir.x	= -1.0f;
//				move	= true;
//			}
//			if(GetAsyncKeyState('D') & 0x8000)
//			{
//				dir.x	= 1.0f;
//				move	= true;
//			}
//
//			if(move)
//			{
//				dir *= timer->getDeltaTime();
//				camera->move(dir);
//			}
//			
//			float dx = 0, dy = 0, pitch = 0, yAngle = 0;
//			if(input->mouseMovement(dx, dy))
//			{
//				pitch	= 4 * dy * timer->getDeltaTime();
//				yAngle	= 4 * dx * timer->getDeltaTime();
//				
//				camera->RotateX(pitch);
//				camera->RotateY(yAngle);
//			}
//
//			engine->render();
//		}
//	}
//	
//	SAFE_DELETE(input);
//	SAFE_DELETE(camera);
//	SAFE_DELETE(timer);
//	SAFE_DELETE(engine);
//
//	_CrtDumpMemoryLeaks();
	return 0;
}