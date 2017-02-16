/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 14 November 2016 15:20 PM
File Name     : ObjectManager.h
Purpose       : Defines the object manager
*/

#ifndef _OBJECT_CONTAINER_H
#define _OBJECT_CONTAINER_H

#include "../Miscellaneous/DetectMemoryLeak.h"
#include "../../Engine/Internal/SingletonTemplate.h"
#include "../../Engine/Objects/CharacterEntity.h"
#include "../Objects/Miscellaneous/Projectile.h"
#include "../Objects/Miscellaneous/Particle.h"
//#include "../Objects/Items/Item.h"

class ObjectManager : public SingletonTemplate<ObjectManager>
{
public:
	void Exit();
	void AddNewCharacter(CharacterEntity*);
	void AddNewProjectile(Projectile*);
	void AddNewParticle(Particle*);
	//void AddNewItem(Item*);

	std::vector<CharacterEntity*>& GetCharacterList();
	std::vector<Projectile*>& GetProjectileList();
	std::vector<Particle*>& GetParticleList();
	//std::vector<Item*>& GetItemList();

	float WorldWidth;
	float WorldHeight;
	const int MaxItems = 20;

private:
	//Internals
	std::vector<CharacterEntity*> InternalCharacterList;
	std::vector<Projectile*> InternalProjectileList;
	std::vector<Particle*> InternalParticleList;
	//std::vector<Item*> InternalItemList;
};

#endif // _OBJECT_CONTAINER_H