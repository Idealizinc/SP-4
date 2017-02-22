/*
Copyright (c) Lim Rui An, Ryan 2016

Creator(s)    : Lim Rui An, Ryan
Creation Date : 31 September 2016 9:22 AM
File Name     : RenderSystem.h
Purpose       : Defines a RenderSystem
*/

#ifndef _RENDER_SYSTEM_H
#define _RENDER_SYSTEM_H

#include "../Objects/SceneEntity.h"
#include "../../Game/Miscellaneous/MeshBuilder.h"
#include "Vector3.h"
#include "Light.h"
#include <map>
#include "DepthFBO.h"
#include <algorithm>
#include "shader.hpp"
#include "LoadTGA.h"
#include "LoadOBJ.h"
#include  "../../Engine/Objects/SpriteObject.h"

#ifndef MAX_NUM_LIGHTS
#define MAX_NUM_LIGHTS 2
#endif

class RenderSystem : public SceneEntity
{
public:
	RenderSystem();
	virtual ~RenderSystem();
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,

		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,

		U_LIGHTENABLED,
		U_NUMLIGHTS,

		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,

		U_LIGHT1_TYPE,
		U_LIGHT1_POSITION,
		U_LIGHT1_COLOR,
		U_LIGHT1_POWER,
		U_LIGHT1_KC,
		U_LIGHT1_KL,
		U_LIGHT1_KQ,
		U_LIGHT1_SPOTDIRECTION,
		U_LIGHT1_COSCUTOFF,
		U_LIGHT1_COSINNER,
		U_LIGHT1_EXPONENT,

		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE_ENABLED1,
		U_COLOR_TEXTURE_ENABLED2,

		U_COLOR_TEXTURE,
		U_COLOR_TEXTURE1,
		U_COLOR_TEXTURE2,

		U_SHADOW_COLOR_TEXTURE,
		U_SHADOW_COLOR_TEXTURE1,
		U_SHADOW_COLOR_TEXTURE2,
		U_SHADOW_COLOR_TEXTURE_ENABLED,
		U_SHADOW_COLOR_TEXTURE_ENABLED1,
		U_SHADOW_COLOR_TEXTURE_ENABLED2,

		U_LIGHT_DEPTH_MVP_GPASS,
		U_LIGHT_DEPTH_MVP,
		U_SHADOW_MAP,

		U_SHADOW_SAMPLING_AMOUNT, // Shadow Sample Count

		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLED,

		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TOTAL,
	};

	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
	};

	std::map<std::string, Mesh*> MeshList;
	std::map<std::string, std::map<std::string, std::vector<GLuint>>> SpriteList; // Key: Character Entity Name, <Key: Animation ID, Value: Vector of GLuint* that represents the frame animations>

	virtual void Init();
	virtual void Update(const float& dt);
	virtual void Render();
	virtual void Exit();

	unsigned m_parameters[U_TOTAL];
	unsigned m_programID;
	unsigned m_vertexArrayID;
	Light lights[MAX_NUM_LIGHTS];

	virtual void RenderText(Mesh& mesh, const std::string &text, Color &color);
	virtual void RenderText(const std::string &text, Color &color);
	virtual void RenderTextOnScreen(Mesh& mesh, const std::string &text, Color &color, const float &size, const float &x, const float &y);
	virtual void RenderTextOnScreen(const std::string &text, Color &color, const float &size, const float &x, const float &y);
	virtual void RenderMeshIn2D(Mesh &mesh, const bool &enableLight, const float &size, const float &x, const float &y);
	virtual void RenderMeshIn2D(Mesh &mesh, const bool &enableLight, const float &sizeX, const float &sizeY, const float &x, const float &y);
	virtual void RenderMesh(Mesh &mesh, const bool &enableLight);
	virtual void RenderText(const std::string &meshName, const std::string &text, Color &color);
	virtual void RenderTextOnScreen(const std::string &meshName, const std::string &text, Color &color, const float &size, const float &x, const float &y);
	virtual void RenderMeshIn2D(const std::string &meshName, const bool &enableLight, const float &size, const float &x, const float &y);
	virtual void RenderMeshIn2D(const std::string &meshName, const bool &enableLight, const float &sizeX, const float &sizeY, const float &x, const float &y);
	virtual void RenderMeshIn2D(Mesh &mesh, const bool &enableLight,const Vector3& Position, const Vector3& Dimensions, const Vector3& Rotation);
	virtual void RenderMesh(const std::string &meshName, const bool &enableLight);

	void SetHUD(const bool&);

	// Shadows
	unsigned m_gPassShaderID;
	DepthFBO m_lightDepthFBO;
	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;
	RENDER_PASS m_renderPass;

private:
	bool OrthogonalModeActive;
	bool LoadCSVMeshes(const std::string &fileLocation);
	bool LoadCSVSpriteMeshes(const std::string &fileLocation);
	Mesh *ExportedFont;
};

#endif //_RENDER_SYSTEM_H