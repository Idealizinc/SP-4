#ifndef _MAGIC_STATE_MANAGER_H_
#define _MAGIC_STATE_MANAGER_H_

#include "../../Engine/State/StateManager.h"
#include "../../Game/Objects/Weapons/Magic/MagicWeapon.h"

class MagicStateManager : public StateManager
{
public:
	MagicStateManager();
	virtual ~MagicStateManager();

	virtual void Init();
	virtual void Update(const float &dt);
	virtual void Exit();
	virtual void SendMessage(const string&){};
	virtual void Render(){};
	MagicWeapon* MWeapon;

private:
	bool DeathCheck(CharacterEntity*);
	bool WithinMagic(CharacterEntity*, CharacterEntity*);
	CharacterEntity* FindNearestEnemy(CharacterEntity*);
	void RandomizeMovement(CharacterEntity*);
	void FixedMovment(CharacterEntity*);
	void FireAtTarget(CharacterEntity*);
	bool MoveToTargetFriend();

	virtual void ReactToMessage();
};

#endif