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