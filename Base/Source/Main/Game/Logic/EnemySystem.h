#ifndef _ENEMY_SYSTEM_H
#define _ENEMY_SYSTEM_H

/*
Copyright (c) Lim Rui An, Ryan 2017

Creator(s)    : Lim Rui An, Ryan
Creation Date : 19 February 2017 12:42 PM
File Name     : EnemySystem.h
Purpose       : Defines the EnemySystem that handles decisions made by the computer opponent
*/

#include "../../Engine/Internal/System.h"
#include "../SceneManagement/Objects/TerrainNode.h"
#include "../Logic/Pieces/EnemyPiece.h"

#include <vector>

class EnemySystem : public System
{
public:
	virtual ~EnemySystem(void);

	virtual void Init(void);

	virtual void Exit(void);

	virtual void Update(const float& dt);

	virtual void Render(void);

	std::vector<UnitPiece*> InternalEnemyContainer;
	int RemainingGold;

private:
	enum TurnState
	{
		S_TURNSTART = 0,
		S_SPAWN,
		S_MOVE,
		S_TURNEND,
	};
	// Internals
	TurnState CurrentTurnState;
	std::vector<std::vector<TerrainNode*>> InternalPathContainer;
	int NumberOfPaths = 100;
	float InternalTimer = 0;
	EnemyPiece* SelectedUnit;
	TerrainNode* TargetedNode;
	const float SpawningCoefficient = 2.f; // Higher == Increased Spawn Rate

	// Functions
	EnemyPiece* GenerateNewEnemy(std::map<std::string, unsigned short>&);
	EnemyPiece* AdvanceSingleUnit();
	EnemyPiece* RandomizePieceSelection();
	std::map<std::string, unsigned short> GenerateBattalion();

	bool AnimationEnded, AnimateUpwards;
	float WaitTimer = 0;

protected:

};

#endif // _ENEMY_SYSTEM_H