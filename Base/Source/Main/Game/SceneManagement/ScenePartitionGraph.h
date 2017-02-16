/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 10 December 2016 14:34 PM
File Name     : ScenePartitionGraph.h
Purpose       : Defines the ScenePartitionGraph. A singular object that divides and holds objects in the scenes in GridNodes; Dependant on their positions
*/

#ifndef _SCENE_GRAPH_PARTITION_H
#define _SCENE_GRAPH_PARTITION_H

#include "../../Engine/Internal/SingletonTemplate.h"
#include "../../Engine/Internal/System.h"
#include "Objects/TerrainNode.h"
#include "Objects\GridNode.h"
#include <vector>
#include <map>

class ScenePartitionGraph : public System, public SingletonTemplate<ScenePartitionGraph>
{
public:
	virtual void Init();
	virtual void Update(const float&);
	virtual void Render();
	virtual void Exit();

	void GenerateGrid();
	Node* CreateNewNode(BaseObject*);
	bool AssignObject(Node*);
	std::vector<GridNode*>& GetContainer();

	GridNode* FindGridForPosition(const Vector3& Position);
	Vector3 FindGridIndexForPosition(const Vector3& Position);

	void AssignGridParameters(const Vector3& GridCenter, const Vector3& GridDimensions, const unsigned& GridDivisions);

	Vector3 GridCenter;
	Vector3 GridDimensions;
	unsigned GridDivisions;

	std::vector<Node*> ReallocationList;
	std::map<std::string, std::vector<Mesh*>> LODList;

	bool ShowPartitions = false;
	TerrainNode* PlayerBase;
	TerrainNode* EnemyBase;
private:
	std::vector<GridNode*> PartitionGraphContainer;

};

#endif //_SCENE_GRAPH_PARTITION_H