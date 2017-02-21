/****************************************************************************/
/*!
\file InputManager.h
\author Ryan Lim Rui An
\par email: 150577L@mymail.nyp.edu.sg
\brief
Defines the Input Manager that handles all forms of input
*/
/****************************************************************************/


#ifndef _INPUTMANAGER_H
#define _INPUTMANAGER_H

#include "Vector3.h"
#include <map>

class InputManager
{
public:
	enum MouseState
	{
		MOUSE_DOWN = 0,
		MOUSE_RELEASE,
		MOUSE_UP,
		MOUSE_HOLD,
		MOUSE_MAX
	};

	enum MouseKeys
	{
		KEY_LMB = 0,
		KEY_RMB = 1,
		KEY_MAX
	};

	InputManager();
	~InputManager();
	void HandleUserInput();
	bool GetKeyValue(char);
	unsigned short GetMouseInput(const unsigned short& key);
	Vector3 GetMousePosition();
	void SetMousePosition(Vector3);
	void SetMouseToScreenCenter();
	void UpdateMouse();

	void SetScreenSize(float, float);

	float cIM_ScreenWidth, cIM_ScreenHeight;
	Vector3 ScreenCenter;
	float cIM_CameraYaw = 0, cIM_CameraPitch = 0;
	bool cIM_inMouseMode = false;

private:
	Vector3 MousePosition;
	bool cIM_Keys[256];
	std::map<unsigned short, unsigned short> MouseButtonState;
};

#endif