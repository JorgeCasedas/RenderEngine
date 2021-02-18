
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

//#include <SFML/OpenGL.hpp>
//#include<SFML/Graphics.hpp>
//#include<SFML/Graphics/Image.hpp>

//#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
//#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
//#include <glm/gtc/type_ptr.hpp>                 // value_ptr
//#include <vector>
#include "Model.hpp"

using namespace std;
using namespace sf;
using namespace example;
#pragma once
class ElevationMesh
{
	enum
	{
		COORDINATES_VBO,
		TEXTURE_VBO,
		COLORS_VBO,
		NORMALS_VBO,
		INDICES_IBO,
		VBO_COUNT
	};

	
	GLuint vbo_ids[VBO_COUNT];
	GLuint vao_id;

	vector<GLuint> indices;
	vector<GLfloat> verts, colors, norms;

	Image heightMap;

	glm::vec3 objectPosition;
	glm::vec3 objectScale;
	
public :
	ElevationMesh();
	ElevationMesh(int widthResolution, int lengthResolution);

	void render(GLint hasTextue_id);

private:
	void genTerrain(int wRes , int lRes );
	Color getPixel(Image surface, int x, int y);

};