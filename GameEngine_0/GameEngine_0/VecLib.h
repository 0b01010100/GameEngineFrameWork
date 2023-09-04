#pragma once
typedef struct vect3
{
	float x, y, z;
} vec3;

struct vertex
{
	vertex()
	{
		
	}
	vec3 position;
	vertex(float x, float y, float z)
	{
		position.x = x;
		position.y = y;
		position.z = z;
	}
	vertex(float* x, float* y, float* z)
	{
		position.x = *x;
		position.y = *y;
		position.z = *z;
	}
};
