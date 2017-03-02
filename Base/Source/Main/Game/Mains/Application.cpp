#include "Application.h"

#ifdef _DEBUG
#include <assert.h>
#endif

// Engine
#include "../../Engine/System/SceneSystem.h"
#include "../../Engine/System/RenderSystem.h"
#include "../../Engine/System/MusicSystem.h"
#include "../../Engine/State/StateList.h"
#include "../Systems/GameLogicSystem.h"

// Scenes
#include "../Scene/GameScene.h"
#include "../Scene/BattleScene.h"
#include "../Scene/MainMenuScene.h"
#include "../Scene/EndOfGameScene.h"


// Miscellaneous
#include "SimpleCommand.h"
#include <sstream>
#include <fstream>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

GLFWwindow* m_window;
const unsigned char FPS = 120; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

int Application::cA_WindowHeight = 0, Application::cA_WindowWidth = 0;

bool Application::ExitGame = false;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	SceneSystem::Instance().cSS_InputManager->SetScreenSize((float)w, (float)h);
}

bool Application::IsKeyPressed(unsigned short key)
{
	return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

bool Application::IsMousePressed(unsigned short key) //0 - Left, 1 - Right, 2 - Middle
{
	return glfwGetMouseButton(m_window, key) != 0;
}

int Application::GetWindowWidth()
{
	return cA_WindowWidth;
}
int Application::GetWindowHeight()
{
	return cA_WindowHeight;
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	// Initialize the default values of the Scene_System
	SceneSystem::Instance().Init();

	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4);								//Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					//Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);					//Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			//To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	//We don't want the old OpenGL 
	//Borderless
	glfwWindowHint(GLFW_DECORATED, GL_FALSE);

	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());	//Obtain Width and Height values from the monitor;
	cA_WindowWidth = 800;
	cA_WindowHeight = 600;
	cA_WindowWidth = mode->width;
	cA_WindowHeight = mode->height;
	SceneSystem::Instance().cSS_InputManager->SetScreenSize((float)cA_WindowWidth, (float)cA_WindowHeight);
	m_window = glfwCreateWindow(cA_WindowWidth, cA_WindowHeight, "SP4 Framework", NULL, NULL); // Create a window with attained values.

	// Active Window Detection
	WindowActiveHandle = GetActiveWindow();

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	m_dElaspedTime = 0.0;
	// Accumulator Threads
	m_dAccumulatedTime_ThreadOne = 0.0;
	m_dAccumulatedTime_ThreadTwo = 0.0;

	// Hide mouse
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	RenderSystem* Renderer = new RenderSystem();
	SceneSystem::Instance().GenerateTransitionLayer(15, 1.f, "LoadTexture2");

#ifdef _DEBUG
    assert(LoadKeybinds());
#else
    LoadKeybinds();
#endif

	// Initiallize used scenes
	//Scene_MainMenu *MM = new Scene_MainMenu();	
	
	SceneEntity* Scenes;
	Scenes = new MainMenuScene();
	Scenes = new GameScene();
	Scenes = new BattleScene();
	Scenes = new EndOfGameScene();
	SceneSystem::Instance().SwitchScene("MainMenuScene");
	SceneSystem::Instance().RenderTransitionEffects();
	glfwSwapBuffers(m_window);

    MusicSystem::Instance().Init();
}

void Application::Run()
{
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	
	//Main Loop
	while (!ExitGame && !glfwWindowShouldClose(m_window))
	{
		// Get the elapsed time
        m_dElaspedTime = m_timer.getElapsedTime();
        //if (hwnd == GetActiveWindow())
		{
			Update();
			if (!SceneSystem::Instance().AnimationDirectionInwards)
				SceneSystem::Instance().GetCurrentScene().Render();
			else if (SceneSystem::Instance().PreviousScene != nullptr)
				SceneSystem::Instance().GetPreviousScene().Render();
        }
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
		// Frame rate limiter. Limits each frame to a specified time in ms.   
		m_timer.waitUntil(frameTime);

	} //Check if the ESC key had been pressed or if the window has been closed
	StateList::Instance().Exit();
	GameLogicSystem::Instance().Exit();
	SceneSystem::Instance().ClearMemoryUsage();
	MusicSystem::Instance().ClearMemoryUsage();
	MusicSystem::Instance().Exit();
}

void Application::Update()
{
	// Update threads
	m_dAccumulatedTime_ThreadOne += m_dElaspedTime;

	if ((WindowActiveHandle == GetActiveWindow()) && m_dAccumulatedTime_ThreadOne > 1 / frameTime)
	{
		SceneSystem::Instance().Update((float)m_dElaspedTime);
		SceneSystem::Instance().cSS_InputManager->UpdateMouse();
		SceneSystem::Instance().cSS_InputManager->HandleUserInput();
		m_dAccumulatedTime_ThreadOne = 0.0;
	}
	m_dAccumulatedTime_ThreadTwo += m_dElaspedTime;
	if (m_dAccumulatedTime_ThreadTwo > 1 / frameTime * 5)
	{
		SceneSystem::Instance().GetCurrentScene().Update((float)m_dElaspedTime);
		MusicSystem::Instance().Update((float)m_dElaspedTime);
		m_dAccumulatedTime_ThreadTwo = 0.0;
	}
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
	_CrtDumpMemoryLeaks();
}

bool Application::LoadKeybinds()
{
    std::ifstream file("CSVFiles//GlobalDriven.csv");
#ifdef _DEBUG
    assert(file.is_open());
#endif
    if (file.is_open())
    {
        std::string data = "";
        while (getline(file, data))
        {
            //Security check at it's finest
            if (data == "" || data == "\n" || data == "\r")
                continue;
            std::istringstream ss(data);
            std::string dataFromToken = "";
            std::string KeyAndToken[2] = { "" };
            unsigned num = 0;
            while (getline(ss, KeyAndToken[num], ','))
            {
                if (num == 0)
                    CapitalizeString(KeyAndToken[num]);
                ++num;
            }
            if (KeyAndToken[0] == "FORWARD_BUTTON")
            {
                SimpleCommand::m_allTheKeys[SimpleCommand::FORWARD_COMMAND] = KeyAndToken[1][0];
            }
            else if (KeyAndToken[0] == "BACKWARD_BUTTON")
            {
                SimpleCommand::m_allTheKeys[SimpleCommand::BACK_COMMAND] = KeyAndToken[1][0];
            }
            else if (KeyAndToken[0] == "RIGHT_BUTTON")
            {
                SimpleCommand::m_allTheKeys[SimpleCommand::RIGHT_COMMAND] = KeyAndToken[1][0];
            }
            else if (KeyAndToken[0] == "LEFT_BUTTON")
            {
                SimpleCommand::m_allTheKeys[SimpleCommand::LEFT_COMMAND] = KeyAndToken[1][0];
            }
            else if (KeyAndToken[0] == "JUMP_BUTTON")
            {
                SimpleCommand::m_allTheKeys[SimpleCommand::JUMP_COMMAND] = KeyAndToken[1][0];
            }
            else if (KeyAndToken[0] == "INTERACT_BUTTON")
            {
                SimpleCommand::m_allTheKeys[SimpleCommand::INTERACT_COMMAND] = KeyAndToken[1][0];
            }
            else if (KeyAndToken[0] == "PAUSE_MENU_BUTTON")
            {
                SimpleCommand::m_allTheKeys[SimpleCommand::PAUSE_MENU_COMMAND] = KeyAndToken[1][0];
            }
        }
        file.close();
        return true;
    }
    return false;
}
