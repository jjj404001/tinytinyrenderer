#pragma pack(push,1) // for tight pack for struct.
#include "../ImageFormats/TGAImage.h"
#include "../Line_Algorithm/LineRasterizeAlgorithm.h"
#include "../GeometryFormats/OBJGeometry.h"
#include <string>
int main(int argc, const char* argv)
{
	OBJ_Geometry  obj("Cube.obj");
	
	TGA_Image image(100, 100, 3);

	obj.DrawWireframe(image);

	image.SaveToTGAFile("b.tga", true);
	
	//getchar();
	return 0;
}