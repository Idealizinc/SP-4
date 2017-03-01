#include "Level.h"

Level::~Level()
{

}

std::string Level::GetLevelMapName()
{
	return MapName;
}
std::string Level::GetLevelMapLayoutName()
{
	return MapLayoutName;
}
std::string Level::GetLevelMapMeshName()
{
	return MapMeshName;
}

void Level::SetLevelMapName(const std::string& MapName)
{
	this->MapName = MapName;
}
void Level::SetLevelMapLayoutName(const std::string& MapName)
{
	this->MapLayoutName = MapName;
}
void Level::SetLevelMapMeshName(const std::string& MapMesh)
{
	this->MapMeshName = MapMesh;
}