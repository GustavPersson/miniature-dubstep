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

#ifndef __ACTOR_CREATE_INFO_H__
#define __ACTOR_CREATE_INFO_H__

#include "../Utilities/Macros.h"
#include <sstream>
#include "Actor.h"
#include "Tank.h"
#include "Enemy.h"
#include "Driver.h"
#include "Munitions.h"
#include "StaticEnvironment.h"

namespace logic
{
/**
* Base class holding information about an actor
*/
class CreateActorInfo
{
public:

	virtual void read(std::ostream& out) const;
	virtual void write(std::istream& in);

	utilities::Position mPos;
	utilities::Direction mDir;
	int mNetworkId;
};

/**
* Holding Tank specific information
*/
class CreateTankInfo : public CreateActorInfo
{
public:

	virtual void read(std::ostream& out);
	virtual void write(std::istream& in);

	Tank::TankTypes mTankType;
	Actor::PlayerMovingInfo mPlayerMovingInfo;
};

/**
* Holding Enemy specific information
*/
class CreateEnemyInfo : public CreateActorInfo
{
public:

	virtual void read(std::ostream& out);
	virtual void write(std::istream& in);

	Enemy::EnemyTypes mEnemyType;
};

/**
* Holding Driver specific information
*/
class CreateDriverInfo : public CreateActorInfo
{
public:

	virtual void read(std::ostream& out);
	virtual void write(std::istream& in);

	Actor::PlayerMovingInfo mPlayerMovingInfo;
};
/**
/* Holding Spectator Specific Information
 */
class CreateSpectatorInfo : public CreateActorInfo
{
public:

	virtual void read(std::ostream& out);
	virtual void write(std::istream& in);

	Actor::PlayerMovingInfo mPlayerMovingInfo;
};

/**
* Holding Munition specific information
*/
class CreateMunitionInfo : public CreateActorInfo
{
public:

	virtual void read(std::ostream& out);
	virtual void write(std::istream& in);

	Munitions::MunitionTypes mMunitionType;
	KeyType mCreatorId;
};

/**
* Holding StaticEnvironment specific information
*/
class CreateStaticEnvironmentInfo : public CreateActorInfo
{
public:

	virtual void read(std::ostream& out);
	virtual void write(std::istream& in);

	StaticEnvironment::StaticEnvironmentTypes mStaticEnvironmentType;
	utilities::Vec3Float mScale;
};

}

#endif