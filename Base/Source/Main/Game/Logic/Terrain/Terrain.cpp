#include "Terrain.h"

Terrain::~Terrain()
{
	Exit();
}

void Terrain::Exit()
{
	// No need to delete for the Unit Pieces are pointers that are stored within and handled by their respective systems
	//for (auto it : PlayerUnitList)
	//	delete it;
	PlayerUnitList.clear();
	//for (auto it : EnemyUnitList)
	//	delete it;
	EnemyUnitList.clear();
}

// Getters
std::string Terrain::GetTerrainName()
{
	return TerrainName;
}
int Terrain::GetTerrainType()
{
	return TerrainType;
}
float Terrain::GetUnitAdvantage(int stat)
{
	return UnitAdvantage.find(stat)->second;
}
float Terrain::GetUnitDisadvantage(int stat)
{
	return UnitDisadvantage.find(stat)->second;
}

// Setters
void Terrain::SetTerrainName(std::string TerrainName)
{
	this->TerrainName = TerrainName;
}
void Terrain::SetTerrainType(int TerrainType)
{
	this->TerrainType = TerrainType;
}
void Terrain::SetUnitAdvantage(int stat, float value)
{
	UnitAdvantage.insert(std::pair<int, float>(stat, value));
}
void Terrain::SetUnitDisadvantage(int stat, float value)
{
	UnitDisadvantage.insert(std::pair<int, float>(stat, value));
}