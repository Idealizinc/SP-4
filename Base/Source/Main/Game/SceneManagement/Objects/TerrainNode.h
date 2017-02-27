#ifndef _TERRAIN_NODE_H
#define _TERRAIN_NODE_H

/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 14 February 2017 11:55 AM
File Name     : TerrainNode.h
Purpose       : Defines a Node that represents a plot of terrain
*/

#include "Node.h"
#include "../../Logic/Terrain/Terrain.h"

class Terrain;

class TerrainNode : public Node
{
public:
	Terrain* TerrainTile = nullptr;

	virtual ~TerrainNode();

	virtual void Exit();

	void AssignTerrainNode(TerrainNode* TN);

	std::vector<TerrainNode*> LinkedTerrainNodes;

	float CalculateRemainderPathCost(TerrainNode* End);
};



#endif //_TERRAIN_NODE_H