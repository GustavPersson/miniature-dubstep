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

#include "ActorCreateInfo.h"

using namespace logic;

// CreateActorInfo funcions
void CreateActorInfo::read(std::ostream &out) const
{
	out << mPos << " " << mDir << " " << mNetworkId << " ";
}

void CreateActorInfo::write(std::istream &in)
{
	in >> mPos;
	in >> mDir;
	in >> mNetworkId;
}

// CreateTankInfo funcions
void CreateTankInfo::read(std::ostream& out)
{
	CreateActorInfo::read(out);
	out << mTankType << " " << mPlayerMovingInfo << " ";
}

void CreateTankInfo::write(std::istream& in)
{
	CreateActorInfo::write(in);
	in >> mTankType;
	in >> mPlayerMovingInfo;
}

// CreateEnemyInfo funcions
void CreateEnemyInfo::read(std::ostream& out)
{
	CreateActorInfo::read(out);
	out << mEnemyType << " ";
}

void CreateEnemyInfo::write(std::istream& in)
{
	CreateActorInfo::write(in);
	in >> mEnemyType;
}

// CreateDriverInfo funcions
void CreateDriverInfo::read(std::ostream& out)
{
	CreateActorInfo::read(out);
	out << mPlayerMovingInfo << " ";
}

void CreateDriverInfo::write(std::istream& in)
{
	CreateActorInfo::write(in);
	in >> mPlayerMovingInfo;
}

// CreateSpectatorInfo funcions
void CreateSpectatorInfo::read(std::ostream& out)
{
	CreateActorInfo::read(out);
	out << mPlayerMovingInfo << " ";
}

void CreateSpectatorInfo::write(std::istream& in)
{
	CreateActorInfo::write(in);
	in >> mPlayerMovingInfo;
}

// CreateMunitionInfo funcions
void CreateMunitionInfo::read(std::ostream& out)
{
	CreateActorInfo::read(out);
	out << mMunitionType << " " << mCreatorId << " ";
}

void CreateMunitionInfo::write(std::istream& in)
{
	CreateActorInfo::write(in);
	in >> mMunitionType;
	in >> mCreatorId;
}

// CreateStaticEnvironmentInfo funcions
void CreateStaticEnvironmentInfo::read(std::ostream& out)
{
	CreateActorInfo::read(out);
	out << mStaticEnvironmentType << " " << mScale << " ";
}

void CreateStaticEnvironmentInfo::write(std::istream& in)
{
	CreateActorInfo::write(in);
	in >> mStaticEnvironmentType;
	in >> mScale;
}