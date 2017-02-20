#ifndef _PLAYER_SYSTEM_H
#define _PLAYER_SYSTEM_H

/*
Copyright (c) Lim Rui An, Ryan 2017

Creator(s)    : Lim Rui An, Ryan
Creation Date : 19 February 2017 16:21 PM
File Name     : PlayerSystem.h
Purpose       : Defines the PlayerSystem that handles the player's input
*/

#include "../../Engine/Internal/System.h"
#include "../Logic/Pieces/UnitPiece.h"
#include "../SceneManagement/Objects/TerrainNode.h"
#include <vector>


class PlayerSystem : public System
{
public:
	virtual ~PlayerSystem(void);

	virtual void Init(void);

	virtual void Exit(void);

	virtual void Update(const float& dt);

	virtual void Render(void);

private:
	enum TurnState
	{
		S_TURNSTART = 0,
		S_ACTION,
		S_TURNEND,
	};
	// Internals
	TurnState CurrentTurnState;
	UnitPiece* SelectedUnit;
	std::vector<UnitPiece*> InternalUnitContainer;
	TerrainNode* MouseDownSelection;
	TerrainNode* MouseUpSelection;

	// Functions
	UnitPiece* GenerateNewUnit();
	UnitPiece* AdvanceSingleUnit(UnitPiece* Selection, TerrainNode* Target);
	void HandleUserInput();

protected:

};

#endif // _PLAYER_SYSTEM_H