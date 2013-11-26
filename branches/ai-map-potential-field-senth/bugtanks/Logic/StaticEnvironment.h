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

#ifndef __STATICENVIRONMENT_H__
#define __STATICENVIRONMENT_H__

#include "../Utilities/Macros.h"
#include "StaticActor.h"

namespace logic
{
	
class StaticEnvironment : public StaticActor
{
public:
	enum StaticEnvironmentTypes
	{
		StaticEnvironmentType_Invalid = 0,
		StaticEnvironmentType_First = 1,
		StaticEnvironmentType_CityMall = 1,
		StaticEnvironmentType_CornerStore,
		StaticEnvironmentType_GreenOffice,
		StaticEnvironmentType_LBuidling,
		StaticEnvironmentType_Skyscraper,
		StaticEnvironmentType_StandAloneBuilding,
		StaticEnvironmentType_Taipei,
		StaticEnvironmentType_TallGlassBuilding,
		StaticEnvironmentType_TurningTorso,
		StaticEnvironmentType_WindowBuilding,
		StaticEnvironmentType_WTFBuilding,
		StaticEnvironmentType_Lim
	};

	StaticEnvironment(int networkId, StaticEnvironmentTypes type, utilities::Position pos, utilities::Direction dir, utilities::Vec3Float scale);
	~StaticEnvironment();

	void update(float deltaTime);

	/**
	 * Returns the subtype of the actor.
	 * @return Return the subtype.
	 */
	inline virtual int getActorSubType() const {return mType;}

	std::string getName(void){return "Building";}

private:
	StaticEnvironmentTypes mType;
};
}

std::istream& operator>>(std::istream &in, logic::StaticEnvironment::StaticEnvironmentTypes& staticEnvironmentType);

#endif
