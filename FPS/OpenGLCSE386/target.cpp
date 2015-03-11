#include "target.h"

#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
#include "Cube.h"

target::target( OpenGLApplicationBase * OpenGLApp, string fName)
	: VisualObject( OpenGLApp ),filename(fName)
{
		
		GLfloat w = 2.0f, h = 3.0f, d = 0.1f;
		int rows = 1, columns = 1;
		VisualObject *target = new cube(OpenGLApp, w, h, d, vec4( 0.2f, 0.2, 0.5f, 1.0f), rows, columns );
		target->fixedTransformation = translate(mat4(1.0f), vec3(0, h/2.0f, 0));
		target->setTextureName(filename);
		this->addChild(target);

		/*w = 2.5f, h = 0.3f, d = 0.3f;
		VisualObject *bottom = new cube(OpenGLApp, w, h, d, vec4( 0.2f, 0.2, 0.5f, 1.0f), rows, columns );
		bottom->fixedTransformation = translate(mat4(1.0f), vec3(0, 0.0f, 0));
		bottom->setTextureName("wood.bmp");
		this->addChild(bottom);*/


} // end target constructor

void target::setShader( GLuint shaderProgram ) 
{ 
	for (unsigned int i = 0; i < children.size(); i++) {

		children[i]->setShader(shaderProgram);
	}
}



