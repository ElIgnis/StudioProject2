#include "SP2.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "Mtx44.h"

#include "Application.h"
#include "MeshBuilder.h"

#include "Vertex.h"
#include "LoadTGA.h"

SP2::SP2()
{
}

SP2::~SP2()
{
}

void SP2::Init()
{
	fps = 0;
	count = 0;

	std::string data = " ";

	//File reading
	std::ifstream inFile;
	inFile.open("Source//charWidth.txt");
	if(inFile.good())
	{
		while(getline(inFile, data))
		{
			textWidth[count] = std::stof(data);
			count++;
		}
		inFile.close();
	}

	// Set background color to black
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//Enable depth buffer and depth testing
	glEnable(GL_DEPTH_TEST);
	//Enable back face culling
	glEnable(GL_CULL_FACE);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//Default to fill mode
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	// Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Load vertex and fragment shaders
	m_programID = LoadShaders( "Shader//Texture.vertexshader", "Shader//MultiLight.fragmentshader" );
	// Use our shader
	glUseProgram(m_programID);
	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);

	//Initialize camera settings
	camera.Init(Vector3(0, 0, 0.1), Vector3(0, 0, 0), Vector3(0, 1, 0));
	
	//cl check
	double plX = camera.position.x;
	double plZ = camera.position.z;

	//Initialize all meshes to NULL
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference", 1000, 1000, 1000);

	Mtx44 projection;
	projection.SetToPerspective(45.f, 4.f / 3.f, 0.1f, 4000.f);
	projectionStack.LoadMatrix(projection);

	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID,"lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID,"lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID,"lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID,"lights[0].exponent");

	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID,"lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID,"lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID,"lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID,"lights[1].exponent");

	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID,"numLights");
	glUseProgram(m_programID);
	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	lights[0].type = Light::LIGHT_POINT;
	lights[0].position.Set(8.5f, 0.f, 260.f);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 0;
	lights[0].kC = 1.0f;
	lights[0].kL = 0.0001f;
	lights[0].kQ = 0.00001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_DIRECTIONAL;
	lights[1].position.Set(-40.f, 50.f, 550.f);
	lights[1].color.Set(1, 1, 1);
	lights[1].power = 1;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	// Make sure you pass uniform parameters after glUseProgram()
	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	glUniform1i(m_parameters[U_NUMLIGHTS], 2);
	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

	//Text
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureID = LoadTGA("Image//HelveticaLTStd-Cond.tga");

	//Skybox
	meshList[GEO_TOP] = MeshBuilder::GenerateQuad("UP", Color(1, 1, 1), 1.f);
	meshList[GEO_TOP]->textureID = LoadTGA("Image//Skybox_top.tga");
	meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("UP", Color(1, 1, 1), 1.f);
	meshList[GEO_BOTTOM]->textureID = LoadTGA("Image//Skybox_down.tga");
	meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("UP", Color(1, 1, 1), 1.f);
	meshList[GEO_RIGHT]->textureID = LoadTGA("Image//Skybox_right.tga");
	meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("UP", Color(1, 1, 1), 1.f);
	meshList[GEO_LEFT]->textureID = LoadTGA("Image//Skybox_left.tga");
	meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("UP", Color(1, 1, 1), 1.f);
	meshList[GEO_FRONT]->textureID = LoadTGA("Image//Skybox_front.tga");
	meshList[GEO_BACK] = MeshBuilder::GenerateQuad("UP", Color(1, 1, 1), 1.f);
	meshList[GEO_BACK]->textureID = LoadTGA("Image//Skybox_back.tga");

	//Wall&Floor
	meshList[GEO_WALL] = MeshBuilder::GenerateOBJ("WALL", "OBJ//Wall.obj");
	//meshList[GEO_WALL]->textureID = LoadTGA("Image//chair.tga");
	meshList[GEO_WALL]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WALL]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_WALL]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_WALL]->material.kShininess = 5.f;

	//ColdShelves
	meshList[GEO_COLDSHELVE] = MeshBuilder::GenerateOBJ("ColdShelve", "OBJ//ColdShelves.obj");
	meshList[GEO_COLDSHELVE]->textureID = LoadTGA("Image//ColdShelve.tga");
	meshList[GEO_COLDSHELVE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_COLDSHELVE]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_COLDSHELVE]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_COLDSHELVE]->material.kShininess = 5.f;

	//Doorman
	meshList[GEO_DOORMAN] = MeshBuilder::GenerateOBJ("Doorman", "OBJ//doorman.obj");
	meshList[GEO_DOORMAN]->textureID = LoadTGA("Image//doorman.tga");
	meshList[GEO_DOORMAN]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_DOORMAN]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_DOORMAN]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_DOORMAN]->material.kShininess = 5.f;


	//Trolley
	//meshList[Geo_TROLLEY] = MeshBuilder::GenerateOBJ("Trolley", "OBJ//Trolleys.obj");
	//meshList[Geo_TROLLEY]->textureID = LoadTGA("Image//Shiny_Silver_Text.tga");
	//meshList[Geo_TROLLEY]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	//meshList[Geo_TROLLEY]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	//meshList[Geo_TROLLEY]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	//meshList[Geo_TROLLEY]->material.kShininess = 5.f;

	//Stander
	meshList[GEO_STANDER] = MeshBuilder::GenerateOBJ("Stander", "OBJ//Stander.obj");
	meshList[GEO_STANDER]->textureID = LoadTGA("Image//Stander.tga");
	meshList[GEO_STANDER]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_STANDER]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_STANDER]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_STANDER]->material.kShininess = 5.f;

	//Shelf
	meshList[GEO_SHELF] = MeshBuilder::GenerateOBJ("Shelf", "OBJ//Shelf.obj");
	meshList[GEO_SHELF]->textureID = LoadTGA("Image//Shelf.tga");
	meshList[GEO_SHELF]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_SHELF]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_SHELF]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_SHELF]->material.kShininess = 5.f;

	meshList[GEO_REFRIDGE] = MeshBuilder::GenerateOBJ("Refridge", "OBJ//Refridge.obj");
	meshList[GEO_REFRIDGE]->textureID = LoadTGA("Image//Refridge.tga");
	meshList[GEO_REFRIDGE]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_REFRIDGE]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_REFRIDGE]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_REFRIDGE]->material.kShininess = 5.f;

	meshList[GEO_THIRDSHELF] = MeshBuilder::GenerateOBJ("Refridge", "OBJ//thirdShelf.obj");
	meshList[GEO_THIRDSHELF]->textureID = LoadTGA("Image//thirdShelf.tga");
	meshList[GEO_THIRDSHELF]->material.kAmbient.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_THIRDSHELF]->material.kDiffuse.Set(1.f, 1.f, 1.f);
	meshList[GEO_THIRDSHELF]->material.kSpecular.Set(0.8f, 0.8f, 0.8f);
	meshList[GEO_THIRDSHELF]->material.kShininess = 5.f;
}

static float ROT_LIMIT = 45.f;
static float SCALE_LIMIT = 5.f;

void SP2::Update(double dt)
{
	if(Application::IsKeyPressed('5')) //enable back face culling
		glEnable(GL_CULL_FACE);
	if(Application::IsKeyPressed('2')) //disable back face culling
		glDisable(GL_CULL_FACE);
	if(Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //default fill mode
	if(Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //wireframe mode

	camera.Update(dt);

	//FPS value
	fps = 1 / dt;
	std::stringstream ss;
	ss << fps;
	fpsText = ss.str();

	//Pos checker
	std::stringstream s_plX;
	std::stringstream s_plZ;

	s_plX << camera.position.x;
	s_plZ << camera.position.z;

	plXCoord = s_plX.str();
	plZCoord = s_plZ.str();

}

void SP2::Render()
{
	float textSize = 3.f;
	//clear depth and color buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Temp variables
	Mtx44 MVP;

	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z, camera.target.x, camera.target.y, camera.target.z, camera.up.x, camera.up.y, camera.up.z);
	modelStack.LoadIdentity();

	//Ceiling light
	if(lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	//Exterior light
	if(lights[1].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[1].position.x, lights[1].position.y, lights[1].position.z);
		Mtx44 rotation;
		rotation.SetToRotation(-45.f, 1.f, 0.f, 0.f);
		Vector3 lightDirection_cameraspace = viewStack.Top() * rotation * lightDir;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if(lights[1].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);

		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
		glUniform3fv(m_parameters[U_LIGHT1_POSITION], 1, &lightPosition_cameraspace.x);
	}

	modelStack.PushMatrix();
	modelStack.Translate(-20.f, 0.f, -20.f);
	modelStack.Scale(10.f, 10.f, 10.f);
	float a = 0.1f;
	modelStack.PopMatrix();

	RenderSkyBox();
	RenderObject();

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	//Text display for FPS
	RenderTextOnScreen(meshList[GEO_TEXT], "FPS:"+fpsText, Color(1, 0, 0), textSize, 22.5f, 19.f);
	RenderTextOnScreen(meshList[GEO_TEXT], "X:"+plXCoord, Color(0, 1, 0), 5.f, 0.5f, 1.5f);
	RenderTextOnScreen(meshList[GEO_TEXT], "Z:"+plZCoord, Color(0, 1, 0), 5.f, 0.5f, 2.5f);
}

void SP2::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;
	
	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	if(mesh->textureID > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureID);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}

	if(enableLight)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView_inverse_transpose.a[0]);

		//load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{	
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}

	mesh->Render();

	if(mesh->textureID > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}

void SP2::RenderSkyBox()
{
	Mtx44 MVP;

	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Translate(0, 0, -0.5);
	RenderMesh(meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Translate(0, 0, 0.5);
	modelStack.Rotate(180, 0, 1, 0);
	RenderMesh(meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Translate(-0.5, 0, 0);
	modelStack.Rotate(90, 0, 1, 0);
	RenderMesh(meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Translate(0.5,0, 0);
	modelStack.Rotate(90, 0, -1, 0);
	RenderMesh(meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Translate(0, 0.5, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, 1, 0, 0);
	RenderMesh(meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Scale(1000, 1000, 1000);
	modelStack.Translate(0, -0.5, 0);
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Rotate(90, -1, 0, 0);
	RenderMesh(meshList[GEO_BOTTOM], false);
	modelStack.PopMatrix();
}
void SP2::RenderObject()
{
	Mtx44 MVP;

	//wall
	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1, 1, 1);
	modelStack.Translate(0, 0, -25);
	RenderMesh(meshList[GEO_WALL], true);
	modelStack.PopMatrix();

	//Coldshelve
	for (int i = 0; i > -75; i-=15)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, i);
		{
			modelStack.PushMatrix();
			modelStack.Scale(1, 1, 1);
			modelStack.Translate(40.1, -2.1, 10);
			modelStack.Rotate(180, 0, 1, 0);
			RenderMesh(meshList[GEO_COLDSHELVE], true);
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1, 1, 1);
	modelStack.Translate(35, -2.125, 0);
	RenderMesh(meshList[GEO_DOORMAN], true);
	modelStack.PopMatrix();

	//modelStack.PushMatrix();
	////scale, translate, rotate
	//modelStack.Scale(1, 1, 1);
	//modelStack.Translate(0, 0, 0);
	//RenderMesh(meshList[Geo_TROLLEY], true);
	//modelStack.PopMatrix();

	modelStack.PushMatrix();
	//scale, translate, rotate
	modelStack.Scale(1, 1, 1);
	modelStack.Translate(25, -2.25, 4);
	RenderMesh(meshList[GEO_STANDER], true);
	modelStack.PopMatrix();

	for (int i = 0; i > -60; i -= 15)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0,0,i);
		{
			modelStack.PushMatrix();
			//scale, translate, rotate
			modelStack.Scale(1, 1, 1);
			modelStack.Translate(18, -2.20, -2);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_SHELF], true);
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}
	for (int i = 0; i > -54; i -= 9)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, i);
		{
			modelStack.PushMatrix();
			//scale, translate, rotate
			modelStack.Scale(1, 1, 1);
			modelStack.Translate(-2, -2.20, -2);
			RenderMesh(meshList[GEO_REFRIDGE], true);
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}
	for (int i = 0; i > -54; i -= 8.5)
	{
		modelStack.PushMatrix();
		modelStack.Translate(0, 0, i);
		{
			modelStack.PushMatrix();
			//scale, translate, rotate
			modelStack.Scale(1, 1, 1);
			modelStack.Translate(-20, -2.20, -2);
			modelStack.Rotate(90, 0, 1, 0);
			RenderMesh(meshList[GEO_THIRDSHELF], true);
			modelStack.PopMatrix();
		}
		modelStack.PopMatrix();
	}
}

void SP2::RenderText(Mesh* mesh, std::string text, Color color)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;
	
	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	float temp = 0;
	float widthDivide = 100;
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(temp * 1.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		mesh->Render((unsigned)text[i] * 6, 6);
		temp += (textWidth[text[i]] / 100);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	glEnable(GL_DEPTH_TEST);
}

void SP2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if(!mesh || mesh->textureID <= 0) //Proper error check
		return;

	glDisable(GL_DEPTH_TEST);
	//Add these code just after glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, 80, 0, 60, -10, 10); //size of screen UI
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity(); //No need camera for ortho mode
	modelStack.PushMatrix();
	modelStack.LoadIdentity(); //Reset modelStack
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureID);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	float temp = 0;
	float widthDivide = 100;
	for(unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(temp * 1.5f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
		temp += textWidth[text[i]] / widthDivide;
		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	//Add these code just before glEnable(GL_DEPTH_TEST);
	projectionStack.PopMatrix();
	viewStack.PopMatrix();
	modelStack.PopMatrix();

	glEnable(GL_DEPTH_TEST);
}
	
void SP2::Exit()
{
	// Cleanup here
	for(int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if(meshList[i] != NULL)
			delete meshList[i];
	}
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}