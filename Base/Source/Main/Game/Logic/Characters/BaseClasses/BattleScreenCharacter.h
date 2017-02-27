#ifndef _BATTLE_SCREEN_CHARACTER_H
#define _BATTLE_SCREEN_CHARACTER_H

/*
Copyright (c) Lim Rui An, Ryan 2017

Creator(s)    : Lim Rui An, Ryan
Creation Date : 25 February 2017 16:21 PM
File Name     : BattleScreenCharacter.h
Purpose       : Defines the base statistics for a battle screen character
*/

#include "../../../../Engine/Objects/CharacterEntity.h"
#include "../../UnitData/UnitType.h" 
#include "../../UnitData/UnitRace.h"
#include "../../Weapons/Weapon.h"
#include "../../Terrain/Terrain.h"
#include "../../../Systems/GameLogicSystem.h"

/*
Description
The BattleScreenCharacter acts as a generic entity that can be used to represent any character in the battle scene.
This class will hold all shared variables that are required by battle screen character classes.
Collision and Partition based functions within this class assume that the character is placed in the correct scene.
As they utillize the Partition based objects to check collision against.
*/

class BattleScreenCharacter : public CharacterEntity
{
public:
	// Mains
	BattleScreenCharacter();
	virtual ~BattleScreenCharacter();

	virtual void Init();
	virtual void Update(const float& dt);
	virtual void Render();	
	virtual void Exit();
	
	// Miscellaneous
	virtual void InitiallizeCharacter(UnitType* Type, UnitRace* Race, Terrain* Terrain, bool GetsAdvantage);
	Weapon* InternalWeapon;
	float WaitTime;
	bool IsPlayerCharacter = false;
	int CharacterFaction = -1;

protected:
	float InterpolationSpeed = 5.f;
	// Functions
	void SetCharacter(UnitType* Type, UnitRace* Race, Terrain* Terrain, bool GetsAdvantage);
	// Collision
	void HandlePartitionCollision();
	void HandleCharacterCollision();
	void HandleProjectileCollision();

};

#endif // _BATTLE_SCREEN_CHARACTER_H