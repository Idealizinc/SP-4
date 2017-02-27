/*
Copyright (c) Clinton Teo Jun Quan 2016

Creator(s)    : Clinton Teo Jun Quan
Creation Date : 20 February 2016 13:13 PM
File Name     : UnitSpawnSystem.h
Purpose       : Class that stores units to be spawned
*/

#ifndef UNIT_SPAWN_SYSTEM_H
#define UNIT_SPAWN_SYSTEM_H
#include <map>
#include "../Systems/BattleSystem.h"
#include "../Logic/UnitData/UnitType.h"
#include "../../Engine/Interface/InterfaceLayer.h"

class UnitSpawnSystem
{
public:
	UnitSpawnSystem();
	~UnitSpawnSystem();

	void CreateUnitUIElement(InterfaceLayer*);

	void editUnit(std::string type, unsigned short count);
	std::map<std::string, unsigned short> returnRecordedUnitMap();

	int getCurrentUnitCount();
	void resetUnitMap();

	std::map<std::string, InterfaceElement*> UnitAdditionIconMap;
	std::map<std::string, InterfaceElement*> UnitSubtractionIconMap;

	void HandleUserInput(const Vector3& MousePos, const Vector3& LayerPos);

	int CalculateCost();

private:
	std::map<std::string, unsigned short> RecordedUnitMap;
	//int UnitCost;
	//std::map<std::string, int> UnitCost;

	InterfaceLayer* DisplayIcons;

	const int maxUnitCost = 30;
	int interval = 0;

};

#endif
