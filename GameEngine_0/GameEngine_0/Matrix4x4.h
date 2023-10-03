#pragma once
#include <memory>
#include "Vector3.h"
#include <cmath>
class Matrix4x4
{
public:
	Matrix4x4() 
	{
	}

	void setIdentity()
	{
		::memset(m_mat, 0, sizeof(float) * 16);
		m_mat[0][0] = 1;
		m_mat[1][1] = 1;
		m_mat[2][2] = 1;
		m_mat[3][3] = 1;
	}

	void setTrasnlation(const Vector3D& translation)
	{
		setIdentity();
		m_mat[3][0] = translation.m_x;
		m_mat[3][1] = translation.m_y;
		m_mat[3][2] = translation.m_z;
	}

	void setOrthLH(float width, float height, float near_plane, float far_plane) 
	{
		setIdentity();
		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (far_plane - near_plane);
		m_mat[3][2] = -(far_plane / (far_plane - near_plane));
	}
	~Matrix4x4()
	{
	}

public: 
	float m_mat[4][4] = {};
	
};