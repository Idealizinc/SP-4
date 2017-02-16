/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 11 November 2016 11:51 AM
File Name     : StateList.h
Purpose       : Defines the list that holds all states within the framework
*/

#ifndef _STATE_LIST_H
#define _STATE_LIST_H

#include "State.h"

#include "../Internal/SingletonTemplate.h"

#include <set>
using std::set;

#include <string>
using std::string;

class StateList : public SingletonTemplate<StateList>
{
	public:
		~StateList();
		State* FindState(const string&);
		void CreateState(const string&);
		bool RemoveState(const string&);

		int GetStateCount();
		void Exit();

	//private:
		set<State*> CollectiveStates;

	protected:


};

#endif //_STATE_LIST_H