#include "UnitSpawnSystem.h"
#include "../Systems/GameLogicSystem.h"
#include <sstream>

UnitSpawnSystem::UnitSpawnSystem()
{
	//UnitMap.insert(std::pair<int, int>(U_MELEE, 0));

	//UnitMap.insert(std::pair<int, int>(U_MAGIC, 0));

	//UnitMap.insert(std::pair<int, int>(U_RANGE, 0));

	//GameLogicSystem::Instance().InternalBattleSystem->UnitData.UnitMap;

}

UnitSpawnSystem::~UnitSpawnSystem()
{
}


void UnitSpawnSystem::CreateUnitUIElement(InterfaceLayer* layer)
{
	Vector3 HalfDimension = SceneSystem::Instance().cSS_InputManager->ScreenCenter;
	Vector3 Offset(0, HalfDimension.y*0.4f, 0);
	unsigned short counter = 0;
	Vector3 BackingDimension = Vector3(HalfDimension.x *0.65f, HalfDimension.y*0.3f, 1);

	std::stringstream ss;

	std::map<std::string, UnitType*>UnitMap;
	if (GameLogicSystem::Instance().PlayerFaction == GameLogicSystem::F_LIVING)
		UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.LivingMap;
	else
		UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.UndeadMap;
	for (auto it : UnitMap)
	{
		Vector3 Pos = Vector3(0, HalfDimension.y * 1.8f, 0) - Offset * counter;
		
		InterfaceElement* tempElement = nullptr;
				// Image Icon
		tempElement = layer->CreateNewInterfaceElement("UnitKImage", it.second->GetMeshName(), Pos - Vector3(BackingDimension.x * 0.18f, BackingDimension.y * 0.15f), Vector3(HalfDimension.x *0.12f, HalfDimension.y*0.2f, 1));
		tempElement->SetTargetPosition(Pos - Vector3(BackingDimension.x * 0.18f, BackingDimension.y * 0.15f));
		tempElement = layer->CreateNewInterfaceElement("UnitInfoName", "Transparent", Pos - Vector3(BackingDimension.x * 0.25f, -BackingDimension.y * 0.35f), Vector3(HalfDimension.x *0.15f, HalfDimension.y*0.125f, 1));
		tempElement->SetTargetPosition(Pos - Vector3(BackingDimension.x * 0.25f, -BackingDimension.y * 0.35f));
		tempElement->SetText(it.first);
		tempElement->SetTextColor(0);

		tempElement = layer->CreateNewInterfaceElement("UnitInfoType", "Transparent", Pos - Vector3(BackingDimension.x * 0.4f, -(BackingDimension.y * -0.15f)), Vector3(HalfDimension.x *0.12f, HalfDimension.y*0.1f, 1));
		tempElement->SetTargetPosition(Pos - Vector3(BackingDimension.x * 0.37f, -(BackingDimension.y * -0.15f)));
		if (it.second->GetType() == 0)
		{
			tempElement->SetText("Warrior");
		}
		else if (it.second->GetType() == 1)
		{
			tempElement->SetText("Mage");
		}
		else if (it.second->GetType() == 2)
		{
			tempElement->SetText("Ranger");
		}
		tempElement->SetTextColor(0);

		// Stats
		/*tempElement = layer->CreateNewInterfaceElement("Stats", "quad", Pos , Vector3(HalfDimension.x *0.2f, HalfDimension.y*0.2f, 1));
		tempElement->SetTargetPosition(Pos );
		ss.str("");
		ss << "Health: " << it.second->GetMaxHealth();
		tempElement->SetText(ss.str());*/

		//tempElement = layer->CreateNewInterfaceElement("HealthImage", "health", Pos - Vector3(BackingDimension.x * 0.f, BackingDimension.y * -0.3f), Vector3(HalfDimension.x *0.05f, HalfDimension.y*0.1f, 1));
		//tempElement->SetTargetPosition(Pos - Vector3(BackingDimension.x * 0.f, BackingDimension.y * -0.3f));

		tempElement = layer->CreateNewInterfaceElement("CostStats", "Transparent", Pos + Vector3(BackingDimension.x * 0.25f), Vector3(HalfDimension.x *0.25f, HalfDimension.y*0.15f, 1));
		tempElement->SetTargetPosition(Pos + Vector3(BackingDimension.x * 0.25f));
		ss.str("");
		ss << "$" << it.second->GetCost();
		tempElement->SetText(ss.str());
		tempElement->SetTextColor(0);

		tempElement = layer->CreateNewInterfaceElement("HealthImage", "health", Pos - Vector3(BackingDimension.x * 0.f, BackingDimension.y * -0.3f), Vector3(HalfDimension.x *0.05f, HalfDimension.y*0.1f, 1));
		tempElement->SetTargetPosition(Pos - Vector3(BackingDimension.x * 0.f, BackingDimension.y * -0.3f));

		tempElement = layer->CreateNewInterfaceElement("HealthStats", "Transparent", Pos + Vector3(BackingDimension.x * 0.1f, BackingDimension.y * 0.3f), Vector3(HalfDimension.x *0.2f, HalfDimension.y*0.1f, 1));
		tempElement->SetTargetPosition(Pos + Vector3(BackingDimension.x * 0.1f, BackingDimension.y * 0.3f));
		ss.str("");
		ss << it.second->GetMaxHealth();
		tempElement->SetText(ss.str());
		tempElement->SetTextColor(0);

		tempElement = layer->CreateNewInterfaceElement("DamageImage", "sword", Pos - Vector3(BackingDimension.x * 0.f), Vector3(HalfDimension.x *0.05f, HalfDimension.y*0.1f, 1));
		tempElement->SetTargetPosition(Pos - Vector3(BackingDimension.x * 0.f));

		int lowestWepDamage = NULL;
		int highestWepDamage = NULL;
		for (unsigned int i = 0; i < it.second->PossibleWeapon.size(); ++i)
		{
			if (lowestWepDamage == NULL)
			{
				lowestWepDamage = it.second->PossibleWeapon[i]->GetDamage();
				highestWepDamage = it.second->PossibleWeapon[i]->GetDamage();
			}
			else
			{
				if (it.second->PossibleWeapon[i]->GetDamage() > highestWepDamage)
				{
					highestWepDamage = it.second->PossibleWeapon[i]->GetDamage();
				}
				else if (it.second->PossibleWeapon[i]->GetDamage() < lowestWepDamage)
				{
					lowestWepDamage = it.second->PossibleWeapon[i]->GetDamage();
				}
			}
		}

		
		tempElement = layer->CreateNewInterfaceElement("DamageStats", "Transparent", Pos + Vector3(BackingDimension.x * 0.1f), Vector3(HalfDimension.x *0.2f, HalfDimension.y*0.1f, 1));
		tempElement->SetTargetPosition(Pos + Vector3(BackingDimension.x * 0.1f));
		ss.str("");
		if (lowestWepDamage != highestWepDamage)
			ss  << lowestWepDamage << "-" << highestWepDamage;
		else
			ss << lowestWepDamage;
		tempElement->SetText(ss.str());
		tempElement->SetTextColor(0);

		tempElement = layer->CreateNewInterfaceElement("WalkImage", "boot", Pos - Vector3(BackingDimension.x * 0.f, BackingDimension.y * 0.3f), Vector3(HalfDimension.x *0.05f, HalfDimension.y*0.1f, 1));
		tempElement->SetTargetPosition(Pos - Vector3(BackingDimension.x * 0.f, BackingDimension.y * 0.3f));

		tempElement = layer->CreateNewInterfaceElement("WalkStats", "Transparent", Pos + Vector3(BackingDimension.x * 0.1f, BackingDimension.y * -0.3f), Vector3(HalfDimension.x *0.2f, HalfDimension.y*0.1f, 1));
		tempElement->SetTargetPosition(Pos + Vector3(BackingDimension.x * 0.1f, BackingDimension.y * -0.3f));
		ss.str("");
		ss  << it.second->GetWalkspeed();
		tempElement->SetText(ss.str());
		tempElement->SetTextColor(0);

		// Plus Icon and Push to Plus Map
		tempElement = layer->CreateNewInterfaceElement("Plus", "GreenBar", Pos + Vector3(BackingDimension.x * 0.4f, BackingDimension.y * 0.25f), Vector3(HalfDimension.y *0.1f, HalfDimension.y*0.1f, 1));
		tempElement->SetTargetPosition(Pos + Vector3(BackingDimension.x * 0.4f, BackingDimension.y * 0.25f));
		tempElement->SetText("+");
		tempElement->SetTextColor(0);
		UnitAdditionIconMap.insert(std::pair<std::string, InterfaceElement*>(it.first, tempElement));
		// Minus Iconand Push to Minus Map
		tempElement = layer->CreateNewInterfaceElement("Minus", "RedBar", Pos + Vector3(BackingDimension.x * 0.4f, -BackingDimension.y * 0.25f), Vector3(HalfDimension.y *0.1f, HalfDimension.y*0.1f, 1));
		tempElement->SetTargetPosition(Pos + Vector3(BackingDimension.x * 0.4f, -BackingDimension.y * 0.25f));
		tempElement->SetText("-");
		//tempElement->SetTextColor(0);
		UnitSubtractionIconMap.insert(std::pair<std::string, InterfaceElement*>(it.first, tempElement));

		//scroller
		

		// Background
		tempElement = layer->CreateNewInterfaceElement("UnitInfoBacking", "Background", Pos, BackingDimension);
		tempElement->SetTargetPosition(Pos);

		//RecordedUnitMap.insert(std::pair<std::string, unsigned short>(it.first, 0));

		// Increment the counter after all UI for this bar has been made
		++counter;
	}
	
	
}



void UnitSpawnSystem::MapReset()
{
	
	UnitAdditionIconMap.clear();
	UnitSubtractionIconMap.clear();

}

void UnitSpawnSystem::editUnit(std::string type, unsigned short count)
{
	RecordedUnitMap.find(type)->second += count;
}

std::map<std::string, unsigned short> UnitSpawnSystem::returnRecordedUnitMap()
{
	return RecordedUnitMap;
}

int UnitSpawnSystem::getCurrentUnitCount()
{
	int returnvalue = 0;
	for (auto it : RecordedUnitMap)
	{
		returnvalue += it.second;
	}
	return returnvalue;

}

void UnitSpawnSystem::resetUnitMap()
{
	RecordedUnitMap.clear();
	//UnitMap.insert(std::pair<int, int>(U_MELEE, 0));
	//UnitMap.insert(std::pair<int, int>(U_MAGIC, 0));
	//UnitMap.insert(std::pair<int, int>(U_RANGE, 0));
}

void UnitSpawnSystem::HandleUserInput(const Vector3& MousePos, const Vector3& LayerPos)
{
	for (auto it : UnitAdditionIconMap)
	{
		if (it.second->DetectUserInput(MousePos,LayerPos))
		{
			auto it2 = RecordedUnitMap.find(it.first);
			bool EntryExists = true;
			if (it2 == RecordedUnitMap.end())
			{
				// I cannot find so i will make a new entry
				EntryExists = false;
			}

			// Set up the correct container
			std::map<std::string, UnitType*>UnitMap;
			if (GameLogicSystem::Instance().PlayerFaction == GameLogicSystem::F_LIVING)
				UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.LivingMap;
			else
				UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.UndeadMap;

			if (CalculateCost() + UnitMap.find(it.first)->second->GetCost() <= maxUnitCost)
			{
				// Create a new entry if needed
				if (!EntryExists)
					RecordedUnitMap.insert(std::pair<std::string, unsigned short>(it.first, 1));
				else
				{
					// Add to a previously existing entry
					std::map<std::string, unsigned short>::iterator NewEntry = RecordedUnitMap.find(it.first);

					// Add it in
					++NewEntry->second;
				}
			}
		}
	}

	for (auto it : UnitSubtractionIconMap)
	{
		if (it.second->DetectUserInput(MousePos, LayerPos))
		{
			auto it2 = RecordedUnitMap.find(it.first);
			if (it2 != RecordedUnitMap.end())
			{
				//if found and already existing
				--it2->second;
				if (it2->second <= 0)
				{
					RecordedUnitMap.erase(it2->first);
				}
			}
			
		}
	}
}

int UnitSpawnSystem::CalculateCost()
{
	std::map<std::string, UnitType*>UnitMap;
	if (GameLogicSystem::Instance().PlayerFaction == GameLogicSystem::F_LIVING)
		UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.LivingMap;
	else
		UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.UndeadMap;
	int CumulativeCost = 0;
	for (auto it2 : RecordedUnitMap)
	{
		auto it = UnitMap.find(it2.first);
		if (it != UnitMap.end())
		{
			// This unit exists, getting the cost.
			CumulativeCost += it->second->GetCost() * it2.second;
		}
	}
	return CumulativeCost;
}