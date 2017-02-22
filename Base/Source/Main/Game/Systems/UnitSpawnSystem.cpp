#include "UnitSpawnSystem.h"

UnitSpawnSystem::UnitSpawnSystem()
{
	UnitMap.insert(std::pair<int, int>(U_MELEE, 0));

	UnitMap.insert(std::pair<int, int>(U_MAGIC, 0));

	UnitMap.insert(std::pair<int, int>(U_RANGE, 0));
}

UnitSpawnSystem::~UnitSpawnSystem()
{

}

void UnitSpawnSystem::editUnit(int type, int count)
{
	UnitMap.find(type)->second += count;
}

std::map<int, int> UnitSpawnSystem::returnUnitMap()
{
	return UnitMap;
}

int UnitSpawnSystem::getCurrentUnitCount()
{
	return UnitMap.find(U_MELEE)->second + UnitMap.find(U_MAGIC)->second + UnitMap.find(U_RANGE)->second;
}

void UnitSpawnSystem::resetUnitMap()
{
	UnitMap.clear();
	UnitMap.insert(std::pair<int, int>(U_MELEE, 0));

	UnitMap.insert(std::pair<int, int>(U_MAGIC, 0));

	UnitMap.insert(std::pair<int, int>(U_RANGE, 0));
}