#pragma once
#include "Vector.h"
#include "Graphics.h"
#include "Utils.h"
#include "Mesh.h"
#include "EdgeWalker.h"
#include <algorithm>
#include <functional>



class Camera
{
public:
	V2f pos;

	Camera() {

	}

	void draw(std::vector<Mesh> meshes, Graphics& gfx) {
		for each (Mesh mesh in meshes) {
			draw(mesh, gfx);
		}
	}

	void draw(Mesh mesh, Graphics& gfx) {
		std::vector<Vec3f> wscoords = mesh.vertices;
		for (int i = 0; i < mesh.vertices.size(); i++) {
			mesh.shader->vert(mesh.vertices[i]);
		}

		for (int i = 0; i < mesh.faces.size(); i += 3) {
			Vec3f v1 = wscoords[mesh.faces[i]];
			Vec3f v2 = wscoords[mesh.faces[i + 1]];
			Vec3f v3 = wscoords[mesh.faces[i + 2]];
			Vec3f normal = v2.sub(v1).cross(v3.sub(v1)).normalise();
			if (normal.dot(v1.sub(Vec3f(0, 0, 0))) < 0) {
				triangle(mesh, mesh.faces[i], mesh.faces[i + 1], mesh.faces[i + 2], gfx, [&](Vec3i pos, EdgeWalker walker) {
					Color c = mesh.shader->frag(i, pos.toFloat(), walker, normal, gfx);
					gfx.PutPixel(pos.x, pos.y, c);
				});
			}
		}
	}

	void triangle(Mesh mesh, int a, int b, int c, Graphics& gfx, std::function<void(Vec3i, EdgeWalker)> locationGiver) {
		Package vs[] = { mesh.getPackage(a),mesh.getPackage(b),mesh.getPackage(c) };
		std::sort(vs, vs + 3, [](Package a, Package b) -> bool {return a.vertex.y < b.vertex.y; });
		Package top = vs[0];
		Package middle = vs[1];
		Package bot = vs[2];

		float ratio = (middle.vertex.y - top.vertex.y) / (float)(bot.vertex.y - top.vertex.y);
		Package middleRight = top.lerp(bot, ratio);
		if (middle.vertex.x > middleRight.vertex.x)std::swap(middle, middleRight);

		auto fromTo = [&](Package topLeft, Package topRight, Package bottomLeft, Package bottomRight) {
			std::vector<FromTo> fromtosLeft;
			EdgeWalker leftWalker = EdgeWalker::copyOver(topLeft, bottomLeft, EdgeWalkerCode::y, std::vector<EdgeWalkerCode>{EdgeWalkerCode::x, EdgeWalkerCode::z, EdgeWalkerCode::uvx, EdgeWalkerCode::uvy});
			EdgeWalker rightWalker = EdgeWalker::copyOver(topRight, bottomRight, EdgeWalkerCode::y, std::vector<EdgeWalkerCode>{EdgeWalkerCode::x, EdgeWalkerCode::z, EdgeWalkerCode::uvx, EdgeWalkerCode::uvy});

			float distToPixelCenter = (ceil(leftWalker.counter - 0.5) + 0.5) - leftWalker.counter;
			leftWalker.step(distToPixelCenter);
			rightWalker.step(distToPixelCenter);

			//float roofPixelCoverage = ceil(topLeft.get(EdgeWalkerCode::y)) - topLeft.get(EdgeWalkerCode::y);
			//float basePixelCoverage = bottomLeft.get(EdgeWalkerCode::y) - floor(bottomLeft.get(EdgeWalkerCode::y));

			while (leftWalker.counter < (int)ceil(bottomLeft.vertex.y - 0.5f)) {
				EdgeWalker horizontalWalker = EdgeWalker::copyOver(leftWalker, rightWalker, EdgeWalkerCode::x, std::vector<EdgeWalkerCode>{EdgeWalkerCode::z, EdgeWalkerCode::uvx, EdgeWalkerCode::uvy});
				horizontalWalker.step((ceil(horizontalWalker.counter - 0.5f) + 0.5) - horizontalWalker.counter);

				while (horizontalWalker.counter < (int)ceil(rightWalker.get(EdgeWalkerCode::x) - 0.5f)) {
					locationGiver(Vec3i(horizontalWalker.counter, leftWalker.counter, 0), horizontalWalker);
					horizontalWalker.step();
				}
				leftWalker.step();
				rightWalker.step();
			}
		};

		fromTo(top, top, middle, middleRight);//upper triangle/flatbot
		fromTo(middle, middleRight, bot, bot);//lower triangle/flattop
	}
};
