#ifndef LEVEL_H
#define LEVEL_H

#include "../../../Engine/Internal/Entity.h"
#include <map>

class Level : public Entity
{
public:
	virtual ~Level();

	virtual void Init(){};
	virtual void Update(const float&){};
	virtual void Render(){};
	virtual void Exit(){};

	std::string GetLevelMapName();
	std::string GetLevelMapMesh();
	std::string GetLevelHeightMap();
	std::string GetLevelTexture1();
	std::string GetLevelTexture2();

	void SetLevelMapName(std::string MapName);
	void SetLevelMapMesh(std::string MapMesh);
	void SetLevelHeightMap(std::string HeightMap);
	void SetLevelTexture1(std::string Texture1);
	void SetLevelTexture2(std::string Texture2);

private:
	std::string MapName;
	std::string MapMesh;
	std::string HeightMap;
	std::string Texture1;
	std::string Texture2;

};

#endif