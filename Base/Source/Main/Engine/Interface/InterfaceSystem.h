/*
Copyright (c) Lim Rui An, Ryan 2017

Creator(s)    : Lim Rui An, Ryan
Creation Date : 21 February 2017 14:34 PM
File Name     : InterfaceSystem.h
Purpose       : Defines an InterfaceSystem, a storage element that holds child InterfaceElements.
*/

#ifndef _INTERFACE_SYSTEM_H
#define _INTERFACE_SYSTEM_H

#include "../Internal/System.h"
#include "InterfaceLayer.h"

class InterfaceSystem : public System
{
public:
	InterfaceSystem(){};
	virtual ~InterfaceSystem(void){};

	virtual void Init(void);
	virtual void Update(const float& dt);
	virtual void Render(void);
	virtual void Exit(void);

	void ResetAllToOriginal();

	InterfaceLayer* CreateNewInterfaceLayer(const std::string& LayerID, const Vector3& OriginalPosition, const Vector3& TargetPosition);

private:

protected:
	std::vector<InterfaceLayer*> InternalLayerContainer;

};


#endif //_INTERFACE_SYSTEM_H