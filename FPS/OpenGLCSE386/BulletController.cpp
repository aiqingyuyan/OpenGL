#include "BulletController.h"


BulletController::BulletController(  vec3 direction, GLfloat spd ) : direction(direction)
{
	speed = spd;
	destoried = false;
	//timer = 0;
}

void BulletController::update( float elapsedTimeSeconds )
{
	if(!destoried)
	{
		target->position += direction * speed * elapsedTimeSeconds;

		target->modelMatrix = translate( mat4(1.0f), target->position );

		//cout<<timer<<endl;

		if( glm::distance(target->position, vec3(0,0,0)) >= 100 )
		{
			destoried = true;
			//delete target;
			//cout<<"Bullet being destoried"<<endl;
		}
	}
}

void BulletController::destoryBullet()
{

}

void BulletController::fire(vec3 pos, vec3 dir, float spd){
		target->position = pos;
		this->direction = dir;
		this->speed = spd;
		target->modelMatrix = translate( mat4(1.0f), target->position );

				//ullet->position = model->position + glm::normalize(MCamera->mView - MCamera->mPos) * 5.0f;
			//bullet->modelMatrix = translate( mat4(1.0f), bullet->position );

}
