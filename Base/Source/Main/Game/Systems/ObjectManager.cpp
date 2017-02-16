#include "ObjectManager.h"

void ObjectManager::Exit()
{
	while (InternalCharacterList.size() > 0)
	{
		CharacterEntity *obj = InternalCharacterList.back();
		obj->Exit();
		delete obj;
		InternalCharacterList.pop_back();
	}
	while (InternalProjectileList.size() > 0)
	{
		BaseObject *obj = InternalProjectileList.back();
		obj->Exit();
		delete obj;
		InternalProjectileList.pop_back();
	}
	while (InternalParticleList.size() > 0)
	{
		BaseObject *obj = InternalParticleList.back();
		obj->Exit();
		delete obj;
		InternalParticleList.pop_back();
	}
	/*while (InternalItemList.size() > 0)
	{
		Item *obj = InternalItemList.back();
		obj->Exit();
		delete obj;
		InternalItemList.pop_back();
	}*/
}

void ObjectManager::AddNewCharacter(CharacterEntity* CE)
{
	CharacterEntity* Fetched = nullptr;
	for (std::vector<CharacterEntity*>::iterator it = InternalCharacterList.begin(); it != InternalCharacterList.end(); ++it)
		if ((*it)->Active == false)
		{
			Fetched = *it;
			InternalCharacterList.erase(it);
			delete Fetched;
			Fetched = CE;
			InternalCharacterList.push_back(Fetched);
			break;
		}
	if (Fetched == nullptr){
		InternalCharacterList.push_back(CE);
	}
}

void ObjectManager::AddNewProjectile(Projectile* P)
{
	Projectile* Fetched = nullptr;
	for (std::vector<Projectile*>::iterator it = InternalProjectileList.begin(); it != InternalProjectileList.end(); ++it)
		if ((*it)->Active == false)
		{
			Fetched = *it;
			InternalProjectileList.erase(it);
			delete Fetched;
			Fetched = P;
			InternalProjectileList.push_back(Fetched);
			break;
		}
	if (Fetched == nullptr){
		InternalProjectileList.push_back(P);
	}
}

void ObjectManager::AddNewParticle(Particle* P)
{
	Particle* Fetched = nullptr;
	for (std::vector<Particle*>::iterator it = InternalParticleList.begin(); it != InternalParticleList.end(); ++it)
		if ((*it)->Active == false)
		{
			Fetched = *it;
			InternalParticleList.erase(it);
			delete Fetched;
			Fetched = P;
			InternalParticleList.push_back(Fetched);
			break;
		}
	if (Fetched == nullptr){
		InternalParticleList.push_back(P);
	}
}

//void ObjectManager::AddNewItem(Item* I)
//{
//	int Counter = InternalItemList.size();
//	while (Counter > MaxItems)
//	{
//		Item* T = InternalItemList.front();
//		T->Active = false;
//		Counter -= 1;
//	}
//	Item* Fetched = nullptr;
//	for (std::vector<Item*>::iterator it = InternalItemList.begin(); it != InternalItemList.end(); ++it)
//		if ((*it)->Active == false)
//		{
//			Fetched = *it;
//			InternalItemList.erase(it);
//			delete Fetched;
//			Fetched = I;
//			InternalItemList.push_back(Fetched);
//			break;
//		}
//	if (Fetched == nullptr){
//		InternalItemList.push_back(I);
//	}
//}

std::vector<CharacterEntity*>& ObjectManager::GetCharacterList()
{
	return InternalCharacterList;
}

std::vector<Projectile*>& ObjectManager::GetProjectileList()
{
	return InternalProjectileList;
}

std::vector<Particle*>& ObjectManager::GetParticleList()
{
	return InternalParticleList;
}

//std::vector<Item*>& ObjectManager::GetItemList()
//{
//	return InternalItemList;
//}
