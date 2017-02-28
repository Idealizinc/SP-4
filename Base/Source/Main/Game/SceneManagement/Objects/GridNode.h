/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 11 December 2016 14:59 PM
File Name     : GridNode.h
Purpose       : Defines a Grid Node that acts as a spatial partition that holds other nodes within it
*/


#ifndef _GRID_NODE_H
#define _GRID_NODE_H

#include "Node.h"
#include "../../../Engine/Objects/GameObject.h"

class GridNode : public Node
{
public:
	GridNode();
	~GridNode();

	void SetDimensions(const Vector3&);
	void SetPosition(const Vector3&);

	Vector3 GetDimensions() const;
	Vector3 GetPosition() const;

	bool CheckWithin(const Vector3&);
	bool CheckWithin(BaseObject*);
	bool CheckWithin(Node*);

	virtual void Render();
	virtual void Update(const float& dt);

	unsigned LODLevel;

private:
	Vector3 Position;
	Vector3 Dimensions;

protected:

};

#endif //_GRID_NODE_H