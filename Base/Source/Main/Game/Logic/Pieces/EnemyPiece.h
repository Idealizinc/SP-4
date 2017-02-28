#ifndef _ENEMY_PIECE_H
#define _ENEMY_PIECE_H

/*
Copyright (c) Lim Rui An, Ryan 2017

Creator(s)    : Lim Rui An, Ryan
Creation Date : 18 February 2017 21:16 PM
File Name     : EnemyPiece.h
Purpose       : Defines a EnemyPiece, a piece that represents an Enemy Battalion in the Map Scene
*/

#include "../../SceneManagement/Objects/TerrainNode.h"
#include "UnitPiece.h"
#include <vector>

class EnemyPiece : public UnitPiece
{
public:
	EnemyPiece()
	{
		SetParameters("EnemyPiece", 1, 0, Vector3(3, 3, 3), 0, 0, Vector3(0, 1, 0));

		// for testing
		InternalBattalionList.insert(std::make_pair("Beastman", 5));
		InternalBattalionList.insert(std::make_pair("Crusader", 10));
	}
//private: < Temp
	std::vector<TerrainNode*> InternalDefinedPath;

protected:


};

#endif // _ENEMY_PIECE_H