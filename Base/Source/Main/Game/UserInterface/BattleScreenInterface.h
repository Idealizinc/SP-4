#ifndef BATTLESCREENINTERFACE_H
#define BATTLESCREENINTERFACE_H

#include "../../Engine/Interface/InterfaceSystem.h"
#include "../../Engine/System/SceneSystem.h"
#include "../../Game/Logic/Terrain/TerrainDataLoader.h"

class BattleScreenInterface : public InterfaceSystem
{
public:
	BattleScreenInterface();
	~BattleScreenInterface();

	virtual void Init(void);
	virtual void Update(const float& dt);
	virtual void Render(void);
	virtual void Exit(void);
	
	void SetTerrain(Terrain* T);

private:
	InterfaceLayer* CreateNewInterfaceLayer(const std::string& LayerID, const Vector3& OriginalPosition, const Vector3& TargetPosition);

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

	//InterfaceElement* PlayerTurnPopup;

	float TurnPopupTimer = 0;
	bool PoppedUp = 0;
	bool PopUpDone = 0;
	bool PoppedUp2 = 0;
	bool PopUpDone2 = 0;
	Vector3 CenterTarget = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);
	float floatValueReader;

	std::vector<InterfaceLayer*> InternalLayerContainer;

	bool Initiation;
	bool Followup;
	bool Finished;
};

#endif