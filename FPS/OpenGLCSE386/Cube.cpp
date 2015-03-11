#include "cube.h"

#include <vector>

#include "vertexStructs.h"

cube::cube(OpenGLApplicationBase * OpenGLApp, GLfloat w, GLfloat h, GLfloat d, vec4 c,int rows, int columns)
	:VisualObject( OpenGLApp ),width(w), height(h), depth(d), objectColor(c),rows(rows), columns(columns)
{
	 t0 = vec2(0.0f, rows);
	 t1 = vec2(0.0f, 0.0f);
	 t2 = vec2(columns, 0.0f);
	 t3 = vec2(columns, rows);
	 
}

cube::~cube(void){
	glDeleteVertexArrays(1,&vertexArrayObject);
}

void cube::buildShaderProgram()
{
	if ( shaderProgram==0 )  {

		// Create array of ShaderInfo structs that specifies the vertex and
		// fragment shaders to be compiled and linked into a program.
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "pVSpSolidColor.vert" },
			{ GL_FRAGMENT_SHADER, "pFSc.frag"},
			{ GL_NONE, NULL } // signals that there are no more shaders
		};

		// Read the files and create the OpenGL shader program.
		shaderProgram = LoadShaders(shaders);
	}
	 glUseProgram(shaderProgram);

	modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	assert(modelLocation != 0xFFFFFFFF);

	solidColorLocation = glGetUniformLocation(shaderProgram, "objectColor");

	material = Material( getUniformLocation( shaderProgram, "object.ambientMat"),
						 getUniformLocation( shaderProgram, "object.diffuseMat"),
						 getUniformLocation( shaderProgram, "object.specularMat"),
						 getUniformLocation( shaderProgram, "object.specularExp"),
						 getUniformLocation( shaderProgram, "object.emissiveMat" ),
						 getUniformLocation( shaderProgram, "object.textureMapped") );
	
	//material.setAmbientAndDiffuseMat( objectColor );
	material.setAmbientAndDiffuseMat( vec4(0.5f) );
	material.setSpecularMat( vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	material.setSpecularExponentMat( 64.0f );
	material.setTextureMapped(true);

} // end buildShaderProgram

int pushHelper(vector<unsigned int>* i, int currIndex, int f1,int f2,int f3, int s1,int s2,int s3){
	(*i).push_back(currIndex + f1);
	(*i).push_back(currIndex + f2);
	(*i).push_back(currIndex + f3);

	(*i).push_back(currIndex + s1);
	(*i).push_back(currIndex + s2);
	(*i).push_back(currIndex + s3);

	currIndex+=4;
	return currIndex;
}

void cube::initialize()
{

	textureObject = setupTexture( textureFileName );

	GLfloat H = height/2.0f;
	GLfloat L = width/2.0f;
	GLfloat W = depth/2.0f;

	buildShaderProgram();
	glUseProgram(shaderProgram);

	GLuint VBO, IBO;

	glGenVertexArrays (1, &vertexArrayObject); 
	glBindVertexArray( vertexArrayObject );

	vector<pntVertexData> v;
	vector<unsigned int> i;
	int currentIndex = 0;

	vec3 v0 = vec3( -L, H, W  );
	vec3 v1 = vec3( -L, -H, W );
	vec3 v2 = vec3(  L, -H, W );
	vec3 v3 = vec3(  L, H, W  );
	vec3 v4 = vec3(  L, -H, -W );
	vec3 v5 = vec3(  L, H, -W );
	vec3 v6 = vec3( -L, H, -W );
	vec3 v7 = vec3( -L, -H, -W);

	vec3 zp = vec3(0.0f,0.0f, 1.0f);//Z-positive
	vec3 zn = vec3(0.0f,0.0f,-1.0f);//Z-negative
	vec3 xp = vec3(1.0f,0.0f, 0.0f);//X-positive
	vec3 xn = vec3(-1.0f,0.0f,0.0f);//X-negative
	vec3 yp = vec3(0.0f,1.0f, 0.0f);//Y-positive
	vec3 yn = vec3(0.0f,-1.0f,0.0f);//Y-negative


	//front1
	v.push_back( pntVertexData ( v0, zp, t0 ) );
	v.push_back( pntVertexData ( v1, zp, t1 ) );
	v.push_back( pntVertexData ( v2, zp, t2 ) );
	v.push_back( pntVertexData ( v3, zp, t3 ) );

	//front2
	v.push_back( pntVertexData ( v0, yp, t0) );
	v.push_back( pntVertexData ( v1, yn, t1 ) );
	v.push_back( pntVertexData ( v2, yn, t2 ) );
	v.push_back( pntVertexData ( v3, yp, t3 ) );
	
	//front3
	v.push_back( pntVertexData ( v0, xn, t0 ) );
	v.push_back( pntVertexData ( v1, xn, t1 ) );
	v.push_back( pntVertexData ( v2, xp, t2 ) );
	v.push_back( pntVertexData ( v3, xp, t3 ) );
			
	for( int j=0; j<3; j++)
		currentIndex =  pushHelper(&i, currentIndex, 0,1,2, 0,2,3);

	//Left 
	v.push_back( pntVertexData ( v6, xn, t0 ) );
	v.push_back( pntVertexData ( v7, xn, t1 ) );
	v.push_back( pntVertexData ( v1, xn, t2 ) );
	v.push_back( pntVertexData ( v0, xn, t3 ) );

	v.push_back( pntVertexData ( v6, yp, t0 ) );
	v.push_back( pntVertexData ( v7, yn, t1 ) );
	v.push_back( pntVertexData ( v1, yp, t2 ) );
	v.push_back( pntVertexData ( v0, yn, t3 ) );

	v.push_back( pntVertexData ( v6, zn, t0) );
	v.push_back( pntVertexData ( v7, zn, t1) );
	v.push_back( pntVertexData ( v1, zp, t2 ) );
	v.push_back( pntVertexData ( v0, zp, t3 ) );
	for( int j=0; j<3; j++)
		currentIndex =  pushHelper(&i, currentIndex, 0,1,2, 3,0,2);

	//Right
	v.push_back( pntVertexData ( v3, xp, t0 ) );
	v.push_back( pntVertexData ( v2, xp, t1) );
	v.push_back( pntVertexData ( v4, xp, t2 ) );
	v.push_back( pntVertexData ( v5, xp, t3 ) );

	v.push_back( pntVertexData ( v3, yp, t0 ) );
	v.push_back( pntVertexData ( v2, yn, t1 ) );
	v.push_back( pntVertexData ( v4, yn, t2 ) );
	v.push_back( pntVertexData ( v5, yp, t3 ) );

	v.push_back( pntVertexData ( v3, zp, t0 ) );
	v.push_back( pntVertexData ( v2, zp, t1 ) );
	v.push_back( pntVertexData ( v4, zn, t2 ) );
	v.push_back( pntVertexData ( v5, zn, t3 ) );

	for( int j=0; j<3; j++)
		currentIndex =  pushHelper(&i, currentIndex, 0,1,2, 0,2,3);

	//back
	v.push_back( pntVertexData ( v5, zn, t0 ) );
	v.push_back( pntVertexData ( v4, zn, t1 ) );
	v.push_back( pntVertexData ( v7, zn, t2 ) );
	v.push_back( pntVertexData ( v6, zn, t3 ) );

	v.push_back( pntVertexData ( v5, xp, t0 ) );
	v.push_back( pntVertexData ( v4, xp, t1 ) );
	v.push_back( pntVertexData ( v7, xn, t2 ) );
	v.push_back( pntVertexData ( v6, xn, t3 ) );
	
	v.push_back( pntVertexData ( v5, yp, t0 ) );
	v.push_back( pntVertexData ( v4, yn, t1 ) );
	v.push_back( pntVertexData ( v7, yn, t2 ) );
	v.push_back( pntVertexData ( v6, yp, t3 ) );

	for( int j=0; j<3; j++)
		currentIndex =  pushHelper(&i, currentIndex, 0,1,2, 0,2,3);

	//up
	v.push_back( pntVertexData ( v6, yp, t0) );
	v.push_back( pntVertexData ( v0, yp, t1 ) );
	v.push_back( pntVertexData ( v3, yp, t2) );
	v.push_back( pntVertexData ( v5, yp, t3) );

	v.push_back( pntVertexData ( v6, xn, t0 ) );
	v.push_back( pntVertexData ( v0, xn, t1 ) );
	v.push_back( pntVertexData ( v3, xp, t2) );
	v.push_back( pntVertexData ( v5, xp, t3 ) );
	
	v.push_back( pntVertexData ( v6, zn, t0 ) );
	v.push_back( pntVertexData ( v0, zp, t1 ) );
	v.push_back( pntVertexData ( v3, zp, t2 ) );
	v.push_back( pntVertexData ( v5, zn, t3 ) );
	for( int j=0; j<3; j++)
		currentIndex =  pushHelper(&i, currentIndex, 0,1,2, 2,3,0);

	//down
	v.push_back( pntVertexData ( v1, yn, t0) );
	v.push_back( pntVertexData ( v7, yn, t1 ) );
	v.push_back( pntVertexData ( v4, yn, t2 ) );
	v.push_back( pntVertexData ( v2, yn, t3) );

	v.push_back( pntVertexData ( v1, xn, t0 ) );
	v.push_back( pntVertexData ( v7, xn, t1 ) );
	v.push_back( pntVertexData ( v4, xp, t2 ) );
	v.push_back( pntVertexData ( v2, xp, t3 ) );

	v.push_back( pntVertexData ( v1, zp, t0 ) );
	v.push_back( pntVertexData ( v7, zn, t1 ) );
	v.push_back( pntVertexData ( v4, zn, t2 ) );
	v.push_back( pntVertexData ( v2, zp, t3 ) );

	for( int j=0; j<3; j++)
		currentIndex =  pushHelper(&i, currentIndex, 0,1,2, 0,2,3);

 	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pntVertexData), &v[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData),(const GLvoid*)sizeof(vec3));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)(2 * sizeof(vec3)) );
	glEnableVertexAttribArray(3);

	numberOfIndices = i.size();

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, i.size() * sizeof(unsigned int), &i[0], GL_STATIC_DRAW);

	VisualObject::initialize();
} // end initialize

// Preform drawing operations
void cube::draw()
{
	glUseProgram(shaderProgram);

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelAndFixed));
	glUniform4fv(solidColorLocation, 1, value_ptr(objectColor));

	material.setShaderMaterialProperties();
	glBindTexture(GL_TEXTURE_2D, textureObject);

	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();

} // end draw

