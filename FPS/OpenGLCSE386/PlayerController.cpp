#include "PlayerController.h"
#include "Sphere.h"

PlayerController::PlayerController( vec3 pos, GLfloat spd) : position( pos ), last_position(pos)
{
	speed = spd;

	//this->floor_border = floor_sideLength / 2;
	rotation = 0;

	up = vec3(0, 1, 0);

	gun_direction = vec3(0, 0, -1);

	left_vector = vec3(-1.0, 0.0f, 0.0f);
	gunRotating = false;

	//velocity = vec3(.0f, .0f, -1.0f) * spd;
}

//Used to get MouseCamera coordinates
void PlayerController::inputPosition(vec3 position, vec3 center){
	this->position = position;
	this->center = center;
}

void PlayerController::update( float elapsedTimeSeconds )
{
	target->position = position;
	target->modelMatrix =  glm::inverse(glm::lookAt( position ,center,up));
	target->fixedTransformation = translate(mat4(1.0f),vec3(1.5f,-1.0f, -4.0f))*rotate(mat4(1.0f), 180.0f, vec3(0,1.0f,0));
}

void PlayerController::rotate_view( float angle )
{
	//target->modelMatrix = rotate( mat4(1.0f), angle, up );
	rotation += angle;

	gun_direction = ( vec4(gun_direction, 0) * rotate(mat4(1.0), -angle, up) ).xyz;
	gun_direction = glm::normalize( gun_direction );

	left_vector = glm::cross(up, gun_direction);
	left_vector = glm::normalize(left_vector);
}

void PlayerController::move_forward()
{
	//position.z -= .1;
	position = position + gun_direction * .1f;
}

void PlayerController::move_backward()
{
	//position.z += .1;
	position = position - gun_direction * .1f;
}

void PlayerController::move_right()
{
	//position.x += .1;
	position = position - left_vector * .1f;
}

void PlayerController::move_left()
{
	//position.x -= .1;
	position = position + left_vector * .1f;
}