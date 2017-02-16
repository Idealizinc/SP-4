#include "StateManager.h"
#include "StateList.h"

StateManager::StateManager()
{
	CurrentState = nullptr;
	MaxStateHistory = 10;
}

StateManager::~StateManager()
{
	/*if (CurrentState)
		delete CurrentState;*/
}

bool StateManager::SetCurrentState(const string& StateName)
{
	set<State*>::iterator Iter = AssignedStates.begin();
	while (Iter != AssignedStates.end())
	{
		if ((*Iter)->GetStateName() == StateName)
		{
			CurrentState = *Iter;
			return true;
		}
		++Iter;
	}
	CurrentState = nullptr;
	return false;
}

State* StateManager::GetCurrentState()
{
	return CurrentState;
}

State* StateManager::GetPreviousState()
{
	return StateHistory.back(); 
}

int StateManager::GetMaxHistory()
{
	return MaxStateHistory;
}

void StateManager::SetMaxHistory(const int& NewMax)
{
	MaxStateHistory = NewMax;
}

bool StateManager::AttachState(const string& StateName)
{
	// Access StateList
	State* Temp = StateList::Instance().FindState(StateName);
	if (Temp != nullptr)
	{
		AssignedStates.insert(Temp);
		return true;
	}
	return false;
}

void StateManager::SetInternalCharacter(CharacterEntity* Character)
{
	InternalCharacter = Character;
}