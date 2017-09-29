#pragma once
#include "Vector.h"
#include <vector>
#include <map>

enum EdgeWalkerCode {
	x, y, z, uvx, uvy
};

struct FromTo {
	EdgeWalkerCode edgeWalkerCode;
	float from, to;

	FromTo(EdgeWalkerCode edgeWalkerCode, float from, float to) :edgeWalkerCode(edgeWalkerCode), from(from), to(to) {

	}
};

struct IEdgeCodeGetter {
	virtual float get(EdgeWalkerCode) = 0;
};

struct Package : public IEdgeCodeGetter {
	V3f vertex;
	V2f uv;

	Package() = default;

	Package(V3f vertex, V2f uv) :vertex(vertex), uv(uv) {

	}

	Package lerp(Package& v, float weight) {
		Package package;
		package.vertex = vertex.lerp(v.vertex, weight);
		package.uv = uv.lerp(v.uv, weight);
		return package;
	}

	float get(EdgeWalkerCode code) {
		switch (code) {
		case x:
			return vertex.x;
		case y:
			return vertex.y;
		case z:
			return vertex.z;
		case uvx:
			return uv.x;
		case uvy:
			return uv.y;
		default:
			return 0;
		}
	}
};

struct EdgeWalker : public IEdgeCodeGetter {
	float counter;
	std::map<EdgeWalkerCode, float> posMap;
	std::map<EdgeWalkerCode, float> incsMap;

	EdgeWalker(float from, float to, std::vector<FromTo> fromtos) {

		counter = from;
		float diff = to - from;

		for each (FromTo package in fromtos) {
			posMap[package.edgeWalkerCode] = package.from;
			incsMap[package.edgeWalkerCode] = (package.to - package.from) / diff;
		}

	}



	static EdgeWalker copyOver(IEdgeCodeGetter& a, IEdgeCodeGetter& b, EdgeWalkerCode walker, std::vector<EdgeWalkerCode> codes) {
		std::vector<FromTo> fromtos;
		for each (EdgeWalkerCode code in codes) {
			fromtos.push_back(FromTo(code, a.get(code), b.get(code)));
		}

		return EdgeWalker(a.get(walker), b.get(walker), fromtos);
	}

	void step(float steps = 1) {
		counter += steps;
		std::map<EdgeWalkerCode, float>::iterator it = posMap.begin();
		while (it != posMap.end()) {
			posMap[it->first] += incsMap[it->first] * steps;
			it++;
		}
	}


	float get(EdgeWalkerCode code) {
		return posMap[code];
	}
};