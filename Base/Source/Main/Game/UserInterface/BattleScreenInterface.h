#ifndef BATTLESCREENINTERFACE_H
#define BATTLESCREENINTERFACE_H

#include "../../Engine/Interface/InterfaceSystem.h"
#include "../../Engine/System/SceneSystem.h"
#include "../../Game/Logic/Terrain/TerrainDataLoader.h"

class BattleScreenInterface : public InterfaceSystem
{
public:
	BattleScreenInterface();
	virtual ~BattleScreenInterface();

	virtual void Init(void);
	virtual void Update(const float& dt);
	virtual void Render(void);
	virtual void Exit(void);
	
	void SetTerrain(Terrain* T);

private:
	void PopUpDelay(const float& dt);
	void PopUpReset();
	void ResetAll();

	InterfaceLayer* TurnPopup;
	InterfaceElement* BattleWordPopup;
	InterfaceElement* StartWordPopup;

	InterfaceLayer* TerrainInfoLayer;
	InterfaceElement* TerrainInfoBackElement;
	InterfaceElement* TerrainInfoFrame;
	InterfaceElement* TerrainHealthBoostValue;
	InterfaceElement* TerrainMeleeBoostValue;
	InterfaceElement* TerrainMagicBoostValue;
	InterfaceElement* TerrainRangeBoostValue;

	Terrain* currentTerrain;

	float TurnPopupTimer = 0;
	bool PoppedUp = 0;
	bool PopUpDone = 0;
	Vector3 CenterTarget = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.5, 0);

	bool Initiation;
	bool Followup;
	bool Finished;
};

#endif