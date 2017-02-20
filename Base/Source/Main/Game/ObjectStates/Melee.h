#ifndef _MELEE_H_
#define _MELEE_H_

#include "../../Engine/State//StateManager.h"
#include "../../Game/Objects/Weapons/Melee/MeleeWeapon.h"

class MeleeStateManager : public StateManager
{
public:
	MeleeStateManager();
	virtual ~MeleeStateManager();

	virtual void Init();
	virtual void Update(const float &dt);
	virtual void Exit();
	virtual void SendMessage(const string&){};
	virtual void Render(){};
	MeleeWeapon* MWeapon;

private:
	
	bool DeathCheck(CharacterEntity*);
	bool WithinRange(CharacterEntity*, CharacterEntity*);
	CharacterEntity* FindNearestEnemy(CharacterEntity*);
	void RandomizeMovement(CharacterEntity*);
	void FixedMovement(CharacterEntity*);
	void MeleeTarget(CharacterEntity*);
	void MoveToTargetEnemy(CharacterEntity*);
	bool MoveToTargetFriend();

	virtual void ReactToMessage();
};

#endif