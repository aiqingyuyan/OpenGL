#include "MouseCamera.h"


MouseCamera::MouseCamera(vec3 position, vec3 view, vec3 up)
{
	mPos	= position; // set position
	mView	= view; // set view
	mUp		= up; // set the up vector	
	limitX = 24.5f;
	limitZ = 49.5f;
	onAir = false;
	ladder1 = false;
	ladder2 = false;
	ladder3 = false;
	onBuilding =false;
	dropping = false;
	crouching = false;
	raising = false;
	tempVec.y = 2.5f;
}

void MouseCamera::update(float elapsedTimeSeconds)
{	
	if(dropping){
		if(mPos.y>tempVec.y){
			mPos.y -= 0.3f;
			mView.y -= 0.3f;
		} else{
			dropping = false;
			onAir = false;
		}
	}

	if(raising){
		if(mPos.y<tempVec.y){
			mPos.y += 0.1f;
			mView.y += 0.1f;
		} else{
			raising = false;
			crouching = false;
		}
	} 

	if(crouching){
		if(mPos.y>tempVec.y-1.0f){
			mPos.y -=0.1f;
			mView.y -=0.1f;
		} else {
			crouching = false;
			raising = true;
		}
	} else if(onAir){
		if(mPos.y<tempVec.y+2.5f){
			mPos.y +=0.2f;
			mView.y +=0.2f;
		} else {
			onAir = false;
			dropping = true;
		}
	} else if(!onBuilding&&mPos.y>2.6f&&!ladder1&&!ladder2&&!ladder3){
		tempVec.y=2.5f;
		dropping = true;
		//onAir = true;
	} else if(mPos.y<2.5f){
		mPos.y +=0.1f;
		mView.y +=0.1f;
	} else if((tempVec.y-mPos.y)<0.5f&&(tempVec.y-mPos.y)>-0.5f)
		mPos.y = tempVec.y;


	//cout<<"Ladder 1 :"<<ladder1<<" onAir: "<<onAir<<" Dropping "<<dropping<<" onBuilding: "<<onBuilding<<" Crouching: "<<crouching<<" raising: "<<raising<<endl;
	Camera::update(elapsedTimeSeconds);
}

void MouseCamera::Move_Camera(float speed)
{
	vec3 vVector = mView - mPos;	// Get the view vector
	float tempX = mPos.x  + vVector.x * speed;
	float tempZ = mPos.z  + vVector.z * speed;
	float tempY = mPos.y  + vVector.y * speed;

	if(ladder1 || ladder2 || ladder3){
		mPos.y  = mPos.y  + vVector.y * speed;
		mView.y = mView.y + vVector.y * speed;
	} else if(checkBarrier(tempX,tempZ)){
		// forward positive cameraspeed and backward negative -cameraspeed.
		mPos.x  = mPos.x  + vVector.x * speed;
		mPos.z  = mPos.z  + vVector.z * speed;
		mView.x = mView.x + vVector.x * speed;
		mView.z = mView.z + vVector.z * speed;
	}
}


void MouseCamera::Mouse_Crouch(){
	if(!onAir&&!crouching&&!raising&&!dropping&&!ladder1&&!ladder2&&!ladder3){
		crouching = true;
		tempVec.y = mPos.y;
	}
}

void MouseCamera::Mouse_Jump(){
	if(!dropping&&!ladder1&&!ladder2&&!ladder3&&!onAir&&!raising&&!crouching){
		onAir = true;
		tempVec.y = mPos.y;
	}
}

bool MouseCamera::checkLadder(){
	int ladderCount = 0;
	if(mPos.x<-19.0f&&mPos.x>-21.0f
		&&mPos.z>47.5f&&mPos.z<48.2f
		&&mView.x<-15.0f&&mView.x>-16.0f
		&&mView.z>47.5f&&mView.z<48.2f&&mPos.y<27.0f){
			//cout<<"Climbing Ladder 1"<<endl;
			ladder1=true;
			ladderCount++;
	} else 
		ladder1=false;

	if(mPos.x>7.5f&&mPos.x<8.5f
		&&mPos.z>40.0f&&mPos.z<41.2f
		&&mView.x>7.5f&&mView.x<8.5f
		&&mView.z>35.0f&&mView.z<36.2f&&mPos.y<12.0f){
			//cout<<"Climbing Ladder 2"<<endl;
			ladder2=true;
			ladderCount++;
	} else 
		ladder2=false;

	if(mPos.x>2.0f&&mPos.x<3.0f
		&&mPos.z>-25.0f&&mPos.z<-23.8f
		&&mView.x>1.8f&&mView.x<3.0f
		&&mView.z>-30.0f&&mView.z<-28.8f&&mPos.y<12.0f){
			//cout<<"Climbing Ladder 3"<<endl;
			ladder3=true;
			ladderCount++;
	} else 
		ladder3=false;


	return ladderCount;
}

bool MouseCamera::checkOnBuilding(){
	onBuilding = false;
	if((mPos.x>-21.5f&&mPos.x<-3.5f&&mPos.z>38.5f&&mPos.z<limitZ&&mPos.y>=27.0f&&mPos.y<29.0f)){
		onBuilding = true;tempVec.y = 27.0f;
	}
	if((mPos.x>4.0f&&mPos.x<limitX&&mPos.z>-26.5f&&mPos.z<41.5f&&mPos.y>=12.0f&&mPos.y<15.0f)){
		onBuilding = true;tempVec.y = 12.0f;
	}
	if((mPos.x>-1.5f&&mPos.x<6.5f&&mPos.z>-31.5f&&mPos.z<-23.5f&&mPos.y>=12.0f&&mPos.y<15.0f)){
		onBuilding = true;tempVec.y = 12.0f;
	}
	return onBuilding;
}

bool MouseCamera::checkBarrier(float tempX,float tempZ) {
	bool result = true;
	if(!(tempX>-limitX&&tempX<limitX&&tempZ>-limitZ&&tempZ<limitZ)){
		result = false;
	}

	if(!onBuilding){
		//Building 0
		if((tempX>-20.5f&&tempX<-4.5f&&tempZ>39.5f&&tempZ<limitZ)){
			result = false;
		}
		//Building 1
		if((tempX>-20.5f&&tempX<-4.5f&&tempZ>-25.5f&&tempZ<25.5f)){
			result = false;
		}

		//Building 2
		if((tempX>-20.5f&&tempX<limitX&&tempZ>-limitZ&&tempZ<-39.5f)){
			result = false;
		}

		//Building 3
		if((tempX>-8.0f&&tempX<-2.0f&&tempZ>-38.0f&&tempZ<-32.0f)){
			result = false;
		}

		//Building 4
		if((tempX>-0.5f&&tempX<5.5f&&tempZ>-30.5f&&tempZ<-24.5f)){
			result = false;
		}

		//Building 5
		if((tempX>4.5f&&tempX<limitX&&tempZ>-25.5f&&tempZ<40.5f)){
			result = false;
		}
	}
	return result;
}


void MouseCamera::Rotate_View(float speed)
{
	vec3 vVector = mView - mPos;	// Get the view vector

	mView.z = (float)(mPos.z + sin(speed)*vVector.x + cos(speed)*vVector.z);
	mView.x = (float)(mPos.x + cos(speed)*vVector.x - sin(speed)*vVector.z);
}

void MouseCamera::Strafe_Camera(float speed)
{

	vec3 vVector = mView - mPos;	// Get the view vector
	vec3 vOrthoVector;              // Orthogonal vector for the view vector

	vOrthoVector.x = -vVector.z;
	vOrthoVector.z =  vVector.x;

	// left positive cameraspeed and right negative -cameraspeed.
	float tempX = mPos.x  + vOrthoVector.x * speed;
	float tempZ = mPos.z  + vOrthoVector.z * speed;
	if(checkBarrier(tempX,tempZ)){
		mPos.x  = mPos.x  + vOrthoVector.x * speed;
		mPos.z  = mPos.z  + vOrthoVector.z * speed;
		mView.x = mView.x + vOrthoVector.x * speed;
		mView.z = mView.z + vOrthoVector.z * speed;
	}
}

void MouseCamera::Mouse_Move(float wndWidth, float wndHeight)
{
	POINT mousePos;	
	float mid_x = wndWidth/2.0;	
	float mid_y = wndHeight/2.0;	
	float angle_y  = 0.0f;				
	float angle_z  = 0.0f;							

	GetCursorPos(&mousePos);	// Get the 2D mouse cursor (x,y) position					

	if( (mousePos.x == mid_x) && (mousePos.y == mid_y) ) return;

	SetCursorPos(mid_x, mid_y);	// Set the mouse cursor in the center of the window						
	//glutWarpPointer(mid_x, mid_y);

	// Get the direction from the mouse cursor, set a resonable maneuvering speed
	angle_y = (float)( (mid_x - mousePos.x) ) / 1000;		
	angle_z = (float)( (mid_y - mousePos.y) ) / 1000;

	// The higher the value is the faster the camera looks around.
	mView.y += angle_z * 3;

	// limit the rotation around the x-axis
	if((mView.y - mPos.y) > 10)  mView.y = mPos.y + 10;
	if((mView.y - mPos.y) <-10)  mView.y = mPos.y - 10;

	Rotate_View(-angle_y); // Rotate
}
