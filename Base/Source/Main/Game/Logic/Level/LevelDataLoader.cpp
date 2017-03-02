#include "LevelDataLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "MyMath.h"
#include "../../../Engine/System/RenderSystem.h"
#include "../../../Engine/System/SceneSystem.h"



LevelDataLoader::LevelDataLoader()
{

}

LevelDataLoader::~LevelDataLoader()
{
	ClearLevelData();
}

void LevelDataLoader::ClearLevelData()
{
	for (auto it : LevelMap)
		delete it.second;

	LevelMap.clear();
}

bool LevelDataLoader::LoadLevelData(const char* file_path)
{
	if (LevelMap.size())
		ClearLevelData();

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
			for (unsigned int i = 0; i < CSV_Keys.size(); ++i)
			{
				getline(iss, CurrentFragmentedSubString, ',');
				CSV_Values.push_back(CurrentFragmentedSubString);
			}

			std::vector<std::string>::iterator it;
			it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "LEVELID");
			size_t pos = it - CSV_Keys.begin();
			if (LevelMap.find(CSV_Values[pos]) == LevelMap.end())
			{
				Level* Temp = new Level();
				Temp->SetEntityID(CSV_Values[pos]);

				it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "LEVELNAME");
				pos = it - CSV_Keys.begin();
				Temp->SetLevelMapName(CSV_Values[pos]);

				it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "LEVELMAP");
				pos = it - CSV_Keys.begin();
				Temp->SetLevelMapLayoutName(CSV_Values[pos]);

				it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "HEIGHTMAP");
				pos = it - CSV_Keys.begin();
				Temp->SetLevelMapMeshName(CSV_Values[pos]);

				it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "TEXTURE1");
				pos = it - CSV_Keys.begin();
				std::string Tex1Path = CSV_Values[pos];

				it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "TEXTURE2");
				pos = it - CSV_Keys.begin();
				std::string Tex2Path = CSV_Values[pos];


				if (Temp->GetLevelMapMeshName() != "")
				{
					RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
					auto it = Renderer->MeshList.find(Temp->GetLevelMapName());
					if (it == Renderer->MeshList.end())
					{
						// Initiallise Model Specific Meshes Here
						Mesh* newMesh = MeshBuilder::GenerateTerrain(Temp->GetLevelMapName(), Temp->GetLevelMapMeshName(), Temp->TerrainHeightMap);
						newMesh->material.kAmbient.Set(0.4f, 0.4f, 0.4f);
						newMesh->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
						newMesh->material.kSpecular.Set(0.0f, 0.0f, 0.0f);
						if (Tex1Path != "")
							newMesh->textureArray[0] = LoadTGA(Tex1Path.c_str());
						if (Tex2Path != "")
							newMesh->textureArray[1] = LoadTGA(Tex2Path.c_str());
						Renderer->MeshList.insert(std::pair<std::string, Mesh*>(Temp->GetLevelMapName(), newMesh));
					}
				}
				LevelMap.insert(std::pair<std::string, Level*>(Temp->GetLevelMapName(), Temp));
			}
		}
		CSV_Values.clear();
	}
	fileStream.close();
	return true;
}

bool LevelDataLoader::removingSpecificCharInStr(std::string &theStr, const char &theChar)
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

bool LevelDataLoader::checkWhetherTheWordInThatString(const std::string &theWord, const std::string &theStr)
{
	if (theStr.find(theWord) != std::string::npos)
		return true;
	return false;
}

void LevelDataLoader::convertStringToUpperCaps(std::string &theString)
{
	for (std::string::iterator it = theString.begin(), end = theString.end(); it != end; ++it) {
		(*it) = toupper(*it);
	}
}

Level* LevelDataLoader::GetLevel(std::string LevelName)
{
	return LevelMap.find(LevelName)->second;
}