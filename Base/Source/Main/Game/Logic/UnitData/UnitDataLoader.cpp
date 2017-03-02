#include <iostream>
#include <fstream>
#include <sstream>
#include "UnitDataLoader.h"
#include <vector>

UnitDataLoader::~UnitDataLoader()
{
	for (auto it : WeaponMap)
		delete it.second;
	WeaponMap.clear();

	for (auto it : RaceMap)
		delete it.second;
	RaceMap.clear();

	for (auto it : TerrainMap)
		delete it.second;
	TerrainMap.clear();
}

bool UnitDataLoader::LoadWeaponData(const char *file_path)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}
	std::string CurrentLineOfText = "";
	std::vector<std::string> CSV_Keys;
	std::vector<std::string> CSV_Values;

	while (getline(fileStream, CurrentLineOfText))
	{
		if (CurrentLineOfText == "" || CurrentLineOfText == "\n" || CurrentLineOfText == "\r")
			continue;
		removingSpecificCharInStr(CurrentLineOfText, '\r');
		std::string CurrentFragmentedSubString;
		std::istringstream iss(CurrentLineOfText);
		if (CSV_Keys.empty())
		{
			// Store the key values if the keys have yet to be stored
			while (getline(iss, CurrentFragmentedSubString, ','))
			{
				convertStringToUpperCaps(CurrentFragmentedSubString);
				CSV_Keys.push_back(CurrentFragmentedSubString);
			}
		}
		else
		{
			WeaponType* Temp = new WeaponType();
			for (unsigned int i = 0; i < CSV_Keys.size(); ++i)
			{
				getline(iss, CurrentFragmentedSubString, ',');
				CSV_Values.push_back(CurrentFragmentedSubString);
			}

			std::vector<std::string>::iterator it;
			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "WEAPONID");
			size_t pos = it - CSV_Keys.begin();
			Temp->SetEntityID(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "WEAPONNAME");
			pos = it - CSV_Keys.begin();
			Temp->SetName(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "WEAPONTYPE");
			pos = it - CSV_Keys.begin();
			Temp->SetType(stoi(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "WEAPONDAMAGE");
			pos = it - CSV_Keys.begin();
			Temp->SetDamage(stoi(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "ATTACKRATE");
			pos = it - CSV_Keys.begin();
			Temp->SetRate(stof(CSV_Values[pos]));

			WeaponMap.insert(std::pair<std::string, WeaponType*>(Temp->GetName(), Temp));

		}
		CSV_Values.clear();
	}
	fileStream.close();
	return true;
}

bool UnitDataLoader::LoadRaceData(const char *file_path)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}
	std::string CurrentLineOfText = "";
	std::vector<std::string> CSV_Keys;
	std::vector<std::string> CSV_Values;

	while (getline(fileStream, CurrentLineOfText))
	{
		if (CurrentLineOfText == "" || CurrentLineOfText == "\n" || CurrentLineOfText == "\r")
			continue;
		removingSpecificCharInStr(CurrentLineOfText, '\r');
		std::string CurrentFragmentedSubString;
		std::istringstream iss(CurrentLineOfText);
		if (CSV_Keys.empty())
		{
			// Store the key values if the keys have yet to be stored
			while (getline(iss, CurrentFragmentedSubString, ','))
			{
				convertStringToUpperCaps(CurrentFragmentedSubString);
				CSV_Keys.push_back(CurrentFragmentedSubString);
			}
		}
		else
		{
			UnitRace* Temp = new UnitRace();
			for (unsigned int i = 0; i < CSV_Keys.size(); ++i)
			{
				getline(iss, CurrentFragmentedSubString, ',');
				CSV_Values.push_back(CurrentFragmentedSubString);
			}

			std::vector<std::string>::iterator it;
			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "RACEID");
			size_t pos = it - CSV_Keys.begin();
			Temp->SetEntityID(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "RACENAME");
			pos = it - CSV_Keys.begin();
			Temp->SetName(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "RACETYPE");
			pos = it - CSV_Keys.begin();
			Temp->SetRace(stoi(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "HEALTHMODIFIER");
			pos = it - CSV_Keys.begin();
			Temp->SetHealthModifier(stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "MELEEDAMAGEMODIFIER");
			pos = it - CSV_Keys.begin();
			Temp->SetMeleeDamageModifier(stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "MAGICDAMAGEMODIFIER");
			pos = it - CSV_Keys.begin();
			Temp->SetMagicDamageModifier(stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "RANGEDAMAGEMODIFIER");
			pos = it - CSV_Keys.begin();
			Temp->SetRangeDamageModifier(stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "WALKSPEEDMODIFIER");
			pos = it - CSV_Keys.begin();
			Temp->SetWalkspeedModifier(stof(CSV_Values[pos]));

			RaceMap.insert(std::pair<std::string, UnitRace*>(Temp->GetName(), Temp));

		}
		CSV_Values.clear();
	}
	fileStream.close();
	return true;
}

bool UnitDataLoader::LoadLivingFactionData(const char *file_path)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}
	std::string CurrentLineOfText = "";
	std::vector<std::string> CSV_Keys;
	std::vector<std::string> CSV_Values;

	while (getline(fileStream, CurrentLineOfText))
	{
		if (CurrentLineOfText == "" || CurrentLineOfText == "\n" || CurrentLineOfText == "\r")
			continue;
		removingSpecificCharInStr(CurrentLineOfText, '\r');
		std::string CurrentFragmentedSubString;
		std::istringstream iss(CurrentLineOfText);
		if (CSV_Keys.empty())
		{
			// Store the key values if the keys have yet to be stored
			while (getline(iss, CurrentFragmentedSubString, ','))
			{
				convertStringToUpperCaps(CurrentFragmentedSubString);
				CSV_Keys.push_back(CurrentFragmentedSubString);
			}
		}
		else
		{
			UnitType* Temp = new UnitType();
			for (unsigned int i = 0; i < CSV_Keys.size(); ++i)
			{
				getline(iss, CurrentFragmentedSubString, ',');
				CSV_Values.push_back(CurrentFragmentedSubString);
			}

			std::vector<std::string>::iterator it;
			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "UNITID");
			size_t pos = it - CSV_Keys.begin();
			Temp->SetEntityID(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "UNITNAME");
			pos = it - CSV_Keys.begin();
			Temp->SetName(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "MESHNAME");
			pos = it - CSV_Keys.begin();
			Temp->SetMeshName(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "UNITTYPE");
			pos = it - CSV_Keys.begin();
			Temp->SetType(stoi(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "HEALTH");
			pos = it - CSV_Keys.begin();
			Temp->SetHealth(stoi(CSV_Values[pos]));
			Temp->SetMaxHealth(Temp->GetHealth());

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "DAMAGE");
			pos = it - CSV_Keys.begin();
			Temp->SetDamage(stoi(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "WALKSPEED");
			pos = it - CSV_Keys.begin();
			Temp->SetWalkspeed(stoi(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "RANGE");
			pos = it - CSV_Keys.begin();
			Temp->SetRange(stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "COST");
			pos = it - CSV_Keys.begin();
			Temp->SetCost(stoi(CSV_Values[pos]));

			int num = 0;
			while (CSV_Values.size() - 1 > pos)
			{
				num++;
				std::ostringstream ss;
				ss << "WEAPON" << num; //keyname
				it = std::find(CSV_Keys.begin(), CSV_Keys.end(), ss.str());
				pos = it - CSV_Keys.begin();
				//Temp->PossibleWeapon
				if (CSV_Values[pos] != "")
				{
					WeaponType* SelectdWeapon = WeaponMap.find(CSV_Values[pos])->second;
					if (SelectdWeapon != nullptr)
						Temp->PossibleWeapon.push_back(SelectdWeapon);
				}
			}
			LivingMap.insert(std::pair<std::string, UnitType*>(Temp->GetName(), Temp));

		}
		CSV_Values.clear();
	}
	fileStream.close();
	return true;
}

bool UnitDataLoader::LoadUndeadFactionData(const char *file_path)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}
	std::string CurrentLineOfText = "";
	std::vector<std::string> CSV_Keys;
	std::vector<std::string> CSV_Values;

	while (getline(fileStream, CurrentLineOfText))
	{
		if (CurrentLineOfText == "" || CurrentLineOfText == "\n" || CurrentLineOfText == "\r")
			continue;
		removingSpecificCharInStr(CurrentLineOfText, '\r');
		std::string CurrentFragmentedSubString;
		std::istringstream iss(CurrentLineOfText);
		if (CSV_Keys.empty())
		{
			// Store the key values if the keys have yet to be stored
			while (getline(iss, CurrentFragmentedSubString, ','))
			{
				convertStringToUpperCaps(CurrentFragmentedSubString);
				CSV_Keys.push_back(CurrentFragmentedSubString);
			}
		}
		else
		{
			UnitType* Temp = new UnitType();
			for (unsigned int i = 0; i < CSV_Keys.size(); ++i)
			{
				getline(iss, CurrentFragmentedSubString, ',');
				CSV_Values.push_back(CurrentFragmentedSubString);
			}

			std::vector<std::string>::iterator it;
			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "UNITID");
			size_t pos = it - CSV_Keys.begin();
			Temp->SetEntityID(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "UNITNAME");
			pos = it - CSV_Keys.begin();
			Temp->SetName(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "MESHNAME");
			pos = it - CSV_Keys.begin();
			Temp->SetMeshName(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "UNITTYPE");
			pos = it - CSV_Keys.begin();
			Temp->SetType(stoi(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "HEALTH");
			pos = it - CSV_Keys.begin();
			Temp->SetHealth(stoi(CSV_Values[pos]));
			Temp->SetMaxHealth(Temp->GetHealth());

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "DAMAGE");
			pos = it - CSV_Keys.begin();
			Temp->SetDamage(stoi(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "WALKSPEED");
			pos = it - CSV_Keys.begin();
			Temp->SetWalkspeed(stoi(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "RANGE");
			pos = it - CSV_Keys.begin();
			Temp->SetRange(stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "COST");
			pos = it - CSV_Keys.begin();
			Temp->SetCost(stoi(CSV_Values[pos]));

			int num = 0;
			while (CSV_Values.size() - 1 > pos)
			{
				num++;
				std::ostringstream ss;
				ss << "WEAPON" << num; //keyname
				it = std::find(CSV_Keys.begin(), CSV_Keys.end(), ss.str());
				pos = it - CSV_Keys.begin();
				//Temp->PossibleWeapon
				if (CSV_Values[pos] != "")
				{
					WeaponType* SelectdWeapon = WeaponMap.find(CSV_Values[pos])->second;
					if (SelectdWeapon != nullptr)
						Temp->PossibleWeapon.push_back(SelectdWeapon);
				}
			}
			UndeadMap.insert(std::pair<std::string, UnitType*>(Temp->GetName(), Temp));

		}
		CSV_Values.clear();
	}
	fileStream.close();
	return true;
}

bool UnitDataLoader::LoadTerrainData(const char* file_path)
{
	std::ifstream fileStream(file_path, std::ios::binary);
	if (!fileStream.is_open())
	{
		std::cout << "Impossible to open " << file_path << ". Are you in the right directory ?\n";
		return false;
	}
	std::string CurrentLineOfText = "";
	std::vector<std::string> CSV_Keys;
	std::vector<std::string> CSV_Values;

	while (getline(fileStream, CurrentLineOfText))
	{
		if (CurrentLineOfText == "" || CurrentLineOfText == "\n" || CurrentLineOfText == "\r")
			continue;
		removingSpecificCharInStr(CurrentLineOfText, '\r');
		std::string CurrentFragmentedSubString;
		std::istringstream iss(CurrentLineOfText);
		if (CSV_Keys.empty())
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
			TerrainType* Temp = new TerrainType();
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
			Temp->SetName(CSV_Values[pos]);

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "TERRAINTYPE");
			pos = it - CSV_Keys.begin();
			Temp->SetTerrainBoost(stoi(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "HEALTHBOOST");
			pos = it - CSV_Keys.begin();
			Temp->SetT_HealthModifier(stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "MELEEBOOST");
			pos = it - CSV_Keys.begin();
			Temp->SetT_MeleeDmgModifier(stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "MAGICBOOST");
			pos = it - CSV_Keys.begin();
			Temp->SetT_MagicDmgModifier(stof(CSV_Values[pos]));

			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "RANGEBOOST");
			pos = it - CSV_Keys.begin();
			Temp->SetT_RangeDmgModifier(stof(CSV_Values[pos]));

			TerrainMap.insert(std::pair<std::string, TerrainType*>(Temp->GetName(), Temp));

		}
		CSV_Values.clear();
	}
	fileStream.close();
	return true;
}

bool UnitDataLoader::removingSpecificCharInStr(std::string &theStr, const char &theChar)
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

//Mesh UnitDataLoader::*getMeshPtr(const std::string &meshName)
//{
//	Mesh *thePtr = nullptr;
//	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
//	if (SceneGraphics)
//	{
//		std::map<std::string, Mesh*>::iterator iter = SceneGraphics->meshList.find(meshName);
//		if (iter != SceneGraphics->meshList.end())
//		{
//			thePtr = iter->second;
//		}
//	}
//	return thePtr;
//}
//
//Mesh UnitDataLoader::&getMeshRef(const std::string &meshName)
//{
//	Mesh *thePtr = nullptr;
//	GraphicsEntity *SceneGraphics = dynamic_cast<GraphicsEntity*>(&Scene_System::accessing().getGraphicsScene());
//	if (SceneGraphics)
//	{
//		std::map<std::string, Mesh*>::iterator iter = SceneGraphics->meshList.find(meshName);
//		if (iter != SceneGraphics->meshList.end())
//		{
//			thePtr = iter->second;
//		}
//	}
//	return (*thePtr);
//}

bool UnitDataLoader::checkWhetherTheWordInThatString(const std::string &theWord, const std::string &theStr)
{
	if (theStr.find(theWord) != std::string::npos)
		return true;
	return false;
}

void UnitDataLoader::convertStringToUpperCaps(std::string &theString)
{
	for (std::string::iterator it = theString.begin(), end = theString.end(); it != end; ++it) {
		(*it) = toupper(*it);
	}
}
