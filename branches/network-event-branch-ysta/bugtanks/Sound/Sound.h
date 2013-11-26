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
	Sound_MenuButtonRullover,  
	Sound_MenuButtonPressedDefault, 
	Sound_MenuButtonPressedUpgradeWeapon,
	Sound_MenuButtonPressedBuyWeapon,
	Sound_MenuError,
	Sound_MenuConnect,

//Player Sounds
	Sound_PlayerTankMovementStart,
	Sound_PlayerTankMovementStop,
	Sound_PlayerTankDeath,
	Sound_PlayerTankEject,
	Sound_PlayerTankCollisionHouse,
	Sound_PlayerTankCollisionLargeEnemy,
	Sound_PlayerFootDeath,

//Enemy Sounds
	Sound_EnemyAntDeathTank, 
	Sound_EnemyAntDeathFire,
	Sound_EnemyAntDeathDefault,
	Sound_EnemyAntSpawn,
	Sound_EnemyAntAttackRanged,
	Sound_EnemyAntAttackMelee,

	Sound_EnemySpiderDeathTank,
	Sound_EnemySpiderDeathFire,
	Sound_EnemySpiderDeathDefault,
	Sound_EnemySpiderSpawn,
	Sound_EnemySpiderAttackRanged,
	Sound_EnemySpiderAttackMelee,
	Sound_EnemySpiderJumpAttack,
	Sound_EnemySpiderChargeAttack,

	Sound_EnemyBeetleDeathTank,
	Sound_EnemyBeetleDeathFire,
	Sound_EnemyBeetleDeathDefault,
	Sound_EnemyBeetleSpawn,
	Sound_EnemyBeetleAttackRanged,
	Sound_EnemyBeetleAttackMelee,

//Civilian Sounds
	Sound_CivilianDeathBug,
	Sound_CivilianDeathTank,
	Sound_CivilianDeathFire,
	Sound_CivilianDeathDefault,
	Sound_CivilianStunned,
	Sound_CivilianScreamFleeing,
	Sound_CivilianScreamAngry,
	Sound_CivlianScreamHijack,

//Weapon Sounds
	Sound_WeaponMinigunImpact,
	Sound_WeaponMinigunStart,
	Sound_WeaponMinigunEnd,

	Sound_WeaponFlamethrowerImpact,
	Sound_WeaponFlamethrowerStart,
	Sound_WeaponFlamethrowerEnd,

	Sound_WeaponPulecannonFire,
	Sound_WeaponPulecannonImpact,
	Sound_WeaponPulecannonStart,
	Sound_WeaponPulecannonEnd,

	Sound_WeaponCannonFire,
	Sound_WeaponCannonImpact,
	Sound_WeaponCannonStart,
	Sound_WeaponCannonEnd,

	Sound_WeaponGausscannonFire,
	Sound_WeaponGausscannonImpact,
	Sound_WeaponGausscannonStart,
	Sound_WeaponGausscannonEnd,

	Sound_WeaponBeamcannonFire,
	Sound_WeaponBeamcannonImpact,
	Sound_WeaponBeamcannonStart,
	Sound_WeaponBeamcannonEnd,

	Sound_WeaponShotgunFire,
	Sound_WeaponShotgunImpact,
	Sound_WeaponShotgunStart,
	Sound_WeaponShotgunEnd,

//Other Sounds
	Sound_OtherExplosionCar, 
	Sound_OtherExplosionDefault, 


//Loop sounds
	Sound_PlayerTankLongRangeMovementMoving,
	Sound_PlayerTankCloseRangeMovementMoving,
	Sound_PlayerTankLongRangeMovementRotation,
	Sound_PlayerTankCloseRangeMovementRotation,
	Sound_PlayerFootMovement,
	Sound_EnemyAntMovement,
	Sound_EnemySpiderAntMovement, 
	Sound_EnemyBeetleAntMovement,
	Sound_CivilianMovement,
	Sound_WeaponMinigunFire,
	Sound_WeaponFlamethrowerFire,


//Music (Streams)
	Music_Gameplay,
	Music_Defeat,
	Music_Victory,
	Music_Menu,
};

enum Dialogue
{
//Dialogue (Streams)
//	DIALOGUE_GENERAL_
//	DIALOGUE_HANDLER_
};

enum Channel
{
	//loopable sounds
	Channel_Movement, 
	Channel_Rotation, 
	Channel_Attack,
};

}
#endif
