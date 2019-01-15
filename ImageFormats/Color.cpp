#include "Color.h"

bool Color::operator==(Color _input)
{
	return (_input.red != red || _input.green != green || _input.blue != blue || _input.alpha != alpha);
}

bool Color::operator!=(Color _input)
{
	return !(*this == _input);
}
