#include "Model.h"

Model::Model(OpenGLApplicationBase * OpenGLApp, Obj_Loader * loader, vec4 color) : VisualObject( OpenGLApp )
{
	Loader = loader;
	objectColor = color;
}

void Model::buildShaderProgram()
{
	if ( shaderProgram==0 )  {
		ShaderInfo shaders[] = {
			{ GL_VERTEX_SHADER, "pVSpSolidColor.vert" },
			{ GL_FRAGMENT_SHADER, "pFSc.frag"},
			{ GL_NONE, NULL } // signals that there are no more shaders
		};
	
		// Read the files and create the OpenGL shader program.
		shaderProgram = LoadShaders(shaders);
	}

	// This program will stay in effect for all draw calls until it is 
	// replaced with another or explicitly disabled (and the 
	// fixed function pipeline is enabld) by calling glUseProgram with NULL
	glUseProgram(shaderProgram);
	
	modelLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	assert(modelLocation != 0xFFFFFFFF);
	
	solidColorLocation = glGetUniformLocation(shaderProgram, "objectColor");
	//assert(solidColorLocation != 0xFFFFFFFF);
	
	material = Material( getUniformLocation( shaderProgram, "object.ambientMat"),
						 getUniformLocation( shaderProgram, "object.diffuseMat"),
						 getUniformLocation( shaderProgram, "object.specularMat"),
						 getUniformLocation( shaderProgram, "object.specularExp"),
						 getUniformLocation( shaderProgram, "object.emissiveMat" ),
						 getUniformLocation( shaderProgram, "object.textureMapped") );
	
	//material.setAmbientAndDiffuseMat( objectColor );
	material.setAmbientAndDiffuseMat( vec4(1.0f) );
	material.setSpecularMat( vec4(1.0f, 1.0f, 1.0f, 1.0f) );
	material.setSpecularExponentMat( 64.0f );
	material.setTextureMapped(true);
}

void Model::initialize()
{
	textureObject = setupTexture( "aor1.bmp" );
	buildShaderProgram();

	GLuint VBO, IBO;
	
	glGenVertexArrays (1, &vertexArrayObject);
	glBindVertexArray( vertexArrayObject );

	vector<pntVertexData> v;
	int faces_size = Loader->getFaces().size();

	vector<vec3>& v_Pos = Loader->getVertexPos();
	vector<vec3>& v_Norm = Loader->getVertexNorm();
	vector<vec2>& v_text = Loader->getVertexTex();
	vector<Obj_face>& faces = Loader->getFaces();

	for(int i = 0; i < faces_size; i++)
	{
		int num_of_points = faces[i].index_of_vPos.size();

		for(int j = 0; j < num_of_points; j++)
		{
			int index_of_v_Pos = faces[i].index_of_vPos[j];
			int index_of_v_Norm = faces[i].index_of_v_Norm[j];
			int index_of_v_Text = faces[i].index_of_vTex[j];

			v.push_back( pntVertexData(v_Pos[ index_of_v_Pos ], v_Norm[ index_of_v_Norm ], v_text[index_of_v_Text]) );		
		}
		
	}

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, v.size() * sizeof(pntVertexData), &v[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), 0);
	glEnableVertexAttribArray(0);
	
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)sizeof(vec3) );
	glEnableVertexAttribArray(1);
	
	glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(pntVertexData), (const GLvoid*)(2 * sizeof(vec3)) );
	glEnableVertexAttribArray(3);
	
	vector<unsigned int> indices;

	for(int k = 0; k < v.size(); k++)
	{
		indices.push_back(k);
	}

	numberOfIndices = indices.size();

	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
}

void Model::draw()
{
	glUseProgram(shaderProgram);
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelAndFixed));
	glUniform4fv(solidColorLocation, 1, value_ptr(objectColor));
	
	material.setShaderMaterialProperties();
	
	glBindTexture(GL_TEXTURE_2D, textureObject);

	// Draw Model
	glBindVertexArray(vertexArrayObject);
	glDrawElements(GL_TRIANGLES, numberOfIndices, GL_UNSIGNED_INT, 0);

	// Draw all children
	VisualObject::draw();
}