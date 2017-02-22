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

	virtual ~TerrainNode()
	{
		//Exit();
		if (Object != nullptr)
			delete Object;
		Object = nullptr;
		if (TerrainTile != nullptr)
			delete TerrainTile;
		TerrainTile = nullptr;
	}

	virtual void Exit()
	{
		for (std::vector<TerrainNode*>::iterator it = LinkedTerrainNodes.begin(); it != LinkedTerrainNodes.end(); ++it)
		{
			delete *it;
			it = LinkedTerrainNodes.erase(it); 
		}
	}

	void AssignTerrainNode(TerrainNode* TN)
	{
		LinkedTerrainNodes.push_back(TN);
	}

	std::vector<TerrainNode*> LinkedTerrainNodes;

	float CalculateRemainderPathCost(TerrainNode* End)
	{
		Vector3 AxialNodeDistance = End->GetEntity()->GetPosition() - GetEntity()->GetPosition();
		float CumulativeNodeDistanceForXZ = abs(AxialNodeDistance.x) + abs(AxialNodeDistance.z);
		return CumulativeNodeDistanceForXZ;

		//return (abs(End->GetEntity()->GetPosition().x - GetEntity()->GetPosition().x) + abs(End->GetEntity()->GetPosition().z - GetEntity()->GetPosition().z));
	}
};



#endif //_TERRAIN_NODE_H