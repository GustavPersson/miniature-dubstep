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

#include "ActorConstants.h"
#include "../Utilities/CustomGetPrivateProfile.cpp"
#include <shlobj.h>

using namespace logic;

/////////////////////////////
/// PLAYER CONSTANTS      ///
/////////////////////////////

//CloseCombatTank Constants
float ActorConstants::CLOSE_COMBAT_TANK_HP					= 0.0f;
float ActorConstants::CLOSE_COMBAT_TANK_ARMOR				= 0.0f;
float ActorConstants::CLOSE_COMBAT_TANK_ROTATE_SPEED		= 0.0f;
float ActorConstants::CLOSE_COMBAT_TANK_ACCELERATION		= 0.0f;
float ActorConstants::CLOSE_COMBAT_TANK_FRICTION_HIGH_SPEED	= 0.0f;
float ActorConstants::CLOSE_COMBAT_TANK_MAX_SPEED			= 0.0f;

//LongRangeTank Constants
float ActorConstants::LONG_RANGE_TANK_HP					= 0.0f;
float ActorConstants::LONG_RANGE_TANK_ARMOR					= 0.0f;
float ActorConstants::LONG_RANGE_TANK_ROTATE_SPEED			= 0.0f;
float ActorConstants::LONG_RANGE_TANK_ACCELERATION			= 0.0f;
float ActorConstants::LONG_RANGE_TANK_FRICTION_HIGH_SPEED	= 0.0f;
float ActorConstants::LONG_RANGE_TANK_MAX_SPEED				= 0.0f;

//Driver Constants
float ActorConstants::DRIVER_HP				= 0.0f;
float ActorConstants::DRIVER_SPEED			= 0.0f;
float ActorConstants::DRIVER_ROTATE_SPEED	= 0.0f;
float ActorConstants::DRIVER_STRAFE_SPEED	= 0.0f;
float ActorConstants::DRIVER_EJECT_RANGE    = 0.0f;
float ActorConstants::DRIVER_EJECT_SPEED    = 0.0f;

//Spectator Constants
float ActorConstants::SPECTATOR_SPEED = 0.0f;

/////////////////////////////
/// ENEMY CONSTANTS       ///
/////////////////////////////

//Ant Constants
float ActorConstants::ANT_HP		= 0.0f;
float ActorConstants::ANT_SPEED		= 0.0f;
float ActorConstants::ANT_RANGE		= 0.0f;
float ActorConstants::ANT_DAMAGE	= 0.0f;

//KamikazeAnt Constants
float ActorConstants::KAMIKAZEANT_HP		= 0.0f;
float ActorConstants::KAMIKAZEANT_SPEED		= 0.0f;
float ActorConstants::KAMIKAZEANT_RANGE		= 0.0f;
float ActorConstants::KAMIKAZEANT_DAMAGE	= 0.0f;
float ActorConstants::KAMIKAZEANT_RADIUS	= 0.0f;

//FireAnt Constants
float ActorConstants::FIREANT_HP		= 0.0f;
float ActorConstants::FIREANT_SPEED		= 0.0f;
float ActorConstants::FIREANT_RANGE		= 0.0f;
float ActorConstants::FIREANT_DAMAGE	= 0.0f;

//Tarantula Constants
float ActorConstants::TARANTULA_HP		= 0.0f;
float ActorConstants::TARANTULA_SPEED	= 0.0f;
float ActorConstants::TARANTULA_RANGE	= 0.0f;
float ActorConstants::TARANTULA_DAMAGE	= 0.0f;

//BlackWidow Constants
float ActorConstants::BLACKWIDOW_HP		= 0.0f;
float ActorConstants::BLACKWIDOW_SPEED	= 0.0f;
float ActorConstants::BLACKWIDOW_RANGE	= 0.0f;
float ActorConstants::BLACKWIDOW_DAMAGE	= 0.0f;

//Jumper Constants
float ActorConstants::JUMPER_HP				= 0.0f;
float ActorConstants::JUMPER_SPEED			= 0.0f;
float ActorConstants::JUMPER_RANGE			= 0.0f;
float ActorConstants::JUMPER_DAMAGE			= 0.0f;
float ActorConstants::JUMPER_JUMP_SPEED		= 0.0f;
float ActorConstants::JUMPER_JUMP_RANGE_MAX	= 0.0f;
float ActorConstants::JUMPER_JUMP_RANGE_MIN	= 0.0f;

//Scarab Constants
float ActorConstants::SCARAB_HP		= 0.0f;
float ActorConstants::SCARAB_SPEED	= 0.0f;
float ActorConstants::SCARAB_RANGE	= 0.0f;
float ActorConstants::SCARAB_DAMAGE	= 0.0f;

//PlasmaBeetle Constants
float ActorConstants::PLASMABEETLE_HP		= 0.0f;
float ActorConstants::PLASMABEETLE_SPEED	= 0.0f;
float ActorConstants::PLASMABEETLE_RANGE	= 0.0f;
float ActorConstants::PLASMABEETLE_DAMAGE	= 0.0f;

/////////////////////////////
/// WEAPON CONSTANTS      ///
/////////////////////////////

//BeamCannon Constants
float ActorConstants::BEAMCANNON_DAMAGE		= 0.0f;
float ActorConstants::BEAMCANNON_COOLDOWN	= 0.0f;

//Cannon Constants
float ActorConstants::CANNON_DAMAGE		= 0.0f;
float ActorConstants::CANNON_COOLDOWN	= 0.0f;

//Flamethrower Constants
float ActorConstants::FLAMETHROWER_DAMAGE	= 0.0f;
float ActorConstants::FLAMETHROWER_COOLDOWN	= 0.0f;

//Gausscannon Constants
float ActorConstants::GAUSSCANNON_DAMAGE	= 0.0f;
float ActorConstants::GAUSSCANNON_COOLDOWN	= 0.0f;

//Minigun Constants
float ActorConstants::MINIGUN_DAMAGE	= 0.0f;
float ActorConstants::MINIGUN_COOLDOWN	= 0.0f;

//Pedestriangun Constants
float ActorConstants::PEDESTRIANGUN_DAMAGE		= 0.0f;
float ActorConstants::PEDESTRIANGUN_COOLDOWN	= 0.0f;

//PulseCannon Constants
float ActorConstants::PULSECANNON_DAMAGE	= 0.0f;
float ActorConstants::PULSECANNON_COOLDOWN	= 0.0f;

//Shotgun Constants
float ActorConstants::SHOTGUN_DAMAGE	= 0.0f;
float ActorConstants::SHOTGUN_COOLDOWN	= 0.0f;

/////////////////////////////
/// MUNITION CONSTANTS    ///
/////////////////////////////

//Bullet Constants
float ActorConstants::BULLET_SPEED = 0.0f;

//Pulse Constants
float ActorConstants::PULSE_SPEED = 0.0f;

//Fire Constants
float ActorConstants::FIRE_SPEED = 0.0f;

//Acid Constants
float ActorConstants::ACID_SPEED = 0.0f;

/////////////////////////////
/// MISC CONSTANTS        ///
/////////////////////////////

//Misc Constants
float ActorConstants::GRAVITATION = 0.0f;




/*
// Actors speed
float ActorConstants::ANT_MOVE_SPEED = 0.0f;
float ActorConstants::SPIDER_MOVE_SPEED = 0.0f;
float ActorConstants::BEETLE_MOVE_SPEED = 0.0f;
float ActorConstants::PLAYER_MOVE_SPEED = 0.0f;
float ActorConstants::TANK_MOVE_SPEED = 0.0f;
float ActorConstants::BULLET_MOVE_SPEED = 0.0f;
float ActorConstants::FLAME_MOVE_SPEED = 0.0f;

float ActorConstants::BEAM_MOVE_SPEED = 0.0f;
float ActorConstants::PULSE_MOVE_SPEED = 0.0f;
float ActorConstants::FIRE_MOVE_SPEED = 0.0f;
float ActorConstants::PLASMA_MOVE_SPEED = 0.0f;
float ActorConstants::ACID_MOVE_SPEED = 0.0f;

float ActorConstants::GRAVITATION = 0.0f;

float ActorConstants::JUMPER_JUMP_RANGE_MAX = 0.0f;
float ActorConstants::JUMPER_JUMP_RANGE_MIN = 0.0f;
float ActorConstants::JUMPER_JUMP_SPEED = 0.0f;
float ActorConstants::FIREANT_RANGE = 0.0f;
float ActorConstants::BLACKWIDOW_RANGE = 0.0f;
float ActorConstants::PLASMABEETLE_RANGE = 0.0f;
*/
void ActorConstants::setConstants(const char *path)
{
	/////////////////////////////
	/// PLAYER CONSTANTS      ///
	/////////////////////////////

	//CloseCombatTank Constants
	CLOSE_COMBAT_TANK_HP = ::GetPrivateProfileFloat(
		"CLOSE_COMBAT_TANK_CONSTANTS", "Close_Combat_Tank_Hp", 0, path);

	CLOSE_COMBAT_TANK_ARMOR = ::GetPrivateProfileFloat(
		"CLOSE_COMBAT_TANK_CONSTANTS", "Close_Combat_Tank_Armor", 0, path);

	CLOSE_COMBAT_TANK_ROTATE_SPEED = ::GetPrivateProfileFloat(
		"CLOSE_COMBAT_TANK_CONSTANTS", "Close_Combat_Tank_Rotate_Speed", 0, path);

	CLOSE_COMBAT_TANK_ACCELERATION = ::GetPrivateProfileFloat(
		"CLOSE_COMBAT_TANK_CONSTANTS", "Close_Combat_Tank_Acceleration", 0, path);

	CLOSE_COMBAT_TANK_FRICTION_HIGH_SPEED = ::GetPrivateProfileFloat(
		"CLOSE_COMBAT_TANK_CONSTANTS", "Close_Combat_Tank_Friction_High_Speed", 0, path);

	CLOSE_COMBAT_TANK_MAX_SPEED = ::GetPrivateProfileFloat(
		"CLOSE_COMBAT_TANK_CONSTANTS", "Close_Combat_Tank_Max_Speed", 0, path);

	//LongRangeTank Constants
	LONG_RANGE_TANK_HP = ::GetPrivateProfileFloat(
		"LONG_RANGE_TANK_CONSTANTS", "Long_Range_Tank_Hp", 0, path);

	LONG_RANGE_TANK_ARMOR = ::GetPrivateProfileFloat(
		"LONG_RANGE_TANK_CONSTANTS", "Long_Range_Tank_Armor", 0, path);

	LONG_RANGE_TANK_ROTATE_SPEED = ::GetPrivateProfileFloat(
		"LONG_RANGE_TANK_CONSTANTS", "Long_Range_Tank_Rotate_Speed", 0, path);

	LONG_RANGE_TANK_ACCELERATION = ::GetPrivateProfileFloat(
		"LONG_RANGE_TANK_CONSTANTS", "Long_Range_Tank_Acceleration", 0, path);

	LONG_RANGE_TANK_FRICTION_HIGH_SPEED = ::GetPrivateProfileFloat(
		"LONG_RANGE_TANK_CONSTANTS", "Long_Range_Tank_Friction_High_Speed", 0, path);

	LONG_RANGE_TANK_MAX_SPEED = ::GetPrivateProfileFloat(
		"LONG_RANGE_TANK_CONSTANTS", "Long_Range_Tank_Max_Speed", 0, path);

	//Driver Constants
	DRIVER_HP = ::GetPrivateProfileFloat(
		"DRIVER_CONSTANTS", "Driver_Hp", 0, path);

	DRIVER_SPEED = ::GetPrivateProfileFloat(
		"DRIVER_CONSTANTS", "Driver_Speed", 0, path);

	DRIVER_ROTATE_SPEED = ::GetPrivateProfileFloat(
		"DRIVER_CONSTANTS", "Driver_Rotate_Speed", 0, path);

	DRIVER_STRAFE_SPEED = ::GetPrivateProfileFloat(
		"DRIVER_CONSTANTS", "Driver_Strafe_Speed", 0, path);

	DRIVER_EJECT_SPEED = ::GetPrivateProfileFloat(
		"DRIVER_CONSTANTS", "Driver_Eject_Speed", 0, path);

	DRIVER_EJECT_RANGE = ::GetPrivateProfileFloat(
		"DRIVER_CONSTANTS", "Driver_Eject_Range", 0, path);

	//Spectator Constants
	SPECTATOR_SPEED = ::GetPrivateProfileFloat(
		"SPECTATOR_CONSTANTS", "Spectator_Speed", 0, path);

	/////////////////////////////
	/// ENEMY CONSTANTS       ///
	/////////////////////////////

	//Ant Constants
	ANT_HP = ::GetPrivateProfileFloat(
		"ANT_CONSTANTS", "Ant_Hp", 0, path);

	ANT_SPEED = ::GetPrivateProfileFloat(
		"ANT_CONSTANTS", "Ant_Speed", 0, path);

	ANT_RANGE = ::GetPrivateProfileFloat(
		"ANT_CONSTANTS", "Ant_Range", 0, path);

	ANT_DAMAGE = ::GetPrivateProfileFloat(
		"ANT_CONSTANTS", "Ant_Damage", 0, path);

	//KamikazeAnt Constants
	KAMIKAZEANT_HP = ::GetPrivateProfileFloat(
		"KAMIKAZEANT_CONSTANTS", "Kamikazeant_Hp", 0, path);

	KAMIKAZEANT_SPEED = ::GetPrivateProfileFloat(
		"KAMIKAZEANT_CONSTANTS", "Kamikazeant_Speed", 0, path);

	KAMIKAZEANT_RANGE = ::GetPrivateProfileFloat(
		"KAMIKAZEANT_CONSTANTS", "Kamikazeant_Range", 0, path);

	KAMIKAZEANT_DAMAGE = ::GetPrivateProfileFloat(
		"KAMIKAZEANT_CONSTANTS", "Kamikazeant_Damage", 0, path);

	KAMIKAZEANT_RADIUS = ::GetPrivateProfileFloat(
		"KAMIKAZEANT_CONSTANTS", "Kamikazeant_Radius", 0, path);

	//FireAnt Constants
	FIREANT_HP = ::GetPrivateProfileFloat(
		"FIREANT_CONSTANTS", "Fireant_Hp", 0, path);

	FIREANT_SPEED = ::GetPrivateProfileFloat(
		"FIREANT_CONSTANTS", "Fireant_Speed", 0, path);

	FIREANT_RANGE = ::GetPrivateProfileFloat(
		"FIREANT_CONSTANTS", "Fireant_Range", 0, path);

	FIREANT_DAMAGE = ::GetPrivateProfileFloat(
		"FIREANT_CONSTANTS", "Fireant_Damage", 0, path);

	//Tarantula Constants
	TARANTULA_HP = ::GetPrivateProfileFloat(
		"TARANTULA_CONSTANTS", "Tarantula_Hp", 0, path);

	TARANTULA_SPEED = ::GetPrivateProfileFloat(
		"TARANTULA_CONSTANTS", "Tarantula_Speed", 0, path);

	TARANTULA_RANGE = ::GetPrivateProfileFloat(
		"TARANTULA_CONSTANTS", "Tarantula_Range", 0, path);

	TARANTULA_DAMAGE = ::GetPrivateProfileFloat(
		"TARANTULA_CONSTANTS", "Tarantula_Damage", 0, path);

	//BlackWidow Constants
	BLACKWIDOW_HP = ::GetPrivateProfileFloat(
		"BLACKWIDOW_CONSTANTS", "Blackwidow_Hp", 0, path);

	BLACKWIDOW_SPEED = ::GetPrivateProfileFloat(
		"BLACKWIDOW_CONSTANTS", "Blackwidow_Speed", 0, path);

	BLACKWIDOW_RANGE = ::GetPrivateProfileFloat(
		"BLACKWIDOW_CONSTANTS", "Blackwidow_Range", 0, path);

	BLACKWIDOW_DAMAGE = ::GetPrivateProfileFloat(
		"BLACKWIDOW_CONSTANTS", "Blackwidow_Damage", 0, path);

	//Jumper Constants
	JUMPER_HP = ::GetPrivateProfileFloat(
		"JUMPER_CONSTANTS", "Jumper_Hp", 0, path);

	JUMPER_SPEED = ::GetPrivateProfileFloat(
		"JUMPER_CONSTANTS", "Jumper_Speed", 0, path);

	JUMPER_RANGE = ::GetPrivateProfileFloat(
		"JUMPER_CONSTANTS", "Jumper_Range", 0, path);

	JUMPER_DAMAGE = ::GetPrivateProfileFloat(
		"JUMPER_CONSTANTS", "Jumper_Damage", 0, path);

	JUMPER_JUMP_SPEED = ::GetPrivateProfileFloat(
		"JUMPER_CONSTANTS", "Jumper_Jump_Speed", 0, path);

	JUMPER_JUMP_RANGE_MAX = ::GetPrivateProfileFloat(
		"JUMPER_CONSTANTS", "Jumper_Jump_Range_Max", 0, path);

	JUMPER_JUMP_RANGE_MIN = ::GetPrivateProfileFloat(
		"JUMPER_CONSTANTS", "Jumper_Jump_Range_Min", 0, path);

	//Scarab Constants
	SCARAB_HP = ::GetPrivateProfileFloat(
		"SCARAB_CONSTANTS", "Scarab_Hp", 0, path);

	SCARAB_SPEED = ::GetPrivateProfileFloat(
		"SCARAB_CONSTANTS", "Scarab_Speed", 0, path);

	SCARAB_RANGE = ::GetPrivateProfileFloat(
		"SCARAB_CONSTANTS", "Scarab_Range", 0, path);

	SCARAB_DAMAGE = ::GetPrivateProfileFloat(
		"SCARAB_CONSTANTS", "Scarab_Damage", 0, path);

	//PlasmaBeetle Constants
	PLASMABEETLE_HP = ::GetPrivateProfileFloat(
		"PLASMABEETLE_CONSTANTS", "Plasmabeetle_Hp", 0, path);

	PLASMABEETLE_SPEED = ::GetPrivateProfileFloat(
		"PLASMABEETLE_CONSTANTS", "Plasmabeetle_Speed", 0, path);

	PLASMABEETLE_RANGE = ::GetPrivateProfileFloat(
		"PLASMABEETLE_CONSTANTS", "Plasmabeetle_Range", 0, path);

	PLASMABEETLE_DAMAGE = ::GetPrivateProfileFloat(
		"PLASMABEETLE_CONSTANTS", "Plasmabeetle_Damage", 0, path);

	/////////////////////////////
	/// WEAPON CONSTANTS      ///
	/////////////////////////////

	//Beamcannon Constants
	BEAMCANNON_DAMAGE = ::GetPrivateProfileFloat(
		"BEAMCANNON_CONSTANTS", "Beamcannon_Damage", 0, path);

	BEAMCANNON_COOLDOWN = ::GetPrivateProfileFloat(
		"BEAMCANNON_CONSTANTS", "Beamcannon_Cooldown", 0, path);

	//Cannon Constants
	CANNON_DAMAGE = ::GetPrivateProfileFloat(
		"CANNON_CONSTANTS", "Cannon_Damage", 0, path);

	CANNON_COOLDOWN = ::GetPrivateProfileFloat(
		"CANNON_CONSTANTS", "Cannon_Cooldown", 0, path);

	//Flamethrower Constants
	FLAMETHROWER_DAMAGE = ::GetPrivateProfileFloat(
		"FLAMETHROWER_CONSTANTS", "Flamethrower_Damage", 0, path);

	FLAMETHROWER_COOLDOWN = ::GetPrivateProfileFloat(
		"FLAMETHROWER_CONSTANTS", "Flamethrower_Cooldown", 0, path);

	//Gausscannon Constants
	GAUSSCANNON_DAMAGE = ::GetPrivateProfileFloat(
		"GAUSSCANNON_CONSTANTS", "Gausscannon_Damage", 0, path);

	GAUSSCANNON_COOLDOWN = ::GetPrivateProfileFloat(
		"GAUSSCANNON_CONSTANTS", "Gausscannon_Cooldown", 0, path);

	//Minigun Constants
	MINIGUN_DAMAGE = ::GetPrivateProfileFloat(
		"MINIGUN_CONSTANTS", "Minigun_Damage", 0, path);

	MINIGUN_COOLDOWN = ::GetPrivateProfileFloat(
		"MINIGUN_CONSTANTS", "Minigun_Cooldown", 0, path);

	//Pedestriangun Constants
	PEDESTRIANGUN_DAMAGE = ::GetPrivateProfileFloat(
		"PEDESTRIANGUN_CONSTANTS", "Pedestriangun_Damage", 0, path);

	PEDESTRIANGUN_COOLDOWN = ::GetPrivateProfileFloat(
		"PEDESTRIANGUN_CONSTANTS", "Pedestriangun_Cooldown", 0, path);

	//PulseCannon Constants
	PULSECANNON_DAMAGE = ::GetPrivateProfileFloat(
		"PULSECANNON_CONSTANTS", "Pulsecannon_Damage", 0, path);

	PULSECANNON_COOLDOWN = ::GetPrivateProfileFloat(
		"PULSECANNON_CONSTANTS", "Pulsecannon_Cooldown", 0, path);

	//Shotgun Constants
	SHOTGUN_DAMAGE = ::GetPrivateProfileFloat(
		"SHOTGUN_CONSTANTS", "Shotgun_Damage", 0, path);

	SHOTGUN_COOLDOWN = ::GetPrivateProfileFloat(
		"SHOTGUN_CONSTANTS", "Shotgun_Cooldown", 0, path);

	/////////////////////////////
	/// MUNITION CONSTANTS    ///
	/////////////////////////////

	//Bullet Constants
	BULLET_SPEED = ::GetPrivateProfileFloat(
		"BULLET_CONSTANTS", "Bullet_Speed", 0, path);

	//Pulse Constants
	PULSE_SPEED = ::GetPrivateProfileFloat(
		"PULSE_CONSTANTS", "Pulse_Speed", 0, path);

	//Fire Constants
	FIRE_SPEED = ::GetPrivateProfileFloat(
		"FIRE_CONSTANTS", "Fire_Speed", 0, path);

	//Acid Constants
	ACID_SPEED = ::GetPrivateProfileFloat(
		"ACID_CONSTANTS", "Acid_Speed", 0, path);

	/////////////////////////////
	/// MISC CONSTANTS        ///
	/////////////////////////////

	//Misc Constants
	GRAVITATION = ::GetPrivateProfileFloat(
		"MISC_CONSTANTS", "Gravitation", 0, path);


















	/*ANT_MOVE_SPEED = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Ant_Speed", 0, path);

	SPIDER_MOVE_SPEED = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Spider_Speed", 0, path);

	BEETLE_MOVE_SPEED = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Beetle_Speed", 0, path);

	PLAYER_MOVE_SPEED = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Player_Speed", 0, path);

	BULLET_MOVE_SPEED = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Bullet_Speed", 0, path);

	FLAME_MOVE_SPEED = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Flame_Speed", 0, path);

	BEAM_MOVE_SPEED = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Beam_Speed", 0, path);

	PULSE_MOVE_SPEED = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Pulse_Speed", 0, path);

	FIRE_MOVE_SPEED = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Fire_Speed", 0, path);

	PLASMA_MOVE_SPEED = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Plasma_Speed", 0, path);

	ACID_MOVE_SPEED = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Acid_Speed", 0, path);
	
	GRAVITATION = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Gravitation", 0, path);

	JUMPER_JUMP_RANGE_MAX = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Jumper_Jump_Range_Max", 0, path);

	JUMPER_JUMP_RANGE_MIN = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Jumper_Jump_Range_Min", 0, path);

	JUMPER_JUMP_SPEED = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Jumper_Jump_Speed", 0, path);
	
	FIREANT_RANGE = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "Fireant_Range", 0, path);
	
	BLACKWIDOW_RANGE = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "BlackWidow_Range", 0, path);
	
	PLASMABEETLE_RANGE = ::GetPrivateProfileFloat(
		"ACTOR_CONSTANTS", "PlasmaBeetle_Range", 0, path);*/

}