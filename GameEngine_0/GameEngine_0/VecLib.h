#pragma once
typedef struct vector3
{
	struct vector3(){	}
	float x, y, z;
} vec3;

struct vertex
{
	struct vertex()
	{
		
	}
	vec3 position;
	struct vertex(float x, float y, float z)
	{
		position.x = x;
		position.y = y;
		position.z = z;
	}
	struct vertex(float* x, float* y, float* z)
	{
		position.x = *x;
		position.y = *y;
		position.z = *z;
	}
};
