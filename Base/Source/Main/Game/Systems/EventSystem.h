/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 26 November 2016 18:52 PM
File Name     : ObjectManager.h
Purpose       : Defines the object manager
*/

#ifndef EVENT_SYSTEM_H
#define EVENT_SYSTEM_H

#include "../../Engine/Internal/System.h"
#include "../../Engine/Internal/SingletonTemplate.h"
#include  "ObjectManager.h"
#include "Vector3.h"
#include <map>
#include <vector>

//#include "../Objects/Characters/ScavengerCharacter.h"
//#include "../Objects/Characters/DroneCharacter.h"
//#include "../Objects/Characters/WildLifeCharacter.h"
//#include "../Objects/Characters/ZombieCharacter.h"
#include "../Objects/Characters/RobotCharacter.h"

class EventSystem : public System, public SingletonTemplate<EventSystem>
{
public:
	virtual void Init();
	virtual void Update(const float& dt);
	virtual void Render(){}; 
	//virtual void SendMessage(const string&){};
	virtual void Exit(){};

	/*Vector3 SpawnPosition_Scavenger;
	Vector3 SpawnPosition_Drone;
	Vector3 SpawnPosition_Wildlife;
	Vector3 SpawnPosition_Zombie;
	Vector3 SpawnPosition_Robot;*/

	/*const float ScavengerSize = 2.f;
	const float DroneSize = 2.5f;
	const float WildlifeSize = 1.5f;
	const float ZombieSize = 1.5f;
	const float ItemSize = 3.f;*/
	const float RobotSize = 1.f;

	std::map<std::string, int> CurrentCharacterCount;
	void ClearCharacterCount();

	bool GameOver = false;

private:
	void SpawnCharacter(std::string);

	/*const std::string Scavenger = "Scavenger";
	const std::string Drone = "Drone";
	const std::string Wildlife = "WildLife";
	const std::string Zombie = "Zombie";*/
	const std::string Robot = "Robot";

	std::map<std::string, int> CharacterCountLimit;
	std::map<std::string, float> SpawnTime;
	std::map<std::string, float> CollectiveTimers;
};

#endif // EVENT_SYSTEM_H