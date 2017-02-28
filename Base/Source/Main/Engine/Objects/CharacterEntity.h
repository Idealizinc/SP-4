/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 11 November 2016 13:00 PM
File Name     : CharacterEntity.h
Purpose       : Defines a base character class
*/

#ifndef _CHARACTER_ENTITY_H
#define _CHARACTER_ENTITY_H

#include "../Internal/Entity.h"
#include "../State/StateManager.h"
#include "BaseObject.h"

#include "SpriteObject.h"

#include <set>

class StateManager;

class CharacterEntity : public SpriteObject
{
public:
	CharacterEntity();
	virtual ~CharacterEntity();

	virtual void Init();
	virtual void Render();
	virtual void Update(const float& dt);
	virtual void Exit();

	State* GetCurrentState();

	void AddToObjectList(BaseObject*);
	bool RemoveFromObjectList(string);

	CharacterEntity* TargetEnemy = nullptr;
	BaseObject* TargetItem = nullptr;

	Vector3 LookVector;

	int HealthPoints;
	int MaxHealthPoints;
	std::vector<float> InternalTimerVector;
	float WalkSpeed;
	float DetectionRadius;
	int FieldOfView;
	float HealRate;
	int RaceType;

private:
	
protected:
	StateManager* InternalStateManager;
	std::set<BaseObject*> CharacterObjectList;

};

#endif //_CHARACTER_ENTITY_H