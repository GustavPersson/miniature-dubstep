/**
 * @file
 * @author Viktor Svensson <arkangel88@gmail.com>
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
 * Creates an enumeration with playable sounds
 */

#ifndef SOUND_H
#define SOUND_H

namespace sound
{
enum Sounds 
{

//Menu Sound
	Sound_MenuButtonRollover,  
	Sound_MenuButtonPressedDefault, 
	Sound_MenuError,
	Sound_MenuConnect,

//Player Sounds
	Sound_PlayerTankLongRangeMovementStop,
	Sound_PlayerTankCloseCombatMovementStop,
	Sound_PlayerTankDeath,
	Sound_PlayerTankEject,
	Sound_PlayerTankCollisionHouse1,
	Sound_PlayerTankCollisionHouse2,
	Sound_PlayerTankCollisionLargeEnemy,
	Sound_PlayerTankCollisionTank,
	Sound_PlayerTankHitMelee,
	Sound_PlayerTankHitRangedFire, 
	Sound_PlayerTankHitRangedAcid, 
	Sound_PlayerTankHitRangedPlasma,
	Sound_PlayerFootDeath,
	Sound_PlayerFootEnterTank,
	Sound_PlayerFootHit,

//Enemy Sounds
	Sound_EnemyAntDeath,
	Sound_EnemyAntSpawn,
	Sound_EnemyAntAttackRanged,
	Sound_EnemyAntAttackRangedImpact, 
	Sound_EnemyAntAttackMelee,
	Sound_EnemyAntAttackExplosion, 
	Sound_EnemyAntHit,

	Sound_EnemySpiderDeath,
	Sound_EnemySpiderSpawn,
	Sound_EnemySpiderAttackRanged,
	Sound_EnemySpiderAttackRangedImpact, 
	Sound_EnemySpiderAttackMelee,
	Sound_EnemySpiderJumpAttack,
	Sound_EnemySpiderHit,

	Sound_EnemyBeetleDeath,
	Sound_EnemyBeetleSpawn,
	Sound_EnemyBeetleAttackRanged,
	Sound_EnemyBeetleAttackRangedImpact,
	Sound_EnemyBeetleAttackMelee,
	Sound_EnemyBeetleHit,

//Civilian Sounds
	Sound_CivilianDeath,
	Sound_CivilianStunned,
	Sound_CivilianScreamFleeing,
	Sound_CivilianScreamHijack,
	Sound_CivilianEnterTank,
	Sound_CivilianHit,

//Weapon Sounds
	Sound_WeaponMinigunImpact,
	Sound_WeaponMinigunStart,
	Sound_WeaponMinigunEnd,

	Sound_WeaponFlamethrowerImpact,
	Sound_WeaponFlamethrowerStart,
	Sound_WeaponFlamethrowerEnd,

	Sound_WeaponPulsecannonFire,
	Sound_WeaponPulsecannonImpact,
	Sound_WeaponPulsecannonStart,
	Sound_WeaponPulsecannonEnd,

	Sound_WeaponCannonFire,
	Sound_WeaponCannonImpact,

	Sound_WeaponGausscannonFire,
	Sound_WeaponGausscannonImpact,
	Sound_WeaponGausscannonStart,

	Sound_WeaponBeamcannonFire,
	Sound_WeaponBeamcannonImpact,

	Sound_WeaponShotgunFire,
	Sound_WeaponShotgunImpact,

	Sound_WeaponPedestrianGunFire,
	Sound_WeaponPedestrianGunImpact,

//Other Sounds
	Sound_OtherExplosionDefault, 
	Sound_EnemyEnterBuilding, 


//Loop sounds
	Sound_PlayerTankLongRangeMovementMoving,
	Sound_PlayerTankCloseCombatMovementMoving,
	Sound_PlayerTankLongRangeMovementRotation,
	Sound_PlayerTankCloseCombatMovementRotation,
	Sound_PedestrianMovement, 
	Sound_EnemyAntMovement,
	Sound_EnemySpiderMovement, 
	Sound_EnemyBeetleMovement,
	Sound_PlayerTankTurretRotation,
	Sound_WeaponMinigunFire,
	Sound_WeaponFlamethrowerFire,


//Music (Streams)
	Music_Menu,
	Music_Gameplay,
	Music_Credits,
	Music_Space,
};

enum Dialogue
{
//Dialogue (Streams)
//	DIALOGUE_GENERAL_
//	DIALOGUE_HANDLER_
};

enum Channel
{
	Channel_Movement, 
	Channel_Rotation, 
	Channel_Attack,
};

}
#endif
