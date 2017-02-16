/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 11 November 2016 10:51 AM
File Name     : StateManager.h
Purpose       : Defines the system that manages the logic for its respective states
*/

#ifndef _STATE_MANAGER_H
#define _STATE_MANAGER_H

#include "State.h"

#include "../Internal/System.h"
#include "../Objects/CharacterEntity.h"

#include <set>
using std::set;

#include <queue>
using std::queue;

class CharacterEntity;

// State System that allows state creation and usage
class StateManager : public System
{
public:
	StateManager();
	virtual ~StateManager();

	virtual void Init(){};
	virtual void Update(const float& dt){};
	virtual void Exit(){};
	virtual void SendMessage(const string&){};
	virtual void Render(){};

	// Getters And Setters
	bool SetCurrentState(const string&);
	State* GetCurrentState();
	State* GetPreviousState();
	int GetMaxHistory();
	void SetMaxHistory(const int&);
	void SetInternalCharacter(CharacterEntity*);
		
private:
	set<State*> AssignedStates;
	State* CurrentState;
	int MaxStateHistory;

protected:
	CharacterEntity* InternalCharacter;
	queue<State*> StateHistory;
	bool AttachState(const string&);
};

#endif //_STATE_MANAGER_H
