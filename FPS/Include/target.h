#ifndef ___target_H___
#define ___target_H___

#include "visualobject.h"
#include "LoadShaders.h"

class target : public VisualObject
{
	public:
		target( OpenGLApplicationBase * OpenGLApp, string fname = "target1.bmp");

		virtual void setShader( GLuint shaderProgram );

	protected:

		GLuint projectionLocation; 
		GLuint modelViewLocation;
		string filename;
};

#endif // ___target_H___
