#pragma pack(push,1) // for tight pack for struct.
#include "../ImageFormats/TGAImage.h"
#include "../Line_Algorithm/LineRasterizeAlgorithm.h"
#include "../Triangle_Algorithm/Triangle.h"
#include "../GeometryFormats/OBJGeometry.h"
#include <string>
int main(int argc, const char* argv)
{
	//OBJ_Geometry  obj("Cube.obj");
	
	TGA_Image image(100, 100, 3);

	//obj.DrawWireframe(image);
	Triangle tri0(Vec2i(0, 0), Vec2i(40, 40), Vec2i(10, 70));
	Triangle tri1(Vec2i(99, 99), Vec2i(40, 40), Vec2i(10, 70));
	Triangle tri2(Vec2i(0, 99), Vec2i(0, 0), Vec2i(10, 70));
	Color red(255, 0, 0, 255);
	Color green(0, 255, 0, 255);
	Color blue(0, 0, 255, 255);

	Triangle::TriangleRasterize(image, tri0, red);
	Triangle::TriangleRasterize(image, tri1, green);
	Triangle::TriangleRasterize(image, tri2, blue);
	
	image.SaveToTGAFile("a.tga", true);
	
	//getchar();
	return 0;
}