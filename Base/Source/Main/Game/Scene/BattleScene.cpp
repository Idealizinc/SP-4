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
	Player = nullptr;
	SceneSystem::Instance().AddScene(*this);
	Init();
}

BattleScene::~BattleScene()
{

}

void BattleScene::QuickInit()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());

	// Set Terrain Size
	TerrainScale.Set(400.f, 25.f, 400.f);
	ScenePartition = new ScenePartitionGraph();
	ScenePartition->AssignGridParameters(Vector3(), Vector3(TerrainScale.x, TerrainScale.z), 4);

	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth / SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight, 0.1f, 10000.0f);
	projectionStack->LoadMatrix(perspective);

	CameraAerial* CA = new CameraAerial();
	camera = CA;
	CA->AltInit(/*Player Character Position*/Vector3(0, 0, 0), Vector3(0, 100, 1), Vector3(0, 1, 0));
	CenterPosition.Set(SceneSystem::Instance().cSS_InputManager->cIM_ScreenWidth * 0.5f, SceneSystem::Instance().cSS_InputManager->cIM_ScreenHeight * 0.5f, 0);

	// Initiallise Model Specific Meshes Here
	Mesh* newMesh = MeshBuilder::GenerateTerrain("Town 1", "HeightMapFiles//heightmap_Town1.raw", m_heightMap);
	newMesh->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	newMesh->material.kDiffuse.Set(0.2f, 0.2f, 0.2f);
	newMesh->material.kSpecular.Set(0.0f, 0.0f, 0.0f);
	newMesh->textureArray[0] = LoadTGA("Image//RockTex.tga");
	newMesh->textureArray[1] = LoadTGA("Image//GrassStoneTex.tga");
	Renderer->MeshList.insert(std::pair<std::string, Mesh*>(newMesh->name, newMesh));

	InteractiveMap = new GameMap();
	GameMap *theMap = dynamic_cast<GameMap*>(InteractiveMap);
	theMap->ScenePartition = ScenePartition;
	theMap->SetEntityID("Battlefield");
	theMap->LoadMap("CSVFiles//BattlefieldLayout1.csv", false, m_heightMap, TerrainScale, EntityList, BManager);

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
	for (auto it : EntityList)
	{
		delete it;
	}
	EntityList.clear();
	if (Player)
		delete Player;
	if (camera)
		delete camera;
}
void BattleScene::Init()
{
	QuickInit();
}

void BattleScene::Update(const float& dt)
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());

	Renderer->Update(dt);

	float Speed = 50.f;
	CameraAerial* CA = (CameraAerial*)camera;

	if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::FORWARD_COMMAND]))
	{
		CA->CameraMoveTargetPosition.z -= Speed*dt;
	}
	if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::BACK_COMMAND]))
	{
		CA->CameraMoveTargetPosition.z += Speed*dt;
	}
	if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::RIGHT_COMMAND]))
	{
		CA->CameraMoveTargetPosition.x += Speed*dt;
	}
	if (SceneSystem::Instance().cSS_InputManager->GetKeyValue(SimpleCommand::m_allTheKeys[SimpleCommand::LEFT_COMMAND]))
	{
		CA->CameraMoveTargetPosition.x -= Speed*dt;
	}

	if (Application::IsKeyPressed(VK_OEM_MINUS))
	{
		ScenePartition->ShowPartitions = false;
	}

	if (Application::IsKeyPressed(VK_OEM_PLUS))
	{
		ScenePartition->ShowPartitions = true;
	}

	CA->Update(dt);
	//MusicSystem::Instance().playBackgroundMusic("battle");
	BManager.UpdateContainer(dt, CA->position);

	ScenePartition->Update(dt);

	GameLogicSystem::Instance().Update(dt);

	framerates = 1 / dt;
}

//Start of Changes
void BattleScene::UpdateCharacterLogic(double dt)
{
	//std::vector<CharacterEntity*> Container = ObjectManager::Instance().GetCharacterList();
	//NumCharacters = 0;
	//for (std::vector<CharacterEntity*>::iterator it = Container.begin(); it != Container.end(); ++it)
	//{
	//	CharacterEntity* CE = *it;
	//	if (CE->Active)
	//	{
	//		BS.CurrentPlayerUnitCount.find(CE->GetEntityID())->second += 1;
	//		Vector3 Pos = CE->GetPosition();
	//		//Inverse Velocity if Character is about to leave screen
	//		float DistBuffer = CE->GetDimensions().x * 3.f;
	//		//if ((CE->GetPosition().x > ObjectManager::Instance().WorldWidth - DistBuffer && CE->GetVelocity().x > 0) || (CE->GetPosition().x < DistBuffer && CE->GetVelocity().x < 0))
	//		//{
	//		//	Vector3 Velocity = CE->GetVelocity();
	//		//	Velocity.x = 2 * -CE->GetVelocity().x;
	//		//	CE->SetVelocity(Velocity);
	//		//}
	//		//if ((CE->GetPosition().z > ObjectManager::Instance().WorldWidth - DistBuffer && CE->GetVelocity().z > 0) || (CE->GetPosition().z < DistBuffer && CE->GetVelocity().z < 0))
	//		//{
	//		//	Vector3 Velocity = CE->GetVelocity();
	//		//	Velocity.z = 2 * -CE->GetVelocity().z;
	//		//	CE->SetVelocity(Velocity);
	//		//}
	//		//// 
	//		//if ((CE->GetPosition().z < ObjectManager::Instance().WorldHeight && CE->GetPosition().z > 0) && (CE->GetPosition().x < ObjectManager::Instance().WorldWidth && CE->GetPosition().x > 0))
	//		//{
	//		//	CE->SetVelocity(CE->GetVelocity() - CE->GetVelocity() * 0.5f * (float)dt);
	//		//}
	//		//else
	//		//{
	//		//	float SpeedCap = (float)CE->WalkSpeed * 1.5f;
	//		//	if (CE->GetVelocity().x > SpeedCap)
	//		//	{
	//		//		Vector3 Vel = CE->GetVelocity();
	//		//		if (Vel.x > 0.f)
	//		//			Vel.x = SpeedCap;
	//		//		else Vel.x = -SpeedCap;
	//		//		CE->SetVelocity(Vel);
	//		//	}
	//		//	if (CE->GetVelocity().z > SpeedCap)
	//		//	{
	//		//		Vector3 Vel = CE->GetVelocity();
	//		//		if (Vel.z > 0.f)
	//		//			Vel.z = SpeedCap;
	//		//		else Vel.z = -SpeedCap;
	//		//		CE->SetVelocity(Vel);
	//		//	}
	//		//}
	//		CE->Update((float)dt);
	//		NumCharacters++;
	//	}
	//}
}

void BattleScene::UpdateInternals(double dt)
{
	//std::vector<Projectile*> Container = ObjectManager::Instance().GetProjectileList();
	//for (std::vector<Projectile*>::iterator it = Container.begin(); it != Container.end(); ++it)
	//{
	//	BaseObject *obj = (BaseObject*)*it;
	//	Vector3 Pos = obj->GetPosition();

	//	if (Pos.z > ObjectManager::Instance().WorldHeight) obj->Active = false;
	//	if (Pos.z < 0) obj->Active = false;
	//	if (Pos.x > ObjectManager::Instance().WorldWidth) obj->Active = false;
	//	if (Pos.x < 0) obj->Active = false;

	//	if (obj->Active)
	//	{
	//		for (std::vector<CharacterEntity*>::iterator it2 = ObjectManager::Instance().GetCharacterList().begin(); it2 != ObjectManager::Instance().GetCharacterList().end(); ++it2)
	//		{
	//			BaseObject *obj2 = dynamic_cast<BaseObject*>(*it2);
	//			if (obj2->Active && (((*it)->OwnerID == "Magic" && obj2->GetEntityID() != "Magic") || (((*it)->OwnerID == "Mercedes" && obj2->GetEntityID() != "Mercedes")) && ((*it)->OwnerID == "Range" && obj2->GetEntityID() != "Melee")))
	//			{
	//				if (CheckCollision(obj, obj2))
	//				{
	//					obj->Active = false;
	//					CharacterEntity* CE = *it2;
	//					//Get Hitted
	//					CE->HealthPoints -= (*it)->GetDamageDealt();

	//					int NumParticles = Math::RandIntMinMax(2, 4);
	//					for (int i = 0; i < NumParticles; ++i)
	//					{
	//						float ParticleSpeed = Math::RandFloatMinMax(3.f, 6.f);
	//						float ParticleLifeTime = Math::RandFloatMinMax(0.75f, 1.5f);

	//						//ObjectManager::Instance().AddNewParticle(new Particle(obj2->GetEntityID(), 1, CE->GetPosition() * Math::RandFloatMinMax(0.3f, 0.75f), Vector3(Math::RandFloatMinMax(-ParticleSpeed, ParticleSpeed), Math::RandFloatMinMax(-ParticleSpeed, ParticleSpeed), Math::RandFloatMinMax(0, ParticleSpeed)), camera.position, ParticleLifeTime));
	//					}
	//				}
	//			}
	//		}
	//	}
	//	//Update if the object is still exists
	//	if (obj->Active && !obj->Static)
	//		obj->Update((float)dt);

	//	//Particle Update
	//	for (std::vector<Particle*>::iterator it = ObjectManager::Instance().GetParticleList().begin(); it != ObjectManager::Instance().GetParticleList().end(); ++it)
	//	{
	//		Particle *obj = (Particle*)*it;
	//		if (obj->Active && !obj->Visible)
	//			obj->Update((float)dt);
	//	}
	//}
}

bool BattleScene::CheckCollision(BaseObject *o1, BaseObject *o2, std::string type)
{
	if (type == "Circle")
	{
		float CombinedRadiusSquared = (o1->GetDimensions().x + o2->GetDimensions().x) * (o1->GetDimensions().x + o2->GetDimensions().x);
		float DistSquared = (o1->GetPosition() - o2->GetPosition()).LengthSquared();
		if (DistSquared < CombinedRadiusSquared)
		{
			return true;
		}
		return false;
	}
	else if (type == "AABB")
		return false;
	else if (type == "OBB")
		return false;
	else return false;
}

void BattleScene::RenderObjects(BaseObject *obj)
{
	if (obj->Active && obj->Visible && obj->GetEntityID() != "")
	{
		obj->Render();
	}
}
//End of changes

void BattleScene::RenderTerrain()
{
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());
	modelStack->PushMatrix();
	modelStack->Scale(TerrainScale.x, TerrainScale.y, TerrainScale.z);
	Renderer->RenderMesh("Town 1", true);
	modelStack->PopMatrix();
}

void BattleScene::RenderShadowCasters()
{
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//RenderTerrain();
	RenderSystem *Renderer = dynamic_cast<RenderSystem*>(&SceneSystem::Instance().GetRenderSystem());

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

	Renderer->RenderMesh("reference", false);

	RenderTerrain();
	RenderSkybox();
	RenderShadowCasters();

	//Changes
	//Projectile
	//for (std::vector<Projectile*>::iterator it = ObjectManager::Instance().GetProjectileList().begin(); it != ObjectManager::Instance().GetProjectileList().end(); ++it)
	//{
	//	BaseObject *obj = (BaseObject*)*it;
	//	if (obj->Active && obj->Visible)
	//		RenderObjects(obj);
	//}
	////Characters
	//for (std::vector<CharacterEntity*>::iterator it = ObjectManager::Instance().GetCharacterList().begin(); it != ObjectManager::Instance().GetCharacterList().end(); ++it)
	//{
	//	CharacterEntity *obj = (CharacterEntity*)*it;
	//	if (obj->Active && obj->Visible)
	//	{
	//		float TextScale = 2.5f;
	//		float TextOffset = 0.5f + 0.5f * TextScale;

	//		RenderObjects(obj);

	//		//HealthBar
	//		//Vector3 BarDimensionDefault = Vector3(obj->GetDimensions().x * 2.f, obj->GetDimensions().y * 0.4f, 1);
	//		//float HPBarDimensionX = ((float)obj->HealthPoints / (float)obj->MaxHealthPoints) * BarDimensionDefault.x + Math::EPSILON;
	//		//float HPBarPosX = obj->GetPosition().x - (BarDimensionDefault.x - HPBarDimensionX) * 0.5f;
	//		//float HPBarPosY;
	//		//
	//		//modelStack->PushMatrix();
	//		//modelStack->Translate(HPBarPosX, HPBarPosY, 1);
	//		//modelStack->Scale(HPBarDimensionX, BarDimensionDefault.y, BarDimensionDefault.z);
	//		//if (obj->HealthPoints > obj->MaxHealthPoints * 0.25f)
	//		//	Renderer->RenderMesh("GreenBar", false);
	//		//else Renderer->RenderMesh("RedBar", false);
	//		//modelStack->PopMatrix();

	//		//modelStack->PushMatrix();
	//		//modelStack->Top(obj->GetPosition().x, HPBarPosY + obj->GetDimensions().y, 1);
	//		//float Ratio = (obj->MessageAnimationTimer / obj->MessageAnimationMaxTime);
	//		//float Size = TextScale * 2.f;
	//		//float Scale = Math::Clamp(Size*Ratio, Math::EPSILON, Size);
	//		//modelStack->Scale(Scale, Scale, 1);
	//		//if (obj->FoundMessage)
	//		//	Renderer->RenderMesh("ShowMessage", false);
	//		//modelStack->PopMatrix();
	//	}
	//}
	//End of changes

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