/**
 * @file
 * @author Dennis Jönsson <zelebsel@gmail.com>
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
 * 
 * @section DESCRIPTION
 *
 * Subclass to munitions, it holds the beamcannons shot
 */

#include "Beam.h"

using namespace logic;

Beam::Beam(utilities::Position position, int efficiency, double damage) :
	Munitions(position, efficiency, damage)
{
}

Beam::~Beam()
{
}

void Beam::update(float deltaTime)
{
	Actor::mPosition.x += (BEAM_MOVE_SPEED * Actor::mDirection.x * deltaTime);
	Actor::mPosition.y += (BEAM_MOVE_SPEED * Actor::mDirection.y * deltaTime);
	Actor::mPosition.z += (BEAM_MOVE_SPEED * Actor::mDirection.z * deltaTime);
}

void Beam::handleEventId(EventData const &eventMessage)
{

}