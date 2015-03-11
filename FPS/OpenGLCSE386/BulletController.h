#ifndef ___BULLET_CONTROLLER_H___
#define ___BULLET_CONTROLLER_H___

#include "controller.h"
class BulletController : public Controller
{
public:
	BulletController( vec3 direction, GLfloat spd = 0.1f);
	//~BulletController(void);

	// Update method that is call by the Visual Object.
	virtual void update(float elapsedTimeSeconds);
	
	virtual void fire(vec3 pos, vec3 direction, float speed);


	//virtual void fireBullet( );

	virtual void destoryBullet();

	//virtual void resetBullet();

	vec3 launchPoint;

	vec3 direction;

	vec3 position;

	//int timer;

	float speed;

	bool destoried;

	//vec3 velocity;
};

#endif