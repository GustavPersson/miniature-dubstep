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

#ifndef DX10MODULE_H
#define DX10MODULE_H
#include "WindowHandler.h"
#include "RenderHandler.h"
#include "ShaderHandler.h"
#include "TextureHandler.h"
#include "ObjectHandler.h"
#include "ParticleHandler.h"
#include "MeshLoader.h"
#include "AnimationHandler.h"
#include "Camera.h"
#include "Light.h"
#include "ScreenQuad.h"
#include "Shapes.h"
#include "WorldPlane.h"
#include "../Utilities/Timer.h"
#include "../Utilities/Thread.h"
#include "../Logic/IEventManager.h"
#include "../Utilities/VectorList.h"

namespace gfx
{
	/**
	 * Main interface for the graphics component.
	 */
	class DX10Module : public utilities::Thread
	{
		public:
			/**
			 * Default destructor.
			 */
			virtual ~DX10Module();

			/**
			 * Initialises the DX10Module and all its handlers.
			 * @param width Width of the window.
			 * @param height Height of the window.
			 * @return Returns S_OK or E_FAIL.
			 */
			HRESULT init(long width, long height);

			/**
			 * Returns instance to DX10Module 
			 */
			static DX10Module* getInstance();

			/**
			 * Mainloop for the GFX thread
			 */
			void mainLoop();

			/**
			 * Updates all of the DX10Module's handlers and camera.
			 */
			void update(float deltaTime);

			/**
			 * Renders the non-culled objects from the ObjectHandler.
			 */
			void render();

			/**
			 * Retrieves the array index and texture ID of a loaded texture.
			 * @param textureName Name of the texture to look for.
			 * @param arrayIndex Outgoing reference to the array index.
			 * @param textureID Outgoing reference to the texture ID.
			 * @return Returns true if the texture was found.
			 */
			bool getTextureIndex(const char *textureName, int &arrayIndex, int &textureID);

			/**
			 * Retrieves the array index and texture ID of a loaded texture.
			 * @param textureName Name of the texture to look for.
			 * @param arrayIndex Outgoing reference to the array index.
			 * @param textureID Outgoing reference to the texture ID.
			 * @return Returns true if the texture was found.
			 */
			bool getTextureIndex(const std::string &textureName, int &arrayIndex, int &textureID);

			/**
			 * Loads a mesh geometry file and creates an instance base of the mesh model.
			 *  This instance base is not added to the normal mObjectList in the ObjectHandler.
			 *  All draw calls and updates of the derived instances are performed from the base.
			 * @param filePath Absolute/relative file path to the MeshGeometry.txt file.
			 * @param mtrlFilePath Absolute/relative file path to MeshMaterial.txt file. If none exist, set it to ""
			 * @param instanceID ID to assign to the instance base.
			 * @param instanceName Name to assign to the instance base.
			 * @param maxInstances Maximum number of possible instances.
			 * @param updateFromBase Specifies if the instance base should update all instances each frame.
			 */
			void loadMeshInstance(const char *filePath, const char *mtrlFilePath, int instanceID, const char *instanceName,
				unsigned int maxInstances, bool updateFromBase);

			void loadAnimationMeshInstance(const char *filePath, const char *mtrlFilePath, const char *weightFilePath,
				int instanceID, const char *instanceName, unsigned int maxInstances, bool updateFromBase);

			/**
			 * Creates an instance base of an object shape.
			 *  This instance base is not added to the normal mObjectList in the ObjectHandler.
			 *  All draw calls and updates of the derived instances are performed from the base.
			 * @param type Type of the shape.
			 * @param instanceID ID to assign to the instance base.
			 * @param instanceName Name to assign to the instance base.
			 * @param maxInstances Maximum number of possible instances.
			 */
			void loadObjectShapeInstance(ShapeTypes::Types type, int instanceID, const char *instanceName, unsigned int maxInstances);

			/**
			 * Creates an instance base of a GUI sprite.
			 *  This instance base is not added to the normal mObjectList in the ObjectHandler.
			 *  All draw calls and updates of the derived instances are performed from the base.
			 * @param instanceID ID to assign to the instance base.
			 * @param instanceName Name to assign to the instance base.
			 * @param maxInstances Maximum number of possible instances.
			 */
			void loadGUISpriteInstance(int instanceID, const char *instanceName, unsigned int maxInstances);

			void loadAnimation(const char *animationFilePath, const char *animationName);

			int getAnimationIndex(const char *animationName);
			int getAnimationIndex(const std::string &animationName);

			/**
			 * Retrieves an instance base.
			 * @param instanceID ID of the instance base to retrieve.
			 */
			InstanceMesh *getMeshInstanceBase(int instanceID);

			/**
			 * Retrieves an instance base.
			 * @param instanceName Name of the instance base to retrieve.
			 */
			InstanceMesh *getMeshInstanceBase(const char *instanceName);

			/**
			 * Retrieves an instance base.
			 * @param instanceID ID of the instance base to retrieve.
			 */
			InstanceObject *getObjectInstanceBase(int instanceID);

			/**
			 * Retrieves an instance base.
			 * @param instanceName Name of the instance base to retrieve.
			 */
			InstanceObject *getObjectInstanceBase(const char *instanceName);

			/**
			 * Retrieves an instance base.
			 * @param instanceID ID of the instance base to retrieve.
			 */
			InstanceSprite *getGUISpriteInstanceBase(int instanceID);

			/**
			 * Retrieves an instance base.
			 * @param instanceName Name of the instance base to retrieve.
			 */
			InstanceSprite *getGUISpriteInstanceBase(const char *instanceName);

			/**
			 * Creates a basic shape object and adds it to the ObjectHandler.
			 *  The user should not call update(), render() or delete the object.
			 * @param type Type of the shape.
			 * @return Returns a pointer to the object.
			 */
			TransformObject *createObjectShape(ShapeTypes::Types type);

			/**
			 * Creates an instance of an object instance base.
			 *  The user should not call update(), render() or delete the object.
			 * @param instanceID ID of the instance base.
			 * @return Returns a pointer to the object.
			 */
			TransformObject *createObjectInstanced(int instanceID);

			/**
			 * Creates an instance of an object instance base.
			 *  The user should not call update(), render() or delete the object.
			 * @param instanceName Name of the instance base.
			 * @return Returns a pointer to the object.
			 */
			TransformObject *createObjectInstanced(const char *instanceName);

			/**
			 * Creates a new mesh instance of an instance base.
			 *  Note: Please assign an ID and/or name to the mesh immediately or it
			 *  will not be able to be retrieved from the getObject() method!
			 * @param instanceID ID of the instance base to create from.
			 * @return Returns a pointer to the new instance.
			 */
			InstanceMesh *createMeshInstanced(int instanceID);

			/**
			 * Creates a new mesh instance of an instance base.
			 *  Note: Please assign an ID and/or name to the mesh immediately or it
			 *  will not be able to be retrieved from the getObject() method!
			 * @param instanceName Name of the instance base to create from.
			 * @return Returns a pointer to the new instance.
			 */
			InstanceMesh *createMeshInstanced(const char *instanceName);

			/**
			 * Retrieves a RenderObject based on its ID.
			 *  Can not retrieve mesh models or instance bases.
			 * @param objectID ID of the object to retrieve.
			 * @return Returns a pointer to the object.
			 */
			RenderObject *getRenderObject(int objectID);

			/**
			 * Retrieves a RenderObject based on its name.
			 *  Can not retrieve mesh models or instance bases.
			 * @param objectName Name of the object to retrieve.
			 * @return Returns a pointer to the object.
			 */
			RenderObject *getRenderObject(const char *objectName);

			/**
			 * Removes (deletes) a RenderObject.
			 *  Can not remove mesh models or instance bases.
			 * @param object Pointer to the object to remove.
			 */
			void removeRenderObject(RenderObject *object);

			/**
			 * Removes (deletes) a RenderObject based on its ID.
			 *  Can not remove mesh models or instance bases.
			 * @param objectID ID of the object to remove.
			 */
			void removeRenderObject(int objectID);

			/**
			 * Removes (deletes) a RenderObject based on its Name.
			 *  Can not remove mesh models or instance bases.
			 * @param objectName Name of the object to remove.
			 */
			void removeRenderObject(const char *objectName);

			void createWorldPlane(int tileX, int tileZ, float tileSizeX, float tileSizeZ);

			/**
			 * Retrieves all mesh and object instance bases.
			 * @param baseVector Outgoing vector with all instance bases.
			 */
			inline void getAllInstanceBases(std::vector<RenderObject *> &baseVector){mObjectHandler.getAllInstanceBases(baseVector);}

			/**
			 * Adds all bounding volumes to a quad tree.
			 */
			void copyBoundingVolumes(std::vector<CollisionObject *> *pBVs);

			/**
			 * Returns the HWND as a const reference.
			 * @return Returns a const reference to the HWND.
			 */
			inline const HWND &getHWND() const {return mWindowHandler.getHWND();}

			/**
			 * Returns the HWND as a pointer.
			 * @return Returns a pointer to the HWND.
			 */
			inline HWND *getHWNDPointer(){return mWindowHandler.getHWNDPointer();}

			/**
			 * Returns the HINSTANCE as a const reference.
			 * @return Returns a const reference to the HINSTANCE.
			 */
			inline const HINSTANCE &getHInstance() const {return mWindowHandler.getInstance();}

			/**
			 * Returns the HINSTANCE as a pointer.
			 * @return Returns a pointer to the HINSTANCE.
			 */
			inline HINSTANCE *getHInstancePointer(){return mWindowHandler.getInstancePointer();}

			/**
			 * Returns the ID3D10Device.
			 * @return Returns a pointer to the ID3D10Device.
			 */
			inline ID3D10Device *getDevice(){return mWindowHandler.getDevice();}

			/**
			 * Returns the client width of the window.
			 * @return Returns the client width as a long.
			 */
			inline long getClientWidth() const {return mWindowHandler.getClientWidth();}

			/**
			 * Returns the client height of the window.
			 * @return Returns the client height as a long.
			 */
			inline long getClientHeight() const {return mWindowHandler.getClientHeight();}

			/**
			 * Retrieves a pointer to the world plane.
			 * @return Returns mpWorldPlane.
			 */
			inline WorldPlane *getWorldPlane(){return mpWorldPlane;}

			/**
			 * Returns the camera.
			 * @return Returns a pointer to the camera.
			 */
			inline Camera *getCamera(){return mCamera;}

			/**
			 * Returns the Light.
			 * @return Returns a pointer to the light.
			 */
			inline Light *getLight() {return mLight;}

			void handleEvent(utilities::VectorList<logic::EventDataPtr> &events);

			ParticleHandler		mParticleHandler;

		protected:
			/**
			 * Loads all shaders.
			 * @return Returns S_OK or E_FAIL.
			 */
			HRESULT loadShaders();

			/**
			 * Loads and creates a font texture array.
			 */
			HRESULT loadFont();

			/**
			 * Updates all "once" constant buffers in all shaders.
			 */
			void updateShaderOnceBuffers();

			/**
			 * Updates all "per frame" constant buffer in all shaders.
			 */
			void updateShaderPerFrameBuffers(float deltaTime);

		private:
			/**
			 * Default constructor.
			 */
			DX10Module();

			/**
			 * Default copy constructor.
			 */
			DX10Module(const DX10Module &module);

			/**
			 * Default assignment operator.
			 */
			const DX10Module &operator=(const DX10Module &module);

			WindowHandler mWindowHandler;
			RenderHandler mRenderHandler;
			RenderHandler mShadowRenderHandler;
			ShaderHandler mShaderHandler;
			ObjectHandler mObjectHandler;
			TextureHandler *mpTextureHandler;
			MeshLoader	*mpMeshLoader;
			AnimationHandler *mpAnimationHandler;

			WorldPlane *mpWorldPlane;
			Camera *mCamera;
			Light *mLight;
			
			static DX10Module *mpsThreadInstance;
			utilities::Timer *mTimer;
			bool mShadowsEnabled;
			bool mUpdateAnimations;
	};
}

#endif