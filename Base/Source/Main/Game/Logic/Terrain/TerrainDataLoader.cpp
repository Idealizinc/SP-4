#include "TerrainDataLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "MyMath.h"

TerrainDataLoader::TerrainDataLoader()
{

}

TerrainDataLoader::~TerrainDataLoader()
{

}

bool TerrainDataLoader::LoadTerrainData(const char* file_path)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}
	std::string CurrentLineOfText = "";
	std::vector<std::string> CSV_Keys; //takes in the first lines which is the name of the values or keys
	std::vector<std::string> CSV_Values; //values of keys

	while (getline(fileStream, CurrentLineOfText))//while not at a empty row
	{
		if (CurrentLineOfText == "" || CurrentLineOfText == "\n" || CurrentLineOfText == "\r")//while not at an empty column
			continue;
		removingSpecificCharInStr(CurrentLineOfText, '\r'); //remove space
		std::string CurrentFragmentedSubString;
		std::istringstream iss(CurrentLineOfText);
		if (CSV_Keys.empty()) //first line
		{
			// Store the key values if the keys have yet to be stored
			while (getline(iss, CurrentFragmentedSubString, ','))
			{
				convertStringToUpperCaps(CurrentFragmentedSubString);
				CSV_Keys.push_back(CurrentFragmentedSubString);
			}
		}
		else //any other line
		{
			Terrain* Temp = new Terrain();
			for (unsigned int i = 0; i < CSV_Keys.size(); ++i)
			{
				getline(iss, CurrentFragmentedSubString, ',');
				CSV_Values.push_back(CurrentFragmentedSubString);
			}

			std::vector<std::string>::iterator it;
			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "TERRAINID");
			size_t pos = it - CSV_Keys.begin();
			Temp->SetEntityID(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "TERRAINNAME");
			pos = it - CSV_Keys.begin();
			Temp->SetTerrainName(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "TERRAINTYPE");
			pos = it - CSV_Keys.begin();
			Temp->SetTerrainType(stoi(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "HEALTHADVANTAGE");
			pos = it - CSV_Keys.begin();
			Temp->SetUnitAdvantage(Terrain::T_HEALTH, stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "MELEEADVANTAGE");
			pos = it - CSV_Keys.begin();
			Temp->SetUnitAdvantage(Terrain::T_MELEE, stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "MAGICADVANTAGE");
			pos = it - CSV_Keys.begin();
			Temp->SetUnitAdvantage(Terrain::T_MAGIC, stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "RANGEADVANTAGE");
			pos = it - CSV_Keys.begin();
			Temp->SetUnitAdvantage(Terrain::T_RANGE, stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "HEALTHDISADVANTAGE");
			pos = it - CSV_Keys.begin();
			Temp->SetUnitDisadvantage(Terrain::T_HEALTH, stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "MELEEDISADVANTAGE");
			pos = it - CSV_Keys.begin();
			Temp->SetUnitDisadvantage(Terrain::T_MELEE, stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "MAGICDISADVANTAGE");
			pos = it - CSV_Keys.begin();
			Temp->SetUnitDisadvantage(Terrain::T_MAGIC, stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "RANGEDISADVANTAGE");
			pos = it - CSV_Keys.begin();
			Temp->SetUnitDisadvantage(Terrain::T_RANGE, stof(CSV_Values[pos]));

			TerrainMap.insert(std::pair<std::string, Terrain*>(Temp->GetTerrainName(), Temp));

		}
		CSV_Values.clear();
	}
	fileStream.close();
	return true;
}

bool TerrainDataLoader::removingSpecificCharInStr(std::string &theStr, const char &theChar)
{
	for (size_t num = 0, sizeOfStr = theStr.size(); num < sizeOfStr; ++num)
	{
		if (theStr[num] == theChar)
		{
			theStr.erase(num);
			return true;
			break;
		}
	}
	return false;
}

bool TerrainDataLoader::checkWhetherTheWordInThatString(const std::string &theWord, const std::string &theStr)
{
	if (theStr.find(theWord) != std::string::npos)
		return true;
	return false;
}

void TerrainDataLoader::convertStringToUpperCaps(std::string &theString)
{
	for (std::string::iterator it = theString.begin(), end = theString.end(); it != end; ++it) {
		(*it) = toupper(*it);
	}
}

//int TerrainDataLoader::getTerrainFileSize(const char* file_path)
//{
//	int size = 0;
//	std::ifstream fileStream(file_path, std::ios::binary);
//	if (!fileStream.is_open())
//	{
//		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
//		return false;
//	}
//	std::string CurrentLineOfText = "";
//	while (getline(fileStream, CurrentLineOfText))//while not at a empty row
//	{
//		++size;
//	}
//	return size;
//
//}

Terrain* TerrainDataLoader::GetTerrain(std::string TerrainName)
{
	return TerrainMap.find(TerrainName)->second;
}

