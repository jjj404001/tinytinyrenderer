// filename : TGAImage.h
// author   : Jaejun Jang
// email    : jjj404001@gmail.com
// brief    : Image file structure for TGA image format.

// TGA format details:
// http://tfc.duke.free.fr/coding/tga_specs.pdf


#include <vector>
#include "Color.h"



class TGA_Image
{
    // header : 
    // ID Length 1 byte
    // Color Map Type 1 byte
    // Image Type 1 byte
    // Color Map Specification 5 bytes
    // - color map origin 2 bytes
    // - color map length 2 bytes
    // Color map deph 1 byte
    // Image Specification 10 bytes
    // - x origin 2 bytes
    // - y origin 2 bytes
    // - width 2 bytes
    // - height 2 bytes
    // - bits per pixel 1 byte
    // - image descriptor 1 byte
    struct TGA_Header
    {
        struct ColorMapSpec
        {
            short first_entry_index_;
            short colorMap_length_;
			char colorMap_entry_size_;
        };

        struct ImageSpec
        {
            short x_origin_;
            short y_origin_;
            short width_;
            short height_;
            char bits_per_pixel_;
            char image_origin_;
        };
        char id_length_;
        char colorMap_type_;
        char image_type_;
        ColorMapSpec colorMap_spec_;
        ImageSpec image_spec_;
    };

	void FlipVertically();
	void FlipHorizontally();

	//https://en.wikipedia.org/wiki/Run-length_encoding
	bool UncompressRLE(std::ifstream& _input_file);
	bool CompressRLE(std::ofstream& _input_file);



	std::string filename_;
	TGA_Header header_;
	short width_;
	short height_;
	char byte_per_pixel;
	std::vector<Color> data_;

public:
	TGA_Image(std::string _filename);
	~TGA_Image();
	bool LoadFromTGAFile(std::string _filename);
	bool SaveToTGAFile(std::string _filename, bool is_compress);

	enum TGA_Format
	{
		GRAYSCALE = 1, RGB = 3, RGBA = 4
	};

	// Image type
	// 0   No Image Data Included
	// 1   Uncompressed, Color - mapped Image
	// 2   Uncompressed, True - color Image
	// 3   Uncompressed, Black - and-white Image
	// 9   Run - length encoded, Color - mapped Image
	// 10  Run - length encoded, True - color Image
	// 11  Run - length encoded, Black - and-white Image
	enum TGA_Type
	{
		NO_IMAGE = 0,  UNCOMP_COLOR_MAPPED = 1, UNCOMP_TRUE_COLOR = 2, UNCOMP_BLACK_WHITE = 3,
		RLE_COLOR_MAPPED = 9, RLE_TRUE_COLOR = 10, RLE_BLACK_WHITE = 11,
	};

	// Image origin	
	// Screen destination	: bit 5 : bit 4
	// of first pixel	    :       :     
	//	bottom left			:      0:     0
	//	bottom right		:      0:     1
	//	top left			:      1:     0
	//	top right			:      1:     1
	enum TGA_Origin // TODO: change to byte code.
	{
		FLIP_HORIZONTALLY = 0b10000, FLIP_VERTICALLY = 0b100000
	};
};