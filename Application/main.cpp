#pragma pack(push,1) // for tight pack for struct.
#include "../ImageFormats/TGAImage.h"
#include "../Line_Algorithm/LineRasterizeAlgorithm.h"
#include <string>
int main(int argc, const char* argv)
{
	
	//TGA_Image image("a.tga");
	//image.SetPixel(1, 1, Color(255, 0, 0, 255));

	
	/*
	for (int i = 0; i < 30; i++)
	{
		for (int j = 0; j < 30; j++)
		{
			TGA_Image image(30, 30, 3);
			Bresenhams::Line(image, Vec2i(0, 0), Vec2i(i, j), Color(255, 0, 0, 255));
			image.SaveToTGAFile(std::to_string(i) + ',' + std::to_string(j) + ".tga", true);
		}
			
	}
	*/
	
	TGA_Image image(100, 100, 3);
	// 1
	Bresenhams::LineRasterize(image, Line(Vec2i(49, 49), Vec2i(64, 79)), Color(255, 0, 0, 255));
	Bresenhams::LineRasterize(image, Vec2i(49, 49), Vec2i(79, 64), Color(255, 0, 0, 255));
	// 2
	Bresenhams::LineRasterize(image, Vec2i(49, 49), Vec2i(34, 79), Color(0, 255, 0, 255));
	Bresenhams::LineRasterize(image, Vec2i(49, 49), Vec2i(19, 64), Color(0, 255, 0, 255));
	// 3
	Bresenhams::LineRasterize(image, Vec2i(49, 49), Vec2i(19, 34), Color(0, 0, 255, 255));
	Bresenhams::LineRasterize(image, Vec2i(49, 49), Vec2i(34, 19), Color(0, 0, 255, 255));
	// 4
	Bresenhams::LineRasterize(image, Vec2i(49, 49), Vec2i(64, 19), Color(255, 255, 0, 255));
	Bresenhams::LineRasterize(image, Vec2i(49, 49), Vec2i(79, 34), Color(255, 255, 0, 255));

	// Right angle test
	Bresenhams::LineRasterize(image, Vec2i(49, 49), Vec2i(49, 79), Color(0, 255, 255, 255));
	Bresenhams::LineRasterize(image, Vec2i(49, 49), Vec2i(79, 49), Color(0, 255, 255, 255));
	Bresenhams::LineRasterize(image, Vec2i(49, 49), Vec2i(49, 19), Color(0, 255, 255, 255));
	Bresenhams::LineRasterize(image, Vec2i(49, 49), Vec2i(19, 49), Color(0, 255, 255, 255));

	image.SaveToTGAFile("b.tga", true);
	
	//getchar();
	return 0;
}