#ifndef _SCENE_GAME_H
#define _SCENE_GAME_H

#include "../../Engine/Objects/SceneEntity.h"
#include "../../Engine/System/SceneSystem.h"
#include "../../Engine/System/RenderSystem.h"
#include "../../Engine/Objects//GameObject.h"
#include "../Systems/BillboardManager.h"
#include "../Objects/GameMap.h"
#include "../Miscellaneous/CameraAerial.h"
#include "../UserInterface/GameScreenInterface.h"
#include "../Logic/Level/Level.h"

class GameScene : public SceneEntity
{
public:
    static std::string id_;
    GameScene();
    virtual ~GameScene();

    virtual void Init();
	virtual void Update(const float& dt);
    virtual void Render();
    virtual void Exit();
	void SetUpForLevel(Level*);

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
	std::string TerrainMeshName;

	bool cameraZoomOut = false;
	
};

#endif // _SCENE_GAME_H