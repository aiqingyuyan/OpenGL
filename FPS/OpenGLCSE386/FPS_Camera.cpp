#include "FPS_Camera.h"


FPS_Camera::FPS_Camera( vec3 p , vec3 d , vec3 u ): Camera(p, d, u)
{
	this->playerCont = NULL;
	left_vector = vec3(-1, 0, 0);
}

void FPS_Camera::update(float elapsedTimeSeconds)
{
	if(playerCont != NULL)
	{
		viewPosition = playerCont->getPosition() + left_vector * 0.3f + viewUp * 1.3f + viewDirection * -3.5f;
	}

	Camera::update(elapsedTimeSeconds);
}

void FPS_Camera::update_direction(float angle)
{
	viewDirection = ( vec4(viewDirection, 0) * rotate(mat4(1.0), angle, viewUp) ).xyz;
	viewDirection = glm::normalize( viewDirection );

	left_vector = glm::cross(viewUp, viewDirection);
	left_vector = glm::normalize( left_vector );
}