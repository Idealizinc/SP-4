/*
Copyright (c) Clinton Teo Jun Quan 2016

Creator(s)    : Clinton Teo Jun Quan
Creation Date : 20 February 2016 13:13 PM
File Name     : Level.h
Purpose       : Level Class
*/

#ifndef LEVEL_H
#define LEVEL_H

#include "../../../Engine/Internal/Entity.h"
#include <vector>

class Level : public Entity
{
public:
	virtual ~Level();

	virtual void Init(){};
	virtual void Update(const float&){};
	virtual void Render(){};
	virtual void Exit(){};

	std::string GetLevelMapName(); 
	std::string GetLevelMapLayoutName();
	std::string GetLevelMapMeshName();

	void SetLevelMapName(const std::string& MapName);
	void SetLevelMapLayoutName(const std::string& MapName);
	void SetLevelMapMeshName(const std::string& MapMesh);

	std::vector<unsigned char> TerrainHeightMap;

private:
	std::string MapName;
	std::string MapLayoutName;
	std::string MapMeshName;

};

#endif