#ifndef _RANGE_STATE_MANAGER_H_
#define _RANGE_STATE_MANAGER_H_

#include "../../Engine/State/StateManager.h"
#include "../../Game/Objects/Weapons/Ranged/RangeWeapon.h"

class RangeStateManager : public StateManager
{
public:
	RangeStateManager();
	virtual ~RangeStateManager();

	virtual void Init();
	virtual void Update(const float &dt);
	virtual void Exit();
	virtual void SendMessage(const string&){};
	virtual void Render(){};
	RangeWeapon* RWeapon;

private:
	bool DeathCheck(CharacterEntity*);
	bool WithinRange(CharacterEntity*, CharacterEntity*);
	CharacterEntity* FindNearestEnemy(CharacterEntity*);
	void RandomizeMovement(CharacterEntity*);
	void FireAtTarget(CharacterEntity*);
	bool MoveToTargetFriend();

	virtual void ReactToMessage();
};

#endif