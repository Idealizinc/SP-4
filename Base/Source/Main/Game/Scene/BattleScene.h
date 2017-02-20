#ifndef _BATTLESCENE_H
#define _BATTLESCENE_H

#include "../../Engine/Objects/SceneEntity.h"
#include "../../Engine/System/SceneSystem.h"
#include "../../Engine/System/RenderSystem.h"
#include "../Objects/Miscellaneous/GameObject.h"
#include "../Systems/BillboardManager.h"
#include "../Objects/Miscellaneous/GameMap.h"
#include "../Miscellaneous/CameraAerial.h"

#include "../../Game/Objects/UnitData/UnitDataLoader.h"
#include "../../Game/Objects/Characters/MeleeCharacter.h"
#include "../../Game/Objects/Characters/RangeCharacter.h"

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

	//changes
	Vector3 SpawnPosition_Melee;
	Vector3 SpawnPosition_Range;
	const float MeleeSize = 10.f;
	const float RangeSize = 10.f;
	UnitDataLoader loader;

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

	const std::string Melee = "Crusader";
	const std::string Range = "Mercedes";

	CharacterEntity* NewChar;
	CharacterEntity* NewChar2;

protected:
	int NumCharacters;
};

#endif // _SCENE_TOWN_1_H