#include "BattleSystem.h"
#include "../../Engine/System/SceneSystem.h"
#include "../Objects/Characters/MeleeCharacter.h"

BattleSystem::BattleSystem()
{
}

BattleSystem::~BattleSystem()
{
}

void BattleSystem::Init()
{
	UnitData.LoadWeaponData("CSVFiles/WeaponDataLoader.csv");
	UnitData.LoadUnitData("CSVFiles/UnitDataLoader.csv");
	UnitData.LoadRaceData("CSVFiles/RaceDataLoader.csv");

	SpawnPosition_Enemy = Vector3(-20, 1, 0);
	SpawnPosition_Player = Vector3(20, 1, 0);
}

void BattleSystem::Update(const float& dt)
{
	// I will need to update all my characters, projectiles and other miscellaneous game objects
	UpdateCharacterLogic(InternalPlayerCharacterList, dt);
	UpdateCharacterLogic(InternalEnemyCharacterList, dt);
	UpdateProjectileLogic(dt);
}

void BattleSystem::Render()
{
	for (auto it : InternalPlayerCharacterList)
		it->Render();
	for (auto it : InternalEnemyCharacterList)
		it->Render();
	for (auto it : InternalProjectileList)
		it->Render();
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
	while (InternalProjectileList.size() > 0)
	{
		Projectile* obj = InternalProjectileList.back();
		obj->Exit();
		delete obj;
		InternalProjectileList.pop_back();
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
	// For every of the Units within the tile, I am  going to put the entirety of the battalions into my counters
	// Set up the Player
	for (auto it : BattlefieldTile->PlayerUnitList){
		for (auto it2 : it->InternalBattalionList)
			CurrentPlayerUnitCount.insert(std::pair<std::string, unsigned short>(it2.first, it2.second));
	}
	// Set up the Enemy
	for (auto it : BattlefieldTile->EnemyUnitList){
		for (auto it2 : it->InternalBattalionList)
			CurrentEnemyUnitCount.insert(std::pair<std::string, unsigned short>(it2.first, it2.second));
	}
	// Spawn the characters
	SpawnPlayerCharacters(CurrentPlayerUnitCount);
	SpawnEnemyCharacters(CurrentEnemyUnitCount);
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

void BattleSystem::ClearCharacterCounters()
{
	CurrentPlayerUnitCount.clear();
	CurrentEnemyUnitCount.clear();
}

void BattleSystem::UpdateCharacterLogic(std::vector<CharacterEntity*>& CharacterList, const float& dt)
{
	for (auto it : CharacterList)
	{
		// The character will update it's StateManager as well as it's own Kinematic State if possible
		// I will need to do other updates here such as friction/limiting walkspeed
		if (!it->GetVelocity().IsZero())
		{
			Vector3 Direction = it->GetVelocity();
			if (it->GetVelocity().LengthSquared() > it->WalkSpeed * it->WalkSpeed)
			{
				it->SetVelocity(Direction.Normalize() * it->WalkSpeed);
			}
		}
		it->Update(dt);
	}
}

bool CheckCollision(BaseObject *o1, BaseObject *o2, std::string type)
{
	if (type == "Circle")
	{
		float CombinedRadiusSquared = (o1->GetDimensions().x + o2->GetDimensions().x) * (o1->GetDimensions().x + o2->GetDimensions().x);
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
								it->Active = false;
							}
						}
					}
				}
				if (it->Active)
				{
					std::vector<CharacterEntity*> Container;
					if (it->PlayerTeam)
						Container = InternalPlayerCharacterList;
					else Container = InternalEnemyCharacterList;

					for (auto it2 : Container)
					{
						BaseObject *obj2 = dynamic_cast<BaseObject*>(it2);
						if (obj2->Active)
						{
							if (CheckCollision(it, obj2, "Circle"))
							{
								it->Active = false;
								CharacterEntity* CE = it2;
								//Gets Hit
								CE->HealthPoints -= (it)->GetDamageDealt();

								//int NumParticles = Math::RandIntMinMax(2, 4);
								//for (int i = 0; i < NumParticles; ++i)
								//{
								//	float ParticleSpeed = Math::RandFloatMinMax(3.f, 6.f);
								//	float ParticleLifeTime = Math::RandFloatMinMax(0.75f, 1.5f);
								//
								//	//ObjectManager::Instance().AddNewParticle(new Particle(obj2->GetEntityID(), 1, CE->GetPosition() * Math::RandFloatMinMax(0.3f, 0.75f), Vector3(Math::RandFloatMinMax(-ParticleSpeed, ParticleSpeed), Math::RandFloatMinMax(-ParticleSpeed, ParticleSpeed), Math::RandFloatMinMax(0, ParticleSpeed)), camera.position, ParticleLifeTime));
								//}
							}
						}
					}
				}
			}
		}
	}
}

void BattleSystem::SpawnPlayerCharacters(std::map<std::string, unsigned short> PlayerCharacterList)
{
	for (auto PCL : PlayerCharacterList)
	{
		UnitType* UT = UnitData.UnitMap.find(PCL.first)->second;
		if (UT != nullptr)
		{
			for (unsigned int i = 0; i < PCL.second; ++i)
			{
				CharacterEntity* NewChar = nullptr;
				if (UT->GetType() == UnitType::T_MELEE)
				{
					NewChar = new MeleeCharacter();
					MeleeCharacter* MC = dynamic_cast<MeleeCharacter*>(NewChar);
					MC->SetCharacter(UT, UnitData.RaceMap.find("Human")->second);
					MC->isPlayer = true;
					NewChar->SetDimensions(Vector3(UnitSize, UnitSize, UnitSize));
					//int RNG = Math::RandIntMinMax(0, PlayerSpawnPoints.size() - 1);
					//MC->TargetDir.x = (SpawnPosition_Enemy.x - SpawnPosition_Player.x) / 20;
					NewChar->SetPosition(SpawnPosition_Player);
				}

				if (NewChar != nullptr)
				{
					InternalPlayerCharacterList.push_back(NewChar);
				}
			}
		}
	}
}

void BattleSystem::SpawnEnemyCharacters(std::map<std::string, unsigned short> EnemyCharacterList)
{
	for (auto ECL : EnemyCharacterList)
	{
		UnitType* UT = UnitData.UnitMap.find(ECL.first)->second;
		if (UT != nullptr)
		{
			for (unsigned int i = 0; i < ECL.second; ++i)
			{
				CharacterEntity* NewChar = nullptr;
				if (UT->GetType() == UnitType::T_MELEE)
				{
					NewChar = new MeleeCharacter();
					MeleeCharacter* MC = dynamic_cast<MeleeCharacter*>(NewChar);
					MC->SetCharacter(UT, UnitData.RaceMap.find("Elven")->second);
					MC->isPlayer = false;
					NewChar->SetDimensions(Vector3(UnitSize, UnitSize, UnitSize));
					//int RNG = Math::RandIntMinMax(0, EnemySpawnPoints.size() - 1);
					//MC->TargetDir.x = (SpawnPosition_Player.x - SpawnPosition_Enemy.x) / 20;
					NewChar->SetPosition(SpawnPosition_Enemy);
				}

				if (NewChar != nullptr)
				{
					InternalEnemyCharacterList.push_back(NewChar);
				}
			}
		}
	}
}