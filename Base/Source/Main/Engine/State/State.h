/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 30 September 2016 10:42 AM
File Name     : State.h
Purpose       : Defines a Basic State Object
*/

#ifndef _STATE_H
#define _STATE_H

#include <string>
using std::string;

// Individual State that contains intermediate completion statuses
class State
{
public:
	State() : StateName("") {};
	State(const string& StateName) : StateName(StateName) {};
	~State(){};

	string GetStateName(){ return StateName; };

private:
	string StateName;

protected:

};

#endif //_STATE_H