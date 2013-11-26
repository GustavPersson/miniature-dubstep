/**
 * @file
 * @author Martin <mail@mail.nu>, Victor Lundberg <lundberg.victor@gmail.com>
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

#ifndef EFFECT_H
#define EFFECT_H
#include <d3d10.h>
#include <d3dx10.h>
#include <string>
#include "InputLayoutHandler.h"
#include "../Utilities/Macros.h"

namespace gfx
{
	/**
	 * Class for encapsuling shaders.
	 */
	class Effect
	{
		public:
			/**
			 * Default constructor.
			 * @param effectName Name identifier of this effect.
			 * @param pDevice Pointer to the ID3D10Device.
			 */
			Effect(const char *effectName, ID3D10Device *pDevice);

			/**
			 * Default copy constructor.
			 *  Shallow copy.
			 */
			Effect(const Effect &effect);

			/**
			 * Default destructor.
			 */
			virtual ~Effect();

			/**
			 * Default assignment operator.
			 *  Shallow copy.
			 */
			const Effect &operator=(const Effect &effect);

			/**
			 * Creates an effect from file.
			 * @param fileName Relative/absolute path of the file.
			 */
			virtual HRESULT createEffect(const char *fileName);

			/**
			 * Calls mInputLayoutHandler.createInputLayout() with this effect as argument.
			 * @param techIndex Index of the shader technique.
			 * @param vertexType Type of the vertex structure.
			 */
			virtual HRESULT createInputLayout(unsigned int techIndex, VertexTypes::Types vertexType);

			/**
			 * Retrieves the ID3D10Effect pointer.
			 * @return Returns mpEffect.
			 */
			inline ID3D10Effect *getEffect(){return mpEffect;}

			/**
			 * Retrieves a reference to the InputLayoutHandler.
			 * @return Returns a reference to mInputLayoutHandler.
			 */
			InputLayoutHandler &getInputLayoutHandler(){return mInputLayoutHandler;}

			/**
			 * Retrieves a const reference to the InputLayoutHandler.
			 * @return Returns a const reference to mInputLayoutHandler.
			 */
			const InputLayoutHandler &getInputLayoutHandlerConst() const {return mInputLayoutHandler;}

			/**
			 * Retrieves the effect's name as a const char pointer.
			 * @return Returns mName as a const char pointer.
			 */
			inline const char *getName() const {return mName.c_str();}

			/**
			 * Retrieves the effect's name as a string.
			 * @return Returns mName.
			 */
			inline const std::string &getNameString() const {return mName;}

			/**
			 *
			 */
			inline void bindShadowMap(ID3D10ShaderResourceView* shadowmap) { mpFXShadowMap->SetResource(shadowmap); }
		
			/**
			 * Updates the viewport size shader variable.
			 *  Note: This is only a temporary solution until the constant buffer
			 *  updating methods are completed.
			 * @param size Const reference to the size vector.
			 */
			void updateViewportSize(const D3DXVECTOR2 &size);

			/**
			 * Updates all light matrices
			 * @param V Const reference to the view matrix
			 * @param P Const reference to the projection matrix
			 */
			void updateLightMatrices(const D3DXMATRIX &V, const D3DXMATRIX &P);

			/**
			 * Updates the camera position shader variable.
			 *  Note: This is only a temporary solution until the constant buffer
			 *  updating methods are completed.
			 * @param position Const reference to the position vector.
			 */
			void updateVCameraPosition(const D3DXVECTOR3 &position);

			/**
			 * Updates the camera direction shader variable.
			 *  Note: This is only a temporary solution until the constant buffer
			 *  updating methods are completed.
			 * @param direction Const reference to the direction vector.
			 */
			void updateVCameraDirection(const D3DXVECTOR3 &direction);

			/**
			 * Updates all camera matrices.
			 *  Note: This is only a temporary solution until the constant buffer
			 *  updating methods are completed.
			 * @param V Const reference to the view matrix.
			 * @param P Const reference to the projection matrix.
			 */
			void updateCameraMatrices(const D3DXMATRIX &V, const D3DXMATRIX &P, const D3DXMATRIX &ortP);

			/**
			 * Updates the shader texture array.
			 * @param pTexture Pointer to a texture array shader resource.
			 */
			void updateTextureArray(ID3D10ShaderResourceView *pTexture);

			void updateAnimations(ID3D10ShaderResourceView *pTexture);

		protected:

		private:
			InputLayoutHandler mInputLayoutHandler;
			ID3D10Device *mpDevice;
			ID3D10Effect *mpEffect;
			std::string mName;
			ID3D10EffectVariable *mpFXViewportSize;
			ID3D10EffectVariable *mpFXVCameraPosition;
			ID3D10EffectVariable *mpFXVCameraDirection;
			ID3D10EffectMatrixVariable *mpFXLVP;		/**< Lights View and Projection matrix */
			ID3D10EffectMatrixVariable *mpFXLV;			/**< Lights View matrix */
			ID3D10EffectMatrixVariable *mpFXLP;			/**< Lights Projection matrix */
			ID3D10EffectMatrixVariable *mpFXMVP;
			ID3D10EffectMatrixVariable *mpFXMV;
			ID3D10EffectMatrixVariable *mpFXMP;
			ID3D10EffectMatrixVariable *mpFXMOrtP;
			ID3D10EffectShaderResourceVariable *mpFXTextureArray;
			ID3D10EffectShaderResourceVariable* mpFXShadowMap;

			ID3D10EffectShaderResourceVariable *mpFXAnimations;
	};
}

#endif