#pragma once
#include "controller.h"
class targetController :
	public Controller
{
public:

	targetController( vec3 pos ,  int face);

	// Update method that is call by the Visual Object.
	virtual void update(float elapsedTimeSeconds);
	virtual void flip();
	virtual bool Collided(vec3 input);
	virtual bool Impacted(vec3 input);
	public:

	vec3 position;

	float radius;
	bool destroyed;
	bool detect;
	bool flipped;
	float moving;
	float degree;
	int facing;
};

