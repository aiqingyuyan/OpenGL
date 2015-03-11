#include "objLoader.h"

Obj_Loader::Obj_Loader(std::string file_name) 
{
	fstream_flag = initilize_file_stream(file_name);
}

void Obj_Loader::read_obj_file()
{
	if( fstream_flag )
	{
		string line;
		int line_num = 0;;

		while( !ifs->eof() )
		{
			getline( *ifs, line );
			if(line.empty() || ( (line[0] != 'v') && (line[0] != 'f') )) continue;

			vector<string> tokens_of_the_line = tokenize_line(line, ' ');

			//cout<<tokens_of_the_line[0]<<endl;

			if( tokens_of_the_line[0] == "v" )					// vertex position data
			{
				//cout<<"vertex position data"<<endl;
				fetch_vertexPosData(tokens_of_the_line);
			}
			else if ( tokens_of_the_line[0] == "vt" )			// vertex texture data
			{
				//cout<<"vertex texture data"<<endl;
				fetch_vertexTextureData(tokens_of_the_line);
			}
			else if ( tokens_of_the_line[0] == "vn" )			// vertex normal data
			{
				//cout<<"vertex normal data"<<endl;
				fetch_vertexNormData(tokens_of_the_line);
			}
			else												// face data
			{
				//cout<<"face data"<<endl;
				fetch_faceData(tokens_of_the_line);
			}

		}

		close_file_stream();
	}

	cout<<vertexPos.size()<<endl
		<<vertexNorm.size()<<endl
		<<vertexTex.size()<<endl
		<<faces.size()<<endl;
}

void Obj_Loader::fetch_vertexPosData( vector<string> tokens ) 
{
	vec3 temp = vec3(0.0f);
	float t = .0f;

	from_string_to_number(t, tokens[1], std::dec);
	temp.x = t;

	from_string_to_number(t, tokens[2], std::dec);
	temp.y = t;

	from_string_to_number(t, tokens[3], std::dec);
	temp.z = t;

	vertexPos.push_back(temp);
}

void Obj_Loader::fetch_vertexNormData( vector<string> tokens ) 
{
	vec3 temp = vec3(0.0f);
	float t = .0f;

	from_string_to_number(t, tokens[1], std::dec);
	temp.x = t;

	from_string_to_number(t, tokens[2], std::dec);
	temp.y = t;

	from_string_to_number(t, tokens[3], std::dec);
	temp.z = t;

	vertexNorm.push_back(temp);
}

void Obj_Loader::fetch_vertexTextureData( vector<string> tokens ) 
{
	vec2 temp = vec2(0.0f);
	float t = .0f;

	from_string_to_number(t, tokens[1], std::dec);
	temp.x = t;

	from_string_to_number(t, tokens[2], std::dec);
	temp.y = t;

	vertexTex.push_back(temp);
}

void Obj_Loader::fetch_faceData(vector<string> tokens) 
{
	Obj_face t;
	for(int i = 1; i < tokens.size(); i++)
	{
		vector<string> temp = tokenize_line(tokens[i], '/');
		//cout<<temp[j]<<endl;
		int index;

		from_string_to_number(index, temp[0], std::dec);
		t.index_of_vPos.push_back( index - 1 );

		from_string_to_number(index, temp[1], std::dec);
		t.index_of_vTex.push_back( index - 1 );

		from_string_to_number(index, temp[2], std::dec);
		t.index_of_v_Norm.push_back( index - 1);
	}

	faces.push_back(t);
}

vector<string> tokenize_line(std::string &line, char delimiter)
{
	stringstream ssm(line);
	string token;
	vector<string> tokens;

	while( getline(ssm, token, delimiter) )
	{
		//cout<<token<<endl;
		tokens.push_back(token);
	}

	return tokens;
}

bool Obj_Loader::initilize_file_stream(std::string file_name)
{
	ifs = new ifstream(file_name);

	if( !ifs->is_open() ) 
	{
		cout<<"Error! Cannot open "<<file_name<<endl;
		return false;
	}

	return true;
}

void Obj_Loader::close_file_stream() { ifs->close(); } 