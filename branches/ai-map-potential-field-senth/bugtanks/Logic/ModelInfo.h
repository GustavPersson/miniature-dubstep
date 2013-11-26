/**
 * @file
 * @author Jonathan Udd <jonis.x@gmail.com>
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

#ifndef __MODELINFO_H__
#define __MODELINFO_H__

#include "../Utilities/Macros.h"
#include <map>
#include <vector>
#include <string>
#include "../GFX/DX10Module.h"

namespace logic
{	
	class ActorHandler;


/**
 * This class is used to bind models to actorTypes.
 */

class ModelInfo
{
public:
/**
 * Returns the instance of this object.
 * @return The instance of this object.
 */
	static ModelInfo* getInstance();

/**
 * The destroctor.
 */
	~ModelInfo();

/**
 * Bind a model to a actortype.
 */
	void bindMeshToActorType(int actorType, int actorSubType, const std::string& modelName,
		const char *geometryFilePath, const char *materialFilePath, const char *weightFilePath);

/**
 * Get the modelname that are bound to a actortype.
 */
	const char* getModelName(int actorType, int actorSubType) const;


private:
/**
 * The constructor.
 */
	ModelInfo();

	static ModelInfo *mpsInstance;
	std::map<int, std::map<int, std::string> > mModelNames;
	std::map<std::string, bool> mLoadedModels;
	gfx::DX10Module *mpDXModule;
	int mNrOfModels;
};
}
#endif