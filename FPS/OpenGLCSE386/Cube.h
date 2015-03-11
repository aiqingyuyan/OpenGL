#ifndef ___cube_H___
#define ___cube_H___

#include "visualobject.h"
#include "LoadShaders.h"
#include "Lighting.h"

class cube : public VisualObject
{
	public:

		cube(OpenGLApplicationBase * OpenGLApp, GLfloat w = 1.0f, GLfloat h = 1.0f, GLfloat d = 1.0f, vec4 c = vec4(0.0f, 0.1f, .8f, 1.0f), int rows = 1, int columns = 1);

		
		virtual ~cube(void);
		/**
		 * Builds shader program and initializes vertex array object that will be used to render the 
		 * the object.
		 */
		virtual void initialize();

		/*
		 * Renders the object using the visual object using the vertex array object that was created by
		 * the initialize method.
		 */
		virtual void draw();

	protected:

		/**
		 * Builds the shader program.
		 */
		void buildShaderProgram();
		
		int numberOfIndices; // number of indices to be used during indexed rendering.

		int rows, columns;
		
		vec2 t0; 
		vec2 t1;
		vec2 t2;
		vec2 t3;

		const GLfloat width; // width for the cube
		const GLfloat height;
		const GLfloat depth;
			
		GLuint textureObject;// For texture mapping

		GLuint modelLocation;
		GLuint solidColorLocation;

		vec4 objectColor;
		Material material;

};

#endif // ___cube_H___