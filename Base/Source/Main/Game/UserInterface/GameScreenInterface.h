#ifndef GAMESCREENINTERFACE_H
#define GAMESCREENINTERFACE_H

#include "../../Engine/Interface/InterfaceSystem.h"
#include "../../Engine/System/SceneSystem.h"
#include "../Systems/GameLogicSystem.h"


class GameScreenInterface : public InterfaceSystem
{
public:
	GameScreenInterface();
	~GameScreenInterface();

	virtual void Init(void);
	virtual void Update(const float& dt);
	virtual void Render(void);
	virtual void Exit(void);

private:
	InterfaceLayer* CreateNewInterfaceLayer(const std::string& LayerID, const Vector3& OriginalPosition, const Vector3& TargetPosition);

	void PopUpDelay(const float& dt);
	void PopUpReset();

	InterfaceLayer* TurnPopup;
	InterfaceElement* PlayerTurnPopup;
	InterfaceElement* EnemyTurnPopup;
	float TurnPopupTimer = 0;
	bool PoppedUp = 0;
	bool PopUpDone = 0;
	Vector3 CenterTarget = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.9, 0);

	std::vector<InterfaceLayer*> InternalLayerContainer;

	bool PlayerTurn;
};

#endif