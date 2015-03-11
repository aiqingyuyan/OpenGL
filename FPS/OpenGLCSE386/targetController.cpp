#include "targetController.h"


targetController::targetController(vec3 pos, int face )
	:position(pos)
{
	destroyed = false;
	facing = face;
	detect = false;
	degree = -90;
	moving = 0;
	flipped = false;
}

bool targetController::Collided(vec3 input){
	/*if(glm::distance( input, position )<2.0f)
		cout<<glm::distance( input, position )<<endl;*/
	return (glm::distance( input, position )<=1.5f);
}

bool targetController::Impacted(vec3 input){
	return (glm::distance( input, position )<=2.5f);
}

void targetController::update(float elapsedTimeSeconds)
{
	target->position = position;
	if(facing == 1){				
		if(!detect)
			target->modelMatrix = translate(mat4(1.0f), position)  * rotate(mat4(1.0f), 90.0f, vec3(0.0f, 1.0f, 0.0f))*rotate(mat4(1.0f), -90.0f, vec3(1.0f, 0.0f, 0.0f));
		else if(detect && degree<=0){
			target->modelMatrix = translate(mat4(1.0f), position)  * rotate(mat4(1.0f), 90.0f, vec3(0.0f, 1.0f, 0.0f))* rotate(mat4(1.0f), degree, vec3(1.0f, 0.0f, 0.0f));
			degree +=4;
		} else {
			flipped = true;
		}
	}else if(facing == 2){
		if(!detect)
			target->modelMatrix = translate(mat4(1.0f), position);
		else if(detect&&degree<=-45.0f){
			position = vec3(position.x,position.y,position.z+0.2);
			target->modelMatrix = translate(mat4(1.0f), position) * rotate(mat4(1.0f), degree, vec3(0.0f, 1.0f, 0.0f));
			degree +=5;
		} else {
			flipped = true;
		}
	}else if(facing == 3){
		if(!detect)
			target->modelMatrix = translate(mat4(1.0f), position)  * rotate(mat4(1.0f), -90.0f, vec3(1.0f, 0.0f, 0.0f));
		else if(detect && degree<=0){
			target->modelMatrix = translate(mat4(1.0f), position)  * rotate(mat4(1.0f), degree, vec3(1.0f, 0.0f, 0.0f));
			degree +=4;
		} else {
			flipped = true;
		}
	}else {
		if(!detect)
			target->modelMatrix = translate(mat4(1.0f), position);
		else if(detect&&degree<=45.0f){
			position = vec3(position.x-0.08f,position.y,position.z);
			target->modelMatrix = translate(mat4(1.0f), position) * rotate(mat4(1.0f), degree, vec3(0.0f, 1.0f, 0.0f));
			degree +=5;
		} else {
			flipped = true;
		}
	}
}


void targetController::flip(){
	detect = true;
}