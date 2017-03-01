#include "BattleScreenInterface.h"
#include <sstream>
#include <iomanip>
#include "../Systems/GameLogicSystem.h"
#include "../Logic/Characters/BaseClasses/BattleScreenCharacter.h"

template <typename T>
std::string to_string_with_precision(const T a_value, const int n = 2)
{
	std::ostringstream out;
	out << std::setprecision(n) << a_value;
	return out.str();
}

BattleScreenInterface::BattleScreenInterface()
{
	Init();
}

BattleScreenInterface::~BattleScreenInterface()
{
	Exit();
}

void BattleScreenInterface::Init()
{
	//Start Battle Interface
	TurnPopup = CreateNewInterfaceLayer("StartLayer", 0, 0);


	negativeHeight = -((float)SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight);

	CenterTarget = SceneSystem::Instance().cSS_InputManager->ScreenCenter;

	BattleScreenFrame = TurnPopup->CreateNewInterfaceElement("BattleScreenFrame", "BattleScreenFrame", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight, 0));
	BattleScreenFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));


	BattleWordPopup = TurnPopup->CreateNewInterfaceElement("BattleWordPopUp", "BattleFont", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	BattleWordPopup->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0));

	StartWordPopup = TurnPopup->CreateNewInterfaceElement("StartWordPopUp", "StartFont", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	StartWordPopup->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 2.5f, 0));

	//Show terrain info Interface
	TerrainInfoLayer = CreateNewInterfaceLayer("LeftLayer", 0, 0);

	TerrainAdvantage = TerrainInfoLayer->CreateNewInterfaceElement("TerrainAdvantage", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.3f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	TerrainAdvantage->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.3f, 0));
	TerrainAdvantage->SetTextColor(0);

	TerrainHealthBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainHealthBoostValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainHealthBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y  * 1.2f, 0));
	TerrainHealthBoostValue->SetTextColor(0);

	TerrainMeleeBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMeleeBoostValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.35f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 00), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainMeleeBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.35f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y  * 1.2f, 0));
	TerrainMeleeBoostValue->SetTextColor(0);

	TerrainMagicBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMagicBoostValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.55f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainMagicBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.55f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y  * 1.2f, 0));
	TerrainMagicBoostValue->SetTextColor(0);

	TerrainRangeBoostValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainRangeBoostValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.75f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.2f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainRangeBoostValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.75f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y  * 1.2f, 0));
	TerrainRangeBoostValue->SetTextColor(0);

	TerrainDisadvantage = TerrainInfoLayer->CreateNewInterfaceElement("TerrainDisAdvantage", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	TerrainDisadvantage->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));
	TerrainDisadvantage->SetTextColor(0);

	TerrainHealthMinusValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainHealthMinusValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.8f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainHealthMinusValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y  * 0.8f, 0));
	TerrainHealthMinusValue->SetTextColor(0);

	TerrainMeleeMinusValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMeleeMinusValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.35f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.8f, 00), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainMeleeMinusValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.35f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y  * 0.8f, 0));
	TerrainMeleeMinusValue->SetTextColor(0);

	TerrainMagicMinusValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainMagicMinusValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.55f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.8f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainMagicMinusValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.55f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y  * 0.8f, 0));
	TerrainMagicMinusValue->SetTextColor(0);

	TerrainRangeMinusValue = TerrainInfoLayer->CreateNewInterfaceElement("TerrainRangeMinusValue", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.75f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.8f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.2f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	TerrainRangeMinusValue->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.75f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y  * 0.8f, 0));
	TerrainRangeMinusValue->SetTextColor(0);

	TerrainFrameButton = TerrainInfoLayer->CreateNewInterfaceElement("TerrainFrameButton", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.98f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.025f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.075f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0));
	TerrainFrameButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.98f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y* 1.025f, 0));

	TerrainInfoFrame = TerrainInfoLayer->CreateNewInterfaceElement("TerrainInfoBackground", "InfoFrame", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));
	TerrainInfoFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));

	//Show player unit data info interface
	UnitDataLayer = CreateNewInterfaceLayer("RightLayer", 0, 0);

	UnitDataFrameButton = UnitDataLayer->CreateNewInterfaceElement("UnitDtatFrameButton", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 1.02f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.025f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.075f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.3f, 0));
	UnitDataFrameButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.02f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.025f, 0));

	UnitDataInfoText = UnitDataLayer->CreateNewInterfaceElement("UnitDtatInfoText", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 1.53f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.4f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.25f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.15f, 0));
	UnitDataInfoText->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.53f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.4f, 0));
	UnitDataInfoText->SetText("Unit     Weapon Using    Rarity");
	UnitDataInfoText->SetTextColor(0);

	//Show Player unit data interface
	UnitCountInfoLayer = CreateNewInterfaceLayer("UnitCountLayer", 0, 0);

	CNOPU = CNOEU = 0;
	defaultbarSize = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0);

	PlayerFactionNameText = UnitCountInfoLayer->CreateNewInterfaceElement("PlayerUnitCountText", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.55f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight*0.88f, 0), defaultbarSize * 0.8f);
	PlayerFactionNameText->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.55f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.88f, 0));
	PlayerFactionNameText->SetTextColor(0);

	PlayerUnitCount = UnitCountInfoLayer->CreateNewInterfaceElement("PlayerUnitCount", "GreenBar", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight*0.95f, 0), defaultbarSize);
	PlayerUnitCount->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight*0.95f, 0));

	PlayerUnitCountBackground = UnitCountInfoLayer->CreateNewInterfaceElement("PlayerUnitCountMAX", "RedBar", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0), defaultbarSize);
	PlayerUnitCountBackground->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0));

	EnemyFactionNameText = UnitCountInfoLayer->CreateNewInterfaceElement("EnemyUnitCountText", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x*1.45f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.88f, 0), defaultbarSize * 0.8f);
	EnemyFactionNameText->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.45f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.88f, 0));
	EnemyFactionNameText->SetTextColor(0);

	EnemyUnitCount = UnitCountInfoLayer->CreateNewInterfaceElement("EnemyUnitCount", "GreenBar", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0), defaultbarSize);
	EnemyUnitCount->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0));

	EnemyUnitCountBackground = UnitCountInfoLayer->CreateNewInterfaceElement("EnemyUnitCountMAX", "RedBar", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0), defaultbarSize);
	EnemyUnitCountBackground->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.95f, 0));

	PlayerRateofChange = EnemyRateofChange = 0;

	//Show Result Interface
	ResultInfoLayer = CreateNewInterfaceLayer("ResultInfoLayer", 0, 0);

	ResultInfoFrame = ResultInfoLayer->CreateNewInterfaceElement("ResultInfoFrame", "WoodFrameRect", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.54f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.44f, 0));
	CloseResultButton = ResultInfoLayer->CreateNewInterfaceElement("CloseInfo", "WoodButton", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.8f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0));
	ResultInfo = ResultInfoLayer->CreateNewInterfaceElement("ResultInfo", "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.65f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.3f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.2f, 0));
	ResultInfoBackElement = ResultInfoLayer->CreateNewInterfaceElement("ResultInfoBackground", "Background", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * -1.75f, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.4f, 0));

	ResultInfoFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, negativeHeight * 0.2f, 0));
	CloseResultButton->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, negativeHeight * 0.2f, 0));
	ResultInfo->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, negativeHeight * 0.15f, 0));
	ResultInfoBackElement->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, negativeHeight * 0.2f, 0));

	CloseResultButton->SetText("Continue");
	CloseResultButton->SetTextColor(Vector3(1, 1, 0));
	ResultInfo->SetTextColor(0);

	ResetAll();
}

void BattleScreenInterface::SetTerrain(Terrain* T)
{
	currentTerrain = T;
}

void BattleScreenInterface::Update(const float& dt)
{
	if (GetMaxData)
	{
		CNOMPU = GameLogicSystem::Instance().InternalBattleSystem->GetPlayerCharacterList().size();
		CNOMEU = GameLogicSystem::Instance().InternalBattleSystem->GetEnemyCharacterList().size();
		currentPlayerBarPos = PlayerUnitCountBackground->GetPosition();
		currentEnemyBarPos = EnemyUnitCountBackground->GetPosition();
		int Counter = 0; 
		float offSet = SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 0.08f; 
		Vector3 OriginPos = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 1.18f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.25f, 0);
		Vector3 IconDimensions = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0);
		Vector3 WeaponBarDimensions = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0);

		for (auto it : GameLogicSystem::Instance().InternalBattleSystem->GetPlayerCharacterList())
		{
			BattleScreenCharacter * BSC = (BattleScreenCharacter *)it;
			UnitDataIcon = nullptr;

			auto it2 = UnitMap.find(it->GetEntityID());
			if (it2 == UnitMap.end())
			{
				std::map<std::string, int> Temp;
				Temp.insert(std::pair<std::string, int>(BSC->InternalWeapon->GetEntityID(), 1));
				UnitMap.insert(std::pair<std::string, std::map<std::string, int>>(it->GetEntityID(), Temp));
				it2 = UnitMap.find(it->GetEntityID());
			}
			else
			{
				auto it3 = it2->second.find(BSC->InternalWeapon->GetEntityID());
				if (it3 == it2->second.end())
				{
					it2->second.insert(std::pair<std::string, int>(BSC->InternalWeapon->GetEntityID(), 1));
				}
				else
				{
					it3->second++;
				}
			}

			Vector3 CurrentUIPos = OriginPos + Vector3(0, -offSet * Counter);

			//Icon
			UnitDataIcon = UnitDataLayer->CreateNewInterfaceElement("Icon" + it->GetEntityID(), it->GetMeshName(), CurrentUIPos, IconDimensions);
			UnitDataIcon->SetTargetPosition(CurrentUIPos);
			// Weapon Bar[s]

			for (auto it3 : it2->second)
			{
				// iterate the map of equip wepaon for this monster //it3.first = name
				if (it3.first != BSC->InternalWeapon->GetEntityID())
					continue;
				
				// decide how to offset the bars
				float barOffset = SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 0.05f;
				Vector3 barOriginPos = Vector3(UnitDataIcon->GetPosition().x  * 1.27f, UnitDataIcon->GetPosition().y, 0);
				Vector3 textDimensions = Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.15f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 0.1f, 0);

				// create the bars with the info
				UnitWeaponDataName = UnitDataLayer->CreateNewInterfaceElement("Icon" + it->GetEntityID(), "Transparent", barOriginPos, textDimensions);
				UnitWeaponDataName->SetTargetPosition(barOriginPos);
				UnitWeaponDataName->SetText(it3.first);
			}

			Counter++;

			//UnitWeaponDataName = UnitDataLayer->CreateNewInterfaceElement("Name" + BSC->InternalWeapon->GetEntityID(), "Transparent", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x* 1.53f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y * 1.4f, 0), Vector3(50, 50, 0));
		}
		UnitDataInfoFrame = UnitDataLayer->CreateNewInterfaceElement("UnitDataInfoBackground", "InfoFrame2", Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0), Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));
		UnitDataInfoFrame->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 1.5f, SceneSystem::Instance().cSS_InputManager->ScreenCenter.y, 0));

		GetMaxData = false;
	}

	if (StartBattle)
	{
		CNOPU = GameLogicSystem::Instance().InternalBattleSystem->GetPlayerCharacterList().size();
		CNOEU = GameLogicSystem::Instance().InternalBattleSystem->GetEnemyCharacterList().size();


		PlayerRateofChange = defaultbarSize.x - PlayerUnitCount->GetDimensions().x;
		currentPlayerBarPos = PlayerUnitCountBackground->GetPosition() - Vector3(PlayerRateofChange * 0.5f, 0, 0);
		PlayerUnitCount->SetTargetPosition(currentPlayerBarPos);


		EnemyRateofChange = defaultbarSize.x - EnemyUnitCount->GetDimensions().x;
		currentEnemyBarPos = EnemyUnitCountBackground->GetPosition() + Vector3(EnemyRateofChange * 0.5f, 0, 0);
		EnemyUnitCount->SetTargetPosition(currentEnemyBarPos);


		PlayerUnitCount->SetDimensions(Vector3(defaultbarSize.x * ((float)CNOPU / (float)CNOMPU), PlayerUnitCount->GetDimensions().y, 0));
		EnemyUnitCount->SetDimensions(Vector3(defaultbarSize.x * ((float)CNOEU / (float)CNOMEU), EnemyUnitCount->GetDimensions().y, 0));
	}

	for (auto it : InternalLayerContainer)
	{
		if (it->Active)
			it->Update(dt * 10);
	}
	PopUpDelay(dt);
	TerrainAdvantage->SetText("Advantage");	
	TerrainHealthBoostValue->SetText("Health: x" + to_string_with_precision(currentTerrain->UnitAdvantage.find(Terrain::T_HEALTH)->second));
	TerrainMeleeBoostValue->SetText("Melee: x" + to_string_with_precision(currentTerrain->UnitAdvantage.find(Terrain::T_MELEE)->second));
	TerrainMagicBoostValue->SetText("Magic: x" + to_string_with_precision(currentTerrain->UnitAdvantage.find(Terrain::T_MAGIC)->second));
	TerrainRangeBoostValue->SetText("Range: x" + to_string_with_precision(currentTerrain->UnitAdvantage.find(Terrain::T_RANGE)->second));
	TerrainDisadvantage->SetText("Disdvantage");
	TerrainHealthMinusValue->SetText("Health: x" + to_string_with_precision(currentTerrain->UnitDisadvantage.find(Terrain::T_HEALTH)->second));
	TerrainMeleeMinusValue->SetText("Melee: x" + to_string_with_precision(currentTerrain->UnitDisadvantage.find(Terrain::T_MELEE)->second));
	TerrainMagicMinusValue->SetText("Magic: x" + to_string_with_precision(currentTerrain->UnitDisadvantage.find(Terrain::T_MAGIC)->second));
	TerrainRangeMinusValue->SetText("Range: x" + to_string_with_precision(currentTerrain->UnitDisadvantage.find(Terrain::T_RANGE)->second));

	PlayerFactionNameText->SetText("Human Unit Count: " + std::to_string(CNOPU));
	EnemyFactionNameText->SetText("Demon Unit Count: " + std::to_string(CNOEU));

	//if (GameLogicSystem::Instance().PlayerFaction == GameLogicSystem::F_LIVING)
	//	for (int i = 0; i < GameLogicSystem::Instance().InternalBattleSystem->UnitData.LivingMap.size; ++i)
	//	{
	//		
	//		//UnitMap[i]->first = GameLogicSystem::Instance().InternalBattleSystem->UnitData.LivingMap[i].first;
	//		//UnitMap.insert(std::pair<std::string, >(GameLogicSystem::Instance().InternalBattleSystem->UnitData.LivingMap[i].first, std::pair<std::string, int>(GameLogicSystem::Instance().InternalBattleSystem->UnitData.LivingMap[i].second))
	//	}
	//	//UnitMap.insert(std::pair<GameLogicSystem::Instance().InternalBattleSystem->UnitData.LivingMap;
	//else
	//	for (int i = 0; i < GameLogicSystem::Instance().InternalBattleSystem->UnitData.UndeadMap.size; ++i)
	//	{
	//		UnitMap[i]->first = GameLogicSystem::Instance().InternalBattleSystem->UnitData.UndeadMap[i].first;
	//	}


	if (ShowResult)
	{
		ResultInfoLayer->SetTargetPosition(Vector3(0, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 1.3f, 0));
		TerrainInfoLayer->SetTargetPosition(Vector3(-(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x) * 0.925f, 0, 0));
		UnitDataLayer->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.925f, 0, 0));
		if (CNOPU != 0)
		{
			ResultInfo->SetText("Player Won");
		}
		else if (CNOEU != 0)
		{
			ResultInfo->SetText("Enemy Won");
		}
	}
	HandleUserInput();

}

void BattleScreenInterface::Render()
{
	for (auto it : InternalLayerContainer)
	{
		if (it->Active && it->Visible)
			it->Render();
	}
}

void BattleScreenInterface::HandleUserInput()
{
	Vector3 MousePos = SceneSystem::Instance().cSS_InputManager->GetMousePosition();

	if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_DOWN)
	{
		if (CloseResultButton->DetectUserInput(MousePos, ResultInfoLayer->GetPosition()))
		{
			EndResult = true;
		}

		if (TerrainFrameButton->DetectUserInput(MousePos, TerrainInfoLayer->GetPosition()))
		{
			if ((TerrainInfoLayer->GetPosition() - TerrainInfoLayer->GetTargetPosition()).LengthSquared() < 1)
				TerrainInfoLayer->SwapOriginalWithTarget();
			else
				TerrainInfoLayer->SwapOriginalWithTarget();
		}
		else if (UnitDataFrameButton->DetectUserInput(MousePos, UnitDataLayer->GetPosition()))
		{
			if ((UnitDataLayer->GetPosition() - UnitDataLayer->GetTargetPosition()).LengthSquared() < 1)
				UnitDataLayer->SwapOriginalWithTarget();
			else
				UnitDataLayer->SwapOriginalWithTarget();
		}
	}
	else if (SceneSystem::Instance().cSS_InputManager->GetMouseInput(InputManager::KEY_LMB) == InputManager::MOUSE_UP)
	{


	}
}

void BattleScreenInterface::PopUpDelay(const float& dt)
{
	if (ShowTerrainInfo)
	{
		TerrainInfoTimer -= dt;
		if (TerrainInfoTimer <= 0)
		{
			TerrainInfoLayer->SetTargetPosition(Vector3(-(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x) * 0.925f, 0, 0));
			//TerrainInfoLayer->SetPosition(TerrainInfoLayer->GetTargetPosition());
			UnitDataLayer->SetTargetPosition(Vector3(SceneSystem::Instance().cSS_InputManager->ScreenCenter.x * 0.925f, 0, 0));
			//UnitDataLayer->SetPosition(UnitDataLayer->GetTargetPosition());
			if (TerrainInfoLayer->GetPosition().x <= TerrainInfoLayer->GetTargetPosition().x + 3)
			{
				ShowTerrainInfo = false;
				Initiation = true;
			}
		}
	}

	else if (Initiation)
	{
		TurnPopupTimer -= dt;
		if (TurnPopupTimer <= 0)
		{
			TurnPopoutTimer -= dt;
			BattleWordPopup->SetTargetPosition(CenterTarget);
			if (TurnPopoutTimer <= 0)
			{
				BattleWordPopup->SetTargetPosition(BattleWordPopup->GetOriginalPosition());

				if ((BattleWordPopup->GetPosition() - BattleWordPopup->GetTargetPosition()).LengthSquared() < 1)
				{
					Followup = true;
					Initiation = false;
				}
			}
		}
	}

	else if (Followup)
	{
		TurnPopupTimer2 -= dt;
		if (TurnPopupTimer2 <= 0)
		{
			TurnPopoutTimer2 -= dt;
			StartWordPopup->SetTargetPosition(CenterTarget);
			if (TurnPopoutTimer2  <= 0)
			{
				StartWordPopup->SetTargetPosition(StartWordPopup->GetOriginalPosition());
				if (StartWordPopup->GetPosition().y <= StartWordPopup->GetTargetPosition().y + 3)
				{
					Finished = true;
					StartBattle = true;
				}
			}
		}
	}
}

void BattleScreenInterface::ResetAll()
{
	ResetAllToOriginal();
	TurnPopupTimer = 1;
	TurnPopoutTimer = 1;
	TurnPopupTimer2 = 0.5f;
	TurnPopoutTimer2 = 1;
	TerrainInfoTimer = 2;
	ShowTerrainInfo = true;
	Initiation = false;
	Finished = false;
	Followup = false;
	StartBattle = false;
	GetMaxData = true;
	ShowResult = false;
	EndResult = false;
	TerrainInfoLayer->SetTargetPosition(Vector3(0));
	TerrainInfoLayer->SetPosition(TerrainInfoLayer->GetTargetPosition());
	UnitDataLayer->SetTargetPosition(Vector3(0));
	UnitDataLayer->SetPosition(UnitDataLayer->GetTargetPosition());
	ResultInfoLayer->SetTargetPosition(Vector3(0, 0, 0));
	PlayerUnitCount->SetTargetPosition(PlayerUnitCountBackground->GetTargetPosition());
	EnemyUnitCount->SetTargetPosition(EnemyUnitCountBackground->GetTargetPosition());
	ResultInfoFrame->SetTargetPosition(ResultInfoFrame->GetOriginalPosition());
	ResultInfo->SetTargetPosition(ResultInfo->GetOriginalPosition());
	ResultInfoBackElement->SetTargetPosition(ResultInfoBackElement->GetOriginalPosition());
	CloseResultButton->SetTargetPosition(CloseResultButton->GetOriginalPosition());
	PlayerUnitCount->SetDimensions(defaultbarSize);
	EnemyUnitCount->SetDimensions(defaultbarSize);
}
