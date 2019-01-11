#include "../ImageFormats/TGAImage.h"

int main(int argc, const char* argv)
{
	TGA_Image image("a.tga");
	image.SaveToTGAFile("b.tga", false);

	getchar();
	return 0;
}