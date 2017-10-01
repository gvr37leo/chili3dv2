#pragma once
#include "Vector.h"
#include "Graphics.h"
#include "Mesh.h"
#include "EdgeWalker.h"
#include <algorithm>
#include <functional>

struct LocationGiver {
	Mesh& mesh;
	Graphics& gfx;
	int i;
	V3f* normal;
	V2i scrap;

	LocationGiver(Mesh& mesh, Graphics& gfx) :mesh(mesh), gfx(gfx) {

	}

	virtual void operator() (V3i pos, EdgeWalker edgeWalker) {
		scrap.x = pos.x;
		scrap.y = pos.y;
		Color c = mesh.shader.fragmentShader->operator()(i, scrap, edgeWalker, *normal, gfx);
		gfx.PutPixel(pos.x, pos.y, c);
	};
};



class Camera{
public:
	V2f pos;

	Camera() {

	}

	void draw(std::vector<Mesh> meshes, Graphics& gfx) {
		for (int i = 0; i < meshes.size(); i++) {
			draw(meshes[i], gfx);
		}
	}

	void draw(Mesh& mesh, Graphics& gfx) {
		std::vector<V3f> wscoords = mesh.vertices;
		for (int i = 0; i < mesh.vertices.size(); i++) {
			mesh.shader.vertexShader->operator()(mesh.vertices[i]);
		}
			
		LocationGiver locationGiver(mesh, gfx);

		for (int i = 0; i < mesh.faces.size(); i += 3) {
			V3f& v1 = wscoords[mesh.faces[i]];
			V3f& v2 = wscoords[mesh.faces[i + 1]];
			V3f& v3 = wscoords[mesh.faces[i + 2]];
			V3f normal = v2.c().sub(v1).cross(v3.c().sub(v1)).normalise();
			if (normal.dot(v1.c().sub(V3f(0, 0, 0))) < 0) {//backface culling
				locationGiver.i = i;
				locationGiver.normal = &normal;
				triangle(mesh, mesh.faces[i], mesh.faces[i + 1], mesh.faces[i + 2], gfx, locationGiver);
			}
		}
	}

	void drawLine(V3f& a, V3f& b, Graphics& gfx, Color& color){
		RectI screen = gfx.GetScreenRect();
		stf(a,screen.GetWidth(), screen.GetHeight());
		stf(b, screen.GetWidth(), screen.GetHeight());
		
		line(a.resize<2>(), b.resize<2>(), gfx, color);
	}

	void stf(V3f& v,int width, int height){
		v.x /= v.z;
		v.y /= v.z;

		v.x = (v.x + 1) * (width / 2);
		v.y = (-v.y + 1) * (height / 2);
	}



	void line(V2f& a, V2f& b,Graphics& gfx , Color& color){
		V2i a_ = a.round();
		V2i b_ = b.round();
		int n = std::max(abs(a_.x - b_.x), abs(a_.y - b_.y));

		for(int i = 0; i <= n; i++){
			V2i pos = a.lerp(b, (float)i / n).round();
			gfx.PutPixel(pos.x,pos.y,color);
		}
		
	}

	void triangle(Mesh& mesh, int a, int b, int c, Graphics& gfx, LocationGiver& locationGiver) {
		Package vs[] = { mesh.getPackage(a),mesh.getPackage(b),mesh.getPackage(c) };
		std::sort(vs, vs + 3, [](Package& a, Package& b) -> bool {return a.vertex.y < b.vertex.y; });
		Package& top = vs[0];
		Package& middle = vs[1];
		Package& bot = vs[2];

		float ratio = (middle.vertex.y - top.vertex.y) / (float)(bot.vertex.y - top.vertex.y);
		Package& middleRight = top.lerp(bot, ratio);
		if (middle.vertex.x > middleRight.vertex.x)std::swap(middle, middleRight);

		struct FromtToer {
			LocationGiver* locationGiver;

			FromtToer(LocationGiver* locationGiver) {
				this->locationGiver = locationGiver;
			}

			void operator() (Package& topLeft, Package& topRight, Package& bottomLeft, Package& bottomRight) {
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
						(*locationGiver)(V3i(horizontalWalker.counter, leftWalker.counter, 0), horizontalWalker);
						horizontalWalker.step();
					}
					leftWalker.step();
					rightWalker.step();
				}
			}
		};
		FromtToer fromtToer(&locationGiver);
		fromtToer(top, top, middle, middleRight);//upper triangle/flatbot
		fromtToer(middle, middleRight, bot, bot);//lower triangle/flattop
	}
};


