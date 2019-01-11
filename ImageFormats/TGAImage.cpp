#pragma pack(push,1) // for tight pack for struct.

#include <iostream>
#include <fstream>
#include "TGAImage.h"

TGA_Image::TGA_Image(std::string _filename) : filename_(_filename)
{
	LoadFromTGAFile(_filename);
}

bool TGA_Image::LoadFromTGAFile(std::string _filename)
{
	data_.clear();

	std::ifstream input_file(_filename, std::ios_base::binary);

	if (!input_file.is_open())
	{
		std::cerr << "Can't open file : " << _filename.c_str() << "\n";
		input_file.close();
		return false;
	}

	TGA_Header header;
	input_file.read((char *)&header, sizeof(header));
	if (!input_file.good()) 
	{
		std::cerr << "Can't read header from file : " << _filename.c_str() << ".\n";
		input_file.close();
		return false;
	}

	width_ = header.image_spec_.width_;
	height_ = header.image_spec_.height_;
	byte_per_pixel = header.image_spec_.bits_per_pixel_ >> 3;

	if (width_ <= 0)
	{
		std::cerr << "Width is <= 0 from file : " << _filename.c_str() << ".\n";
		input_file.close();
		return false;
	}

	if (height_ <= 0)
	{
		std::cerr << "Height is <= 0 from file : " << _filename.c_str() << ".\n";
		input_file.close();
		return false;
	}

	if (height_ <= 0)
	{
		std::cerr << "Height is <= 0 from file : " << _filename.c_str() << ".\n";
		input_file.close();
		return false;
	}

	if (byte_per_pixel != static_cast<char>(TGA_Format::GRAYSCALE)
	 && byte_per_pixel != static_cast<char>(TGA_Format::RGB)
	 && byte_per_pixel != static_cast<char>(TGA_Format::RGBA))
	{
		std::cerr << "Undefined bit per pixel from file : " << _filename.c_str() << ".\n";
		input_file.close();
		return false;
	}

	unsigned long nPixels = width_ * height_;

	switch (header.image_type_)
	{
		case static_cast<char>(TGA_Type::NO_IMAGE) :
			std::cout << "No image data included." << std::endl;
			break;
		case static_cast<char>(TGA_Type::UNCOMP_COLOR_MAPPED) :
			// TODO
			break;
		case static_cast<char>(TGA_Type::UNCOMP_TRUE_COLOR) :
		case static_cast<char>(TGA_Type::UNCOMP_BLACK_WHITE) :

			while (nPixels)
			{
				Color pixel;

				input_file.read((char*)&pixel.blue, sizeof(unsigned char));
				input_file.read((char*)&pixel.green, sizeof(unsigned char));
				input_file.read((char*)&pixel.red, sizeof(unsigned char));
				input_file.read((char*)&pixel.alpha, sizeof(unsigned char));
				
				data_.push_back(pixel);
				nPixels--;
			}
			

			break;
		case static_cast<char>(TGA_Type::RLE_COLOR_MAPPED) :
			// TODO
			break;
		case static_cast<char>(TGA_Type::RLE_TRUE_COLOR) :
		case static_cast<char>(TGA_Type::RLE_BLACK_WHITE) :
			// TODO
			break;
	}

	if (!(static_cast<int>(header.image_spec_.image_origin_) & static_cast<int>(TGA_Origin::FLIP_VERTICALLY)))
	{
		// TODO: flip vertically
	}
	if ((static_cast<int>(header.image_spec_.image_origin_) & static_cast<int>(TGA_Origin::FLIP_HORIZONTALLY)))
	{
		// TODO: flip horizontally
	}


	filename_ = _filename;

	input_file.close();
	return true;
}
