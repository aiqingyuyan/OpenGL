#ifndef _PLAYER_CONTROLLER_H
#define _PLAYER_CONTROLLER_H

#include "controller.h"

#define G -9.80

class PlayerController : public Controller
{
public:
	PlayerController( vec3 pos = vec3(0), GLfloat spd = 1.0f);
	//~PlayerController(void);

	// Update method that is call by the Visual Object.
	virtual void update(float elapsedTimeSeconds);

	// Move Control
	virtual void move_forward();
	virtual void move_backward();
	virtual void move_right();
	virtual void move_left();

	virtual void inputPosition(vec3 position, vec3 center);
	vec3 center;
	// Fire Control
	virtual void fire() {};

	// View Rotate
	virtual void rotate_view( float angle );

	//virtual void updateVelocity_Y();

	virtual vec3 getPosition() {return position;}

	//virtual vec3 getVelocity() {return velocity;}

	virtual GLfloat getSpeed() {return speed;}

	//virtual vec3 getGunDirection( ) {return gun_direction; };

		bool gunRotating;
protected:

	vec3 position;

	vec3 last_position;

	//vec3 velocity;
	float rotation;

	vec3 up;

	vec3 gun_direction;

	vec3 left_vector;

	GLfloat speed;



	vec3 mPos;	
	vec3 mView;		
	vec3 mUp;	
};

#endif
