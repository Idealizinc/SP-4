#include "UnitPiece.h"

UnitPiece::UnitPiece()
{
	moved = false;
}

UnitPiece::~UnitPiece()
{

}

bool UnitPiece::GetMoved()
{
	return moved;
}

TerrainNode* UnitPiece::GetNextNode()
{
	return nextNode;
}

void UnitPiece::SetMoved(bool moved)
{
	this->moved = moved;
}

void UnitPiece::SetNextNode(TerrainNode* nextNode)
{
	this->nextNode = nextNode;
}