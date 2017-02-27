#ifndef _ENDOFGAME_H
#define _ENDOFGAME_H

#include "../../Engine/Objects/SceneEntity.h"
#include "../../Engine/System/SceneSystem.h"
#include "../../Engine/System/RenderSystem.h"
#include "../../Engine/Objects/GameObject.h"
#include "../Systems/BillboardManager.h"
#include "../Miscellaneous/CameraAerial.h"
#include "../../Game/UserInterface/EndOfGameInterface.h"

//changes
#include <vector>

class EndOfGameScene : public SceneEntity
{
public:
	static std::string id_;
	EndOfGameScene();
	virtual ~EndOfGameScene();

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

	Vector3 CenterPosition;
	EndOfGameInterface* EOGI = new EndOfGameInterface();

protected:
};

#endif // _SCENE_TOWN_1_H