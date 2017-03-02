#include "TerrainDataLoader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "MyMath.h"
#include "../../../Engine/System/RenderSystem.h"
#include "../../../Engine/System/SceneSystem.h"



TerrainDataLoader::TerrainDataLoader()
{

}

TerrainDataLoader::~TerrainDataLoader()
{
	ClearTerrainData();
}

void TerrainDataLoader::ClearTerrainData()
{
	for (std::map<std::string, Terrain*>::iterator mapIt = TerrainMap.begin(), mapEnd = TerrainMap.end(); mapIt != mapEnd; ++mapIt)
	{
		delete mapIt->second;
	}
	TerrainMap.clear();
}

bool TerrainDataLoader::LoadTerrainData(const char* file_path)
{
	if (TerrainMap.size())
		ClearTerrainData();

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

			if (TerrainMap.find(CSV_Values[pos]) == TerrainMap.end())
			{
				it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "TERRAINNAME");
				pos = it - CSV_Keys.begin();
				Temp->SetTerrainName(CSV_Values[pos]);

				it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "TERRAINMAP");
				pos = it - CSV_Keys.begin();
				Temp->CSVMapName = CSV_Values[pos];

				// Generate and assign the mesh
				it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "HEIGHTMAP");
				pos = it - CSV_Keys.begin();
				std::string HeightmapPath = CSV_Values[pos];

				it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "TEXTURE1");
				pos = it - CSV_Keys.begin();
				std::string Tex1Path = CSV_Values[pos];

				it = std::find(CSV_Keys.begin(), CSV_Keys.end(), "TEXTURE2");
				pos = it - CSV_Keys.begin();
				std::string Tex2Path = CSV_Values[pos];

				if (HeightmapPath != "")
				{
					RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
					auto it = Renderer->MeshList.find(Temp->GetTerrainName());
					if (it == Renderer->MeshList.end())
					{
						// Initiallise Model Specific Meshes Here
						Mesh* newMesh = MeshBuilder::GenerateTerrain(Temp->GetTerrainName(), HeightmapPath, Temp->TerrainHeightMap);
						newMesh->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
						newMesh->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
						newMesh->material.kSpecular.Set(0.0f, 0.0f, 0.0f);
						if (Tex1Path != "")
							newMesh->textureArray[0] = LoadTGA(Tex1Path.c_str());
						if (Tex2Path != "")
							newMesh->textureArray[1] = LoadTGA(Tex2Path.c_str());
						Renderer->MeshList.insert(std::pair<std::string, Mesh*>(Temp->GetTerrainName(), newMesh));
						Temp->TerrainMeshName = newMesh->name;
					}
					else Temp->TerrainMeshName = it->second->name;
				}

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

Terrain* TerrainDataLoader::GetTerrain(std::string TerrainName)
{
	return TerrainMap.find(TerrainName)->second;
}


