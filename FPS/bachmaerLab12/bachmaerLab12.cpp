//#include <math.h>
//#define GLM_MESSAGES
//#define GLM_FORCE_RADIANS

#define GLM_SWIZZLE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/swizzle.hpp>
#include <string>
using namespace glm;

#include "OpenGLApplicationBase.h"
#include <time.h>
#include <stdio.h>
#include "Sphere.h"
#include "Cylinder.h"
#include "Torus.h"
#include "Cone.h"
#include "Floor2.h"
#include "Wall.h"
#include "target.h"
#include "SpaceShip.h"
#include "Cube.h"
#include "ladder.h"
#include "SoundSource.h"

#include "Spinner.h"
#include "OrbitController.h"
#include "WaypointController2.h"
#include "targetController.h"
#include "BulletController.h"

#include "MouseCamera.h"
#include "FollowCamera.h"
#include "FPS_Camera.h"

#include "SharedGeneralLighting.h"
#include "SharedProjectionAndViewing.h"

#include "Framework.h"

#include "objLoader.h"
#include "Model.h"
#include "MouseCamera.h"


//LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc
#define CAMERASPEED	0.05f //foward / backward speed
#define SIDESPEED 0.03f //side ways speed

struct Target {
	Target(string tN, vec3 tL, int a){
		targetName = tN;
		targetLoc = tL;
		actionNum = a;
	}
	string targetName;
	vec3 targetLoc;
	int actionNum;
};

void print(vec3 v) {

	cout << "[ " << v.x << " " << v.y << " " << v.z << " ]" << endl;
}

class bachmaerLab12 : public OpenGLApplicationBase
{
public:
	Wall * wall, * wallNorth,* wallSouth, * wallEast, * wallWest, * floor, *sky; 
	vector< VisualObject* > bullets_fired;
	vector< VisualObject* > bullets_left;

	vector<Target*> targetInfo;
	vector<Target*> InfoGroup1;
	vector<Target*> InfoGroup2;
	vector<Target*> InfoGroup3;
	vector<VisualObject *> targets;
	VisualObject* target0;
	VisualObject * ladder0, * ladder1,* ladder2;
	VisualObject * building0,* building1,*building2,* building3,* building4,* building5, *model;
	VisualObject root;
	PlayerController * playerCont;
	targetController * tCont;
	vector<int> BulletID;

	bool Start,isForward,isBackward, isWalking;

	int winX,winY, currentScore;

	GLfloat rotationX, rotationY, viewTranslate;
	MouseCamera * MCamera;
	SharedGeneralLighting generalLighting;
	SharedProjectionAndViewing projectionAndViewing;

	Obj_Loader * loader;

	FollowCamera * followCamera;
	FPS_Camera * fpsCamera;

	SoundSource * walkSound;
	SoundSource * bgSound;
	SoundSource * fireSound;
	SoundSource * reloadSound;
	SoundSource * targetImpactSound;
	SoundSource * targetAppearSound;
	SoundSource * targetDieSound;
	ShaderInfo *shaders;
	int fired_bullets;
	int removed_bullets;
	bool clip_empty;
	/*bool click1;
	bool click2;
	
	float click1Time;
	float click2Time;*/
	clock_t t1, t2;
	int numTargets;
	vector<int> targets_being_hit;
	bool playF,isReloading, gameFinish;
	bachmaerLab12()
	{
		gameFinish = false;
		currentScore = 0;
		numTargets = 0;
		isReloading = false;
		playF = false;/*
		click1 = false;
		click2 = false;
		click1Time = 0;
		click2Time = 0;*/
		fired_bullets = 0;
		removed_bullets = 0;
		t1 = clock();

		ShaderInfo shaders_new[] = {
			{ GL_VERTEX_SHADER, "vfixed-function-shared-proj-view.vert" },
			{ GL_FRAGMENT_SHADER, "pfixed-function-shared-lighting-phong.frag"},
			{ GL_NONE, NULL } // signals that there are no more shaders
		};

		shaders = new ShaderInfo[3];
		shaders[0].type = shaders_new[0].type;
		shaders[0].filename = shaders_new[0].filename;
		shaders[0].shader = shaders_new[0].shader;

		shaders[1].type = shaders_new[1].type;
		shaders[1].filename = shaders_new[1].filename;
		shaders[1].shader = shaders_new[1].shader;

		shaders[2].type = shaders_new[2].type;
		shaders[2].filename = shaders_new[2].filename;
		shaders[2].shader = shaders_new[2].shader;



		// Read the files and create the OpenGL shader program.
		GLuint shaderProgram = setUpShaderProgram(shaders);

		setUpLighting();

		// *** Sound ***
		walkSound = new SoundSource("normalwalk.wav", this);
		walkSound->setRelative(false);
		walkSound->setVolume(0.8f);
		root.addChild(walkSound);

		bgSound = new SoundSource("bg.wav", this);
		bgSound->setRelative(false);
		bgSound->setVolume(0.5f);
		root.addChild(bgSound);

		fireSound = new SoundSource("fire.wav", this);
		fireSound->setRelative(false);
		fireSound->setVolume(0.1f);
		root.addChild(fireSound);

		reloadSound = new SoundSource("reload.wav", this);
		reloadSound->setRelative(false);
		reloadSound->setVolume(0.8f);
		root.addChild(reloadSound);

		targetImpactSound = new SoundSource("target_impact.wav", this);
		targetImpactSound->setLooping(false);
		targetImpactSound->setRelative(false);
		reloadSound->setVolume(0.2f);
		root.addChild(targetImpactSound);

		targetAppearSound = new SoundSource("target_appear.wav", this);
		targetAppearSound->setRelative(false);
		targetAppearSound->setLooping(false);
		targetAppearSound->setVolume(0.8f);
		root.addChild(targetAppearSound);

		targetDieSound = new SoundSource("target_die.wav", this);
		targetDieSound->setLooping(false);
		targetDieSound->setRelative(false);
		targetDieSound->setVolume(0.2f);
		root.addChild(targetDieSound);




		// *** Uniform Blocks *** 
		shaderProgram = setUpShaderProgram(shaders);


		GLfloat wallHeight = 30.0f, wallWidth = 50.0f, rows = 2, columns = 1;
		//North wall
		wallNorth = new Wall(this, "northWall.bmp", wallHeight, wallWidth,rows,columns);
		wallNorth->fixedTransformation = translate(mat4(1.0f), vec3(0.0f, 0.0f, -50.0f));
		wallNorth->setShader(shaderProgram);
		root.addChild(wallNorth);

		//South wall
		wallHeight = 30.0f, wallWidth = 50.0f, rows = 2, columns = 2;
		wallSouth = new Wall(this, "southWall.bmp", wallHeight, wallWidth,rows,columns);
		wallSouth->fixedTransformation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 50.0f)) * rotate(mat4(1.0f), -180.0f, vec3(0,1,0)); ;
		wallSouth->setShader(shaderProgram);
		root.addChild(wallSouth);

		//East wall
		wallHeight = 30.0f, wallWidth = 100.0f, rows = 4, columns = 3;
		wallEast = new Wall(this, "eastWall.bmp", wallHeight, wallWidth,rows,columns);
		wallEast->fixedTransformation = translate(mat4(1.0f), vec3(25.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), -90.0f, vec3(0,1,0)); 
		wallEast->setShader(shaderProgram);
		root.addChild(wallEast);

		//West wall
		wallWest = new Wall(this, "eastWall.bmp", wallHeight, wallWidth,rows,columns);
		wallWest->fixedTransformation = translate(mat4(1.0f), vec3(-25.0f, 0.0f, 0.0f)) * rotate(mat4(1.0f), 90.0f, vec3(0,1,0)); 
		wallWest->setShader(shaderProgram);
		root.addChild(wallWest);
		/*
		//Old floor
		floor = new Floor2(this, 100);
		floor->setShader(shaderProgram);
		root.addChild( floor );*/
		wallHeight = 100.0f, wallWidth = 50.0f, rows = 20, columns = 10;
		floor = new Wall(this, "floor.bmp", wallHeight, wallWidth,rows,columns);
		floor->fixedTransformation = translate(mat4(1.0f), vec3(0.0f, 0.0f, 50.0f)) * rotate(mat4(1.0f), -90.0f, vec3(1,0,0)); 
		floor->setShader(shaderProgram);
		root.addChild( floor );

		wallHeight = 100.0f, wallWidth = 80.0f, rows = 1, columns = 1;
		sky = new Wall(this, "sky.bmp", wallHeight, wallWidth,rows,columns);
		sky->fixedTransformation = translate(mat4(1.0f), vec3(0.0f, 30.0f,- wallHeight/2.0f)) * rotate(mat4(1.0f), 90.0f, vec3(1,0,0)); 
		sky->setShader(shaderProgram);
		root.addChild( sky );

		GLfloat w = 15.0f, h = 25.0f, d = 10.0f;
		rows = 3, columns = 2;
		//building0
		building0 = new cube(this, w, h, d, vec4( 0.2f, 0.2, 0.5f, 1.0f), rows, columns );
		building0->fixedTransformation = translate(mat4(1.0f), vec3(-12.5f, h/2.0f, 45.0f));
		building0->setTextureName("building0.bmp");
		building0->setShader(shaderProgram);
		root.addChild(building0);

		//building1
		w = 15.0f, h = 12.5f, d = 50.0f;
		rows = 1, columns = 2;
		building1 = new cube(this, w, h, d, vec4( 0.2f, 0.2, 0.5f, 1.0f), rows, columns );
		building1->fixedTransformation = translate(mat4(1.0f), vec3(-12.5f, h/2.0f, 0.0f));
		building1->setTextureName("building1.bmp");
		building1->setShader(shaderProgram);
		root.addChild(building1);

		//building2
		w = 45.0f, h = 6.25f, d = 10.0f;
		rows = 1, columns = 1;
		building2 = new cube(this, w, h, d, vec4( 0.2f, 0.2, 0.5f, 1.0f), rows, columns );
		building2->fixedTransformation = translate(mat4(1.0f), vec3(2.5f, h/2.0f, -45.0f));
		building2->setTextureName("building2.bmp");
		building2->setShader(shaderProgram);
		root.addChild(building2);

		//building3
		w = 5.0f, h = 8.0f, d = 5.0f;
		rows = 2, columns = 1;
		building3 = new cube(this, w, h, d, vec4( 0.2f, 0.2, 0.5f, 1.0f), rows, columns );
		building3->fixedTransformation = translate(mat4(1.0f), vec3(-5.0f, h/2.0f, -35.0f));
		building3->setTextureName("building3.bmp");
		building3->setShader(shaderProgram);
		root.addChild(building3);

		//building4
		w = 5.0f, h = 10.0f, d = 5.0f;
		rows = 2, columns = 1;
		building4 = new cube(this, w, h, d, vec4( 0.2f, 0.2, 0.5f, 1.0f), rows, columns );
		building4->fixedTransformation = translate(mat4(1.0f), vec3(2.5f, h/2.0f, -27.5f));
		building4->setTextureName("building4.bmp");
		building4->setShader(shaderProgram);
		root.addChild(building4);

		//building5
		w = 20.0f, h = 10.0f, d = 65.0f;
		rows = 2, columns = 10;
		building5 = new cube(this, w, h, d, vec4( 0.2f, 0.2, 0.5f, 1.0f), rows, columns );
		building5->fixedTransformation = translate(mat4(1.0f), vec3(15.0f, h/2.0f, 7.5f));
		building5->setTextureName("building5.bmp");
		building5->setShader(shaderProgram);
		root.addChild(building5);

		// Model - MP7
		loader = new Obj_Loader("m4.obj");
		loader->read_obj_file();

		model = new Model(this, loader);
		model->setShader(shaderProgram);
		model->position = vec3(.0f, -1.0f, 10.0f);
		model->modelMatrix = translate(mat4(1.0f), model->position);
		model->fixedTransformation = rotate(mat4(1.0f), -180.0f, vec3(0.0f, 1.0f, 0.0f)) * translate(mat4(1.0f), vec3(-0.6f, 0.0f, 0.0f)); 
		playerCont = new PlayerController(model->position);
		model->addController( playerCont );
		//root.addChild(model);

		MCamera = new MouseCamera(vec3(-22.76f, 2.5f, 49.0f),vec3(-22.713f, 2.522f, 44.0693f),vec3(   0.0f, 1.0f, 0.0f));
		MCamera->addChild(model);
		root.addChild(MCamera);

		iniTargetName();
		initGroup();

		for(int i = 0; i<targetInfo.size(); i++){
			target0 = new target(this,targetInfo[i]->targetName);
			target0->setShader(shaderProgram);
			tCont = new targetController(targetInfo[i]->targetLoc, targetInfo[i]->actionNum);
			target0->addController( tCont );
			targets.push_back(target0);
			root.addChild(target0);
		}

		for(int i = 0; i<InfoGroup1.size(); i++){
			target0 = new target(this,InfoGroup1[i]->targetName);
			target0->setShader(shaderProgram);
			tCont = new targetController(InfoGroup1[i]->targetLoc, InfoGroup1[i]->actionNum);
			target0->addController( tCont );
			targets.push_back(target0);
			root.addChild(target0);
		}

		for(int i = 0; i<InfoGroup2.size(); i++){
			target0 = new target(this,InfoGroup2[i]->targetName);
			target0->setShader(shaderProgram);
			tCont = new targetController(InfoGroup2[i]->targetLoc, InfoGroup2[i]->actionNum);
			target0->addController( tCont );
			targets.push_back(target0);
			root.addChild(target0);
		}

		for(int i = 0; i<InfoGroup3.size(); i++){
			target0 = new target(this,InfoGroup3[i]->targetName);
			target0->setShader(shaderProgram);
			tCont = new targetController(InfoGroup3[i]->targetLoc, InfoGroup3[i]->actionNum);
			target0->addController( tCont );
			targets.push_back(target0);
			root.addChild(target0);
		}


		ladder0 = new ladder(this,"wood.bmp", 25.0f);
		ladder0->setShader(shaderProgram);
		ladder0->modelMatrix = translate(mat4(1.0f),vec3(-20.0f, 0.0f, 48.0f)) * rotate(mat4(1.0f), 90.0f, vec3(0.0f, 1.0f, 0.0f))  ;
		//target1->addController( tCont1 );
		root.addChild(ladder0);

		ladder1 = new ladder(this,"wood.bmp", 10.0f);
		ladder1->setShader(shaderProgram);
		ladder1->modelMatrix = translate(mat4(1.0f),vec3(8.0f, 0.0f, 40.0f));
		//target1->addController( tCont1 );
		root.addChild(ladder1);

		ladder2 = new ladder(this,"wood.bmp", 10.0f);
		ladder2->setShader(shaderProgram);
		ladder2->modelMatrix = translate(mat4(1.0f),vec3(2.5f, 0.0f, -25.0f));
		//target1->addController( tCont1 );
		root.addChild(ladder2);

		//Bullets:
		for(int i = 0; i < 30; i++)
		{
			VisualObject *temp = new Sphere(this, 0.06);
			temp->setShader(shaderProgram);
			BulletController* bullC = new BulletController( glm::normalize(MCamera->mView - MCamera->mPos), 1.0f );
			temp->addController(bullC);
			bullets_left.push_back( temp );
		}
		clip_empty = false;

	} // end constructor


	~bachmaerLab12()
	{

	} // end destructor


	// Read the files and create the OpenGL shader program.
	GLuint setUpShaderProgram( ShaderInfo shaders[] ) {

		GLuint shaderProgram = LoadShaders(shaders);

		generalLighting.setUniformBlockForShader( shaderProgram );
		projectionAndViewing.setUniformBlockForShader(shaderProgram);

		return shaderProgram;
	}


	void setUpLighting() {

		// ***** Ambient Light **************
		generalLighting.setEnabled( GL_LIGHT_ZERO, true );
		generalLighting.setAmbientColor( GL_LIGHT_ZERO, vec4(0.2f, 0.2f, 0.2f, 1.0f));

		// *****  Directional Light ****************
		generalLighting.setEnabled( GL_LIGHT_ONE, true );
		generalLighting.setDiffuseColor( GL_LIGHT_ONE, vec4(0.75f, 0.75f, 0.75f, 1.0f) );
		generalLighting.setSpecularColor( GL_LIGHT_ONE, vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		generalLighting.setPositionOrDirection( GL_LIGHT_ONE, vec4(1.0f, 1.0f, 1.0f, 0.0f) );

		// ***** Positional Light ***************
		generalLighting.setEnabled( GL_LIGHT_TWO, true );
		generalLighting.setDiffuseColor( GL_LIGHT_TWO, vec4(0.5f, 0.5f, 0.5f, 1.0f) );
		generalLighting.setSpecularColor( GL_LIGHT_TWO, vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		generalLighting.setPositionOrDirection( GL_LIGHT_TWO, vec4(1.0f, 3.0f, 1.0f, 1.0f) );

		// ***** Spot Light **************
		generalLighting.setEnabled( GL_LIGHT_THREE, true );
		generalLighting.setIsSpot( GL_LIGHT_THREE, true );
		generalLighting.setDiffuseColor( GL_LIGHT_THREE, vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		generalLighting.setSpecularColor( GL_LIGHT_THREE, vec4(1.0f, 1.0f, 1.0f, 1.0f) );
		generalLighting.setPositionOrDirection( GL_LIGHT_THREE, vec4(0.0f, 8.0f, 0.0f, 1.0f) );
		generalLighting.setSpotDirection( GL_LIGHT_THREE, vec3(0.0f, -1.0f, 0.0f) );
		generalLighting.setSpotCutoffCos( GL_LIGHT_THREE, cos(glm::radians(15.0f)) );

	} // end setUpLighting

	virtual bool initialize()
	{
		glutSetCursor(GLUT_CURSOR_NONE);
		//glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		// *** Fog ***
		glClearColor(0.5f, 0.5f, 0.5f, 0.0f);
		glFrontFace(GL_CCW);
		glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
		glEnable( GL_DEPTH_TEST );

		//setUpMenus();

		// Set the viewing transformation that will be used to render all objects 
		// in the scene.
		this->ViewMatrix = translate(mat4(1.0f), vec3( 0.0f, 0.0f,-12.0f ));

		root.initialize();
		for(int i = 0; i < bullets_left.size(); i++)
		{//bullets intialize
			bullets_left[i]->initialize();
		}


		bgSound->setLooping(true);
		bgSound->play();
		//old floor
		//floor->modelMatrix = translate(mat4(1.0f), vec3( 0.0f, 0.0f, 0.0f ));
		Start = false;
		isWalking = false;
		// *** Sound ***


		return true;

	} // end initialize


	// Render scene objects
	virtual void drawScene()
	{ 
		printAim();
		root.draw();
	} // end drawScene

	void collision_detect()
	{
		for(int j = 0; j < bullets_fired.size(); j++)
		{
			for(int i = 0; i < targets.size(); i++)
			{				
				if(!((targetController*)targets[i]->getController())->destroyed)
					if(  (root.getChildByID(bullets_fired[j]->getObjectSerialNumber() ) != NULL) && ( ((BulletController *)bullets_fired[j]->getController())->destoried ) != true )
					{

						if(((targetController*)targets[i]->getController())->flipped){
							//cout<<glm::distance( bullets_fired[j]->position, targets[i]->position )<<endl;
							if( ((targetController*)targets[i]->getController())->Impacted(bullets_fired[j]->position) )
								targetImpactSound->play();
							if( ((targetController*)targets[i]->getController())->Collided(bullets_fired[j]->position))
							{
								((targetController*)targets[i]->getController())->destroyed = true;
								currentScore++;
								targetDieSound->play();
								root.removeChild( bullets_fired[j]->getObjectSerialNumber() );	
								( (BulletController *)bullets_fired[j]->getController() ) -> destoried = false;
								removed_bullets++;
								//((targetController *)targets[i]->getController())->flip_down();
								targets[i]->detachFromParent();
								//root.removeChild( targets[i]->getObjectSerialNumber() );
								//targets_being_hit.push_back(i);
							}
						}

					}
			}

		}
	}


	// Update scene objects inbetween frames
	virtual void updateScene( float elapsedTimeSec ) 
	{ 	//std::cout<<winX<<" "<<winY<<std::endl;
		checkTargetDistances();

		updateBullet();
		collision_detect();	


		setViewPoint();
		//glutWarpPointer(winX/2, winY/2);
		MCamera->Mouse_Move(winX,winY);

		Keyboard_Input();
		playFootStep();


		root.update( elapsedTimeSec );

		//reload();
	} // end updateScene


	// Set the viewing transformations
	void setViewPoint(  ) 
	{
		MCamera->checkLadder();
		MCamera->checkOnBuilding();
		// This while loop to ensure the starting point is set correctly
		while(MCamera->mView.x != -22.713f&& MCamera->mView.y !=2.522f&&MCamera->mView.z != 44.0693f&&Start==false){
			MCamera->mPos = vec3(-22.76f, 2.5f, 49.0f);
			MCamera->mView = vec3(-22.713f, 2.522f, 44.0693f);
			MCamera->mUp = vec3(0.0f, 1.0f, 0.0f);
			Start = true;
		}
		//cout<<MCamera->mPos.x<<" "<<  MCamera->mPos.y<<" "<<  MCamera->mPos.z<<endl;
		playerCont->inputPosition(vec3(MCamera->mPos.x,  MCamera->mPos.y,  MCamera->mPos.z),	vec3(MCamera->mView.x, MCamera->mView.y, MCamera->mView.z));
		ViewMatrix = glm::lookAt(vec3(MCamera->mPos.x,  MCamera->mPos.y , MCamera->mPos.z),	
			vec3(MCamera->mView.x, MCamera->mView.y, MCamera->mView.z),	
			vec3(MCamera->mUp.x,   MCamera->mUp.y,   MCamera->mUp.z));

		// *** Sound ***
		//ViewMatrix = followCamera->getViewMatrix();

		projectionAndViewing.setViewMatrix(ViewMatrix);

	} // end setViewPoint

	void updateBullet()
	{
		for(int i = 0; i < bullets_fired.size(); i++)
		{
			if( root.getChildByID(bullets_fired[i]->getObjectSerialNumber() ) != NULL && 
				((BulletController *)root.getChildByID( bullets_fired[i]->getObjectSerialNumber() )->getController())->destoried )
			{
				//bullets_fired[i]->detachFromParent();
				root.removeChild( bullets_fired[i]->getObjectSerialNumber() );
				( (BulletController *)bullets_fired[i]->getController() ) -> destoried = false;
				removed_bullets++;
				//cout<<"Bullet disappear"<<endl;
			}
		}
	}

	void reload()
	{
		isReloading = true;
		for(int i = 0; bullets_fired.size() > 0; i++)
		{
			//for(unsigned int j = 0; j < 50000000; j++) ;					// add delay for the reload

			bullets_left.push_back( bullets_fired[0] );
			//root.addChild(bullets_fired[0]);
			bullets_fired.erase( bullets_fired.begin() );
			fired_bullets--;
		}
	}

	void playFootStep(){
		if(playF){
			walkSound->setLooping(true);
			if(!walkSound->isPlaying)
				walkSound->play();
		}
	}


	//Key dectection (Windows) - faster and smoother compare to GlutKey detection - need to check
	//if we are allow to use it.
	void Keyboard_Input()
	{
		playF = false;
		walkSound->setVolume(0.5f);
		walkSound->setSoundPosition(MCamera->mPos);
		if((GetKeyState(VK_UP) & 0x80) || (GetKeyState('W') & 0x80))
		{	
			if(MCamera->mPos.y>=2.5f)
				MCamera->Move_Camera( CAMERASPEED);
			else {
				walkSound->setVolume(0.2f);
				MCamera->Move_Camera( CAMERASPEED/2);
			}
			playF = true;
		}

		if((GetKeyState(VK_DOWN) & 0x80) || (GetKeyState('S') & 0x80))
		{
			if(MCamera->mPos.y>=2.5f)
				MCamera->Move_Camera( -CAMERASPEED);
			else {
				walkSound->setVolume(0.2f);
				MCamera->Move_Camera( -CAMERASPEED/2);
			}
			playF = true;
		}

		if((GetKeyState(VK_LEFT) & 0x80) || (GetKeyState('A') & 0x80))
		{	
			if(MCamera->mPos.y>=2.5f)
				MCamera->Strafe_Camera(-SIDESPEED);
			else {
				walkSound->setVolume(0.2f);
				MCamera->Strafe_Camera(-SIDESPEED/2);
			}
			playF = true;
		}

		if((GetKeyState(VK_RIGHT) & 0x80) || (GetKeyState('D') & 0x80))
		{
			if(MCamera->mPos.y>=2.5f)
				MCamera->Strafe_Camera(SIDESPEED);
			else {
				walkSound->setVolume(0.2f);
				MCamera->Strafe_Camera(SIDESPEED/2);
			}
			playF = true;
		}


		if((GetKeyState(VK_LCONTROL) & 0x80))
		{
			MCamera->Mouse_Crouch();
		} 

		if(playF == false)
			walkSound->stop();

		if(clip_empty) {
			if(!isReloading){
				t1 = clock();
				reload();
				reloadSound->play();
			}
			t2 = clock();
			float diff = (((float)t2 - (float)t1) / 1000000.0F ) * 1000;
			if(diff>3){
				isReloading = false;
				clip_empty = false;
				removed_bullets = 0;
			}
			//cout<<"Clip is full"<<endl;
		} else if(!clip_empty){
			t2 = clock();   
			float diff = (((float)t2 - (float)t1) / 1000000.0F ) * 1000; 
			if(diff>0.1) //Control Fire Rate
				if ((GetKeyState(VK_LBUTTON) & 0x80))//left click
				{
					fireSound->play();
					//cout<<"Fire at "<<diff<<endl;
					gunFire();
				} 
		}
	}

	void gunFire(){
		fired_bullets++;
		//std::cout<<fired_bullets<<std::endl;
		vec3 position = model->position + glm::normalize(MCamera->mView - MCamera->mPos) * 5.0f;
		root.addChild(bullets_left[0]);
		( (BulletController *)bullets_left[0]->getController() )->fire( position, glm::normalize(MCamera->mView - MCamera->mPos ), 150.0f);
		bullets_fired.push_back( bullets_left[0] );
		bullets_left.erase( bullets_left.begin() );

		if( bullets_left.size() == 0 ) 
		{
			clip_empty = true;
			//cout<<"Clip is empty"<<endl;
		}	
		//bullCont[fired_bullets]->fire(position, glm::normalize(MCamera->mView - MCamera->mPos), 5);
		t1 = clock();
	}
	void ReshapeCB(int windowWidth, int windowHeight)
	{
		OpenGLApplicationBase::ReshapeCB(windowWidth, windowHeight);
		winX = windowWidth;
		winY = windowHeight;
		projectionAndViewing.setProjectionMatrix(ProjectionMatrix);

	} // end ReshapeCB

	void PassiveMouseCB(int x, int y)
	{

		/*if(abs(winX-x) > 1){
		//playerCont->gunRotating = true;
		if(winX - x < 0)
		playerCont->rotate_view(-1.0f);
		else
		playerCont->rotate_view(1.0f);
		}*/
		//	playerCont->gunMove(winX,winY,x,y);
		//	 cout << "mouse motion with button up at (" << x << ", " << y << ")" << endl;
	} // end PassiveMouseCB


	void SpecialKeyboardCB(int Key, int x, int y)
	{

		switch (Key) {

		case GLUT_KEY_RIGHT:
			rotationY++;
			break;
		case GLUT_KEY_LEFT:
			rotationY--;
			break;
		case GLUT_KEY_UP:
			rotationX++;
			break;
		case GLUT_KEY_DOWN:
			rotationX--;
			break;
		default:
			OpenGLApplicationBase::SpecialKeyboardCB(Key, x, y);
			break;
		}

	} // end SpecialKeyboardCB


	// Call back of letter keys and numbers
	void KeyboardCB(unsigned char Key, int x, int y)
	{
		static SpinnerController * pyramid1Cont;
		isForward = false;
		isBackward = false;
		switch (Key) {
		case 'w': case 'W':
			viewTranslate++;
			//MCamera->Move_Camera(CAMERASPEED);
			break;
		case 's': case 'S':
			viewTranslate--;
			//MCamera->Move_Camera( -CAMERASPEED);
			break; 
		case 'a': case 'A':
			//MCamera->Strafe_Camera(-CAMERASPEED);
			break;
		case 'd': case 'D':
			//MCamera->Strafe_Camera(CAMERASPEED);
			break;

		case ' ':
			playF = false;
			MCamera->Mouse_Jump();
			break;
			/*
			case 'a':

			if (generalLighting.getEnabled( GL_LIGHT_ZERO ) == true ) {
			generalLighting.setEnabled( GL_LIGHT_ZERO,  false );
			}
			else {
			generalLighting.setEnabled( GL_LIGHT_ZERO,  true );
			}

			break;
			case 'd':

			if (generalLighting.getEnabled( GL_LIGHT_ONE ) == true ) {
			generalLighting.setEnabled( GL_LIGHT_ONE,  false );
			}
			else {
			generalLighting.setEnabled( GL_LIGHT_ONE,  true );
			}
			break;
			case 'p':

			if (generalLighting.getEnabled( GL_LIGHT_TWO ) == true ) {
			generalLighting.setEnabled( GL_LIGHT_TWO,  false );
			}
			else {
			generalLighting.setEnabled( GL_LIGHT_TWO,  true );
			}
			break;
			case 'x':

			if (generalLighting.getEnabled( GL_LIGHT_THREE ) == true ) {
			generalLighting.setEnabled( GL_LIGHT_THREE,  false );
			}
			else {
			generalLighting.setEnabled( GL_LIGHT_THREE,  true );
			}
			break;
			*/
		default:				
			OpenGLApplicationBase::KeyboardCB(Key, x, y);
			break;
		}
	} // end KeyboardCB


	friend void mainMenu(int value);

	void setUpMenus()
	{	
		GLuint menu2id = createPolygonMenu();
		GLuint menu3id = createFrontFaceMenu();
		GLuint menu4id = createPointSizeMenu();
		GLuint menu5id = createAntialiasingMenu();

		// Create main menu
		topMenu = glutCreateMenu(mainMenu);
		glutAddSubMenu("Polygon Mode", menu2id); 
		glutAddSubMenu("Rendered Polygon Face ", menu3id);
		glutAddSubMenu("Point Size ", menu4id); 
		glutAddSubMenu("Antialiasing ", menu5id);
		glutAddMenuEntry("Quit", 1); // Specify indentifier for "Quit"			  
		glutAttachMenu(GLUT_RIGHT_BUTTON); // Attach menu to right mouse button

	} //end setUpMenus


	void printAim()
	{ 
		if(currentScore == targets.size())
			gameFinish = true;

		glClear(GL_COLOR_BUFFER_BIT); 
		glLoadIdentity();
		glColor3f(0,1,0);

		float unitX = 1.0/winX;
		float unitY = 1.0/winY;
		if(!gameFinish){
			//print score
			string output1 = "_ _";
			drawBitmapText((char*)output1.c_str(),-14*unitX,0,0);

			string output2 = " | ";
			drawBitmapText((char*)output2.c_str(),0,unitY,0);

			string output3 = " | ";
			drawBitmapText((char*)output3.c_str(),0,-50*unitY,0);

			std::stringstream sstm;
			sstm<<"Score ("<<currentScore<<" / "<<targets.size()<<")";
			string outputScore = sstm.str();
			drawBitmapText((char*)outputScore.c_str(),-0.95,-0.95,0);

			if(isReloading){
				//print score
				string output4 = "RELOADING";
				//cout<<-100*unitX<<endl;
				drawBitmapText((char*)output4.c_str(),-110*unitX,-0.15,0);
			}


		} else {
			string Victory = "Training is Done";
			drawBitmapText((char*)Victory.c_str(),-150*unitX,0,0);
		}
	}

	void drawBitmapText(char *input,float x,float y,float z) 
	{  
		char *c;
		glRasterPos3f(x,y,z);
		for (c=input; *c != '\0'; c++) 
		{
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
		}
	}

	//****************NEW***********************
	void checkTargetDistances(){
		static int count = 0;
		numTargets = 0;
		for(int i=numTargets; i<targetInfo.size(); i++){
			if(glm::distance( targets[i]->position,  MCamera->mPos ) < 12.0f){
				((targetController*)(targets[i]->getController()))->flip();
			}
		}
		numTargets+=targetInfo.size();
		if(MCamera->mPos.x>-20.0f&&MCamera->mPos.x<-6.0f&&MCamera->mPos.z<42.5f&&MCamera->mPos.z>40.0f&&InfoGroup1.size()!=0){
			//if(!targetAppearSound->isPlaying)
			targetAppearSound->play();
			for(int i=numTargets;i<numTargets+InfoGroup1.size();i++){
				((targetController*)(targets[i]->getController()))->flip();
			}
			InfoGroup1.clear();
		}
		numTargets+=3;
		if(MCamera->mPos.x>5.0f&&MCamera->mPos.x<10.0f&&MCamera->mPos.z<25.5f&&MCamera->mPos.z>-5.0f&&InfoGroup2.size()!=0){	
			//if(!targetAppearSound->isPlaying)
			targetAppearSound->play();
			for(int i=numTargets;i<numTargets+InfoGroup2.size();i++){
				((targetController*)(targets[i]->getController()))->flip();
			}
			InfoGroup2.clear();
		}
		numTargets+=3;
		if(MCamera->mPos.x>5.0f&&MCamera->mPos.x<25.0f&&MCamera->mPos.z<-22.5f&&MCamera->mPos.z>-25.0f&&InfoGroup3.size()!=0){
			//if(!targetAppearSound->isPlaying)
			targetAppearSound->play();
			for(int i=numTargets;i<numTargets+InfoGroup3.size();i++){
				((targetController*)(targets[i]->getController()))->flip();
			}
			InfoGroup3.clear();
		}
		targetAppearSound->isPlaying=false;
	}


	void iniTargetName(){
		float temp1 = -0.2f;
		float temp2 = 5.0f;
		float temp4 = -0.2f;
		targetInfo.push_back( new Target( "target1.bmp", vec3(0.0f, temp1, 4.0f), 3));
		targetInfo.push_back( new Target("target1.bmp",vec3(6.0f, temp2, 39.5f), 4));
		targetInfo.push_back( new Target("target1.bmp",vec3(20.0f, temp4, 45.0f), 1));
		targetInfo.push_back( new Target("target1.bmp",vec3(0.0f, temp4, 49.5f), 3));
		targetInfo.push_back( new Target("target1.bmp",vec3(-7.5f, temp4, 37.5f), 1));
		targetInfo.push_back( new Target("target1.bmp",vec3(2.5f, temp4, -23.0f), 3));
		targetInfo.push_back( new Target("target1.bmp",vec3(-6.0f, temp2, 24.8f), 2));
		targetInfo.push_back( new Target("target1.bmp",vec3(15.0f, 9.8f, 7.5f), 3));
		targetInfo.push_back( new Target("target1.bmp",vec3(-14.0f, temp4, -31.0f), 1));
		targetInfo.push_back( new Target("target1.bmp",vec3(-22.0f, temp4, 0.0f), 3));
	}

	void initGroup(){
		float tempX=-22.0f;
		float tempY=12.3f;
		float tempZ=24.0f;
		InfoGroup1.push_back(new Target( "target1.bmp", vec3(-18.0f, tempY, tempZ), 3));
		InfoGroup1.push_back(new Target( "target1.bmp", vec3(-14.0f, tempY, tempZ-3.0f), 3));
		InfoGroup1.push_back(new Target( "target1.bmp", vec3(-10.0f, tempY, tempZ), 3));

		tempX=-6.0f;
		tempY=12.3f;
		tempZ=17.0f;
		InfoGroup2.push_back(new Target( "target1.bmp", vec3(tempX, tempY, tempZ), 1));
		tempZ -= 8.0f;
		InfoGroup2.push_back(new Target( "target1.bmp", vec3(tempX, tempY, tempZ), 1));
		tempZ -= 8.0f;
		InfoGroup2.push_back(new Target( "target1.bmp", vec3(tempX, tempY, tempZ), 1));

		tempX=15.0f;
		tempY=6.03f;
		tempZ=-42.0f;
		InfoGroup3.push_back(new Target( "target1.bmp", vec3(tempX, tempY, tempZ), 3));
		InfoGroup3.push_back(new Target( "target1.bmp", vec3(tempX+5.0f, tempY, tempZ), 3));
		InfoGroup3.push_back(new Target( "target1.bmp", vec3(tempX-5.0f, tempY, tempZ), 3));
	}
}; // end bachmaerLab12


// Global reference to the OpenGLApplicationBase class
// Can be used in friend functions to call methods and 
// access private data members.
bachmaerLab12 * openGLApplicationPtr;

void mainMenu(int value)
{
	switch (value) {

	case(1):
		glutLeaveMainLoop(); // End the program
		break;
	default:
		cout << "Unknown Main Menu Selection!" << endl;

	}

} // end mainMenu


int main(int argc, char** argv)
{
	GLUTBaseInit(argc, argv);

	GLUTBaseCreateWindow( "CSE 386 Final Project - FPS Training" );

	bachmaerLab12 pApp;

	// Set global reference that friends= functions can use
	openGLApplicationPtr = &pApp;

	pApp.initialize();

	GLUTBaseRunApplication(&pApp);

	return 0;

} // end main