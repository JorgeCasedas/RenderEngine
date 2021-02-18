
/**********************************************************************
*Project           : Enviroment Render
*
*Author : Jorge Cásedas
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

class Skybox 
{
private:

	unsigned int textureID;
	vector<string> faces;

public:

    enum
    {
        COORDINATES_VBO,
        VBO_COUNT
    };


    GLuint vbo_ids[VBO_COUNT];      // Ids de los VBOs que se usan
    GLuint vao_id;                  // Id del VAO del cubo

	static const string sky_vertex_shader_code;
	static const string sky_fragment_shader_code;

	GLint  model_view_matrix_id;
	GLint  projection_matrix_id;

	float angleX;
	float angleY;

	glm::vec3 cameraXRotation;
	glm::vec3 cameraYRotation;

private:

	void LoadSkybox();
public:
	Skybox();
	void render(int width, int height);
};