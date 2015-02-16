#include "Scene1.h"
#include "GL\glew.h"
#include "Mtx44.h"
#include "shader.hpp"


Scene1::Scene1()
{
}

Scene1::~Scene1()
{
}

void Scene1::Init()
{
	// Init VBO here

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
		-0.3f, 0.1f, 0.0f,	//vertex 0(S.P)
		-0.2f, 0.2f, 0.0f,	//vertex 1
		-0.4f, 0.2f, 0.0f,	//vertex 2(line)
		-0.2f, 0.3f, 0.0f,	//vertex 3
		-0.3f, 0.4f, 0.0f,	//vertex 4
		-0.4f, 0.2f, 0.0f,	//vertex 5(line)
		-0.4f, 0.4f, 0.0f,	//vertex 6
		-0.5f, 0.3f, 0.0f,	//vertex 7
		-0.4f, 0.2f, 0.0f,	//vertex 8(line)
		-0.5f, 0.2f, 0.0f,	//vertex 9
		-0.4f, 0.1f, 0.0f,	//vertex 10
		-0.4f, 0.2f, 0.0f,	//vertex 11(line)
		-0.3f, 0.1f, 0.0f,	//vertex 12(E.P)
	};

	//Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);

	//Transfer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data),vertex_buffer_data, GL_STATIC_DRAW);

	static const GLfloat color_buffer_data[] = {
		1.0f, 0.7f, 0.3f,	//color 0 of polygon
		1.0f, 0.7f, 0.3f,	//color 1 of polygon
		1.0f, 0.7f, 0.3f,	//color 2 of polygon
		1.0f, 0.7f, 0.3f,	//color 3 of polygon
		1.0f, 0.7f, 0.3f,	//color 4 of polygon
		1.0f, 0.7f, 0.3f,	//color 5 of polygon
		1.0f, 0.7f, 0.3f,	//color 6 of polygon
		1.0f, 0.7f, 0.3f,	//color 7 of polygon
		1.0f, 0.7f, 0.3f,	//color 8 of polygon
		1.0f, 0.7f, 0.3f,	//color 9 of polygon
		1.0f, 0.7f, 0.3f,	//color 10 of polygon
		1.0f, 0.7f, 0.3f,	//color 11 of polygon
		1.0f, 0.7f, 0.3f,	//color 12 of polygon
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data),
		color_buffer_data, GL_STATIC_DRAW);

	// 5-sided star
	//An array of 3 vectors which represent 3 vertices
	static const GLfloat vertex_buffer_data2[] = {
		0.2f, 0.1f, 0.0f,	//vertex 0(L.R Point)
		0.15f, 0.3f, 0.0f,	//vertex 1(Inward pull)
		0.1f, 0.2f, 0.0f,	//vertex 2(line)
		0.25f, 0.3f, 0.0f,	//vertex 3(U.R Point)
		0.1f, 0.3f, 0.0f,	//vertex 4(Inward pull)
		0.15f, 0.3f, 0.0f,	//vertex 5(line)
		0.1f, 0.45f, 0.0f,	//vertex 6(T.M Point)
		0.05f, 0.3f, 0.0f,	//vertex 7(Inward pull)
		0.1f, 0.2f, 0.0f,	//vertex 8(line)
		-0.05f, 0.3f, 0.0f,	//vertex 9(U.L Point)
		0.05f, 0.3f, 0.0f,	//vertex 10(Inward pull)
		0.1f, 0.2f, 0.0f,	//vertex 11(line)
		0.0f, 0.1f, 0.0f,	//vertex 12((L.L Point))
	};

	//Set the current active buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);

	//Transfer vertices to OpenGL
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertex_buffer_data2),vertex_buffer_data2, GL_STATIC_DRAW);

	static const GLfloat color_buffer_data2[] = {
		1.0f, 0.7f, 0.3f,	//color 0 of polygon
		1.0f, 0.7f, 0.3f,	//color 1 of polygon
		1.0f, 0.7f, 0.3f,	//color 2 of polygon
		1.0f, 0.7f, 0.3f,	//color 3 of polygon
		1.0f, 0.7f, 0.3f,	//color 4 of polygon
		1.0f, 0.7f, 0.3f,	//color 5 of polygon
		1.0f, 0.7f, 0.3f,	//color 6 of polygon
		1.0f, 0.7f, 0.3f,	//color 7 of polygon
		1.0f, 0.7f, 0.3f,	//color 8 of polygon
		1.0f, 0.7f, 0.3f,	//color 9 of polygon
		1.0f, 0.7f, 0.3f,	//color 10 of polygon
		1.0f, 0.7f, 0.3f,	//color 11 of polygon
		1.0f, 0.7f, 0.3f,	//color 12 of polygon
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_buffer_data2),
		color_buffer_data2, GL_STATIC_DRAW);

	//Load vertex and fragment shaders
	m_programID = LoadShaders("Shader//SimpleVertexShader.vertexshader", "Shader//SimpleFragmentShader.fragmentshader" );
	
	//Use our shader
	glUseProgram(m_programID);

	//Enable depth test
	glEnable(GL_DEPTH_TEST);
}

void Scene1::Update(double dt)
{
}

void Scene1::Render()
{
	// Render VBO here

	//Clear color buffer ever frame
	//glClear(GL_COLOR_BUFFER_BIT);
	//Clear color & depth buffer every frame
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(0);	//1st attribute buffer: vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(
		0,	// attribute 0. Must match the layout in the shader. Usually 0 is for vertex
		3,	// size
		GL_FLOAT,	// type
		GL_FALSE,	//normalized?
		0,	// stride
		0	//array buffer offset
		);

	glEnableVertexAttribArray(1);	//2nd attribute buffer: colors
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_1]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0 , 0);

	// Draw 8-sided polygon
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 13); //Starting for vertex 0; 3 vertices = 1 triangle

	glDisableVertexAttribArray(0);

	glDisableVertexAttribArray(1);


	// Draw 5-sided star
	glEnableVertexAttribArray(0);	//1st attribute buffer: vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(
		0,	// attribute 0. Must match the layout in the shader. Usually 0 is for vertex
		3,	// size
		GL_FLOAT,	// type
		GL_FALSE,	//normalized?
		0,	// stride
		0	//array buffer offset
		);

	glEnableVertexAttribArray(1);	//2nd attribute buffer: colors
	glBindBuffer(GL_ARRAY_BUFFER, m_colorBuffer[GEO_TRIANGLE_2]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0 , 0);

	//Draw the triangle
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 13); //Starting for vertex 0; 3 vertices = 1 triangle

	glDisableVertexAttribArray(0);

	glDisableVertexAttribArray(1);
}

void Scene1::Exit()
{
	// Cleanup VBO here
	glDeleteBuffers(NUM_GEOMETRY, &m_vertexBuffer[0]);
	glDeleteBuffers(NUM_GEOMETRY, &m_colorBuffer[0]);
	glDeleteVertexArrays(1, &m_vertexArrayID);
	glDeleteProgram(m_programID);
}
