#ifndef _UNIT_PIECE_H
#define _UNIT_PIECE_H

/*
Copyright (c) Lim Rui An, Ryan 2017

Creator(s)    : Lim Rui An, Ryan
Creation Date : 18 February 2017 21:16 PM
File Name     : UnitPiece.h
Purpose       : Defines a UnitPiece, a piece that represents an Enemy/Player's Battalion in the Map Scene
*/

#include "../../../Engine/Objects/BaseObject.h"
#include <map>

/*
Description
A UnitPiece is one that exists above a TerrainNode within the Map Scene.
A player or an enemy can decide to move a UnitPiece from one TerrainNode to another that is directly linked.
The UnitPiece is to then interpolate towards the selected node.

A UnitPiece contains vital information about BattleScene specific stats, such as:
The Type of Units within this UnitPiece's battalion
The Number of Each Type of Unit

A UnitPiece's stats can only be changed upon creation <- May change in future
*/

class UnitPiece : public BaseObject
{
public:
	UnitPiece()
	{
		SetParameters("gbullet", 1, 0, Vector3(3, 3, 3), 0, 0, Vector3(0, 1, 0));
	}
	// Add functions in relation to the IBL

//private: < Temp
	std::map<std::string, unsigned short> InternalBattalionList;
	Vector3 TargetPosition;
protected:

};

#endif // _UNIT_PIECE_H