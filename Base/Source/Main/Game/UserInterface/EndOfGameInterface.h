#ifndef ENDOFGAMEINTERFACE_H
#define ENDOFGAMEINTERFACE_H

#include "../../Engine/Interface/InterfaceSystem.h"
#include "../../Engine/System/SceneSystem.h"

class EndOfGameInterface : public InterfaceSystem
{
public:
	EndOfGameInterface();
	virtual ~EndOfGameInterface();

	virtual void Init(void);
	virtual void Update(const float& dt);
	virtual void Render(void);

	void ResetAll();

private:
	void HandleUserInput();
	void PopUpDelay(const float& dt);

	InterfaceLayer* EndScreenLayer;
	InterfaceElement* EndScreenWinOrLose;
	InterfaceElement* EndScreenReturnButton;
	InterfaceElement* EndScreenBackground;
	InterfaceElement* EndScreenFrame;

	Vector3 CenterTarget;

};

#endif