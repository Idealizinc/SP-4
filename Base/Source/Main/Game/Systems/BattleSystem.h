#ifndef BATTLE_SYSTEM_H
#define BATTLE_SYSTEM_H

#include "../../Engine/Internal/System.h"
#include "../../Engine/Objects/CharacterEntity.h"
#include "../Logic/UnitData/UnitDataLoader.h"
#include "../Logic/Terrain/Terrain.h"
#include "../Objects/Projectile.h"
#include "../UserInterface/BattleScreenInterface.h"
#include "BillboardManager.h"
#include <vector>
#include <map>


/*
Description
A battle system is one that exists within the game logic system and is only called to update and render when a battle occurs.
The primary duty of such a system;
- To act as a link between the game screen's unit pieces and the battle screens battle characters
- To spawn units of both the player and the enemy
- To handle the objects of battle; projectiles, fsms and characters.
- To be generic such that no spawn point or unit is hardcoded.
*/

class BattleSystem : public System
{
public:
	BattleSystem();
	virtual ~BattleSystem();

	virtual void Init();
	virtual void Update(const float& dt);
	virtual void Render();
	virtual void Exit();

	std::vector<CharacterEntity*>& GetPlayerCharacterList();
	std::vector<CharacterEntity*>& GetEnemyCharacterList();
	std::vector<Projectile*>& GetProjectileList();
	void SetUpUnits(Terrain* BattlefieldTile);
	void AddNewProjectile(Projectile* P);
	Terrain* CurrentBattleTile;

	BattleScreenInterface * BSI;

	// Data Store
	UnitDataLoader UnitData;
	const float UnitSize = 10.f;
	// Miscellaneous
	Vector3 SpawnPosition_Player;
	Vector3 SpawnPosition_Enemy;
	BillboardManager ParticleSystem;

private:
	// Internals

	// Containers
	std::vector<Projectile*> InternalProjectileList;
	std::vector<CharacterEntity*> InternalPlayerCharacterList;
	std::vector<CharacterEntity*> InternalEnemyCharacterList;

	// Storage for what units are currently within th battalions
	std::map<std::string, unsigned short> CurrentPlayerUnitCount;
	std::map<std::string, unsigned short> CurrentEnemyUnitCount;

	// Functions
	void ClearCharacterCounters();
	void UpdateCharacterLogic(std::vector<CharacterEntity*>& CharacterList , const float& dt);
	void UpdateProjectileLogic(const float& dt);
	void SpawnPlayerCharacters(std::map<std::string, unsigned short> PlayerCharacterList, Terrain* Tile);
	void SpawnEnemyCharacters(std::map<std::string, unsigned short> EnemyCharacterList, Terrain* Tile);
};


#endif