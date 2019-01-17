#pragma pack(push,1) // for tight pack for struct.

#include "../ImageFormats/TGAImage.h"

int main(int argc, const char* argv)
{
	TGA_Image image(30, 30, 3);
	//TGA_Image image("a.tga");
	//image.SetPixel(1, 1, Color(255, 0, 0, 255));
	image.SaveToTGAFile("b.tga", true);

	//getchar();
	return 0;
}