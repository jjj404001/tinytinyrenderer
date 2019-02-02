#include "Line.h"
#include <cassert>
namespace Bresenhams
{
	void Line(TGA_Image& _target, const Vec2f& _start, const Vec2f& _end, const Color& _color)
	{
		if (!_target.IsInsideBoundary(_start))
		{
			assert(!"Starting value is out of boundary.");
			return;
		}
			
		if (!_target.IsInsideBoundary(_end))
		{
			assert(!"Ending value is out of boundary.");
			return;
		}
			

		const int dy = static_cast<int>(_end.y - _start.y);
		const int dx = static_cast<int>(_end.x - _start.x);


		int x = static_cast<int>(_start.x);
		int y = static_cast<int>(_start.y);

		const float slope = static_cast<float>(dy) / dx;
		const int y_intersect = y - static_cast<int>(slope * x);

		

		int num_of_pixel = abs(dx);

		const int x_step_sign = (dx > 0) ? 1 : -1;
		const int y_step_sign = (dy > 0) ? 1 : -1;

		const int x_step = 2 * dy;
		const int y_step = 2 * -dx;
		
		int determinant = 2 * (dy) + (-dx);

		// Plot initial pixel on starting point.
		_target.SetPixel(x, y, _color);

		auto even_octant = [](int const& _determinant) { return _determinant >= 0; };
		auto odd_octant  = [](int const& _determinant) { return _determinant <= 0; };

		bool(*determining_function)(int const&) = even_octant;

		// TODO : Using references maybe more readable.
		int* major_axis = &x;
		int* minor_axis = &y;

		int major_step = x_step;
		int minor_step = y_step;

		// TODO : Maybe this if statement can be replaced with ? : notation.
		if (abs(dy) > abs(dx))
		{
			num_of_pixel = abs(dy);
			determinant = dy + (2 * -dx); // TODO : Change determinant for all cases.
			determining_function = odd_octant;

			major_axis = &y;
			minor_axis = &x;
			std::swap(major_step, minor_step);
		}

		while (--num_of_pixel)
		{
			if (determining_function(determinant))
			{
				determinant += minor_step;
				(*minor_axis)++; // TODO : Change ++ to minor_sign using x and y step_signs
			}

			determinant += major_step;
			(*major_axis)++; // TODO : Change ++ to minor_sign using x and y step_signs
			_target.SetPixel(x, y, _color);
		}
	}
}