#ifndef _M_CAMERA_H_
#define _M_CAMERA_H_

#include "visualobject.h"
#include "camera.h"

class MouseCamera :
	public Camera 
{
public:
	float limitX;
	float limitZ;
	float tempY;
	vec3 mPos;	
	vec3 mView;		
	vec3 mUp;	
	vec3 tempVec;
	MouseCamera(vec3 position,vec3 view, vec3 up);
	virtual void update(float elapsedTimeSeconds);
	void Strafe_Camera(float speed);

	void Mouse_Move(float wndWidth, float wndHeight);
	void Move_Camera(float speed);
	void Rotate_View(float speed);
	bool checkBarrier(float tempX,float tempZ);
	void Mouse_Crouch();
	void Mouse_Jump();
	bool checkLadder();
	bool checkOnBuilding();

	bool onAir,ladder1,ladder2,ladder3,onBuilding,dropping,raising,crouching;
};


#endif