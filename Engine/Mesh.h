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
	std::vector<V3f> facenormals;
	std::vector<V3f> vertexnormals;
	Shader shader;

	Package getPackage(int i) {
		return Package(vertices[i], uvs[i]);
	}
	
	void calcFaceNormals() {
		for (int i = 0; i < faces.size(); i += 3) {
			V3f& v1 = vertices[faces[i]];
			V3f& v2 = vertices[faces[i + 1]];
			V3f& v3 =vertices[faces[i + 2]];
			V3f normal = v2.c().sub(v1).cross(v3.c().sub(v1)).normalise();
			facenormals.push_back(normal);
		}
	}

	void calcVertexNormals() {
		//to what faces is each vertex connected
		//vertexFaceOnwnership[i] corresponds to mesh.vertices
		//vertexFaceOnwnership[i][x] says which begin index of faces are there
		std::vector <std::vector<int>> vertexFaceOnwnership;
		vertexFaceOnwnership.resize(vertices.size());

		for (int i = 0; i < faces.size(); i += 3) {//at first index of face to al vertices that are part of it
			vertexFaceOnwnership[faces[i]].push_back(i);
			vertexFaceOnwnership[faces[i + 1]].push_back(i);
			vertexFaceOnwnership[faces[i + 2]].push_back(i);
		}

		//average the normals of those faces
		for (int i = 0; i < vertexFaceOnwnership.size(); i++) {//foreach vertex
			//get normal of owned faces
			V3f combined;
			for(int j = 0; j < vertexFaceOnwnership[i].size(); j++){
				combined.add(facenormals[vertexFaceOnwnership[i][j] / 3]);
			}
			vertexnormals.push_back(combined.normalise());
		}
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
			0,2,3,
		};
		return quad;
	}

	static Mesh cube() {
		Mesh cube;
		cube.vertices.emplace_back(-1, 1, -1);
		cube.vertices.emplace_back(1, 1, -1);
		cube.vertices.emplace_back(1, -1, -1);
		cube.vertices.emplace_back(-1, -1, -1);

		cube.vertices.emplace_back(-1, 1, 1);
		cube.vertices.emplace_back(1, 1, 1);
		cube.vertices.emplace_back(1, -1, 1);
		cube.vertices.emplace_back(-1, -1, 1);

		cube.uvs.emplace_back(0, 0);
		cube.uvs.emplace_back(1, 0);
		cube.uvs.emplace_back(1, 1);
		cube.uvs.emplace_back(0, 1);

		cube.uvs.emplace_back(0, 0);
		cube.uvs.emplace_back(1, 0);
		cube.uvs.emplace_back(1, 1);
		cube.uvs.emplace_back(0, 1);

		cube.faces = {
			0,1,3,
			1,2,3,

			1,6,2,
			1,5,6,

			5,4,6,
			4,7,6,

			4,0,3,
			4,3,7,

			0,4,1,
			4,5,1,

			3,6,7,
			3,2,6,
		};
		return cube;
	}
};