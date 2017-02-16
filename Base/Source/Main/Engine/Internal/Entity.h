/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 11 November 2016 13:00 PM
File Name     : Entity.h
Purpose       : Defines a base class for an Entity
*/


#ifndef _ENTITY_H
#define _ENTITY_H

#include <string>

class Entity
{
public:
	Entity() : EntityID(""){};
	virtual ~Entity(){};

	// Main
	virtual void Init() = 0;
	virtual void Update(const float&) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;

	// Getters
	std::string GetEntityID() const
	{ 
		return EntityID; 
	};

	// Setters
	void SetEntityID(std::string ID){ EntityID = ID; };

private:


protected:
	std::string EntityID;

};

#endif //_ENTITY_H