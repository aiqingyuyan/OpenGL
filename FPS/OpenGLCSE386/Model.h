#ifndef _MODEL_H_
#define _MODEL_H_
#include "visualobject.h"
#include "objLoader.h"

#include "Lighting.h"

/*
* This class is used to store the 3D model information loaded by using the Obj_Loader class
* Then we can draw the model by using this class. 
*/

class Model : public VisualObject
{
public:
	Model(OpenGLApplicationBase * OpenGLApp, Obj_Loader * loader, vec4 color = vec4(1.0f));
	~Model(void) {delete Loader;}
	
	virtual void initialize();

	virtual void draw();

protected:

	void buildShaderProgram();

	int numberOfIndices;

	GLuint modelLocation;
	GLuint solidColorLocation;

	// For texture mapping
	GLuint textureObject;

	vec4 objectColor;
	Material material;

	Obj_Loader* Loader;
};

#endif