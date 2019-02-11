#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include "OBJGeometry.h"


OBJ_Geometry::OBJ_Geometry(std::string _filename)
{
	LoadFromOBJFile(_filename);
}

bool OBJ_Geometry::LoadFromOBJFile(std::string _filename)
{
	std::ifstream input_file(_filename);

	if (!input_file.is_open())
	{
		std::cerr << "Can't open file : " << _filename.c_str() << "\n";
		input_file.close();
		return false;
	}
	
	std::string buffer;
	while (getline(input_file, buffer))
	{
		// If line is comment, skip.
		if ((*std::begin(buffer)) == '#')
			continue;

		std::stringstream buffer_stream(buffer);
		std::string stream_segment;
		while (buffer_stream >> stream_segment)
		{
			if (stream_segment == "mtllib") // Material library
			{
				buffer_stream >> stream_segment; // Assume that there are no space character in file name.
				material_library_name_ = stream_segment;
				// TODO : LoadFromMTLFile() or something.
				continue;
			}
			else if (stream_segment == "o") // Object name
			{
				buffer_stream >> stream_segment; // Assume that there are no space character in object name.
				// TODO
				continue;
			}
			else if (stream_segment == "v") // Geometric vertex
			{
				Vec3f vertex(0.0f, 0.0f, 0.0f);

				buffer_stream >> stream_segment;
				vertex.x = std::stof(stream_segment);

				buffer_stream >> stream_segment;
				vertex.y = std::stof(stream_segment);

				buffer_stream >> stream_segment;
				vertex.z = std::stof(stream_segment);

				geometric_vertices.push_back(vertex);
				
				continue;
			}
			else if (stream_segment == "vt") // Texture vertex
			{
				Vec3f vertex(0.0f, 0.0f, 0.0f);

				buffer_stream >> stream_segment;
				vertex.x = std::stof(stream_segment);

				buffer_stream >> stream_segment;
				vertex.y = std::stof(stream_segment);

				buffer_stream >> stream_segment;
				vertex.z = std::stof(stream_segment);

				texture_vertices.push_back(vertex);

				continue;
			}
			else if (stream_segment == "vn") // Vertex normal
			{
				Vec3f vertex(0.0f, 0.0f, 0.0f);

				buffer_stream >> stream_segment;
				vertex.x = std::stof(stream_segment);

				buffer_stream >> stream_segment;
				vertex.y = std::stof(stream_segment);

				buffer_stream >> stream_segment;
				vertex.z = std::stof(stream_segment);

				vertex_normals.push_back(vertex);

				continue;
			}
				
		}
			
	}
	


	return true;
}
