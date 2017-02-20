/*
Copyright (c) Clinton Teo Jun Quan 2016

Creator(s)    : Clinton Teo Jun Quan
Creation Date : 5 February 2016 15:13 PM
File Name     : UnitPiece.h
Purpose       : Generic Class for Units on the overview scene
*/

#ifndef UNITPIECE_H
#define UNITPIECE_H
#include "Vector3.h"
#include <vector>
#include "../../SceneManagement/Objects/TerrainNode.h"

class UnitPiece : public BaseObject
{
public:
	UnitPiece();
	~UnitPiece();

	//causes unresolved externals cuz no function definition
	virtual void Init();
	virtual void Update(const float& dt);
	virtual void Render();
	virtual void Exit();

	TerrainNode* GetNextNode();
	void SetNextNode(TerrainNode* nextNode);
	void SetMoved(bool moved);
	bool GetMoved();

private:

	bool moved;//check if unit is moved
	TerrainNode* nextNode;
};

#endif