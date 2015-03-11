#include "ladder.h"
#include "Cube.h"

ladder::ladder( OpenGLApplicationBase * OpenGLApp, string textureName, float leng)
	: VisualObject( OpenGLApp ), length(leng)
{
		textureFileName = textureName;
		GLfloat w = 0.2f, h = length, d = 0.1f;
		int rows = 1, columns = 1;
		VisualObject *ladderL = new cube(OpenGLApp, w, h, d, vec4( 0.2f, 0.2, 0.5f, 1.0f), rows, columns );
		ladderL->fixedTransformation = translate(mat4(1.0f), vec3(-0.5f, h/2.0f, 0));
		ladderL->setTextureName(textureFileName);
		this->addChild(ladderL);

		
		VisualObject *ladderR = new cube(OpenGLApp, w, h, d, vec4( 0.2f, 0.2, 0.5f, 1.0f), rows, columns );
		ladderR->fixedTransformation = translate(mat4(1.0f), vec3(0.5f, h/2.0f, 0));
		ladderR->setTextureName(textureFileName);
		this->addChild(ladderR);

		w = 1.0f, h = 0.1f, d = 0.1f;

		for( int i=0; i< length/0.2f; i++){
			VisualObject *ladder1 = new cube(OpenGLApp, w, h, d, vec4( 0.2f, 0.2, 0.5f, 1.0f), rows, columns );
			ladder1->fixedTransformation = translate(mat4(1.0f), vec3(0, (0.1 + i*0.2), 0));
			ladder1->setTextureName(textureFileName);
			this->addChild(ladder1);
		}
		
} // end ladder constructor

void ladder::setShader( GLuint shaderProgram ) 
{ 
	for (unsigned int i = 0; i < children.size(); i++) {

		children[i]->setShader(shaderProgram);
	}
}



