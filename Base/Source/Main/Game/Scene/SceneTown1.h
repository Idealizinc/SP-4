#ifndef _SCENE_TOWN_1_H
#define _SCENE_TOWN_1_H

#include "../../Engine/Objects/SceneEntity.h"
#include "../../Engine/System/SceneSystem.h"
#include "../../Engine/System/RenderSystem.h"
#include "../Objects/Miscellaneous/GameObject.h"
#include "../Systems/BillboardManager.h"
#include "../Objects/Miscellaneous/GameMap.h"
#include "../Miscellaneous/CameraAerial.h"
#include"../UserInterface/GameScreenInterface.h"

class Enemy
{
public:
	Vector3 currPos;
	std::vector<TerrainNode*> targetNode;
};

class SceneTown1 : public SceneEntity
{
public:
    static std::string id_;
    SceneTown1();
    virtual ~SceneTown1();

    virtual void Init();
	virtual void Update(const float& dt);
    virtual void Render();
    virtual void Exit();

private:
    // Shadow GPass
    void RenderPassGPass();
    void RenderPassMain();

	void QuickInit();
	void QuickExit();

    // Additional Calls
    void RenderShadowCasters();
    void RenderTerrain();
    void RenderSkybox();

    const float SkyboxSize = 1500;

    BillboardManager BManager;
    GameObject* Player;

    GameMap *InteractiveMap;
	std::vector<GameObject*> EntityList;
	Vector3 CenterPosition;

	GameScreenInterface* GSI = new GameScreenInterface();
	
};

#endif // _SCENE_TOWN_1_H