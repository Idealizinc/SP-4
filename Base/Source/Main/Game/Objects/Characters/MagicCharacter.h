#ifndef _MAGIC_CHARACTER_H_
#define _MAGIC_CHARACTER_H_

#include "../../ObjectStates/Magic.h"
#include "../../../Engine/Objects/CharacterEntity.h"
#include "../UnitData/UnitType.h"
#include "../UnitData/UnitRace.h"

class MagicCharacter : public CharacterEntity
{
public:
	MagicCharacter();
	virtual ~MagicCharacter();

	virtual void Init();
	virtual void Update(double dt);
	void Exit();

	float InternalTimer2;
	float WaitTime;
	float CastTime;
	float MaxCastTime;
	
	void SetCharacter(UnitType* Type, UnitRace* Race);

	CharacterEntity* TargetFriend;
	WeaponType* WT;
};

#endif