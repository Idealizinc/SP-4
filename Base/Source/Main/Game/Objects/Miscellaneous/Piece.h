#ifndef UNITPIECE_H
#define UNITPIECE_H

#include "../../SceneManagement/Objects/TerrainNode.h"
#include "../../../Engine/Objects/BaseObject.h"

class UnitPiece : public BaseObject
{
public:
	//UnitPiece();
	//~UnitPiece();

	//bool opened = false;
	//bool closed = false;
	TerrainNode* prevPos;


	void setPrev(TerrainNode *p)
	{
		prevPos = p;
	}

	TerrainNode* getPrev()
	{
		return prevPos;
	}
	bool hasPrev()
	{
		return prevPos != NULL;
	}


	int g = 0;
	int h = 0;
	int f = 0;

	int getGScore(TerrainNode* CurrentPosition)
	{
		return n->g + ((GetEntity()->GetPosition().x == n->GetEntity()->GetPosition().x || GetEntity()->GetPosition().y == n->GetEntity()->GetPosition().y) ? 10 : 14);
	}

	int getHScore(TerrainNode* input, TerrainNode* curr)
	{
		return (abs(input->GetEntity()->GetPosition().x - curr->GetEntity()->GetPosition().x) + abs(input->GetEntity()->GetPosition().y - curr->GetEntity()->GetPosition().y)) * 10;
	}

	int getGScore()
	{
		return g;
	}

	int getHScore()
	{
		return h;
	}

	int getFScore()
	{
		return f;
	}

	void computeScores(TerrainNode* end, TerrainNode* curr)
	{
		g = getGScore(prevPos, prevPiece, curr);
		h = getHScore(end, curr);
		f = g + h;
	}
};

#endif