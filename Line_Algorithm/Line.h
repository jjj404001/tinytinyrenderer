#pragma once
#include "../Math/Vector.h"
#include "../ImageFormats/TGAImage.h"


namespace Bresenhams
{
	void LineRasterize(TGA_Image& _target, const Vec2f& _start, const Vec2f& _end, const Color& _color);
}