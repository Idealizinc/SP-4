/*
Copyright (c) Clinton Teo Jun Quan 2016

Creator(s)    : Clinton Teo Jun Quan, Lim Rui An, Ryan (Optimization)
Creation Date : 5 February 2016 15:13 PM
File Name     : RouteGeneration.h
Purpose       : Generate a route for enemies to go through, first by finding the next node which is (mostly) closest to the player's base, and when the enemy is at a junction, random a chance for the enemy to go another way other than the closest way to the player base.
*/
#ifndef ROUTEGENERATION_H
#define ROUTEGENERATION_H

#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include "Vector3.h"
#include "MyMath.h"
#include "../SceneManagement/Objects/TerrainNode.h"
#include "../SceneManagement/ScenePartitionGraph.h"


class RouteGeneration
{
public:
	std::vector<TerrainNode*> GeneratePath();
	std::vector<TerrainNode*> GetDefinedPath();

	std::vector<TerrainNode*> DefinedPath;

private:
	// Internals
	std::vector<TerrainNode*> PossibleMoves;
	int MaximumPathCount = 100;
	// Functions
	TerrainNode* RandomizeSelection(std::vector<TerrainNode*> SelectionList);
};

#endif
