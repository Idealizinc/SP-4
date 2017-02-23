#ifndef _BATTLESCENE_H
#define _BATTLESCENE_H

#include "../../Engine/Objects/SceneEntity.h"
#include "../../Engine/System/SceneSystem.h"
#include "../../Engine/System/RenderSystem.h"
#include "../Objects/Miscellaneous/GameObject.h"
#include "../Systems/BillboardManager.h"
#include "../Objects/Miscellaneous/GameMap.h"
#include "../Miscellaneous/CameraAerial.h"
#include "../Systems/BattleSystem.h"

//changes
#include <vector>

class BattleScene : public SceneEntity
{
public:
	static std::string id_;
	BattleScene();
	virtual ~BattleScene();

	virtual void Init();
	virtual void Update(const float& dt);
	virtual void Render();
	virtual void Exit();

	BattleSystem BS;
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

	const float SkyboxSize = 1000;

	BillboardManager BManager;
	GameObject* Player;

	GameMap *InteractiveMap;
	std::vector<GameObject*> EntityList;
	Vector3 CenterPosition;

	//Changes
	void RenderObjects(BaseObject *obj);
	void UpdateCharacterLogic(double dt);
	void UpdateInternals(double dt);
	bool CheckCollision(BaseObject* o1, BaseObject* o2, std::string type = "Circle");

protected:
	int NumCharacters;
};

#endif // _SCENE_TOWN_1_H