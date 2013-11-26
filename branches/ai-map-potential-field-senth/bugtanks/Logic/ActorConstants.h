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

#ifndef __ACTOR_CONSTANTS_H__
#define __ACTOR_CONSTANTS_H__

#include "../Utilities/Macros.h"
#include "../Utilities/Vectors.h"

namespace logic
{

struct ActorConstants
{
	/////////////////////////////
	/// PLAYER CONSTANTS      ///
	/////////////////////////////

	//CloseCombatTank Constants
	static float CLOSE_COMBAT_TANK_HP;
	static float CLOSE_COMBAT_TANK_ARMOR;
	static float CLOSE_COMBAT_TANK_ROTATE_SPEED;
	static float CLOSE_COMBAT_TANK_ACCELERATION;
	static float CLOSE_COMBAT_TANK_FRICTION_HIGH_SPEED;
	static float CLOSE_COMBAT_TANK_MAX_SPEED;

	//LongRangeTank Constants
	static float LONG_RANGE_TANK_HP;
	static float LONG_RANGE_TANK_ARMOR;
	static float LONG_RANGE_TANK_ROTATE_SPEED;
	static float LONG_RANGE_TANK_ACCELERATION;
	static float LONG_RANGE_TANK_FRICTION_HIGH_SPEED;
	static float LONG_RANGE_TANK_MAX_SPEED;

	//Driver Constants
	static float DRIVER_HP;
	static float DRIVER_SPEED;
	static float DRIVER_ROTATE_SPEED;
	static float DRIVER_STRAFE_SPEED;
	static float DRIVER_EJECT_SPEED;
	static float DRIVER_EJECT_RANGE;

	//Spectator Constants
	static float SPECTATOR_SPEED;

	/////////////////////////////
	/// ENEMY CONSTANTS       ///
	/////////////////////////////

	//Ant Constants
	static float ANT_HP;
	static float ANT_SPEED;
	static float ANT_RANGE;
	static float ANT_DAMAGE;

	//KamikazeAnt Constants
	static float KAMIKAZEANT_HP;
	static float KAMIKAZEANT_SPEED;
	static float KAMIKAZEANT_RANGE;
	static float KAMIKAZEANT_DAMAGE;
	static float KAMIKAZEANT_RADIUS;

	//FireAnt Constants
	static float FIREANT_HP;
	static float FIREANT_SPEED;
	static float FIREANT_RANGE;
	static float FIREANT_DAMAGE;

	//Tarantula Constants
	static float TARANTULA_HP;
	static float TARANTULA_SPEED;
	static float TARANTULA_RANGE;
	static float TARANTULA_DAMAGE;

	//BlackWidow Constants
	static float BLACKWIDOW_HP;
	static float BLACKWIDOW_SPEED;
	static float BLACKWIDOW_RANGE;
	static float BLACKWIDOW_DAMAGE;

	//Jumper Constants
	static float JUMPER_HP;
	static float JUMPER_SPEED;
	static float JUMPER_RANGE;
	static float JUMPER_DAMAGE;
	static float JUMPER_JUMP_SPEED;
	static float JUMPER_JUMP_RANGE_MAX;
	static float JUMPER_JUMP_RANGE_MIN;

	//Scarab Constants
	static float SCARAB_HP;
	static float SCARAB_SPEED;
	static float SCARAB_RANGE;
	static float SCARAB_DAMAGE;

	//PlasmaBeetle Constants
	static float PLASMABEETLE_HP;
	static float PLASMABEETLE_SPEED;
	static float PLASMABEETLE_RANGE;
	static float PLASMABEETLE_DAMAGE;

	/////////////////////////////
	/// WEAPON CONSTANTS      ///
	/////////////////////////////

	//BeamCannon Constants
	static float BEAMCANNON_DAMAGE;
	static float BEAMCANNON_COOLDOWN;

	//Cannon Constants
	static float CANNON_DAMAGE;
	static float CANNON_COOLDOWN;

	//Flamethrower Constants
	static float FLAMETHROWER_DAMAGE;
	static float FLAMETHROWER_COOLDOWN;

	//Gausscannon Constants
	static float GAUSSCANNON_DAMAGE;
	static float GAUSSCANNON_COOLDOWN;

	//Minigun Constants
	static float MINIGUN_DAMAGE;
	static float MINIGUN_COOLDOWN;

	//Pedestriangun Constants
	static float PEDESTRIANGUN_DAMAGE;
	static float PEDESTRIANGUN_COOLDOWN;

	//PulseCannon Constants
	static float PULSECANNON_DAMAGE;
	static float PULSECANNON_COOLDOWN;
	
	//Shotgun Constants
	static float SHOTGUN_DAMAGE;
	static float SHOTGUN_COOLDOWN;

	/////////////////////////////
	/// MUNITION CONSTANTS    ///
	/////////////////////////////

	//Bullet Constants
	static float BULLET_SPEED;

	//Pulse Constants
	static float PULSE_SPEED;

	//Fire Constants
	static float FIRE_SPEED;

	//Acid Constants
	static float ACID_SPEED;

	/////////////////////////////
	/// MISC CONSTANTS        ///
	/////////////////////////////

	//Misc Constants
	static float GRAVITATION;













	/*// Actors speed
	static float ANT_MOVE_SPEED;		//= 20.0f;
	static float SPIDER_MOVE_SPEED;		//= 0.2f;
	static float BEETLE_MOVE_SPEED;		//= 0.2f;
	static float PLAYER_MOVE_SPEED;		//= 30.0f;
	static float TANK_MOVE_SPEED;		//= 50.0f;

	static float BULLET_MOVE_SPEED;		//= 150.0f;
	static float FLAME_MOVE_SPEED;		//= 0.2f;
	static float BEAM_MOVE_SPEED;		//= 0.2f;
	static float PULSE_MOVE_SPEED;		//= 100.0f;
	static float FIRE_MOVE_SPEED;		//= 100.0f;
	static float PLASMA_MOVE_SPEED;		//= 0.2f;
	static float ACID_MOVE_SPEED;		//= 230.0f;

	static float GRAVITATION;

	static float JUMPER_JUMP_RANGE_MAX;
	static float JUMPER_JUMP_SPEED;
	static float JUMPER_JUMP_RANGE_MIN;
	static float FIREANT_RANGE;
	static float BLACKWIDOW_RANGE;
	static float PLASMABEETLE_RANGE;

	// Actors modelscale
	static utilities::Vec3Float CLOSE_COMBAT_TANK_SCALE;
	static utilities::Vec3Float LONG_RANGE_TANK_SCALE;
	static utilities::Vec3Float DRIVER_SCALE;
	static utilities::Vec3Float ANT_SCALE;
	static utilities::Vec3Float SPIDER_SCALE;
	static utilities::Vec3Float BEETLE_SCALE;
	static utilities::Vec3Float BULLET_SCALE;
	*/


	/**
	* Sets the value of the ActorConstants
	* @param gameOptions the gameOptions loaded at startup
	*/
	static void setConstants(const char *path);
};
}

#endif