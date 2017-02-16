/****************************************************************************/
/*!
\file UI_Layer.cpp
\author Ryan Lim Rui An
\par email: 150577L@mymail.nyp.edu.sg
\brief
Defines the a UI_Layer that holds UI_Elements
*/
/****************************************************************************/

#ifndef _UI_LAYER_H
#define _UI_LAYER_H

#include "../../../Engine/Internal/System.h"
#include "UI_Element.h"
#include <vector>

class UI_Layer : public System
{
public:
	UI_Layer();
	virtual ~UI_Layer();

	virtual void Init();
	virtual void Update(const float& dt);
	virtual void Render();
	virtual void Exit();

	Vector3 LayerOriginalPosition;

	Vector3 LayerCenterPosition;

	Vector3 LayerTargetPosition;

	void SwapOriginalWithTarget();

	void UI_Layer::AddUIElement(const std::string& name, const Vector3& Position, const Vector3& SpawnPosition, const Vector3& Dimension, const Vector3& TargetPosition, const std::string& UI_Text = "");

	std::vector<UI_Element*> cUI_Layer;
private:

};

#endif // _UI_LAYER_H