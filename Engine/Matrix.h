#pragma once
#include "Vector.h"
#include <iostream>

struct Matrix {
	float vals[4][4] = {};

	Matrix() = default;

	Matrix(float matrix[4][4]) {
		memcpy(vals, matrix, 16 * sizeof(float));
	}

	Matrix(Matrix& other) {
		memcpy(vals, other.vals, 16 * sizeof(float));
	}

	Matrix mul(Matrix& other) {
		Matrix result;
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				float sum = 0;

				for (int k = 0; k < 4; k++) {
					sum += vals[row][k] * other.vals[k][col];
				}
				result.vals[row][col] = sum;
			}
		}
		return result;
	}

	Matrix& mul(V3f& other) {
		float p[4] = {};
		for (int row = 0; row < 4; row++) {
			for (int k = 0; k < 3; k++) {
				p[row] += vals[row][k] * other[k];
			}
			p[row] += vals[row][3];
		}
		memcpy(other.vals, p, sizeof(float) * 3);
		return *this;
	}

	Matrix& mul(float scalar) {
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				vals[row][col] *= scalar;
			}
		}
		return *this;
	}

	static Matrix identity() {
		float matrix[4][4] = {
			{ 1,0,0,0 },
			{ 0,1,0,0 },
			{ 0,0,1,0 },
			{ 0,0,0,1 },
		};
		return Matrix(matrix);
	}

	Matrix inverse() {
		return adjugate().mul(1 / determinant());
	}

	float determinant() {

	}

	float determinant(float size, int row, int column, float** matrix) {
		if (size == 2) {
			return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
		}
	}

	Matrix adjugate() {//adjoint
		return  cofactor().transpose();
	}

	Matrix transpose() {
		Matrix result;
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				result.vals[row][col] = vals[col][row];
			}
		}
		return result;
	}

	Matrix cofactor() {
		Matrix result;
		for (int row = 0; row < 4; row++) {
			for (int col = 0; col < 4; col++) {
				if ((row + col) % 2 == 0) {
					result.vals[row][col] = vals[row][col];
				}
				else {
					result.vals[row][col] = -vals[row][col];
				}

			}
		}
		return result;
	}

	static Matrix MVP(V3f camera, V3f translation, V3f rotation, V3f scale) {
		Matrix modelMatrix = Matrix::local2world(translation, rotation, scale);
		Matrix viewMatrix = Matrix::lookAt(V3f(), camera);
		Matrix projectionMatrix = Matrix::identity();

		return projectionMatrix.mul(viewMatrix).mul(modelMatrix);
		//      model matrix - view matrix - projection matrix
		//Local space - World space - View space - Clip space
	}

	static Matrix lookAt(V3f& target, V3f& pos) {
		V3f direction = target.c().sub(pos).normalise();
		V3f right = V3f(0, 1, 0).cross(direction);
		V3f up = direction.cross(right);

		float matrix[4][4] = {
			{ right.x,right.y,right.z,-pos.x },
			{ up.x,up.y,up.z,-pos.y },
			{ direction.x,direction.y,direction.z,-pos.z },
			{ 0,0,0,1 },
		};
		return Matrix(matrix);
	}

	static Matrix local2world(V3f translation, V3f rotation, V3f scale) {
		Matrix translationMatrix = Matrix::translate(translation);
		Matrix rotationMatrix = Matrix::rot(rotation);
		Matrix scaleMatrix = Matrix::scale(scale);
		//rot scale trans
		return translationMatrix.mul(scaleMatrix).mul(rotationMatrix);
	}

	static Matrix translate(V3f v) {
		float matrix[4][4] = {
			{ 1,0,0,v.x },
			{ 0,1,0,v.y },
			{ 0,0,1,v.z },
			{ 0,0,0,1 },
		};
		return Matrix(matrix);
	}

	static Matrix scale(V3f v) {
		float matrix[4][4] = {
			{ v.x,0,0,0 },
			{ 0,v.y,0,0 },
			{ 0,0,v.z,0 },
			{ 0,0,0,1 },
		};
		return Matrix(matrix);
	}

	static Matrix rot(V3f v) {
		Matrix x = Matrix::rotX(v.x);
		Matrix y = Matrix::rotY(v.y);
		Matrix z = Matrix::rotZ(v.z);
		return x.mul(y).mul(z);
	}

	static Matrix rotX(float theta) {
		float cosTheta = (float)cos(theta);
		float sinTheta = (float)sin(theta);
		float matrix[4][4] = {
			{ 1,         0,                  0,			0 },
			{ 0,         cosTheta,           sinTheta,	0 },
			{ 0,         -sinTheta,          cosTheta,	0 },
			{ 0,0,0,1 },
		};
		return Matrix(matrix);
	}

	static Matrix rotY(float theta) {
		float cosTheta = (float)cos(theta);
		float sinTheta = (float)sin(theta);
		float matrix[4][4] = {
			{ cosTheta,  0 ,     -sinTheta ,0 },
			{ 0,         1,      0 ,0 },
			{ sinTheta, 0,      cosTheta ,0 },
			{ 0,0,0,1 },
		};
		return Matrix(matrix);
	}

	static Matrix rotZ(float theta) {
		float cosTheta = (float)cos(theta);
		float sinTheta = (float)sin(theta);
		float matrix[4][4] = {
			{ cosTheta,  sinTheta,    0 ,0 },
			{ -sinTheta, cosTheta,   0 ,0 },
			{ 0,         0,          1 ,0 },
			{ 0,0,0,1 },
		};
		return Matrix(matrix);
	}


};