#include "Application.h"

int main( void )
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(29839);
	Application &app = Application::GetInstance();
	app.Init();
	app.Run();
	app.Exit();
}