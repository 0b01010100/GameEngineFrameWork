#pragma once

class Vector3D
{
public:
	Vector3D() : m_x(0), m_y(0), m_z(0)
	{
	
	}
	Vector3D(float x, float y, float z) : m_x(x), m_y(y), m_z(z)
	{

	}
	Vector3D(const Vector3D& vector) : m_x(vector.m_x), m_y(vector.m_y), m_z(vector.m_z)
	{

	}

	static Vector3D lerp(Vector3D Start, Vector3D End, float delta)
	{
		Vector3D v;
		v.m_x = Start.m_x * (1.0f - delta) + End.m_x * (delta);
		v.m_y = Start.m_y * (1.0f - delta) + End.m_y * (delta);
		v.m_z = Start.m_z * (1.0f - delta) + End.m_z * (delta);



		return v;
	}

	~Vector3D()
	{

	}
public:
	float m_x, m_y, m_z;
};

struct vertex
{
	struct Vector3D position;
	struct Vector3D position1;
	struct Vector3D color;
	struct Vector3D color1;
};