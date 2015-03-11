#ifndef ___ladder_H___
#define ___ladder_H___

#include "visualobject.h"
#include "LoadShaders.h"

class ladder : public VisualObject
{
	public:
		ladder( OpenGLApplicationBase * OpenGLApp,string textureName, float leng);

		virtual void setShader( GLuint shaderProgram );

		float length;
	protected:

		GLuint projectionLocation; 
		GLuint modelViewLocation;
};

#endif // ___ladder_H___
