#include "InputManager.h"
#include "../../Game/Mains/Application.h"
#include "SceneSystem.h"
#include "SimpleCommand.h"

InputManager::InputManager()
{
	for (int num = 0; num < 256; ++num)
	{
		cIM_Keys[num] = false;
	}
}


InputManager::~InputManager()
{
	MouseButtonState.clear();
}

void InputManager::HandleUserInput()
{
    // hardcoded inputs. A need for change. So i really need everyone's help
    for (unsigned num = 0; num < SimpleCommand::TOTAL_COMMANDS; ++num)
    {
        if (Application::IsKeyPressed(SimpleCommand::m_allTheKeys[num]) && cIM_Keys[SimpleCommand::m_allTheKeys[num]] == false)
        {
            cIM_Keys[SimpleCommand::m_allTheKeys[num]] = true;
        }
        else if (!Application::IsKeyPressed(SimpleCommand::m_allTheKeys[num]) && cIM_Keys[SimpleCommand::m_allTheKeys[num]] == true)
        {
            cIM_Keys[SimpleCommand::m_allTheKeys[num]] = false;
        }
    }
	for (std::map<unsigned short, unsigned short>::iterator it = MouseButtonState.begin(); it != MouseButtonState.end(); ++it)
	{
		// I wasn't pressing it down earlier, but I am now.
		if ((it->second == MOUSE_RELEASE) && Application::IsMousePressed(it->first)){
			it->second = MOUSE_DOWN;
		}
		// I've already pressed it but am still holding down.
		else if ((it->second == MOUSE_DOWN || it->second == MOUSE_HOLD) && Application::IsMousePressed(it->first)){
			it->second = MOUSE_HOLD;
		}
		// I was holding it earlier but I've just let it go
		else if ((it->second == MOUSE_HOLD) && !Application::IsMousePressed(it->first)){
			it->second = MOUSE_UP;
		}
		else{
			it->second = MOUSE_RELEASE;
		}
	}
}

bool InputManager::GetKeyValue(char c)
{
	return cIM_Keys[c];
}

unsigned short InputManager::GetMouseInput(const unsigned short& key)
{
	std::map<unsigned short, unsigned short>::iterator it = MouseButtonState.find(key);
	if (it != MouseButtonState.end())
	{
		return it->second;
	}
	else MouseButtonState.insert(std::pair<unsigned short, bool>(key, MOUSE_RELEASE));
	return MOUSE_RELEASE;
}

Vector3 InputManager::GetMousePosition()
{
	return MousePosition;
}

void InputManager::SetMousePosition(Vector3 v3)
{
	MousePosition = v3;
}

void InputManager::SetScreenSize(float x, float y)
{
	cIM_ScreenWidth = x;
	cIM_ScreenHeight = y;
	ScreenCenter.x = (int)cIM_ScreenWidth / 2;
	ScreenCenter.y = (int)cIM_ScreenHeight / 2;
}

void InputManager::UpdateMouse()
{
	////New Version for use with my camera version
	POINT mousePosition;
	GetCursorPos(&mousePosition);
	
	if (!cIM_inMouseMode)
	{
		//Lock the cursor's position to the center of the screen.
		SetCursorPos(ScreenCenter.x, ScreenCenter.y);
	
		//Calculate the difference between the cursor coordinates between frames
		cIM_CameraYaw = static_cast<float>(mousePosition.x - ScreenCenter.x);
		cIM_CameraPitch = static_cast<float>(mousePosition.y - ScreenCenter.y);
	}
	SetMousePosition(Vector3((float)mousePosition.x, cIM_ScreenHeight - (float)mousePosition.y, 0.f));
}

void InputManager::SetMouseToScreenCenter()
{
	SetCursorPos((int)cIM_ScreenWidth / 2, (int)cIM_ScreenHeight / 2);
}