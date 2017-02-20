#ifndef BATTLE_SYSTEM_H
#define BATTLE_SYSTEM_H

#include "../Miscellaneous/DetectMemoryLeak.h"

#include "../../Engine/Internal/System.h"
#include "../../Engine/Internal/SingletonTemplate.h"
#include "../../Engine/Objects/CharacterEntity.h"
#include "../Objects/Miscellaneous/Projectile.h"
#include "../Objects/Miscellaneous/Particle.h"
#include "../Objects/UnitData/UnitDataLoader.h"
#include <Vector3.h>
#include <map>
#include <vector>

#include "../Objects/Characters/MeleeCharacter.h"

class BattleSystem : public System, public SingletonTemplate<BattleSystem>
{
public:
	BattleSystem();
	~BattleSystem();

	virtual void Init();
	virtual void Update(const float& dt);
	virtual void Render();
	virtual void Exit();

	Vector3 SpawnPosition_Player;
	Vector3 SpawnPosition_Enemy;

	const float UnitSize = 10.f;

	std::map<std::string, unsigned short>CurrentPlayerUnitCount;
	std::map<std::string, unsigned short>CurrentEnemyUnitCount;
	void ClearCharacterCount();

	void AddNewProjectile(Projectile* P);

	std::vector<CharacterEntity*> GetPlayerCharacterList();
	std::vector<CharacterEntity*> GetEnemyCharacterList();
	std::vector<Projectile*>& GetProjectileList();

	std::vector<CharacterEntity*> InternalPlayerCharacterList;
	std::vector<CharacterEntity*> InternalEnemyCharacterList;

private:
	void SpawnPlayerCharacter(std::map<std::string, unsigned short> PlayerCharacterList);
	void SpawnEnemyCharacter(std::map<std::string, unsigned short> EnemyCharacterList);

	UnitDataLoader Loader;
	//std::map <std::string, int> CharacterCountLimit;
	//std::map <std::string, float> SpawnTime;
	std::vector<Projectile*> InternalProjectileList;
};


#endif