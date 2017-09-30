#pragma once
#include <vector>
#include "Vector.h"
#include "Colors.h"
#include "Edgewalker.h"

struct VertexShader {
	virtual void operator() (V3f& vertex) = 0;
};

struct FragmentShader {
	virtual Color operator() (int i, V2i& pos, EdgeWalker& walker, V3f& normal, Graphics& gfx) = 0;
};

struct Shader {
	VertexShader* vertexShader;
	FragmentShader* fragmentShader;
};

struct Mesh {
	std::vector<V3f> vertices;
	std::vector<V2f> uvs;
	std::vector<int> faces;
	Shader shader;

	Package getPackage(int i) {
		return Package(vertices[i], uvs[i]);
	}

	static Mesh quad() {
		Mesh quad;
		quad.vertices.emplace_back(-1, 1, 0);
		quad.vertices.emplace_back( 1, 1, 0);
		quad.vertices.emplace_back( 1,-1, 0);
		quad.vertices.emplace_back(-1,-1, 0);

		quad.uvs.emplace_back(0, 0);
		quad.uvs.emplace_back(1, 0);
		quad.uvs.emplace_back(1, 1);
		quad.uvs.emplace_back(0, 1);
		quad.faces = {
			0,1,2,
			2,3,0
		};
		return quad;
	}
};