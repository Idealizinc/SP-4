#ifndef _CHARACTER_STATE_MANAGER_H
#define _CHARACTER_STATE_MANAGER_H

/*
Copyright (c) Lim Rui An, Ryan 2017

Creator(s)    : Lim Rui An, Ryan
Creation Date : 25 February 2017 16:21 PM
File Name     : CharacterStateManager.h
Purpose       : Defines the base state logic for a character that exists within the battle screen
*/

#include "../BaseClasses/BattleScreenCharacter.h"
#include "../../../../Engine/State/StateManager.h"

class CharacterStateManager : public StateManager
{
public:
	CharacterStateManager();
	virtual ~CharacterStateManager();

	virtual void Init();
	virtual void Update(const float& dt);
	virtual void Exit();
	virtual void SendMessage(const string&){};
	virtual void Render(){};

protected:
	BaseObject* TargetNode;

	// Internal Functions
	// State Machine
	virtual void Idle(const float& dt);
	virtual void Scout(const float& dt);
	virtual void Attack(const float& dt);
	virtual void Dead(const float& dt);

	// Miscellaneous
	bool DeathCheck(CharacterEntity*);
	bool WithinRange(CharacterEntity*, CharacterEntity*);
	void RandomizeMovement(CharacterEntity*);

	// Assumes that you already have a TargetEnemy to move towards
	BaseObject* FindTargetNode();
	CharacterEntity* FindClosestEnemy();
	unsigned int MaxViableTargets = 5;
};

#endif // _CHARACTER_STATE_MANAGER_H