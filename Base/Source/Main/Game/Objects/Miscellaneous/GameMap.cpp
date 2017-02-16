#include "GameMap.h"
#include <fstream>
#include <sstream>
#include <assert.h>
#include "../../../Engine/System/RenderSystem.h"
#include "../../../Engine/System/SceneSystem.h"
#include "../../SceneManagement/ScenePartitionGraph.h"
#include "LoadHmap.h"
#include "GameObject.h"

std::map<std::string, Entity*> GameMap::bunchOfLegends;

GameMap::GameMap(void)
	: theNumOfTiles_MapHeight(0)
	, theNumOfTiles_MapWidth(0)
	, theTileSize(0)
{
	theScreenMap.clear();
	tileSizeXYZ.SetZero();

	if (bunchOfLegends.empty())
	{
		loadThoseLegends("CSVFiles//MapLegend.csv");
	}
}

GameMap::~GameMap(void)
{
	theScreenMap.clear();
	if (bunchOfLegends.empty() == false)
	{
		for (auto it : bunchOfLegends)
			delete it.second;
		bunchOfLegends.clear();
	}
}

int gcd(int a, int b) {
	return b == 0 ? a : gcd(b, a % b);
}

bool GameMap::LoadMap(const std::string &mapName, std::vector<unsigned char> &theHeightMap, Vector3 &terrainSize, std::vector<GameObject*> &theRenderingStuff, BillboardManager &theBBManager)
{
	if (LoadFile(mapName, theHeightMap, terrainSize, theRenderingStuff, theBBManager) == true)
	{
		printf("Map (%s) has been successfully loaded!\n", mapName.c_str());
		return true;
	}
	else printf("Map (%s) failed to load...\n", mapName.c_str());
	return false;
}

bool GameMap::LoadFile(const std::string &mapName, std::vector<unsigned char> &theHeightMap, Vector3 &terrainSize, std::vector<GameObject*> &theRenderingStuff, BillboardManager &theBBManager)
{
	int theLineCounter = 1;
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	std::vector<TerrainNode*> TempNodeList;

	std::ifstream file(mapName.c_str());
	assert(file.is_open());
	int i = 0, k = 0;
	while (file.good())
	{
		std::string aLineOfText = "";
		getline(file, aLineOfText);

		if (aLineOfText == "")
			continue;

		std::string token;
		std::istringstream iss(aLineOfText);
		theScreenMap.insert(std::pair<unsigned, std::vector<std::string>>(theLineCounter, std::vector<std::string>()));
		std::map<unsigned, std::vector<std::string>>::iterator it = theScreenMap.find((unsigned)theLineCounter);
		unsigned num_Column = 0;
		while (getline(iss, token, ','))
		{
			std::string Icon, Rotation;
			bool SpaceFound = false;
			for (unsigned int i = 0; i < token.size(); ++i)
			{
				if (token[i] == ' ')
					SpaceFound = true;
				else {
					if (!SpaceFound)
						Icon += token[i];
					else Rotation += token[i];
				}
			}
			std::map<std::string, Entity*>::iterator itLegend = bunchOfLegends.find(Icon);
			if (itLegend != bunchOfLegends.end())
			{
				if (Icon == "TN" || Icon == "TNP" || Icon == "TNE")
				{
					TerrainNode* TN = new TerrainNode();
					BaseObject *TNBO = dynamic_cast<BaseObject*>(itLegend->second);
					if (TNBO)
					{
						BaseObject *aCopyOfIt = new BaseObject(*TNBO);
						aCopyOfIt->SetPosition(Vector3((float)num_Column, 0, (float)theLineCounter));
						aCopyOfIt->Static = true;
						it->second.push_back(" "); 
						TN->SetEntity(aCopyOfIt);
						TN->SetGridIndex(aCopyOfIt->GetPosition());
						TempNodeList.push_back(TN);
						if (Icon == "TNP")
						{
							Player = TN;
						}
						else if (Icon == "TNE")
						{
							Enemy = TN;
						}
					}
				}
				else
				{
					GameObject *the3Dobject = dynamic_cast<GameObject*>(itLegend->second);
					if (the3Dobject)
					{
						GameObject *aCopyOfIt = new GameObject(*the3Dobject);
						aCopyOfIt->SetPosition(Vector3((float)num_Column, 0, (float)theLineCounter));
						if (Rotation.size() > 0)
						{
							aCopyOfIt->SetRotationAngle(stof(Rotation));
							aCopyOfIt->SetRotationAxis(Vector3(0, 1, 0));
							aCopyOfIt->SetBounds();
						}
						aCopyOfIt->Static = true;
						theRenderingStuff.push_back(aCopyOfIt);
						it->second.push_back(itLegend->first);
					}
					else
					{
						// If it's a billboard in the guise of a particle
						// Assign it to the Billboard manager as it shouldn't collide with the player
						Particle *isParticle = dynamic_cast<Particle*>(itLegend->second);
						theBBManager.AddBillboard(isParticle->GetMeshName(), Vector3((float)num_Column, 0, (float)theLineCounter), isParticle->GetDimensions());
						it->second.push_back(" ");
					}
				}
			}
			else{
				it->second.push_back(" ");
			}
			++num_Column;
		}
		theLineCounter++;
	}
    file.close();
	theNumOfTiles_MapWidth = theScreenMap.begin()->second.size();
	theNumOfTiles_MapHeight = theLineCounter;

	tileSizeXYZ.x = terrainSize.x / theNumOfTiles_MapWidth;
	tileSizeXYZ.z = terrainSize.z / theNumOfTiles_MapHeight;

	int HCF = gcd(theNumOfTiles_MapWidth, theNumOfTiles_MapHeight);

	ScenePartitionGraph::Instance().GridDivisions = HCF;
	ScenePartitionGraph::Instance().GenerateGrid();
	AssignTerrainNodes(&TempNodeList);

	ScenePartitionGraph::Instance().PlayerBase = Player;
	ScenePartitionGraph::Instance().EnemyBase = Enemy;

	for (std::vector<GameObject*>::iterator it = theRenderingStuff.begin(), end = theRenderingStuff.end(); it != end; ++it)
	{
		GameObject *the3DObj = dynamic_cast<GameObject*>(*it);
		the3DObj->SetPosition(Vector3(
			((the3DObj->GetPosition().x - ((float)theNumOfTiles_MapWidth * 0.5f)) * tileSizeXYZ.x) + (tileSizeXYZ.x * 0.5f),
			0,
			((the3DObj->GetPosition().z - ((float)theNumOfTiles_MapHeight * 0.5f)) * tileSizeXYZ.z) - (tileSizeXYZ.z * 0.5f)
			));
		the3DObj->SetPosition(Vector3(the3DObj->GetPosition().x,
			(the3DObj->GetDimensions().y*0.5f) + terrainSize.y * ReadHeightMap(theHeightMap, the3DObj->GetPosition().x / terrainSize.x, the3DObj->GetPosition().z / terrainSize.z) - 0.5f,
			the3DObj->GetPosition().z
			));
		the3DObj->SetBounds();
		ScenePartitionGraph::Instance().CreateNewNode(the3DObj);
	}

	for (std::vector<TerrainNode*>::iterator it = TempNodeList.begin(), end = TempNodeList.end(); it != end; ++it)
	{
		BaseObject *the3DObj = dynamic_cast<BaseObject*>((*it)->GetEntity());
		the3DObj->SetPosition(Vector3(
			((the3DObj->GetPosition().x - ((float)theNumOfTiles_MapWidth * 0.5f)) * tileSizeXYZ.x) + (tileSizeXYZ.x * 0.5f),
			0,
			((the3DObj->GetPosition().z - ((float)theNumOfTiles_MapHeight * 0.5f)) * tileSizeXYZ.z) - (tileSizeXYZ.z * 0.5f)
			));
		the3DObj->SetPosition(Vector3(the3DObj->GetPosition().x,
			(the3DObj->GetDimensions().y*0.5f) + terrainSize.y * ReadHeightMap(theHeightMap, the3DObj->GetPosition().x / terrainSize.x, the3DObj->GetPosition().z / terrainSize.z) - 0.5f,
			the3DObj->GetPosition().z
			));
		(*it)->SetGridIndex(ScenePartitionGraph::Instance().FindGridIndexForPosition(the3DObj->GetPosition()));
		ScenePartitionGraph::Instance().AssignObject(*it);
	}

	for (std::vector<Particle*>::iterator it = theBBManager.BillboardContainer.begin(), end = theBBManager.BillboardContainer.end(); it != end; ++it)
	{
		Particle *theBB = (*it);
		theBB->SetPosition(Vector3(
			((theBB->GetPosition().x - ((float)theNumOfTiles_MapWidth * 0.5f)) * tileSizeXYZ.x) + (tileSizeXYZ.x * 0.5f),
			0,
			((theBB->GetPosition().z - ((float)theNumOfTiles_MapHeight * 0.5f)) * tileSizeXYZ.z) - (tileSizeXYZ.z * 0.5f)
			));
		theBB->SetPosition(Vector3(theBB->GetPosition().x,
			(theBB->GetDimensions().y*0.5f) + terrainSize.y * ReadHeightMap(theHeightMap, theBB->GetPosition().x / terrainSize.x, theBB->GetPosition().z / terrainSize.z),
			theBB->GetPosition().z
			));
		//theBB->SetDefaultLifeTime();
	}
	return true;
}

int GameMap::GetTileSize(void)
{
	return theTileSize;
}

// Get the number of tiles for height of the map
int GameMap::getNumOfTiles_MapHeight(void)
{
	return theNumOfTiles_MapHeight;
}

// Get the number of tiles for width of the map
int GameMap::getNumOfTiles_MapWidth(void)
{
	return theNumOfTiles_MapWidth;
}

Vector3 GameMap::getTileSize3D()
{
	return tileSizeXYZ;
}

bool GameMap::loadThoseLegends(const std::string &fileName)
{
	std::ifstream file(fileName.c_str());
	RenderSystem *graphics = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	if (file.is_open())
	{
		std::vector<std::string> theKeys;
		std::vector<std::string> theValues;
		std::string data = "";
		while (getline(file, data))
		{
			if (data == "" || data == "\n" || data == "\r")
				continue;
			std::string token;
			std::istringstream iss(data);
			if (theKeys.empty())
			{
				while (getline(iss, token, ','))
				{
					if (token == "")
						continue;
					convertStringToUpperCaps(token);
					theKeys.push_back(token);
				}
			}
			else {
				while (getline(iss, token, ','))
				{
					theValues.push_back(token);
				}
				Entity *theObject = new GameObject();
				std::string symbol = " ";
				for (size_t num = 0; num < theKeys.size(); ++num)
				{
					if (theKeys[num].find("NAME") != std::string::npos)
					{
						theObject->SetEntityID(theValues[num]);
					}
					else if (theKeys[num].find("CHARACTERSYMBOL") != std::string::npos)
					{
						symbol = theValues[num];//[0];
					}
					else if (theKeys[num].find("MESHID") != std::string::npos)
					{
						GameObject*go = dynamic_cast<GameObject*>(theObject);
						go->SetMesh(theValues[num]);
					}
					else if (theKeys[num].find("SCALEX") != std::string::npos)
					{
						GameObject*go = dynamic_cast<GameObject*>(theObject);
						go->SetDimensions(Vector3(stof(theValues[num]), go->GetDimensions().y, go->GetDimensions().z));
					}
					else if (theKeys[num].find("SCALEY") != std::string::npos)
					{
						GameObject*go = dynamic_cast<GameObject*>(theObject);
						go->SetDimensions(Vector3(go->GetDimensions().x, stof(theValues[num]), go->GetDimensions().z));
					}
					else if (theKeys[num].find("SCALEZ") != std::string::npos)
					{
						GameObject*go = dynamic_cast<GameObject*>(theObject);
						go->SetDimensions(Vector3(go->GetDimensions().x, go->GetDimensions().y, stof(theValues[num])));
					}
					else if (theKeys[num].find("ACTIVE") != std::string::npos)
					{
						GameObject*go = dynamic_cast<GameObject*>(theObject);
						if (theValues[num] == "0")
							go->Active = false;
						else
							go->Active = true;
					}
					else if (theKeys[num].find("BILLBOARD") != std::string::npos)
					{
						GameObject*go = dynamic_cast<GameObject*>(theObject);
						if (theValues[num] != "0")
						{
							Particle* bb = new Particle();
							bb->Reset();
							bb->SetParameters(go->GetMeshName(), 1, go->GetPosition(), go->GetDimensions(), 0, 0, Vector3(0, 1, 0));
							bb->SetEntityID(go->GetMeshName());
							bb->SetMeshBasedOnID();
							delete go;
							theObject = bb;
						}
                        else {
                                go->SetBounds();
                        }
					}
				}
				bunchOfLegends.insert(std::pair<std::string, Entity*>(symbol, theObject));
				theValues.clear();
			}
		}
        file.close();
		return true;
	}
	return false;
}

void GameMap::convertStringToUpperCaps(std::string &theString)
{
	for (std::string::iterator it = theString.begin(), end = theString.end(); it != end; ++it) {
		(*it) = toupper(*it);
	}
}

void GameMap::AssignTerrainNodes(std::vector<TerrainNode*>* List)
{
	for (auto it : *List)
	{
		TerrainNode* TN = it;
		if (TN == Player)
			Player = Player;

		for (auto it2 : *List)
		{
			if (TN != it2)
			{
				Vector3 GI = it2->GetGridIndex();
				if ((TN->GetGridIndex() == (GI - Vector3(1, 0, 0))) ||
					(TN->GetGridIndex() == (GI - Vector3(0, 0, 1))) ||
					(TN->GetGridIndex() == (GI + Vector3(1, 0, 0))) ||
					(TN->GetGridIndex() == (GI + Vector3(0, 0, 1))))
				{
					TN->LinkedTerrainNodes.push_back(it2);
				}
			}
		}
	}
}