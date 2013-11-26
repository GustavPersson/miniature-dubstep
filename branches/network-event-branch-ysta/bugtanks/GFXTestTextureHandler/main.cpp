/*#include <string>
#include "TextureHandler.h"
#include "DX10Module.h"
#include "DXBase.h"*/

int main()
{
	/*DX10Module * module = new DX10Module();
	module->init("Bugtanks",1024,800);

	TextureHandler *th = myNew TextureHandler(module->getDevice());
	th->addTexture("C:/Bugtanks/trunk/bugtanks/wood.dds","n00b");
	th->addTexture("C:/Bugtanks/trunk/bugtanks/wood.dds","bajs");


	//string textureNames[2];
	//textureNames[0] = "n00b";
	//textureNames[1] = "bajs";

	th->getTextureArray("n00b,bajs");

	//th->getTexture("n00b");

	
	
	//module->addMesh("../trunk/ModelFiles/Dragon/dragonGeometry","Dragon");

	module->loadMeshModel("../../ModelFiles/Dragon/DragonGeometry.txt", 1, "Dragon");
	gfx::Mesh *mesh1 = module->createMesh(1);
	mesh1->setID(2);
	mesh1->setName("Mesh1");
	mesh1->setPosition(D3DXVECTOR3(-30.0f, 0.0f, 0.0f));
	mesh1->setScale(D3DXVECTOR3(3.0f, 3.0f, 3.0f));
	mesh1->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	gfx::Camera *camera = module->getCamera();
	camera->setTarget(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	camera->setPosition(D3DXVECTOR3(-100.0f, 10.0f, -100.0f));

	MSG msg = {0};

	while(WM_QUIT != msg.message)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			module->update();
			module->render();
		}
	}

	SAFE_DELETE(module);
	SAFE_DELETE(th);*/

	return 0;
}