#pragma once
struct Color
{
	unsigned char red;
	unsigned char green;
	unsigned char blue;
	unsigned char alpha;

	bool operator==(Color _input);
	bool operator!=(Color _input);
};