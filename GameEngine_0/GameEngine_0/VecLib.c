#pragma once
struct vector3
{
	float x, y, z;
	struct vector3(){}
	struct vector3(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	
};
struct vertex
{
	struct vector3 position;
	struct vector3 Color;
	struct vertex(float x, float y, float z, float r, float g, float b)
	{
		position.x = x;
		position.y = y;
		position.z = z;
		Color.x = r;
		Color.y = g;
		Color.z = b;
	}
	
	
};