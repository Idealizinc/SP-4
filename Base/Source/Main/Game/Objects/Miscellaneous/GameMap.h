#ifndef GAME_MAP_H
#define GAME_MAP_H

#include "../../../Engine/Internal/Entity.h"
#include <vector>
#include <map>
#include "Vector3.h"
#include "GameObject.h"
#include "../../Systems/BillboardManager.h"
#include "../../SceneManagement/Objects/TerrainNode.h"

class GameMap : public Entity
{
public:
    GameMap(void);
    virtual ~GameMap(void);

    virtual bool LoadMap(const std::string &mapName, std::vector<unsigned char> &theHeightMap, Vector3 &terrainSize, std::vector<GameObject*> &theRenderingStuff, BillboardManager &theBBManager);
    int GetTileSize(void);

    int getNumOfTiles_MapHeight(void);		// Get the number of tiles for height of the map
    int getNumOfTiles_MapWidth(void);		// Get the number of tiles for width of the map

	std::map<unsigned, std::vector<std::string>> theScreenMap;
    Vector3 getTileSize3D();

	virtual void Init(){};
	virtual void Update(const float&){};
	virtual void Render(){};
	virtual void Exit(){};

protected:
    int theTileSize;

    int theNumOfTiles_MapHeight;	// Number of tiles in the map's height
    int theNumOfTiles_MapWidth;		// Number of tiles in the map's width

    virtual bool LoadFile(const std::string &mapName, std::vector<unsigned char> &theHeightMap, Vector3 &terrainSize, std::vector<GameObject*> &theRenderingStuff, BillboardManager &theBBManager);
    Vector3 tileSizeXYZ;

private:
	static std::map<std::string, Entity*> bunchOfLegends;
    bool loadThoseLegends(const std::string &fileName);
    void convertStringToUpperCaps(std::string &theString);
	void AssignTerrainNodes(std::vector<TerrainNode*>*);

	TerrainNode* Player;
	TerrainNode* Enemy;
};

#endif