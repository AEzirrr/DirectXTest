#pragma once
#include <memory.h>
#include <cmath>
#include "Vector3D.h"
#include "Vector4D.h"

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

	void setRotationX(float x) {
		setIdentity();
		m_mat[1][1] = cosf(x);
		m_mat[1][2] = -sinf(x);
		m_mat[2][1] = sinf(x);
		m_mat[2][2] = cosf(x);
	}

	void setRotationY(float y) {
		setIdentity();
		m_mat[0][0] = cosf(y);
		m_mat[0][2] = sinf(y);
		m_mat[2][0] = -sinf(y);
		m_mat[2][2] = cosf(y);
	}

	void setRotationZ(float z) {
		setIdentity();
		m_mat[0][0] = cosf(z);
		m_mat[0][1] = -sinf(z);
		m_mat[1][0] = sinf(z);
		m_mat[1][1] = cosf(z);
	}

	float getDeterminant() const {
		Vector4D minor, v1, v2, v3;

		v1 = Vector4D(m_mat[0][0], m_mat[1][0], m_mat[2][0], m_mat[3][0]);
		v2 = Vector4D(m_mat[0][1], m_mat[1][1], m_mat[2][1], m_mat[3][1]);
		v3 = Vector4D(m_mat[0][2], m_mat[1][2], m_mat[2][2], m_mat[3][2]);

		minor.cross(v1, v2, v3);

		return -(m_mat[0][3] * minor.x + m_mat[1][3] * minor.y + m_mat[2][3] * minor.z + m_mat[3][3] * minor.w);
	}

	void inverse() {
		int a, i, j;
		Matrix4x4 out;
		Vector4D vec[3];
		Vector4D v;
		float det = 0;

		det = getDeterminant();
		if (!det) return;

		for (i = 0; i < 4; i++) {
			for (j = 0; j < 4; j++) {
				if (j != i) {
					a = j;
					if (j > i) a = j - 1;
					vec[a].x = m_mat[j][0];
					vec[a].y = m_mat[j][1];
					vec[a].z = m_mat[j][2];
					vec[a].w = m_mat[j][3];
				}
			}

			v.cross(vec[0], vec[1], vec[2]);

			out.m_mat[0][i] = pow(-1.0f, i) * v.x / det;
			out.m_mat[1][i] = pow(-1.0f, i) * v.y / det;
			out.m_mat[2][i] = pow(-1.0f, i) * v.z / det;
			out.m_mat[3][i] = pow(-1.0f, i) * v.w / det;
		}

		setMatrix(out);
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
		setMatrix(out);
	}

	void setPerspectiveFovLH(float fov, float aspect_ratio, float z_near, float z_far) {
		setIdentity();

		float y_scale = 1.0f / tanf(fov / 2.0f);
		float x_scale = y_scale / aspect_ratio;
		m_mat[0][0] = x_scale;
		m_mat[1][1] = y_scale;
		m_mat[2][2] = z_far / (z_far - z_near);
		m_mat[2][3] = 1.0f;
		m_mat[3][2] = -(z_near * z_far) / (z_far - z_near);
	}

	void setOrthoLH(float width, float height, float z_near, float z_far) {
		setIdentity();
		m_mat[0][0] = 2.0f / width;
		m_mat[1][1] = 2.0f / height;
		m_mat[2][2] = 1.0f / (z_far - z_near);
		m_mat[3][2] = z_near / (z_near - z_far);
	}

	void setMatrix(const Matrix4x4& matrix) {
		::memcpy(m_mat, matrix.m_mat, sizeof(float) * 16);
	}

	Vector3D getXDirection() const {
		return Vector3D(m_mat[0][0], m_mat[0][1], m_mat[0][2]);
	}

	Vector3D getYDirection() const {
		return Vector3D(m_mat[1][0], m_mat[1][1], m_mat[1][2]);
	}

	Vector3D getZDirection() const {
		return Vector3D(m_mat[2][0], m_mat[2][1], m_mat[2][2]);
	}

	Vector3D getTranslation() const {
		return Vector3D(m_mat[3][0], m_mat[3][1], m_mat[3][2]);
	}

	~Matrix4x4() {}

private:
	float m_mat[4][4] = {};
};