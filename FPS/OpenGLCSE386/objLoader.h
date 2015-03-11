#ifndef _OBJ_LOADER_H_
#define _OBJ_LOADER_H_

/*
* @author: Yan Yu
* This library help to load 3D model made by other 3D modeling software like Maya, 3dx
* It only process .obj file format at this time
*/

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

#include "vertexStructs.h"
#include "from_string_to_number.h"

using namespace std;

	//Obj file face information format: index of v/index of vt/index of vn, for instance 1/2/3
	struct Obj_face
	{
		vector<int> index_of_vPos;
		vector<int> index_of_vTex;
		vector<int> index_of_v_Norm;
	};

	class Obj_Loader
	{
	public:
		Obj_Loader(std::string file_name);

		void read_obj_file();

		/*
		*	split a string into tokens based on the delimiter provided
		*/
		friend vector<string> tokenize_line(std::string &line, char delimiter);

		// getter: implicitly inline methods
		vector<vec3>& getVertexPos() {return vertexPos;}
		vector<vec3>& getVertexNorm() {return vertexNorm;}
		vector<vec2>& getVertexTex() {return vertexTex;}
		vector<Obj_face>& getFaces() {return faces;}

	protected:
		bool initilize_file_stream(std::string file_name);

		void close_file_stream();

		/*
		*	fetch vertex position data from tokens vector
		*	@para:tokens vector stores the tokens of a data line that being read from the obj file
		*/
		void fetch_vertexPosData( vector<string> tokens );

		/*
		*	fetch vertex normal data from tokens vector
		*	@para:tokens vector stores the tokens of a data line that being read from the obj file
		*/
		void fetch_vertexNormData( vector<string> tokens );

		/*
		*	fetch vertex texture data from tokens vector
		*	@para:tokens vector stores the tokens of a data line that being read from the obj file
		*/
		void fetch_vertexTextureData( vector<string> tokens );

		/*
		*	fetch face data from tokens vector
		*	@para:tokens vector stores the tokens of a data line that being read from the obj file
		*/
		void fetch_faceData( vector<string> tokens );

		std::ifstream *ifs;

		bool fstream_flag;

		vector<vec3> vertexPos;
		vector<vec3> vertexNorm;
		vector<vec2> vertexTex;

		vector<Obj_face> faces;
		//vector<int[3][3]> faces; 
	};

#endif // !_OBJ_LOADER_H_
