#pragma once

template<typename T>
struct Vector3D
{
	T x = static_cast<T>(0);
	T y = static_cast<T>(0);
	T z = static_cast<T>(0);

	Vector3D(T _x, T _y, T _z);
};

template<typename T>
Vector3D<T>::Vector3D(T _x, T _y,T _z)
{
	x = _x;
	y = _y;
	z = _z;
}