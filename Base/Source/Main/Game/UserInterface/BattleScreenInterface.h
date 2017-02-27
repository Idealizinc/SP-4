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

	bool ShowResult;
	bool EndResult;

private:
	void HandleUserInput();
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
	InterfaceElement* PlayerUnitCountText;
	InterfaceElement* EnemyUnitCount;
	InterfaceElement* EnemyUnitCountText;
	InterfaceElement* PlayerUnitCountBackground;
	InterfaceElement* EnemyUnitCountBackground;

	InterfaceLayer *ResultInfoLayer;
	InterfaceElement* ResultInfoBackElement;
	InterfaceElement* ResultInfoFrame;
	InterfaceElement* ResultInfo;
	InterfaceElement* CloseResultButton;

	Terrain* currentTerrain;

	float TurnPopupTimer = 1;
	float TurnPopoutTimer = 1;
	float TurnPopupTimer2 = 0.5f;
	float TurnPopoutTimer2 = 1;
	float TerrainInfoTimer = 2;

	int CNOPU; // CurrentNumberOfPlayerUnit
	int CNOMPU; // CUrrentNumberOfMaxPlayerUnit
	Vector3 currentPlayerBarPos;
	float PlayerRateofChange;
	int CNOEU; //CurrentNumberOfEnemyUnit
	int CNOMEU; //CurrentNumberOfEnemyMaxUnit;
	Vector3 currentEnemyBarPos;
	float EnemyRateofChange;

	Vector3 CenterTarget = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);

	float negativeHeight = -(SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight);

	bool ShowTerrainInfo;
	bool Initiation;
	bool Followup;
	bool Finished;
	bool GetMaxData;

	Vector3 defaultbarSize;
};

#endif