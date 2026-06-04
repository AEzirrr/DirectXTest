#pragma once
#include <memory.h>
#include "Vector3D.h"

class Matrix4x4 {

public:
	Matrix4x4() {}

	void setIdentity() {

		::memset(m_mat, 0, sizeof(float) * 16);
		m_mat[0][0] = 1.0f;
		m_mat[1][1] = 1.0f;
		m_mat[2][2] = 1.0f;
		m_mat[3][3] = 1.0f;
	}

	void setTranslation(const Vector3D& translation) {

		setIdentity();
		m_mat[3][0] = translation.x;
		m_mat[3][1] = translation.y;
		m_mat[3][2] = translation.z;

	}

	void setScale(const Vector3D& scale) {

		setIdentity();
		m_mat[0][0] = scale.x;
		m_mat[1][1] = scale.y;
		m_mat[2][2] = scale.z;

	}

	void operator *=(const Matrix4x4& matrix) {
		Matrix4x4 out; 

		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				out.m_mat[i][j] =
					m_mat[i][0] * matrix.m_mat[0][j] +
					m_mat[i][1] * matrix.m_mat[1][j] +
					m_mat[i][2] * matrix.m_mat[2][j] +
					m_mat[i][3] * matrix.m_mat[3][j];
			}
		}
		// Copy the final calculated matrix back into this object
		::memcpy(m_mat, out.m_mat, sizeof(float) * 16);
	}

	void setOrthoLH(float width, float height, float z_near, float z_far) {
		setIdentity();
		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (z_far - z_near);
		m_mat[3][2] = -(z_near / (z_far - z_near));
	}

	~Matrix4x4() {}

private:
	float m_mat[4][4] = {};
};