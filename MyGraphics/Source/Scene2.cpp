#include "Scene2.h"
#include "GL\glew.h"
#include "shader.hpp"


Scene2::Scene2()
{
}

Scene2::~Scene2()
{
}

void Scene2::Init()
{
	// Init VBO here
	rotateAngle = 0.0f;
	translateX = 0.0f;
	scaleAll = 1.0f;

	rotateL = true;
	boundaryX = false;

	//Load vertex and fragment shaders
	//m_programID = LoadShaders("Shader//SimpleVertexShader.vertexshader", "Shader//SimpleFragmentShader.fragmentshader" );
	m_programID = LoadShaders(
		"Shader//TransformVertexShader.vertexshader", 
		"Shader//SimpleFragmentShader.fragmentshader");

	// Get a handle for our "MVP" uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");

	//Set background color to dark blue
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	//Generate a default VAO for now
	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	//Generate buffers
	glGenBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glGenBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);

	// 8-sided polygon
	//An array of 3 vectors which represent 3 vertices
	static const GLfloat vertex_buffer_data[] = {
		0.5f, -0.25f, 0.0f,	//vertex 0(S.P)
		-0.5f, -0.25f, 0.0f,	//vertex 1
		0.0f, 1.0f, 0.0f,	//vertex 2(line)
		//-0.2f, 0.3f, 0.0f,	//vertex 3
		//-0.3f, 0.4f, 0.0f,	//vertex 4
		//-0.4f, 0.2f, 0.0f,	//vertex 5(line)
		//-0.4f, 0.4f, 0.0f,	//vertex 6
		//-0.5f, 0.3f, 0.0f,	//vertex 7
		//-0.4f, 0.2f, 0.0f,	//vertex 8(line)
		//-0.5f, 0.2f, 0.0f,	//vertex 9
		//-0.4f, 0.1f, 0.0f,	//vertex 10
		//-0.4f, 0.2f, 0.0f,	//vertex 11(line)
		//-0.3f, 0.1f, 0.0f,	//vertex 12(E.P)
	};

	//Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);

	//Transfer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data),vertex_buffer_data, GL_STATIC_DRAW);

	static const GLfloat color_buffer_data[] = {
		1.0f, 0.7f, 0.3f,	//color 0 of polygon
		1.0f, 0.7f, 0.3f,	//color 1 of polygon
		1.0f, 0.7f, 0.3f,	//color 2 of polygon
		//1.0f, 0.7f, 0.3f,	//color 3 of polygon
		//1.0f, 0.7f, 0.3f,	//color 4 of polygon
		//1.0f, 0.7f, 0.3f,	//color 5 of polygon
		//1.0f, 0.7f, 0.3f,	//color 6 of polygon
		//1.0f, 0.7f, 0.3f,	//color 7 of polygon
		//1.0f, 0.7f, 0.3f,	//color 8 of polygon
		//1.0f, 0.7f, 0.3f,	//color 9 of polygon
		//1.0f, 0.7f, 0.3f,	//color 10 of polygon
		//1.0f, 0.7f, 0.3f,	//color 11 of polygon
		//1.0f, 0.7f, 0.3f,	//color 12 of polygon
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data),
		color_buffer_data, GL_STATIC_DRAW);
	
	//Use our shader
	glUseProgram(m_programID);

	//Enable depth test
	glEnable(GL_DEPTH_TEST);
}

void Scene2::Update(double dt)
{
	if(Application::IsKeyPressed(VK_SPACE))
	{
		if(rotateL == true)
		{
			rotateAngle += (float)(10 * dt);

			if(rotateAngle >= 90)
			{
				rotateL = false;
			}
		}
		if(rotateL == false)
		{
			rotateAngle -= (float)(10 * dt);

			if(rotateAngle <= -90)
			{
				rotateL = true;
			}
		}

		if(translateX <= 100.0)
		{
			translateX += (float)(10 * dt);
		}
		else
		{
			boundaryX = true;
		}
		if(boundaryX)
		{
			translateX = -100.0;
			boundaryX = false;
		}

		if(scaleAll >= 10)
		{
			scaleAll = 1;
		}
		else
		{
			scaleAll += (float)(2 * dt);
		}
	}
}

void Scene2::Render()
{
	//Clear color buffer ever frame
	//glClear(GL_COLOR_BUFFER_BIT);
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0);	//1st attribute buffer: vertices
	glEnableVertexAttribArray(1);	//2nd attribute buffer: colors

	// Render VBO here
	translate.SetToIdentity();
	rotate.SetToIdentity();
	scale.SetToIdentity();
	model.SetToIdentity();
	view.SetToIdentity();
	projection.SetToOrtho(-worldAxisX, worldAxisX, -worldAxisY, worldAxisY, -worldAxisZ, worldAxisZ);

	//No Translation(Object 1)

	scale.SetToScale(30.0f, 30.0f, 30.0f);
	rotate.SetToRotation(rotateAngle, 0.0f, 0.0f, 1.0f);
	translate.SetToTranslation(0.0f, 0.0f, 0.0f);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	//Draw Call
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0 , 0);
	glDrawArrays(GL_TRIANGLES, 0, 13); //Starting for vertex 0; 3 vertices = 1 triangle

	//No Scaling(Object 2)
	scale.SetToScale(1.0f, 1.0f, 1.0f);
	rotate.SetToRotation(0.0f, 0.0f, 0.0f, 1.0f);
	translate.SetToTranslation(translateX, 70.0f, 1.0f);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	//Draw Call
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0 , 0);
	glDrawArrays(GL_TRIANGLES, 0, 13); //Starting for vertex 0; 3 vertices = 1 triangle

	//No Rotation(Object 3)
	scale.SetToScale(scaleAll, scaleAll, scaleAll);
	rotate.SetToRotation(0.0f, 0.0f, 0.0f, 1.0f);
	translate.SetToTranslation(0.0f, -50.0f, 1.0f);
	model = translate * rotate * scale;
	MVP = projection * view * model;
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);	//Drawing model

	//Draw Call
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0 , 0);
	glDrawArrays(GL_TRIANGLES, 0, 13); //Starting for vertex 0; 3 vertices = 1 triangle

	// Draw Object 1 (No translation)
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);


}

void Scene2::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
