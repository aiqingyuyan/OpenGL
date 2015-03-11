#ifndef _FPS_CAMERA_H_
#define _FPS_CAMERA_H_
#include "camera.h"
#include "PlayerController.h"

class FPS_Camera : public Camera
{
public:
	FPS_Camera(vec3 p = vec3(0.0f, 0.0f, 12.0f), vec3 d = vec3(0.0f, 0.0f, -1.0f), vec3 u = vec3(0.0f, 1.0f, 0.0f));
	//~FPS_Camera(void);

	virtual void update(float elapsedTimeSeconds);

	virtual void update_direction(float angle);

	PlayerController *playerCont;

	vec3 left_vector;
};

#endif