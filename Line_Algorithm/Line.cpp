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

		const int x_step = (dx > 0) ? 1 : -1;
		const int y_step = (dy > 0) ? 1 : -1;
		
		int determinant = 2 * (dy * x_step) + (-dx * y_step);

		_target.SetPixel(x, y, _color);


		

		if (abs(dx) > abs(dy))
		{
			// x and y steps are sign of dx and dy.
			const int determinant_x_step = 2 * (dy * x_step);
			const int determinant_y_step = 2 * ((dy * x_step) + (-dx * y_step));


			while (num_of_pixel--)
			{
				if (determinant * x_step * y_step >= 0)
				{
					y += y_step;
					determinant += determinant_y_step;
				}
				else
					determinant += determinant_x_step;


				x += x_step;
				_target.SetPixel(x, y, _color);
			}
		}
		else
		{
			num_of_pixel = abs(dy);
			determinant = (2 * (-dx * y_step)) + (dy * x_step);

			const int determinant_x_step = 2 * ((-dx * y_step) + (dy * x_step));
			const int determinant_y_step = 2 * (-dx * y_step);


			while (num_of_pixel--)
			{
				if (determinant * x_step * y_step >= 0)
				{
					x += x_step;
					determinant += determinant_y_step;
				}
				else
					determinant += determinant_x_step;


				y += y_step;
				_target.SetPixel(x, y, _color);
			}
		}
	}
}