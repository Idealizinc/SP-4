#include "BattleSystem.h"

std::vector<Vector3> PlayerSpawnPoints;
std::vector<Vector3> EnemySpawnPoints;

BattleSystem::BattleSystem()
{
}

BattleSystem::~BattleSystem()
{
}

void BattleSystem::Init()
{
	Loader.LoadWeaponData("CSVFiles/WeaponDataLoader.csv");
	Loader.LoadUnitData("CSVFiles/UnitDataLoader.csv");
	Loader.LoadRaceData("CSVFiles/RaceDataLoader.csv");

	for (float i = 0; i < 50; i += 10)
	{
		PlayerSpawnPoints.push_back(Vector3(-50.f, 10.f, i));
	}

	for (float i = 0; i < 50; i += 10)
	{
		EnemySpawnPoints.push_back(Vector3(50.f, 10.f, i));
	}
	

	std::vector<Vector3> PlayerTempSpawn;
	for (unsigned int i = 0; i < PlayerSpawnPoints.size(); ++i)
	{
		Vector3 Position = PlayerSpawnPoints[i];
		PlayerTempSpawn.push_back(Position);
	}

	std::vector<Vector3> EnemyTempSpawn;
	for (unsigned int i = 0; i < EnemySpawnPoints.size(); ++i)
	{
		Vector3 Position = EnemySpawnPoints[i];
		EnemyTempSpawn.push_back(Position);
	}

	int PRNG = Math::RandIntMinMax(0, PlayerSpawnPoints.size() - 1);
	int ERNG = Math::RandIntMinMax(0, EnemySpawnPoints.size() - 1);

	SpawnPosition_Player = PlayerTempSpawn[PRNG];
	SpawnPosition_Enemy = EnemyTempSpawn[ERNG];

	CurrentPlayerUnitCount.insert(std::pair<std::string, unsigned short>("Crusader", 10));
	CurrentEnemyUnitCount.insert(std::pair<std::string, unsigned short>("Crusader", 10));

	SpawnPlayerCharacter(CurrentPlayerUnitCount);
	SpawnEnemyCharacter(CurrentEnemyUnitCount);
}

void BattleSystem::Update(const float& dt)
{
	for (auto it : InternalPlayerCharacterList)
	{
		it->Update(dt);
	}
	for (auto et : InternalEnemyCharacterList)
	{
		et->Update(dt);
	}
}

void BattleSystem::SpawnPlayerCharacter(std::map<std::string, unsigned short> PlayerCharacterList)
{
	for (auto PCL : PlayerCharacterList)
	{
		UnitType* UT = Loader.UnitMap.find(PCL.first)->second;
		if (UT != nullptr)
		{
			for (unsigned int i = 0; i < PCL.second; ++i)
			{
				CharacterEntity* NewChar = nullptr;
				if (UT->GetType() == UnitType::T_MELEE)
				{
					NewChar = new MeleeCharacter();
					MeleeCharacter* MC = dynamic_cast<MeleeCharacter*>(NewChar);
					MC->SetCharacter(UT, Loader.RaceMap.find("Human")->second);
					MC->isPlayer = true;
					NewChar->SetDimensions(Vector3(UnitSize, UnitSize, UnitSize));
					int RNG = Math::RandIntMinMax(0, PlayerSpawnPoints.size()-1);
					MC->TargetDir.x = (SpawnPosition_Enemy.x - SpawnPosition_Player.x)/10;
					NewChar->SetPosition(PlayerSpawnPoints[RNG]);
				}

				if (NewChar != nullptr)
				{
					InternalPlayerCharacterList.push_back(NewChar);
				}
			}
		}
	}
}

void BattleSystem::SpawnEnemyCharacter(std::map<std::string, unsigned short> EnemyCharacterList)
{
	for (auto ECL : EnemyCharacterList)
	{
		UnitType* UT = Loader.UnitMap.find(ECL.first)->second;
		if (UT != nullptr)
		{
			for (unsigned int i = 0; i < ECL.second; ++i)
			{
				CharacterEntity* NewChar = nullptr;
				if (UT->GetType() == UnitType::T_MELEE)
				{
					NewChar = new MeleeCharacter();
					MeleeCharacter* MC = dynamic_cast<MeleeCharacter*>(NewChar);
					MC->SetCharacter(UT, Loader.RaceMap.find("Elven")->second);
					NewChar->SetDimensions(Vector3(UnitSize, UnitSize, UnitSize));
					int RNG = Math::RandIntMinMax(0, EnemySpawnPoints.size()-1);
					MC->TargetDir.x = (SpawnPosition_Player.x - SpawnPosition_Enemy.x)/10;
					NewChar->SetPosition(EnemySpawnPoints[RNG]);
				}

				if (NewChar != nullptr)
				{
					InternalEnemyCharacterList.push_back(NewChar);
				}
			}
		}
	}
}

void BattleSystem::AddNewProjectile(Projectile* P)
{
	Projectile* Fetched = nullptr;
	for (std::vector<Projectile*>::iterator it = InternalProjectileList.begin(); it != InternalProjectileList.end(); ++it)
	{
		if ((*it)->Active = false)
		{
			Fetched = *it;
			it = InternalProjectileList.erase(it);
			delete Fetched;
			Fetched = P;
			InternalProjectileList.push_back(Fetched);
			break;
		}
	}
	if (Fetched == nullptr)
		InternalProjectileList.push_back(P);
}

void BattleSystem::Render()
{
	for (auto it : InternalPlayerCharacterList)
	{
		it->Render();
	}
	for (auto et : InternalEnemyCharacterList)
	{
		et->Render();
	}
}

void BattleSystem::ClearCharacterCount()
{
	for (std::map<std::string, unsigned short>::iterator it = CurrentPlayerUnitCount.begin(); it != CurrentPlayerUnitCount.end(); ++it)
	{
		it->second = 0;
	}
	for (std::map<std::string, unsigned short>::iterator it2 = CurrentEnemyUnitCount.begin(); it2 != CurrentEnemyUnitCount.end(); ++it2)
	{
		it2->second = 0;
	}
}

std::vector<CharacterEntity*> BattleSystem::GetPlayerCharacterList()
{
	return InternalPlayerCharacterList;
}
std::vector<CharacterEntity*> BattleSystem::GetEnemyCharacterList()
{
	return InternalEnemyCharacterList;
}
std::vector<Projectile*>& BattleSystem::GetProjectileList()
{
	return InternalProjectileList;
}

void BattleSystem::Exit()
{
	while (InternalPlayerCharacterList.size() > 0)
	{
		CharacterEntity* obj = InternalPlayerCharacterList.back();
		obj->Exit();
		delete obj;
		InternalPlayerCharacterList.pop_back();
	}
	while (InternalEnemyCharacterList.size() > 0)
	{
		CharacterEntity* obj = InternalEnemyCharacterList.back();
		obj->Exit();
		delete obj;
		InternalEnemyCharacterList.pop_back();
	}
}