#include <iostream>
#include <sstream>
#include <fstream>
#include <cassert>
#include <random>
#include <utility>
#include <algorithm>
#include "OBJGeometry.h"
#include "../Line_Algorithm/LineRasterizeAlgorithm.h"

void OBJ_Geometry::SetWidth(float _new_width)
{
	width_ = _new_width;
}

void OBJ_Geometry::SetHeight(float _new_height)
{
	height_ = _new_height;
}

void OBJ_Geometry::SetWidthHeight(float _new_width, float _new_height)
{
	SetWidth(_new_width);
	SetHeight(_new_height);
}

void OBJ_Geometry::SetZoom(float _new_zoom)
{
	zoom_ = _new_zoom;
}

OBJ_Geometry::OBJ_Geometry(std::string _filename)
{
	LoadFromOBJFile(_filename);
}

OBJ_Geometry::OBJ_Geometry()
{
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

				geometric_vertices_.push_back(vertex * zoom_);
				
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

				texture_vertices_.push_back(vertex);

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

				vertex_normals_.push_back(vertex);

				continue;
			}
			else if (stream_segment == "f") // Vertex normal
			{
				float half_width = width_ / 2;
				float half_height = height_ / 2;
				// First line.
				buffer_stream >> stream_segment;
				Vec3f startVec3 = geometric_vertices_[std::stoi(stream_segment) - 1];
				Vec3f first_vertex = geometric_vertices_[std::stoi(stream_segment) - 1];
				const Vec3f veryFirstVec3 = startVec3;

				buffer_stream >> stream_segment;
				Vec3f endVec3 = geometric_vertices_[std::stoi(stream_segment) - 1];
				Vec3f second_vertex = geometric_vertices_[std::stoi(stream_segment) - 1];
				wire_frame_.push_back(Line(Vec2i(static_cast<int>(startVec3.x + half_width), static_cast<int>(startVec3.y + half_height)), 
										   Vec2i(static_cast<int>(endVec3.x + half_width), static_cast<int>(endVec3.y + half_height))));
				Vec3f third_vertex = second_vertex;
				bool isTri = false;
				while (buffer_stream >> stream_segment)
				{
					startVec3 = endVec3;

					endVec3 = geometric_vertices_[std::stoi(stream_segment) - 1];
					third_vertex = geometric_vertices_[std::stoi(stream_segment) - 1];
					wire_frame_.push_back(Line(Vec2i(static_cast<int>(startVec3.x + half_width), static_cast<int>(startVec3.y + half_height)),
											   Vec2i(static_cast<int>(endVec3.x + half_width), static_cast<int>(endVec3.y + half_height))));
					isTri++;
				}

				if (isTri)
				{
					Vec3f first((first_vertex.x + half_width), (first_vertex.y + half_height), first_vertex.z);
					Vec3f second((second_vertex.x + half_width), (second_vertex.y + half_height), second_vertex.z);
					Vec3f third((third_vertex.x + half_width), (third_vertex.y + half_height), third_vertex.z);

					Triangle tri(first, second, third);
					tri.SetNormal(Vec3f::CrossProduct((second -= first), (third -= first)));
					faces_.push_back(tri);
				}
					
				wire_frame_.push_back(Line(Vec2i(static_cast<int>(endVec3.x + half_width), static_cast<int>(endVec3.y + half_height)), 
										   Vec2i(static_cast<int>(veryFirstVec3.x + half_width), static_cast<int>(veryFirstVec3.y + half_height))));

				continue;
			}
		}
			
	}
	


	return true;
}

bool OBJ_Geometry::DrawWireframe(TGA_Image & _image, Color _color)
{
	int i = 0;
	const auto maximum = wire_frame_.size();
	for (auto line : wire_frame_)
	{
		Bresenhams::LineRasterize(_image, line, _color);
		std::cout << i++ << " of " << maximum << " lines." << std::endl;
	}
		

	return true;
}

bool OBJ_Geometry::DrawWireframeWithTriangle(TGA_Image & _image, Color _color)
{
	int i = 0;
	const auto maximum = faces_.size();
	auto view = Vec3f(0.0f, 0.0f, -1.0f);

	for (auto& triangles : faces_)
	{
		auto normal = triangles.GetNormal();
		if (Vec3f::DotProduct(normal, view) < 0.0f)
			continue;

		Triangle::TriangleOutlineRasterize(_image, triangles, _color);
		std::cout << i++ << " of " << maximum << " triangles." << std::endl;

		//_image.SaveToTGAFile(std::to_string(i) + ".tga", true);
	}


	return true;
}

bool OBJ_Geometry::DrawWithRandomColor(TGA_Image & _image)
{
	int i = 0;
	const auto maximum = faces_.size();

	std::random_device rd;
	std::mt19937_64 rng(rd());

	std::uniform_int_distribution<int> range(0, 255);



	for (auto triangles : faces_)
	{
		Color rand_color(range(rng), range(rng), range(rng));
		Triangle::TriangleRasterize(_image, triangles, rand_color);
		std::cout << i++ << " of " << maximum << " triangles." << std::endl;
	}

	return true;
}

bool OBJ_Geometry::DrawWithFlatColor(TGA_Image & _image, Color _color)
{
	int i = 0;
	const auto maximum = faces_.size();
	auto view = Vec3f(0.0f, 0.0f, -1.0f);

	for (auto& triangle : faces_)
	{
		auto normal = triangle.GetNormal();
		if (Vec3f::DotProduct(normal, view) < 0.0f)
			continue;

		
		Triangle::TriangleRasterize(_image, triangle, _color);
		std::cout << i++ << " of " << maximum << " triangles." << std::endl;
	}

	return true;
}

bool OBJ_Geometry::DrawWithFlatLight(TGA_Image & _image)
{
	int i = 0;
	const auto maximum = faces_.size();
	auto view = Vec3f(0.0f, 0.0f, -1.0f);

	for (auto& triangle : faces_)
	{
		Vec3f normal = triangle.GetNormal();
		normal.Normalize();
		if (Vec3f::DotProduct(normal, view) < 0.0f)
			continue;

		float light_amount = Vec3f::DotProduct(normal, view);
		int light_color = static_cast<int>(255 * light_amount);

		Color color(light_color, light_color, light_color);

		Triangle::TriangleRasterize(_image, triangle, color);
		std::cout << i++ << " of " << maximum << " triangles." << std::endl;
	}

	return true;
}
