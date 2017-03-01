#include "BattleScene.h"
#include <sstream>

#include "SimpleCommand.h"
#include "../../Engine/System/MusicSystem.h"
#include "../Mains/Application.h"
#include "../SceneManagement/ScenePartitionGraph.h"
#include "../Systems/GameLogicSystem.h"

std::string BattleScene::id_ = "BattleScene";

BattleScene::BattleScene()
: SceneEntity()
{
	framerates = 0;
	SetEntityID(id_);
	InteractiveMap = nullptr;
	ScenePartition = nullptr;
	SceneSystem::Instance().AddScene(*this);
	Init();
}

BattleScene::~BattleScene()
{

}

void BattleScene::QuickInit()
{
	// Set Terrain Size
	TerrainScale.Set(400.f, 50.f, 400.f);

	CameraAerial* CA = new CameraAerial();
	camera = CA;
	CA->AltInit(/*Player Character Position*/Vector3(0, 0, 0), Vector3(0, 100, 50), Vector3(0, 1, 0));
	CenterPosition.Set(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);

	SceneSystem::Instance().cSS_InputManager->cIM_inMouseMode = true;
}

void BattleScene::QuickExit()
{
	if (ScenePartition)
	{
		ScenePartition->Exit();
		delete ScenePartition;
	}
	if (InteractiveMap)
		delete InteractiveMap;
	EntityList.clear();
	if (camera)
		delete camera;
}
void BattleScene::Init()
{
	QuickInit();
}

void BattleScene::Update(const float& dt)
{
	float Delta = dt;
	if (SceneSystem::Instance().AnimationActivated && SceneSystem::Instance().AnimationDirectionInwards)
		Delta = 0;

	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());

	Renderer->Update(Delta);

	float Speed = 50.f;
	CameraAerial* CA = (CameraAerial*)camera;
	float DetectionOffset = 0.2f;
	if (!GameLogicSystem::Instance().UnitInterface->UIDisplayed)
	{
		if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::FORWARD_COMMAND]) ||
			SceneSystem::Instance().cSS_InputManager->GetMousePosition().y > SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * (1.f - DetectionOffset))
		{
			CA->CameraMoveTargetPosition.z -= Speed*Delta;
		}
		if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::BACK_COMMAND]) ||
			SceneSystem::Instance().cSS_InputManager->GetMousePosition().y < SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * DetectionOffset)
		{
			CA->CameraMoveTargetPosition.z += Speed*Delta;
		}
		if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::RIGHT_COMMAND]) ||
			SceneSystem::Instance().cSS_InputManager->GetMousePosition().x > SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * (1.f - DetectionOffset))
		{
			CA->CameraMoveTargetPosition.x += Speed*Delta;
		}
		if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::LEFT_COMMAND]) ||
			SceneSystem::Instance().cSS_InputManager->GetMousePosition().x < SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * DetectionOffset)
		{
			CA->CameraMoveTargetPosition.x -= Speed*Delta;
		}
		float XOffset = TerrainScale.x * 0.5f;
		if (CA->CameraMoveTargetPosition.x < -XOffset)
			CA->CameraMoveTargetPosition.x = -XOffset;
		else if (CA->CameraMoveTargetPosition.x > XOffset)
			CA->CameraMoveTargetPosition.x = XOffset;

		float ZOffset = TerrainScale.z * 0.5f;
		if (CA->CameraMoveTargetPosition.z < -ZOffset)
			CA->CameraMoveTargetPosition.z = -ZOffset;
		else if (CA->CameraMoveTargetPosition.z > ZOffset)
			CA->CameraMoveTargetPosition.z = ZOffset;
	}

	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		ScenePartition->ShowPartitions = false;
	}

	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		ScenePartition->ShowPartitions = true;
	}

	CA->Update(Delta);
	//MusicSystem::Instance().playBackgroundMusic("battle");
	BManager.UpdateContainer(Delta, CA->position);

	ScenePartition->Update(Delta);

	GameLogicSystem::Instance().Update(Delta);

	framerates = 1 / dt;
}

void BattleScene::ReloadMap(Terrain* Terrain)
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());

	// Find the Terrain Specific Mesh Here
	TerrainMeshName = Terrain->TerrainMeshName;
	// The mesh should not be null as it should be generated when the Terrain is created.

	// I will need to clean up old lists if they exist
	if (ScenePartition){
		ScenePartition->Exit();
		delete ScenePartition;
	}
	if (InteractiveMap)
		delete InteractiveMap;
	if (m_heightMap.size())
		m_heightMap.clear();
	BManager.Exit();
	EntityList.clear();

	// Set up HM stats
	m_heightMap = Terrain->TerrainHeightMap;

	// I will now begin generating the map with the csv file from the terrain
	ScenePartition = new ScenePartitionGraph();
	ScenePartition->AssignGridParameters(Vector3(), Vector3(TerrainScale.x, TerrainScale.z), 10);
	InteractiveMap = new GameMap();
	GameMap *theMap = dynamic_cast<GameMap*>(InteractiveMap);
	theMap->ScenePartition = ScenePartition;
	theMap->SetEntityID("Battlefield");
	theMap->LoadMap(Terrain->CSVMapName, false, m_heightMap, TerrainScale, EntityList, BManager);
}

void BattleScene::RenderTerrain()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	modelStack->PushMatrix();
	modelStack->Scale(TerrainScale.x, TerrainScale.y - 5.f, TerrainScale.z);
	Renderer->RenderMesh(TerrainMeshName, true);
	modelStack->PopMatrix();
}

void BattleScene::RenderShadowCasters()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//RenderTerrain();
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());

	RenderTerrain();

	ScenePartition->Render();

	GameLogicSystem::Instance().Render();

	for (std::vector<Particle*>::iterator it = BManager.BillboardContainer.begin(); it != BManager.BillboardContainer.end(); ++it)
	{
		if ((*it)->Active)
		{
			float TimeRatio = 1;
			modelStack->PushMatrix();
			modelStack->Translate((*it)->GetPosition().x, (*it)->GetPosition().y, (*it)->GetPosition().z);
			modelStack->Rotate(Math::RadianToDegree(atan2(camera->position.x - (*it)->GetPosition().x, camera->position.z - (*it)->GetPosition().z)), 0, 1, 0);
			modelStack->Scale(TimeRatio * (*it)->GetDimensions().x, TimeRatio *(*it)->GetDimensions().y, TimeRatio *(*it)->GetDimensions().z);
			Renderer->RenderMesh((*it)->GetMeshName(), false);
			modelStack->PopMatrix();
		}
	}
}

void BattleScene::RenderSkybox()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	//left
	modelStack->PushMatrix();
	modelStack->Rotate(90, 0, 1, 0);
	modelStack->PushMatrix();
	modelStack->Rotate(90, 0, 1, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Left", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(-90, 0, 1, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Right", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Front", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(180, 0, 1, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Back", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(90, 1, 0, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	modelStack->Rotate(90, 0, 0, 1);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Top", false);
	modelStack->PopMatrix();

	modelStack->PushMatrix();
	modelStack->Rotate(-90, 1, 0, 0);
	modelStack->Translate(0, 0, -SkyboxSize / 2 + 2.f);
	//modelStack->Rotate(-90, 0, 0, 1);
	modelStack->Scale(SkyboxSize, SkyboxSize, SkyboxSize);
	Renderer->RenderMesh("SB_Bottom", false);
	modelStack->PopMatrix();
	modelStack->PopMatrix();
}

void BattleScene::RenderPassGPass()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	Renderer->m_renderPass = RenderSystem::RENDER_PASS::RENDER_PASS_PRE;
	Renderer->m_lightDepthFBO.BindForWriting();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(Renderer->m_gPassShaderID);
	//These matrices should change when light position or direction changes
	if (Renderer->lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		// based on scene size [below]
		Renderer->m_lightDepthProj.SetToOrtho(-1200, 1200, -1200, 1200, 0, 4000);
	}
	else
	{
		Renderer->m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 20);
	}
	Renderer->m_lightDepthView.SetToLookAt(Renderer->lights[0].position.x, Renderer->lights[0].position.y, Renderer->lights[0].position.z, 0, 0, 0, 0, 1, 0);
	// Things that the light sees and creates shadows from.
	RenderShadowCasters();
}

void BattleScene::RenderPassMain()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	Renderer->m_renderPass = RenderSystem::RENDER_PASS::RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, (GLsizei)SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth, (GLsizei)SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(Renderer->m_programID);
	//pass light depth texture
	Renderer->m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(Renderer->m_parameters[Renderer->U_SHADOW_MAP], 8);

	if (Renderer->lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(Renderer->lights[0].position.x, Renderer->lights[0].position.y, Renderer->lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack->Top() * lightDir;
		glUniform3fv(Renderer->m_parameters[Renderer->U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (Renderer->lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack->Top() * Renderer->lights[0].position;
		glUniform3fv(Renderer->m_parameters[Renderer->U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack->Top() * Renderer->lights[0].spotDirection;
		glUniform3fv(Renderer->m_parameters[Renderer->U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack->Top() * Renderer->lights[0].position;
		glUniform3fv(Renderer->m_parameters[Renderer->U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	Renderer->SetHUD(false);

	Mtx44 perspective;
	perspective.SetToPerspective(60.0f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth / SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight, 0.1f, 1000.0f);
	projectionStack->LoadMatrix(perspective);

	// Camera matrix
	viewStack->LoadIdentity();
	viewStack->LookAt(
		camera->position.x, camera->position.y, camera->position.z,
		camera->target.x, camera->target.y, camera->target.z,
		camera->up.x, camera->up.y, camera->up.z
		);
	// Model matrix : an identity matrix (model will be at the origin)
	modelStack->LoadIdentity();
	SceneSystem::Instance().RenderTransitionEffects(); 
	SceneSystem::Instance().RenderMouseCursor(Vector3(SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.05f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.05f), "weed");

	Renderer->RenderMesh("reference", false);

	RenderSkybox();
	RenderShadowCasters();

	Renderer->SetHUD(true);
	std::stringstream ss;
	ss.str("");
	ss << "FPS: " << framerates;
	Renderer->RenderTextOnScreen(ss.str(), Color(), SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth* 0.02f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth* 0.01f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.1f);

	Renderer->SetHUD(false);
}

void BattleScene::Render()
{
	//*********************************
	//		PRE RENDER PASS
	//*********************************
	RenderPassGPass();
	//*********************************
	//		MAIN RENDER PASS
	//*********************************
	RenderPassMain();
}

void BattleScene::Exit()
{
	QuickExit();
}