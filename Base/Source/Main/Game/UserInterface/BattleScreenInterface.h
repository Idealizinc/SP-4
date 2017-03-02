#ifndef BATTLESCREENINTERFACE_H
#define BATTLESCREENINTERFACE_H

#include "../../Engine/Interface/InterfaceSystem.h"
#include "../../Engine/System/SceneSystem.h"
//#include "../../Game/Logic/Terrain/TerrainDataLoader.h"

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

	bool PlayerWon;
	bool EnemyWon;
	bool Finished;

	InterfaceLayer* TerrainInfoLayer;
	InterfaceElement* TerrainFrameButton;

	InterfaceLayer* UnitDataLayer;
	InterfaceElement* UnitDataFrameButton;

private:
	void HandleUserInput();
	void PopUpDelay(const float& dt);
	void RecreateDataLayer();

	InterfaceLayer* TurnPopup;
	InterfaceElement* BattleScreenFrame;
	InterfaceElement* BattleWordPopup;
	InterfaceElement* StartWordPopup;

	InterfaceElement* TerrainName;
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
	InterfaceElement* TerrainAdvantageBoost;

	InterfaceElement* UnitDataInfoFrame;
	InterfaceElement* UnitDataInfoText;
	InterfaceElement* UnitDataIcon;
	InterfaceElement* UnitWeaponDataName;
	InterfaceElement* UnitWeaponDataNumber;

	InterfaceLayer* UnitCountInfoLayer;
	InterfaceElement* PlayerUnitCount;
	InterfaceElement* PlayerUnitBarFrame;
	InterfaceElement* PlayerFactionNameText;
	InterfaceElement* PlayerFactionNameBack;
	InterfaceElement* EnemyUnitCount;
	InterfaceElement* EnemyUnitBarFrame;
	InterfaceElement* EnemyFactionNameText;
	InterfaceElement* EnemyFactionNameBack;
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


	float negativeHeight = -(SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight);
	Vector3 CenterTarget;

	bool ShowTerrainInfo;
	bool Initiation;
	bool Followup;
	bool GetMaxData;

	Vector3 defaultbarSize;
	std::map<std::string, std::map<std::string, int>> UnitMap;
};

#endif