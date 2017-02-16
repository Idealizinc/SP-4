#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "../Miscellaneous/DetectMemoryLeak.h"
#include <string>
#include "Vector3.h"

// Notation of c[ClassInitials]_Variable, cA_
class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Update();
	void Exit();

	static bool IsKeyPressed(unsigned short key);
	static bool IsMousePressed(unsigned short key);
	static int GetWindowWidth();
	static int GetWindowHeight();

	static bool ExitGame;
	static bool LoadKeybinds();
	HWND WindowActiveHandle;

private:
	Application();
	~Application();

	// Stored Window Values
	static int cA_WindowHeight;
	static int cA_WindowWidth;

	//Declare a window object
	StopWatch m_timer;
	double m_dElaspedTime;
	double m_dAccumulatedTime_ThreadOne;
	double m_dAccumulatedTime_ThreadTwo;

};

#endif