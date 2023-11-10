#pragma once

struct Vector2D
{
	struct Vector2D() : m_x(0), m_y(0)
	{

	}
	struct Vector2D(float x, float y) : m_x(x), m_y(y)
	{

	}
	struct Vector2D(const Vector2D& vec) : m_x(vec.m_x), m_y(vec.m_y)
	{

	}
	union 
	{
		struct
		{
				float m_x;
				float m_y;
		};
		struct
		{
				float right;
				float forward;
		} Direction;
	};
	
};