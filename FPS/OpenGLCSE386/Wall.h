#ifndef ___WALL_H___
#define ___WALL_H___


#include "visualobject.h"
#include "LoadShaders.h"
#include "Lighting.h"

class Wall : public VisualObject
{
	public:
		Wall( OpenGLApplicationBase * OpenGLApp, string textureFileName = NULL,
			  float height = 6.0f, float width = 8.0f, 
			  int rows = 1, int columns = 1 );

		virtual ~Wall(void);
			
		virtual void initialize();

		virtual void draw();
	
		virtual void setTextureCood(vec2 t00, vec2 t01,vec2 t02,vec2 t03);

	protected:
	
		void buildShaderProgram();

		string textureFileName;

		int rows, columns;
		float height, width;

		int indiceCount;
		GLuint textureObject;

		GLuint modelLocation;

		vec4 objectColor;
		Material material;
		
		vec2 t0; 
		vec2 t1; 
		vec2 t2;
		vec2 t3;

};

#endif // ___WALL_H___

