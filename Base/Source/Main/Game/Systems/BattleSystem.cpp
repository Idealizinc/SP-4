#include "BattleSystem.h"
#include "../../Engine/System/SceneSystem.h"
#include "../Logic/Characters/BaseClasses/BattleScreenCharacter.h"
#include "GameLogicSystem.h"
#include "LoadHmap.h"
#include "../Scene/BattleScene.h"

BattleSystem::BattleSystem()
{
}

BattleSystem::~BattleSystem()
{
	Exit();
}

void BattleSystem::Init()
{
	UnitData.LoadWeaponData("CSVFiles/DataLoaders/WeaponDataLoader.csv");
	UnitData.LoadRaceData("CSVFiles/DataLoaders/RaceDataLoader.csv");
	UnitData.LoadLivingFactionData("CSVFiles/DataLoaders/LivingFactionLoader.csv");
	UnitData.LoadUndeadFactionData("CSVFiles/DataLoaders/UndeadFactionLoader.csv");


	SpawnPosition_Enemy = Vector3(-100, 1, 100);
	SpawnPosition_Player = Vector3(100, 1, -100);
	CurrentBattleTile = nullptr;
	BSI = new BattleScreenInterface();
}

void BattleSystem::Update(const float& dt)
{
	ParticleSystem.UpdateContainer(dt, SceneSystem::Instance().GetCurrentScene().camera->position); 
	BSI->Update(dt);

	if (BSI->StartBattle)
	{
		// I will need to update all my characters, projectiles and other miscellaneous game objects
		UpdateCharacterLogic(InternalEnemyCharacterList, dt);
		UpdateCharacterLogic(InternalPlayerCharacterList, dt);
		UpdateProjectileLogic(dt);
		if (InternalEnemyCharacterList.size() <= 0 && InternalPlayerCharacterList.size() > 0 || InternalEnemyCharacterList.size() > 0 && InternalPlayerCharacterList.size() <= 0)
		{
			BSI->ShowResult = true;
		}

		if (BSI->ShowResult && BSI->EndResult)
		{
			GameLogicSystem::Instance().SetCurrentState(GameLogicSystem::Instance().PlayerTurn);
			SceneSystem::Instance().SwitchScene("1_Scene");
			if (BSI->PlayerWon)
			{
				for (auto it : CurrentBattleTile->EnemyUnitList)
					it->Active = false;
				CurrentBattleTile->EnemyUnitList.clear();
			}
			else if (BSI->EnemyWon)
			{
				for (auto it : CurrentBattleTile->PlayerUnitList)
					it->Active = false;
				CurrentBattleTile->PlayerUnitList.clear();
			}
			ClearCharacterCounters();
			BSI->ResetAll();
			GameLogicSystem::Instance().DetectWinner();
		}
	}
}

void BattleSystem::Render()
{
	ParticleSystem.Render();
	BSI->Render();
	for (auto it : InternalPlayerCharacterList)
		it->Render();
	for (auto it : InternalEnemyCharacterList)
		it->Render();
	for (auto it : InternalProjectileList)
		it->Render();
}

void BattleSystem::Exit()
{
	ClearCharacterCounters();
	if (BSI)
	{
		BSI->StartBattle = false;
		BSI->Exit();
		delete BSI;
		BSI = nullptr;
	}
}

std::vector<CharacterEntity*>& BattleSystem::GetPlayerCharacterList()
{
	return InternalPlayerCharacterList;
}
std::vector<CharacterEntity*>& BattleSystem::GetEnemyCharacterList()
{
	return InternalEnemyCharacterList;
}
std::vector<Projectile*>& BattleSystem::GetProjectileList()
{
	return InternalProjectileList;
}

void BattleSystem::SetUpUnits(Terrain* BattlefieldTile)
{
	ClearCharacterCounters();
	BSI->SetTerrain(BattlefieldTile);
	BattleScene* BS = dynamic_cast<BattleScene*>(&SceneSystem::Instance().GetCurrentScene());
	BS->ReloadMap(BattlefieldTile);
	CurrentBattleTile = BattlefieldTile;
	// For every of the Units within the tile, I am  going to put the entirety of the battalions into my counters
	// Set up the Player
	for (auto it : BattlefieldTile->PlayerUnitList){
		for (auto it2 : it->InternalBattalionList)
		{
			auto it3 = CurrentPlayerUnitCount.find(it2.first);
			if (it3 != CurrentPlayerUnitCount.end())
				it3->second += it2.second;
			else CurrentPlayerUnitCount.insert(std::pair<std::string, unsigned short>(it2.first, it2.second));
		}
	}
	// Set up the Enemy
	for (auto it : BattlefieldTile->EnemyUnitList){
		for (auto it2 : it->InternalBattalionList)
		{
			auto it3 = CurrentEnemyUnitCount.find(it2.first);
			if (it3 != CurrentEnemyUnitCount.end())
				it3->second += it2.second;
			else CurrentEnemyUnitCount.insert(std::pair<std::string, unsigned short>(it2.first, it2.second));
		}
	}
	// Spawn the characters
	SpawnPlayerCharacters(CurrentPlayerUnitCount, BattlefieldTile);
	SpawnEnemyCharacters(CurrentEnemyUnitCount, BattlefieldTile);
}

void BattleSystem::AddNewProjectile(Projectile* P)
{
	//Projectile* Fetched = nullptr;
	//for (std::vector<Projectile*>::iterator it = InternalProjectileList.begin(); it != InternalProjectileList.end(); ++it)
	//{
	//	if ((*it)->Active = false)
	//	{
	//		Fetched = *it;
	//		it = InternalProjectileList.erase(it);
	//		delete Fetched;
	//		Fetched = P;
	//		InternalProjectileList.push_back(Fetched);
	//		break;
	//	}
	//}
	//if (Fetched == nullptr)
		InternalProjectileList.push_back(P);
}

void BattleSystem::ClearCharacterCounters()
{
	CurrentPlayerUnitCount.clear();
	CurrentEnemyUnitCount.clear();
	for (auto it : InternalPlayerCharacterList)
	{
		it->Exit();
		delete it;
	}
	InternalPlayerCharacterList.clear(); 
	
	for (auto it : InternalEnemyCharacterList)
	{
		it->Exit();
		delete it;
	}
	InternalEnemyCharacterList.clear();
	
	for (auto it : InternalProjectileList)
	{
		it->Exit();
		delete it;
	}
	InternalProjectileList.clear();

	CurrentBattleTile = nullptr;
}

void BattleSystem::UpdateCharacterLogic(std::vector<CharacterEntity*>& CharacterList, const float& dt)
{
	for (std::vector<CharacterEntity*>::iterator it = CharacterList.begin(); it != CharacterList.end();)
	{
		if ((*it)->Active)
		{
			// The character will update it's StateManager as well as it's own Kinematic State if possible
			// I will need to do other updates here such as friction/limiting walkspeed
			if (!(*it)->GetVelocity().IsZero())
			{
				Vector3 Direction = (*it)->GetVelocity();
				if ((*it)->GetVelocity().LengthSquared() > (*it)->WalkSpeed * (*it)->WalkSpeed)
				{
					(*it)->SetVelocity(Direction.Normalize() * (float)((*it)->WalkSpeed));
				}

			}
			(*it)->Update(dt);
			++it;
		}
		else {
			(*it)->Exit();
			delete *it;
			it = CharacterList.erase(it);
		}
	}
}

bool CheckCollision(BaseObject *o1, BaseObject *o2, std::string type)
{
	if (type == "Circle")
	{
		float CombinedRadiusSquared = (o1->GetDimensions().x + o2->GetDimensions().x) * (o1->GetDimensions().x + o2->GetDimensions().x)* 0.5f;
		float DistSquared = (o1->GetPosition() - o2->GetPosition()).LengthSquared();
		if (DistSquared < CombinedRadiusSquared)
		{
			return true;
		}
		return false;
	}
	else if (type == "AABB")
		return false;
	else if (type == "OBB")
		return false;
	else return false;
}

void BattleSystem::UpdateProjectileLogic(const float& dt)
{
	for (auto it : InternalProjectileList)
	{
		if (it->Active)
		{
			float TargetPositionY = SceneSystem::Instance().GetCurrentScene().TerrainScale.y * ReadHeightMap(SceneSystem::Instance().GetCurrentScene().m_heightMap, it->GetPosition().x / SceneSystem::Instance().GetCurrentScene().TerrainScale.x, it->GetPosition().z / SceneSystem::Instance().GetCurrentScene().TerrainScale.z);
			if (TargetPositionY > it->GetPosition().y)
			if (it->DespawnOnHit)
				it->Active = false;
		}
		if (it->Active)
		{
			// Find Current Grid
			GridNode* GN = SceneSystem::Instance().GetCurrentScene().ScenePartition->FindGridForPosition(it->GetPosition());
			// Check Collision Here
			if (GN != nullptr)
			{
				std::vector<Node*> NodeList = GN->GetNodeList();
				for (std::vector<Node*>::iterator it2 = NodeList.begin(); it2 != NodeList.end(); ++it2)
				{
					GameObject* GO = dynamic_cast<GameObject*>((*it2)->GetEntity());
					if (GO != nullptr)
					{
						if (GO->Active && GO->Static && GO->GetBoundary() != nullptr)
						{
							Boundary* B = GO->GetBoundary();
							if (B->CheckCollision(it->GetPosition()))
							{
								if (it->DespawnOnHit)
									it->Active = false;
							}
						}
					}
				}
				if (it->Active)
				{
					std::vector<CharacterEntity*> Container;
					if (it->OwnerFaction != GameLogicSystem::Instance().PlayerFaction)
						Container = InternalPlayerCharacterList;
					else Container = InternalEnemyCharacterList;

					for (auto it2 : Container)
					{
						BaseObject *obj2 = dynamic_cast<BaseObject*>(it2);
						if (obj2->Active)
						{
							if (CheckCollision(it, obj2, "Circle"))
							{
								if (it->DespawnOnHit)
									it->Active = false;
								CharacterEntity* CE = it2;
								//Gets Hit
								CE->HealthPoints -= (it)->GetDamageDealt();
								int NumParticles = Math::RandIntMinMax(2, 4);
								for (int i = 0; i < NumParticles; ++i)
								{
									float ParticleSpeed = Math::RandFloatMinMax(1.f, 2.f);
									float ParticleLifeTime = Math::RandFloatMinMax(1.f, 1.5f);
									float Interval = CE->GetDimensions().x * 0.5f;
									Vector3 Dimensions = Vector3(Interval, Interval, Interval);
									Vector3 Velocity = ParticleSpeed * Vector3(Math::RandFloatMinMax(-Interval, Interval), Interval * Math::RandFloatMinMax(1.f, 2.f), Math::RandFloatMinMax(-Interval, Interval));
									GameLogicSystem::Instance().InternalBattleSystem->ParticleSystem.AddWorldSpaceParticle("Blood", CE->GetPosition(), Dimensions, Velocity, SceneSystem::Instance().GetCurrentScene().camera->position, ParticleLifeTime);
								}
							}
						}
					}
					if (it->Active)
					{
						it->Update(dt);
						if (!it->GravityAffected && Math::RandIntMinMax(0, 10) > 8)
						{
							int NumParticles = 1;
							for (int i = 0; i < NumParticles; ++i)
							{
								float ParticleSpeed = Math::RandFloatMinMax(1.f, 2.f);
								float ParticleLifeTime = Math::RandFloatMinMax(0.5f, 1.f);
								float Interval = it->GetDimensions().x * 0.75f;
								Vector3 Dimensions = Vector3(Interval, Interval, Interval);
								Vector3 Velocity = ParticleSpeed * Vector3(Math::RandFloatMinMax(-Interval, Interval), Interval * Math::RandFloatMinMax(1.f, 2.f), Math::RandFloatMinMax(-Interval, Interval));
								GameLogicSystem::Instance().InternalBattleSystem->ParticleSystem.AddWorldSpaceParticle(it->GetMeshName(), it->GetPosition(), Dimensions, Velocity, SceneSystem::Instance().GetCurrentScene().camera->position, ParticleLifeTime);
							}
						}
					}
				}
			}
		}
	}
}

void BattleSystem::SpawnPlayerCharacters(std::map<std::string, unsigned short> PlayerCharacterList, Terrain* Tile)
{
	for (auto PCL : PlayerCharacterList)
	{
		std::map<std::string, UnitType*>UnitMap;
		if (GameLogicSystem::Instance().PlayerFaction == GameLogicSystem::F_LIVING)
			UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.LivingMap;
		else
			UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.UndeadMap;
		UnitType* UT = UnitMap.find(PCL.first)->second;
		if (UT != nullptr)
		{
			for (unsigned int i = 0; i < PCL.second; ++i)
			{
				BattleScreenCharacter* NewChar =  new BattleScreenCharacter();
				NewChar->InitiallizeCharacter(UT, UnitData.RaceMap.find("Human")->second, Tile, true);
				NewChar->IsPlayerCharacter = true;
				NewChar->SetPosition(SpawnPosition_Player);
				NewChar->CharacterFaction = GameLogicSystem::Instance().PlayerFaction;
				InternalPlayerCharacterList.push_back(NewChar);
			}
		}
	}
}

void BattleSystem::SpawnEnemyCharacters(std::map<std::string, unsigned short> EnemyCharacterList, Terrain* Tile)
{
	for (auto ECL : EnemyCharacterList)
	{
		std::map<std::string, UnitType*>UnitMap;
		if (GameLogicSystem::Instance().PlayerFaction != GameLogicSystem::F_LIVING)
			UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.LivingMap;
		else
			UnitMap = GameLogicSystem::Instance().InternalBattleSystem->UnitData.UndeadMap;
		UnitType* UT = UnitMap.find(ECL.first)->second;
		if (UT != nullptr)
		{
			for (unsigned int i = 0; i < ECL.second; ++i)
			{
				BattleScreenCharacter* NewChar = new BattleScreenCharacter();
				NewChar->InitiallizeCharacter(UT, UnitData.RaceMap.find("Human")->second, Tile, true);
				NewChar->IsPlayerCharacter = false;
				NewChar->SetPosition(SpawnPosition_Enemy);
				if (GameLogicSystem::Instance().PlayerFaction == GameLogicSystem::F_LIVING)
					NewChar->CharacterFaction = GameLogicSystem::F_UNDEAD;
				else NewChar->CharacterFaction = GameLogicSystem::F_LIVING;
				InternalEnemyCharacterList.push_back(NewChar);
			}
		}
	}
}