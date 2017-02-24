/*
Copyright (c) Matsuda Kenichi 2016

Creator(s)    : Matsuda Kenichi
Creation Date : 15 February 2017 9:15 AM
File Name     : UnitDataLoader.h
Purpose       : Defines a base unit and weapon loader class
*/

#ifndef UNITDATALOADER_H
#define UNITDATALOADER_H

#include <map>
#include <string>
#include "UnitType.h"
#include "WeaponType.h"
#include "UnitRace.h"
#include "TerrainType.h"

class UnitDataLoader
{
public:
	UnitDataLoader(){};
	~UnitDataLoader();

	std::map<std::string, WeaponType*> WeaponMap;
	//std::map<std::string, UnitType*> UnitMap;
	std::map<std::string, UnitRace*> RaceMap;
	std::map<std::string, TerrainType*> TerrainMap;

	std::map<std::string, UnitType*> LivingMap;
	std::map<std::string, UnitType*> UndeadMap;

	bool LoadWeaponData(const char *file_path);
	/*bool LoadUnitData(const char *file_path);*/
	bool LoadRaceData(const char *file_path);
	bool LoadTerrainData(const char *file_path);
	
	bool LoadLivingFactionData(const char *file_path);
	bool LoadUndeadFactionData(const char *file_path);

	bool removingSpecificCharInStr(std::string &theStr, const char &theChar);
	//Mesh *getMeshPtr(const std::string &meshName);
	//Mesh &getMeshRef(const std::string &meshName);
	bool checkWhetherTheWordInThatString(const std::string &theWord, const std::string &theStr);
	void convertStringToUpperCaps(std::string &theString);
};

#endif
