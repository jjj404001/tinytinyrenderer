#pragma pack(push,1) // for tight pack for struct.
#include "../ImageFormats/TGAImage.h"
#include "../Line_Algorithm/Line.h"
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
			Bresenhams::Line(image, Vec2f(0, 0), Vec2f(i, j), Color(255, 0, 0, 255));
			image.SaveToTGAFile(std::to_string(i) + ',' + std::to_string(j) + ".tga", true);
		}
			
	}
	*/
	
	TGA_Image image(100, 100, 3);
	// 1
	Bresenhams::Line(image, Vec2f(49, 49), Vec2f(64, 79), Color(255, 0, 0, 255));
	Bresenhams::Line(image, Vec2f(49, 49), Vec2f(79, 64), Color(255, 0, 0, 255));
	// 2
	Bresenhams::Line(image, Vec2f(49, 49), Vec2f(34, 79), Color(0, 255, 0, 255));
	Bresenhams::Line(image, Vec2f(49, 49), Vec2f(19, 64), Color(0, 255, 0, 255));
	// 3
	Bresenhams::Line(image, Vec2f(49, 49), Vec2f(19, 34), Color(0, 0, 255, 255));
	Bresenhams::Line(image, Vec2f(49, 49), Vec2f(34, 19), Color(0, 0, 255, 255));
	// 4
	Bresenhams::Line(image, Vec2f(49, 49), Vec2f(64, 19), Color(255, 255, 0, 255));
	Bresenhams::Line(image, Vec2f(49, 49), Vec2f(79, 34), Color(255, 255, 0, 255));
	image.SaveToTGAFile("b.tga", true);
	
	//getchar();
	return 0;
}