
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


#include "Camera.hpp"

Camera::Camera()
{
	cameraDisplacement = glm::vec3(-50.f, -20.f, -140.f);
	//cameraRotation = glm::vec3(1.f, 1.f, 1.f);
	cameraXRotation = glm::vec3(1.f, 0.f, 0.f);
	cameraYRotation = glm::vec3(0.f, 1.f, 0.f);
	
	//cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	//up = glm::vec3(0.0f, 1.0f, 0.0f);
}

void Camera::Update()
{
	/*cameraDirection = glm::normalize(cameraDisplacement - cameraTarget);
	cameraRight = glm::normalize(glm::cross(up, cameraDirection));
	cameraUp = glm::cross(cameraDirection, cameraRight);

	view = glm::lookAt(glm::vec3(cameraRotation.x, 0.0, cameraRotation.z), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));*/
}
