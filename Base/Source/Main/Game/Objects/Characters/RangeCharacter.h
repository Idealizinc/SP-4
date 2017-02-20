#ifndef _RANGE_CHARACTER_H_
#define _RANGE_CHARACTER_H_

#include "../../ObjectStates/Range.h"
#include "../../../Engine/Objects/CharacterEntity.h"
#include "../UnitData/UnitType.h"
#include "../UnitData/UnitRace.h"

class RangeCharacter : public CharacterEntity
{
public:
	RangeCharacter();
	virtual ~RangeCharacter();

	virtual void Init();
	virtual void Update(double dt);
	void Exit();

	float InternalTimer2;
	float WaitTime;

	void SetCharacter(UnitType* Type, UnitRace* Race);

	CharacterEntity* TargetFriend;
	WeaponType* WT;
};

#endif