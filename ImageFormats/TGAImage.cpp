#pragma pack(push,1) // for tight pack for struct.

#include <algorithm>
#include <utility>
#include <iostream>
#include <fstream>
#include <cassert>

#include "TGAImage.h"

void TGA_Image::FlipVertically()
{
	std::reverse(std::begin(data_), std::end(data_));
	FlipHorizontally();
}

void TGA_Image::FlipHorizontally()
{
	int nPixel = width_ * height_;
	for (int current_line = 0; current_line < nPixel; current_line += width_)
	{
		auto begin = std::begin(data_) + current_line;
		auto end   = std::begin(data_) + current_line + width_;
		std::reverse(begin, end);
	}
	
}

bool TGA_Image::UncompressRLE(std::ifstream& _input_file)
{
	auto nPixels = width_ * height_;

	
	if (header_.image_spec_.bits_per_pixel_ == 32)
	{
		while (nPixels)
		{
			assert(nPixels > 0); // if number of remaining pixel is less than 0, something is wrong.
			unsigned char chunk_header = _input_file.get();
			

			if (chunk_header >= 128) // if 8th bit is exist.
			{
				chunk_header -= 127;

				Color pixel;

				_input_file.read((char*)&pixel.blue, sizeof(unsigned char));
				_input_file.read((char*)&pixel.green, sizeof(unsigned char));
				_input_file.read((char*)&pixel.red, sizeof(unsigned char));
				_input_file.read((char*)&pixel.alpha, sizeof(unsigned char));

				for (int i = 0; i < chunk_header; i++)
					data_.push_back(pixel);
			}
			else
			{
				chunk_header++;


				for (int i = 0; i < chunk_header; i++)
				{
					Color pixel;

					_input_file.read((char*)&pixel.blue, sizeof(unsigned char));
					_input_file.read((char*)&pixel.green, sizeof(unsigned char));
					_input_file.read((char*)&pixel.red, sizeof(unsigned char));
					_input_file.read((char*)&pixel.alpha, sizeof(unsigned char));

					data_.push_back(pixel);
				}
			}



			nPixels -= chunk_header;
		}

	}
	else if (header_.image_spec_.bits_per_pixel_ == 24)
	{
		while (nPixels)
		{
			assert(nPixels > 0); // if number of remaining pixel is less than 0, something is wrong.
			unsigned char chunk_header = _input_file.get();
			

			if (chunk_header >= 128) // if 8th bit is exist.
			{
				chunk_header -= 127;

				Color pixel;

				_input_file.read((char*)&pixel.blue, sizeof(unsigned char));
				_input_file.read((char*)&pixel.green, sizeof(unsigned char));
				_input_file.read((char*)&pixel.red, sizeof(unsigned char));
				pixel.alpha = 255;

				for (int i = 0; i < chunk_header; i++)
					data_.push_back(pixel);
			}
			else
			{
				chunk_header++;


				for (int i = 0; i < chunk_header; i++)
				{
					Color pixel;

					_input_file.read((char*)&pixel.blue, sizeof(unsigned char));
					_input_file.read((char*)&pixel.green, sizeof(unsigned char));
					_input_file.read((char*)&pixel.red, sizeof(unsigned char));
					pixel.alpha = 255;

					data_.push_back(pixel);
				}
			}
				


				
			nPixels -= chunk_header;
		}
	}


	return true;
}

bool TGA_Image::CompressRLE(std::ofstream & _output_file)
{
	auto nPixels = width_ * height_;


	auto chunck_begin = std::begin(data_);
	auto color_size = sizeof(Color); // default = 32bits

	if (header_.image_spec_.bits_per_pixel_ == 24)
		color_size -= 1;

	while(nPixels)
	{
		unsigned char chunk_header = 0;
		auto chunck_end = std::end(data_);
		chunck_end = std::adjacent_find(chunck_begin, chunck_end, [](Color _i, Color _j) { return _i == _j; });

		auto chunk_size = (chunck_end - chunck_begin);
		if (!chunk_size)
		{
			chunck_end = std::end(data_);
			chunck_end = std::adjacent_find(chunck_begin, chunck_end, [](Color _i, Color _j) { return _i != _j; });
			chunk_size = (chunck_end - chunck_begin);
			if (chunk_size > 128)
			{
				chunck_end = chunck_begin + 128; // Max length of chunk is 128.;
				chunk_size = (chunck_end - chunck_begin);
			}

			nPixels -= chunk_size;
			chunk_header += chunk_size-1; //-1 To map 1~256 to 0~255 

			_output_file.put(chunk_header);
			std::for_each(chunck_begin, chunck_end, [&] (Color _input_color)
			{
				auto temp = _input_color.red;
				_input_color.red = _input_color.blue;
				_input_color.blue = temp;
				_output_file.write(reinterpret_cast<char*>(&_input_color), color_size);
			}
			);
		}
		else
		{
			if (chunk_size > 128)
				chunk_size = 128;

			nPixels -= chunk_size;

			chunk_header += 128 + chunk_size-1; //-1 To map 1~256 to 0~255 

			_output_file.put(chunk_header);
			Color color = *chunck_begin;
			auto temp = color.red;
			color.red = color.blue;
			color.blue = temp;

			_output_file.write(reinterpret_cast<char*>(&color), color_size);
		}

		if(chunck_end != std::end(data_))
			chunck_begin = chunck_begin + chunk_size;
	}
	

	return true;
}

TGA_Image::TGA_Image(std::string _filename) : filename_(_filename)
{
	LoadFromTGAFile(_filename);
}

TGA_Image::~TGA_Image()
{
	data_.clear();
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


	input_file.read((char *)&header_, sizeof(header_));
	if (!input_file.good()) 
	{
		std::cerr << "Can't read header from file : " << _filename.c_str() << ".\n";
		input_file.close();
		return false;
	}

	width_ = header_.image_spec_.width_;
	height_ = header_.image_spec_.height_;
	byte_per_pixel = header_.image_spec_.bits_per_pixel_ >> 3;

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

	if (byte_per_pixel != TGA_Format::GRAYSCALE
	 && byte_per_pixel != TGA_Format::RGB
	 && byte_per_pixel != TGA_Format::RGBA)
	{
		std::cerr << "Undefined bit per pixel from file : " << _filename.c_str() << ".\n";
		input_file.close();
		return false;
	}

	unsigned long nPixels = width_ * height_;

	switch (header_.image_type_)
	{
		case static_cast<char>(TGA_Type::NO_IMAGE) :
			std::cout << "No image data included." << std::endl;
			break;
		case static_cast<char>(TGA_Type::UNCOMP_COLOR_MAPPED) :
			// TODO
			break;
		case static_cast<char>(TGA_Type::UNCOMP_TRUE_COLOR) :
		case static_cast<char>(TGA_Type::UNCOMP_BLACK_WHITE) :

			if (header_.image_spec_.bits_per_pixel_ == 32)
			{
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
			}
			else if (header_.image_spec_.bits_per_pixel_ == 24)
			{
				while (nPixels)
				{
					Color pixel;

					input_file.read((char*)&pixel.blue, sizeof(unsigned char));
					input_file.read((char*)&pixel.green, sizeof(unsigned char));
					input_file.read((char*)&pixel.red, sizeof(unsigned char));
					pixel.alpha = 255;

					data_.push_back(pixel);
					nPixels--;
				}
			}
			
			

			break;
		case static_cast<char>(TGA_Type::RLE_COLOR_MAPPED) :
		// TODO: RLE comp?
		break;
		case static_cast<char>(TGA_Type::RLE_TRUE_COLOR) :
		case static_cast<char>(TGA_Type::RLE_BLACK_WHITE) :
			UncompressRLE(input_file);
		break;
	}

	if (!(static_cast<int>(header_.image_spec_.image_origin_) & TGA_Origin::FLIP_VERTICALLY))
		FlipVertically();

		
	if ((static_cast<int>(header_.image_spec_.image_origin_) & TGA_Origin::FLIP_HORIZONTALLY))
		FlipHorizontally();



	filename_ = _filename;

	input_file.close();
	return true;
}

bool TGA_Image::SaveToTGAFile(std::string _filename, bool is_compress)
{
	unsigned char developer_area[] = { 0, 0, 0, 0 }; // Let dev area empty.
	unsigned char extension_area[] = { 0, 0, 0, 0 }; // Let ext area empty.
	unsigned char signature_string[] = { 'T','R','U','E','V','I','S','I','O','N','-','X','F','I','L','E','.','\0' };

	std::ofstream output_file(_filename, std::ios_base::binary);

	if (!output_file.is_open())
	{
		std::cerr << "Can't save file : " << _filename.c_str() << ".\n";
		output_file.close();
		return false;
	}
	TGA_Header header;
	memset(&header, 0, sizeof(header));
	header.image_spec_.bits_per_pixel_ = byte_per_pixel << 3;
	header.image_spec_.width_ = width_;
	header.image_spec_.height_ = height_;
	header.image_type_ = (static_cast<int>(byte_per_pixel) == TGA_Format::GRAYSCALE ?
						 (is_compress ? TGA_Type::RLE_BLACK_WHITE : TGA_Type::UNCOMP_BLACK_WHITE) 
					   : (is_compress ? TGA_Type::RLE_TRUE_COLOR : TGA_Type::UNCOMP_TRUE_COLOR));
	header.image_spec_.image_origin_ = 0b100000; // top-left origin

	output_file.write((char *)&header, sizeof(header));
	if (!output_file.good()) 
	{
		output_file.close();
		std::cerr << "Can't save tga file header.\n";
		return false;
	}

	if (!is_compress)
	{
		auto color_size = sizeof(Color); // default = 32bits

		if (header.image_spec_.bits_per_pixel_ == 24)
			color_size -= 1;


		for (auto& itr : data_)
		{
			// TGA pixel color is in form of : BGRA.
			// So swap RGBA to BGRA
			auto temp = itr.red;
			itr.red = itr.blue;
			itr.blue = temp;

			output_file.write(reinterpret_cast<char*>(&itr), color_size);
			if (!output_file.good())
			{
				std::cerr << "Can't save raw data\n";
				output_file.close();
				return false;
			}
		}
	}
	else
		CompressRLE(output_file);


	output_file.write((char *)developer_area, sizeof(developer_area));
	if (!output_file.good()) 
	{
		std::cerr << "Can't save developer area to tga file\n";
		output_file.close();
		return false;
	}
	output_file.write((char *)extension_area, sizeof(extension_area));
	if (!output_file.good()) 
	{
		std::cerr << "Can't save extension area to tga file\n";
		output_file.close();
		return false;
	}
	output_file.write((char *)signature_string, sizeof(signature_string));
	if (!output_file.good()) 
	{
		std::cerr << "Can't save signature string to tga file\n";
		output_file.close();
		return false;
	}
	output_file.close();
	return true;
}
