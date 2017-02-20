#ifndef UNIT_DATA_LOADER_H
#define UNIT_DATA_LOADER_H

#include <map>
#include <string>
#include "Terrain.h"

class TerrainDataLoader
{
public:
	TerrainDataLoader();
	~TerrainDataLoader();

	bool LoadTerrainData(const char *file_path);

	bool removingSpecificCharInStr(std::string &theStr, const char &theChar);
	bool checkWhetherTheWordInThatString(const std::string &theWord, const std::string &theStr);
	void convertStringToUpperCaps(std::string &theString);
	Terrain* GetTerrain(std::string TerrainName);

	//int getTerrainFileSize(const char* file_path);

	std::map<std::string, Terrain*> TerrainMap;



};

#endif