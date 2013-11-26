/**
 * @file
 * @author Victor Lundberg <lundberg.victor@gmail.com>
 * @version 1.0
 * Copyright (©) A-Team.
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "InputLayoutHandler.h"

using namespace gfx;
using std::vector;

InputLayoutHandler::InputLayoutHandler(ID3D10Device *pDevice) : mInputLayoutList()
{
	mpDevice = pDevice;
}

InputLayoutHandler::InputLayoutHandler(const InputLayoutHandler &handler)
{
	mpDevice = handler.mpDevice;
}

InputLayoutHandler::~InputLayoutHandler()
{
	clearInputLayouts();
}

const InputLayoutHandler &InputLayoutHandler::operator=(const InputLayoutHandler &handler)
{
	if(this != &handler)
	{
		mpDevice = handler.mpDevice;
	}
	return *this;
}

InputLayout *InputLayoutHandler::getInputLayout(unsigned int index)
{
	for(vector<InputLayout *>::iterator n = mInputLayoutList.begin();
		n != mInputLayoutList.end(); ++n)
	{
		if((*n)->getID() == (int)index)
		{
			return (*n);
		}
	}

	return NULL;
}

InputLayout *InputLayoutHandler::getInputLayout(const char *name)
{
	for(vector<InputLayout *>::iterator n = mInputLayoutList.begin();
		n != mInputLayoutList.end(); ++n)
	{
		if((*n)->getNameString() == name)
		{
			return (*n);
		}
	}
	return NULL;
}

HRESULT InputLayoutHandler::createInputLayout(ID3D10Effect *pEffect, unsigned int techIndex,
	unsigned int passIndex, VertexTypes::Types vertexType)
{
	D3D10_PASS_DESC passDesc;
	pEffect->GetTechniqueByIndex(techIndex)->GetPassByIndex(passIndex)->GetDesc(&passDesc);

	InputLayout *pLayout = NULL;
	ID3D10InputLayout *pDXLayout = NULL;
	ID3D10InputLayout *pDXLayoutInstanced = NULL;
	unsigned int layoutSize = 0;

	switch(vertexType)
	{
		//VertexTransform.
		case VertexTypes::VERTEX_TRANSFORM:
			//Non-instanced input layout.
			if(FAILED(mpDevice->CreateInputLayout(VertexTransform::pInputDesc, VertexTransform::NUM_ELEMENTS,
												  passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &pDXLayout)))
			{
				ERROR_MESSAGE("Error creating input layout.")
				return E_FAIL;
			}
			//Instanced input layout.
			if(FAILED(mpDevice->CreateInputLayout(VertexTransformInstance::pInputDesc, VertexTransformInstance::NUM_ELEMENTS,
												  passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &pDXLayoutInstanced)))
			{
				ERROR_MESSAGE("Error creating instanced input layout.")
				return E_FAIL;
			}
			layoutSize = sizeof(VertexTransform);
			pLayout = myNew InputLayout(pDXLayout, layoutSize);
			pLayout->setDXInputLayoutInstanced(pDXLayoutInstanced);
			pLayout->setID(passIndex);
			pLayout->setName("VertexTransform");
			pLayout->setLayoutSizeStatic(sizeof(VertexTransformStatic));
			pLayout->setLayoutSizeDynamic(sizeof(VertexTransformDynamic));
			mInputLayoutList.push_back(pLayout);
			break;

		//VertexAnimation.
		case VertexTypes::VERTEX_ANIMATION:
			//Non-instanced input layout.
			if(FAILED(mpDevice->CreateInputLayout(VertexAnimation::pInputDesc, VertexAnimation::NUM_ELEMENTS,
												  passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &pDXLayout)))
			{
				ERROR_MESSAGE("Error creating input layout.")
				return E_FAIL;
			}
			//Instanced input layout.
			if(FAILED(mpDevice->CreateInputLayout(VertexAnimationInstance::pInputDesc, VertexAnimationInstance::NUM_ELEMENTS,
												  passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &pDXLayoutInstanced)))
			{
				ERROR_MESSAGE("Error creating instanced input layout.")
				return E_FAIL;
			}
			layoutSize = sizeof(VertexAnimation);
			pLayout = myNew InputLayout(pDXLayout, layoutSize);
			pLayout->setDXInputLayoutInstanced(pDXLayoutInstanced);
			pLayout->setID(passIndex);
			pLayout->setName("VertexAnimation");
			pLayout->setLayoutSizeStatic(sizeof(VertexAnimationStatic));
			pLayout->setLayoutSizeDynamic(sizeof(VertexAnimationDynamic));
			mInputLayoutList.push_back(pLayout);
			break;

		//VertexMultiTexture.
		case VertexTypes::VERTEX_MULTI_TEXTURE:
			//Non-instanced input layout.
			if(FAILED(mpDevice->CreateInputLayout(VertexMultiTexture::pInputDesc, VertexMultiTexture::NUM_ELEMENTS,
												  passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &pDXLayout)))
			{
				ERROR_MESSAGE("Error creating input layout.")
				return E_FAIL;
			}
			//Instanced input layout.
			if(FAILED(mpDevice->CreateInputLayout(VertexMultiTextureInstance::pInputDesc, VertexMultiTextureInstance::NUM_ELEMENTS,
												  passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &pDXLayoutInstanced)))
			{
				ERROR_MESSAGE("Error creating instanced input layout.")
				return E_FAIL;
			}
			layoutSize = sizeof(VertexMultiTexture);
			pLayout = myNew InputLayout(pDXLayout, layoutSize);
			pLayout->setDXInputLayoutInstanced(pDXLayoutInstanced);
			pLayout->setID(passIndex);
			pLayout->setName("VertexMultiTexture");
			pLayout->setLayoutSizeStatic(sizeof(VertexMultiTextureStatic));
			pLayout->setLayoutSizeDynamic(sizeof(VertexMultiTextureDynamic));
			mInputLayoutList.push_back(pLayout);
			break;

		//VertexGUI.
		case VertexTypes::VERTEX_GUI:
			//Non-instanced input layout.
			if(FAILED(mpDevice->CreateInputLayout(VertexGUI::pInputDesc, VertexGUI::NUM_ELEMENTS,
												  passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &pDXLayout)))
			{
				ERROR_MESSAGE("Failed to create input layout.")
				return E_FAIL;
			}
			//Instanced input layout.
			if(FAILED(mpDevice->CreateInputLayout(VertexGUIInstance::pInputDesc, VertexGUIInstance::NUM_ELEMENTS,
												  passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &pDXLayoutInstanced)))
			{
				ERROR_MESSAGE("Failed to create instanced input layout.")
				return E_FAIL;
			}
			layoutSize = sizeof(VertexGUI);
			pLayout = myNew InputLayout(pDXLayout, layoutSize);
			pLayout->setDXInputLayoutInstanced(pDXLayoutInstanced);
			pLayout->setID(passIndex);
			pLayout->setName("VertexGUI");
			pLayout->setLayoutSizeStatic(sizeof(VertexGUIStatic));
			pLayout->setLayoutSizeDynamic(sizeof(VertexGUIDynamic));
			mInputLayoutList.push_back(pLayout);
			break;

		// VertexParticle
		case VertexTypes::VERTEX_PARTICLES:
			if(FAILED(mpDevice->CreateInputLayout(VertexParticles::pInputDesc, VertexParticles::NUM_ELEMENTS,
												  passDesc.pIAInputSignature, passDesc.IAInputSignatureSize, &pDXLayout)))
			{
				ERROR_MESSAGE("ERROR: GFX: InputLayoutHandler: createInputLayout(). Error creating particle input layout.")
				return E_FAIL;
			}

			layoutSize = sizeof(VertexParticles);
			pLayout = myNew InputLayout(pDXLayout, layoutSize);
			pLayout->setDXInputLayoutInstanced(pDXLayoutInstanced);
			pLayout->setID(passIndex);
			pLayout->setName("VertexParticles");
			pLayout->setLayoutSizeStatic(sizeof(VertexParticles));
			mInputLayoutList.push_back(pLayout);
			break;

		default:
			ERROR_MESSAGE("Error: GFX: InputLayoutHandler: createInputLayout(). Invalid vertex type supplied.")
			return E_FAIL;
	}
	return S_OK;
}

void InputLayoutHandler::clearInputLayouts()
{
	for(vector<InputLayout *>::iterator n = mInputLayoutList.begin();
		n != mInputLayoutList.end(); ++n)
	{
		if((*n))
		{
			delete (*n);
			(*n) = NULL;
		}
	}
	mInputLayoutList.clear();
}