/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 21 July 2016 11:00 AM
File Name     : System.h
Purpose       : Defines an system base class
*/

#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <string>
using std::string;

class System
{
public:
	// All systems need a virtual destructor to have their destructor called 
	virtual ~System(void) {};

	// All systems must update each game loop
	virtual void Update(const float& dt) = 0;

	// Separate the construction and initialization code.
	virtual void Init(void) = 0;

	virtual void Exit(void) = 0;

	// Sends a message to the global message system
	//virtual void SendMessage(const string&) = 0;

	// All systems will try to render with each game loop
	virtual void Render(void) = 0;

private:
		
protected:

};

#endif // _SYSTEM_H