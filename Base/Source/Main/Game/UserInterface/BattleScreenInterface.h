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
	
	void SetTerrain(Terrain* T);
	void ResetAll();
	bool StartBattle;

private:
	void PopUpDelay(const float& dt);

	InterfaceLayer* TurnPopup;
	InterfaceElement* BattleWordPopup;
	InterfaceElement* StartWordPopup;

	InterfaceLayer* TerrainInfoLayer;
	InterfaceElement* TerrainInfoBackElement;
	InterfaceElement* TerrainInfoFrame;
	InterfaceElement* TerrainAdvantage;
	InterfaceElement* TerrainHealthBoostValue;
	InterfaceElement* TerrainMeleeBoostValue;
	InterfaceElement* TerrainMagicBoostValue;
	InterfaceElement* TerrainRangeBoostValue;
	InterfaceElement* TerrainDisadvantage;
	InterfaceElement* TerrainHealthMinusValue;
	InterfaceElement* TerrainMeleeMinusValue;
	InterfaceElement* TerrainMagicMinusValue;
	InterfaceElement* TerrainRangeMinusValue;

	InterfaceLayer* UnitCountInfoLayer;
	InterfaceElement* PlayerUnitCount;
	InterfaceElement* EnemyUnitCount;

	Terrain* currentTerrain;

	float TurnPopupTimer = 1;
	float TurnPopoutTimer = 1;
	float TurnPopupTimer2 = 0.5f;
	float TurnPopoutTimer2 = 1;
	float TerrainInfoTimer = 2;

	Vector3 CenterTarget;

	int negativeHeight;

	bool ShowTerrainInfo;
	bool Initiation;
	bool Followup;
	bool Finished;
};

#endif