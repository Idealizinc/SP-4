/*
Copyright (c) Clinton Teo Jun Quan 2016

Creator(s)    : Clinton Teo Jun Quan
Creation Date : 20 February 2016 13:13 PM
File Name     : Terrain.h
Purpose       : Class that stores units to be spawned
*/

#ifndef UnitSpawnSystem_H
#define UnitSpawnSystem_H
#include <map>

class UnitSpawnSystem
{
private:
	std::map<int, int> UnitMap;
	int maxUnit; //unused
public:
	UnitSpawnSystem();
	~UnitSpawnSystem();
	enum Units
	{
		U_MELEE = 0,
		U_MAGIC,
		U_RANGE,
		U_TOTAL
	};
	void editUnit(int type, int count);
	std::map<int,int> returnUnitMap();
	int getCurrentUnitCount();
	void resetUnitMap();
};

#endif
