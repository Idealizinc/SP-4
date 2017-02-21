#include "RouteGeneration.h"

std::vector<TerrainNode*> RouteGeneration::GeneratePath()
{
	// Define Variables required in pathing
	DefinedPath.clear();
	PossibleMoves.clear();

	TerrainNode* MostViableNode = nullptr;
	TerrainNode* PreviousNode = nullptr;

	TerrainNode* StartNode = ScenePartitionGraph::Instance().EnemyBase;
	TerrainNode* TargetNode = ScenePartitionGraph::Instance().PlayerBase;
	TerrainNode* CurrentNode = StartNode;

	DefinedPath.push_back(StartNode);

	while (DefinedPath.size() < (unsigned)MaximumPathCount)
	{
		float ClosestDistance = FLT_MAX;

		// Assign the nodes connected to the current for selection
		PossibleMoves = CurrentNode->LinkedTerrainNodes;

		// If I have no other choice as the node i'm at only links to one other
		if (PossibleMoves.size() == 1)
		{
			// Possible cases: 2 nodes directly linked with no other [never reaching the target], start/end node directly linked with another.
			// May cause infinite loop here
			PreviousNode = CurrentNode;
			MostViableNode = PossibleMoves.front();
			ClosestDistance = MostViableNode->CalculateRemainderPathCost(TargetNode);
		}
		// A distance comparison between each node, in an attempt to find the closest.
		for (unsigned int i = 0; i < PossibleMoves.size(); ++i)
		{
			if (PossibleMoves[i] != PreviousNode)
			{
				float DistanceBetween = PossibleMoves[i]->CalculateRemainderPathCost(TargetNode);
				if (DistanceBetween < ClosestDistance)
				{
					MostViableNode = PossibleMoves[i];
					ClosestDistance = DistanceBetween;
				}
				else if ((int)DistanceBetween == (int)ClosestDistance)
					float DistanceBetween = PossibleMoves[i]->CalculateRemainderPathCost(TargetNode);


			}
		}

		// A randomizer to add slight deviation to the path
		if (PossibleMoves.size() >=  3)
		{
			// Cause of circle running
			if (Math::RandIntMinMax(0, 10) > 6)
			{
				TerrainNode* Choice = RandomizeSelection(PossibleMoves);
				// Possibility of infinite loop if there are only two linked nodes and the end in not connected
				while (Choice == PreviousNode || Choice == MostViableNode)
				{
					Choice = RandomizeSelection(PossibleMoves);
				}
		
				MostViableNode = Choice;
			}
		}

		// Assign the previous node to be the last entered node of the defined path.
		PreviousNode = DefinedPath.back();
		
		// Insert the newly selected node and assign it to be the current
		DefinedPath.push_back(CurrentNode = MostViableNode);
		
		// Check if I've reached the end
		if (CurrentNode == TargetNode)
			return DefinedPath;
	}
	// I have failed to generate a worthy path, restarting.
	GeneratePath();
	return DefinedPath;
}
	


std::vector<TerrainNode*> RouteGeneration::GetDefinedPath()
{
	return DefinedPath;
}

TerrainNode* RouteGeneration::RandomizeSelection(std::vector<TerrainNode*> SelectionList)
{
	//Math::InitRNG();
	int RandomChoice = Math::RandIntMinMax(0, SelectionList.size() - 1);
	return SelectionList[RandomChoice];
}
