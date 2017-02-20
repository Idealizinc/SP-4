#include "StateList.h"

StateList::~StateList()
{
}

void StateList::Exit()
{
	for (set<State*>::iterator Iter = CollectiveStates.begin(); Iter != CollectiveStates.end();)
	{
		State* temp = *Iter;
		Iter = CollectiveStates.erase(Iter++);
		delete temp;
	}
}

State* StateList::FindState(const string& StateName)
{
	set<State*>::iterator Iter = CollectiveStates.begin();
	while (Iter != CollectiveStates.end())
	{
		if ((*Iter)->GetStateName() == StateName)
		{
			return *Iter;
		}
		++Iter;
	}
	return nullptr;
}

string StateList::CreateState(const string& StateName)
{
	State* temp = new State(StateName);
	CollectiveStates.insert(temp);
	return StateName;
}

bool StateList::RemoveState(const string& StateName)
{
	set<State*>::iterator Iter = CollectiveStates.begin();
	while (Iter != CollectiveStates.end())
	{
		if ((*Iter)->GetStateName() == StateName)
		{
			CollectiveStates.erase(Iter);
			return true;
		}
		++Iter;
	}
	return false;
}

int StateList::GetStateCount()
{
	return CollectiveStates.size();
}
