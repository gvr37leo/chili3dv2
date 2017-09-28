#pragma once
#include "Vector.h"
#include <functional>
#include <vector>
#include "Colors.h"
#include "Graphics.h"
//#include "LitShader.h"



class Mesh {
public:
	std::vector<Vec3f> vertices;
	std::vector<int> faces;
	std::vector<Vec2f> uvs;
	//std::function<Color(int index, Vec2i pos, EdgeWalker walker, Graphics& gfx)>
	Shader* shader;

	Package getPackage(int i) {
		return Package(vertices[i], uvs[i]);
	}

	static Mesh quad() {
		Mesh quad;
		quad.vertices = std::vector<Vec3f>{ Vec3f(-1,1,0),Vec3f(0,1,0),Vec3f(0,0,0),Vec3f(-1,0,0) };
		quad.faces = std::vector<int>{ 0, 1, 2, 2, 3, 0 };
		quad.uvs = std::vector<Vec2f>{ Vec2f(0,0),Vec2f(1,0),Vec2f(1,1),Vec2f(0,1) };
		quad.shader = new LitShader(Color(255, 0, 0));
		return quad;
	}

	static Mesh cube() {
		Mesh cube;
		float f = 0.5f;
		cube.vertices = std::vector<Vec3f>{
			Vec3f(-f,f,f),
			Vec3f(f,f,f),
			Vec3f(f,f,-f),
			Vec3f(-f,f,-f),

			Vec3f(-f,-f,f),
			Vec3f(f,-f,f),
			Vec3f(f,-f,-f),
			Vec3f(-f,-f,-f),
		};
		cube.faces = std::vector<int>{
			0,1,2, 3,0,2,

			7,3,6, 6,3,2,
			6,2,5, 5,2,1,
			1,0,4, 1,4,5,
			0,3,4, 4,3,7,

			4,7,6, 4,6,5
		};
		cube.uvs = std::vector<Vec2f>{
			Vec2f(0,0),
			Vec2f(1,0),
			Vec2f(1,1),
			Vec2f(0,1),

			Vec2f(0,1),
			Vec2f(1,1),
			Vec2f(1,0),
			Vec2f(0,0),
		};
		cube.shader = new LitShader(Color(255, 0, 0));
		return cube;
	}

	Mesh applyTransformation(Matrix m) {
		for (int i = 0; i < vertices.size(); i++) {
			m.mult(vertices[i]);
		}
		return *this;
	}
};