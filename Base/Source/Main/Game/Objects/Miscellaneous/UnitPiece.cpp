#include "UnitPiece.h"

UnitPiece::UnitPiece()
{
	Init();
}

UnitPiece::~UnitPiece()
{

}

void UnitPiece::Init()
{
	moved = false;
}

void UnitPiece::Update(const float& dt)
{

}

void UnitPiece::Render()
{

}

void UnitPiece::Exit()
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