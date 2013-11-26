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

#ifndef WORLD_PLANE_H
#define WORLD_PLANE_H
#include "InstanceShapePlane.h"

namespace gfx
{
	class WorldPlane
	{
		public:
			/**
			 * Default constructor.
			 */
			WorldPlane(int tileX, int tileZ, float tileSizeX, float tileSizeZ);

			/**
			 * Default copy constructor.
			 */
			WorldPlane(const WorldPlane &plane);

			/**
			 * Default destructor.
			 */
			virtual ~WorldPlane();

			/**
			 * Default assignment operator.
			 */
			const WorldPlane &operator=(const WorldPlane &plane);

			/**
			 * Updates the position and scale of the tiles and recreates them if needed.
			 */
			virtual void update();

			/**
			 * Sets the weights for all vertices in tiles in a circular area around the supplied position.
			 * @param position Position in world space to set the weights around.
			 * @param radius Radius of the circular area.
			 * @param weights New weights to set.
			 */
			virtual void setWeights(const D3DXVECTOR3 &position, float radius, const D3DXVECTOR4 &weights);

			/**
			 * Increases the weights by an offset for all vertices in tiles in a circular area around the supplied position.
			 * @param position Position in world space to set the weights around.
			 * @param radius Radius of the circular area.
			 * @param weightOffset Offset to add to the weights.
			 */
			virtual void increaseWeights(const D3DXVECTOR3 &position, float radius, const D3DXVECTOR4 &weightOffset);

			/**
			 * Called when the weight map SRV has been successfully updated.
			 */
			void SRVUpdated();

			/**
			 * Remove all tile instances.
			 */
			void removeTiles();

			/**
			 * Sets the center position of the world plane.
			 * @param position New value for mPosition.
			 */
			void setPosition(const D3DXVECTOR3 &position);

			/**
			 * Sets the number of tiles along the X-axis.
			 * @param tileX New value for mTileX.
			 */
			void setTileX(int tileX);

			/**
			 * Sets the number of tiles along the Z-axis.
			 * @param tileZ New value for mTileZ.
			 */
			void setTileZ(int tileZ);

			/**
			 * Sets the size of the tiles along the X-axis.
			 * @param tileSizeX New value for mTileSizeX.
			 */
			void setTileSizeX(float tileSizeX);

			/**
			 * Sets the size of the tiles along the Z-axis.
			 * @param tileSizeZ New value for mTileSizeZ.
			 */
			void setTileSizeZ(float tileSizeZ);

			/**
			 * Retrieves the center position of the world plane.
			 * @return Returns mPosition.
			 */
			inline const D3DXVECTOR3 &getPosition() const {return mPosition;}

			/**
			 * Retrieves the number of tiles along the X-axis.
			 * @return Returns mTileX.
			 */
			inline int getTileX() const {return mTileX;}

			/**
			 * Retrieves the number of tiles along the Z-axis.
			 * @return Returns mTileZ.
			 */
			inline int getTileZ() const {return mTileZ;}

			/**
			 * Retrieves the size of the tiles along the X-axis.
			 * @return Returns mTileSizeX.
			 */
			inline float getTileSizeX() const {return mTileSizeX;}

			/**
			 * Retrieves the size of the tiles along the Z-axis.
			 * @return Returns mTileSizeZ.
			 */
			inline float getTileSizeZ() const {return mTileSizeZ;}

			/**
			 * Retrieves the number of tiles.
			 * @return Returns mTileCount.
			 */
			inline int getTileCount() const {return mTileCount;}

			/**
			 * Retrieves the weight map SRV.
			 */
			inline ID3D10ShaderResourceView *getWeightMapSRV(){return mpWeightMapSRV;}

			/**
			 * Retrieves the SRV update required flag.
			 *  True if the GFX module should update the weight map SRV.
			 * @return Returns mFlagSRVUpdateRequired.
			 */
			inline bool getFlagSRVUpdateRequired() const {return mFlagSRVUpdateRequired;}

			/**
			 * Retrieves a pointer to the tile array.
			 * @return Returns mpTiles.
			 */
			inline InstanceShapePlane **getTiles(){return mpTiles;}

			/**
			 * Sets all weights in the plane.
			 * @param weights Vector containing weights for all vertices.
			 */
			void setWeights(const std::vector<D3DXVECTOR4> &weights);

			/**
			 * Retrieves the weights in vector form.
			 * @param weights Outgoing reference to the weight vector.
			 */
			void getWeights(std::vector<D3DXVECTOR4> &weights);

		protected:
			/**
			 * Updates all tiles.
			 */
			void updateTiles();

			/**
			 * Updates the weight map for changed tiles.
			 * @return Returns true if successful.
			 */
			bool updateTileWeightMap();

			/**
			 * Creates the plane instance base if it doesn't exist.
			 */
			void createPlaneBase();

			/**
			 * Creates all tile instances.
			 */
			void createTiles();

			void createWeightMap();

		private:
			D3DXVECTOR3 mPosition;
			int mTileX;
			int mTileZ;
			float mTileSizeX;
			float mTileSizeZ;
			int mTileCount;
			InstanceShapePlane **mpTiles;
			ID3D10Texture2D *mpWeightMap;
			ID3D10ShaderResourceView *mpWeightMapSRV;
			bool mFlagUpdateRequired;
			bool mFlagWeightUpdateRequired;
			bool mFlagSRVUpdateRequired;
	};
};

#endif