
/**********************************************************************
*Project           : Enviroment Render
*
*Editor : Jorge Cásedas
*
*Starting date : 21/05/2020
*
*Ending date : 24/05/2020
*
*Purpose : Rendering an enviroment with Opengl
*
**********************************************************************/

#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>
#include <glm/glm.hpp> 
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective

using namespace std;

class FrameBuffer
{

public:
	FrameBuffer();
	void CreateFrameBuffer();
	void CreateTexture();
	void CreateRenderObject();
	void CreateQuad();
	void ConfigurateFrameBuffer();


	void BeforeDrawingScene();
	void AfterDrawingScene(GLuint shaderProgram);

public:

	unsigned int framebuffer;
	unsigned int texColorBuffer;
	unsigned int rbo;

	enum
	{
		COORDINATES_VBO,
		VBO_COUNT
	};


	GLuint vbo_ids[VBO_COUNT];      // Ids de los VBOs que se usan
	GLuint vao_id;

	const static string quad_vertex_shader_code;
	const static string quad_fragment_shader_code;
};