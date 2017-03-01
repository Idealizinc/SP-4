#include "Level.h"

Level::~Level()
{

}

std::string Level::GetLevelMapName()
{
	return MapName;
}
std::string Level::GetLevelMapMesh()
{
	return MapMesh;
}
std::string Level::GetLevelHeightMap()
{
	return HeightMap;
}
std::string Level::GetLevelTexture1()
{
	return Texture1;
}
std::string Level::GetLevelTexture2()
{
	return Texture2;
}

void Level::SetLevelMapName(std::string MapName)
{
	this->MapName = MapName;
}
void Level::SetLevelMapMesh(std::string MapMesh)
{
	this->MapMesh = MapMesh;
}
void Level::SetLevelHeightMap(std::string HeightMap)
{
	this->HeightMap = HeightMap;
}
void Level::SetLevelTexture1(std::string Texture1)
{
	this->Texture1 = Texture1;
}
void Level::SetLevelTexture2(std::string Texture2)
{
	this->Texture2 = Texture2;
}