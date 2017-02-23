/*
Copyright (c) Clinton Teo Jun Quan 2016

Creator(s)    : Clinton Teo Jun Quan
Creation Date : 20 February 2016 13:13 PM
File Name     : UnitSpawnSystem.h
Purpose       : Class that stores units to be spawned
*/

#ifndef UnitSpawnSystem_H
#define UnitSpawnSystem_H
#include <map>
#include "../Systems/BattleSystem.h"
#include "../Objects/UnitData/UnitType.h"
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

private:
	std::map<std::string, unsigned short> RecordedUnitMap;

	InterfaceLayer* DisplayIcons;

	int maxUnit; //unused
	int interval = 0;

};

#endif
