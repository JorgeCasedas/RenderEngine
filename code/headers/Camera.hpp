
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

#include <glm/glm.hpp>                          // vec3, vec4, ivec4, mat4
#include <glm/gtc/matrix_transform.hpp>         // translate, rotate, scale, perspective
#include <glm/gtc/type_ptr.hpp>                 // value_ptr
using namespace glm;
class Camera
{

public:

	vec3 cameraDisplacement; //Camera position
	//vec3 cameraRotation;

	vec3 cameraXRotation;
	vec3 cameraYRotation;

	/*glm::vec3 cameraTarget;
	glm::vec3 cameraDirection;

	glm::vec3 up;
	glm::vec3 cameraRight;

	glm::vec3 cameraUp;

	glm::mat4 view;*/

public:

	Camera();

	void Update();
};