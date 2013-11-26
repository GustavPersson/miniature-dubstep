#include "DX10App.h"
#include "DX10Util.h"
#include "GameTimer.h"
#include "Cube.h"
#include "Camera.h"
#include "Input.h"
#include "GUI.h"
#include "Light.h"
#include "Terrain.h"
#include "Texture2D.h"
#include "Quad.h"
#include "Macros.h"

#include <iostream>
using namespace std;

Light mLight;

int main()
{
	//World matrices
	D3DXMATRIX view, proj;		// camera
	D3DXMATRIX lView, lProj;	// light

	mLight.power		= 1.0f;
	mLight.pos			= D3DXVECTOR3(-1000.0f, 1000.0f, -1000.0f);
	mLight.ambient		= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	mLight.diffuse		= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	mLight.specular		= D3DXCOLOR(0.1f, 0.1f, 0.1f, 0.0f);
	mLight.dir			= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	D3DXMatrixOrthoLH(&lProj, 500.0f, 500.0f, 1.0f, 50.0f);

	DX10App*	gfx		= myNew DX10App("Albanien 2142", 800, 600, false);
	GUI*		gui		= myNew GUI(gfx->getDevice());
	Camera*		camera	= myNew Camera(D3DXVECTOR3(100.0f, 500.0f, 50.0f), D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	Input*		input	= myNew Input(gfx->getHINSTANCE(), gfx->getHWND());
	GameTimer*	timer	= myNew GameTimer();
	Terrain*	terrain = myNew Terrain(gfx->getDevice(), 256, 256, 1, 1.3f);
	Cube*		cube	= myNew Cube(10, 5, gfx->getDevice(), D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	Texture2D*	shadow	= myNew Texture2D(1024, 1024, false);
	Quad*		quad	= myNew Quad(gfx->getDevice());
	
	shadow	->setDevice(gfx->getDevice());
	shadow	->createDepthTexture();
	quad	->bindTexture(shadow->mpDepthBufferRV);

	timer->start();
	bool	move;
	float	time = 0.0f;
	while(gfx->Run() != WM_QUIT)
	{
		timer->tick();

		camera->GetVP(view, proj);

		move = false;
		D3DXVECTOR3 dir(0.0f, 0.0f, 0.0f);
		if(GetAsyncKeyState('W') & 0x8000)
		{
			dir.z	= 1.0f;
			move	= true;
		}
		if(GetAsyncKeyState('S') & 0x8000)
		{
			dir.z	= -1.0f;
			move	= true;
		}
		if(GetAsyncKeyState('A') & 0x8000)
		{
			dir.x	= -1.0f;
			move	= true;
		}
		if(GetAsyncKeyState('D') & 0x8000)
		{
			dir.x	= 1.0f;
			move	= true;
		}
		if(GetAsyncKeyState('Q') & 0x8000)
		{
		}
		if(GetAsyncKeyState('R') & 0x8000)
		{
		}
		if(GetAsyncKeyState('T') & 0x8000)
		{
			mLight.pos = D3DXVECTOR3(100,100,100); 
			mLight.pos = camera->getEyePos();
			mLight.dir = D3DXVECTOR3(0.0f,0.0f,0.0f);
			
			//camera->GetVP(lView, lProj);
		}
		if(GetAsyncKeyState('G') & 0x8000)
		{
		}

		if(move)
		{
			dir *= timer->getDeltaTime();
			camera->Move(dir);
		}
		
		float dx = 0, dy = 0, pitch = 0, yAngle = 0;
		if(input->MouseMovement(dx, dy))
		{
			pitch	= 4 * dy * timer->getDeltaTime();
			yAngle	= 4 * dx * timer->getDeltaTime();
			
			camera->RotateX(pitch);
			camera->RotateY(yAngle);
		}

		D3DXVECTOR3 lightPos;
		lightPos.x = 30.0f*cosf(0.25f*timer->getGameTime());
		lightPos.y = 20.0f;
		lightPos.z = 30.0f*sinf(0.25f*timer->getGameTime());

		// Update light matrix
		D3DXMatrixLookAtLH(&lView, &lightPos, &mLight.dir, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));

		// Updates
		input	->Update();
		gui		->Update(timer->getDeltaTime());
		camera	->update();
		terrain	->Update(view, proj, lView, lProj, camera->getEyePos(), mLight, true);
		cube	->Update(view, proj, lView, lProj, camera->getEyePos(), mLight);

		shadow->begin();
			terrain	->RenderShadow();
			cube	->RenderShadow();
		shadow->end();

		terrain	->Update(view, proj, lView, lProj, camera->getEyePos(), mLight, false);

		terrain	->bindShadow(shadow->mpDepthBufferRV);
		cube	->bindShadow(shadow->mpDepthBufferRV);

		// Rendering
		gfx->Start();
		cube	->Render();	terrain	->Render();
			
			quad	->Render();
		
			gui->print("PRESENTING, ABLANIEN 2142", 5, 25);
			gui->showFPS();
		gfx->End();

		terrain	->bindShadow(NULL);
		cube	->bindShadow(NULL);
	}
	timer->stop();

	delete cube;
	delete quad;
	delete shadow;
	delete terrain;
	delete timer;
	delete camera;
	delete input;
	delete gui;
	delete gfx;

	_CrtDumpMemoryLeaks();

	return 0;
}