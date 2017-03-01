#ifndef Level_DATA_LOADER_H
#define Level_DATA_LOADER_H

#include <map>
#include <string>
#include "Level.h"

class LevelDataLoader
{
public:
	LevelDataLoader();
	~LevelDataLoader();

	void ClearLevelData();

	bool LoadLevelData(const char *file_path);

	bool removingSpecificCharInStr(std::string &theStr, const char &theChar);
	bool checkWhetherTheWordInThatString(const std::string &theWord, const std::string &theStr);
	void convertStringToUpperCaps(std::string &theString);
	Level* GetLevel(std::string LevelName);

	//int getLevelFileSize(const char* file_path);

	std::map<std::string, Level*> LevelMap;



};

#endif // Level_DATA_LOADER_H