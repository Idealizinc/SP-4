#ifndef _MELEE_CHARACTER_H_
#define _MELEE_CHARACTER_H_

#include "../../ObjectStates/Melee.h"
#include "../../../Engine/Objects/CharacterEntity.h"
#include "../UnitData/UnitType.h"
#include "../UnitData/UnitRace.h"

class MeleeCharacter : public CharacterEntity
{
public:
	 
	MeleeCharacter();
	virtual ~MeleeCharacter();

	virtual void Init();
	virtual void Update(double dt);
	void Exit();

	float InternalTimer2;
	float WaitTime;

	void SetCharacter(UnitType* Type, UnitRace* Race);

	CharacterEntity* TargetFriend;
	WeaponType* WT;

private:

protected:
};

#endif