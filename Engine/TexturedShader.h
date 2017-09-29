#pragma once
#include "Vector.h"
#include "Colors.h"
#include "Edgewalker.h"
#include "Surface.h"
#include "Mesh.h"
#include "RectI.h"

struct StdVertexShader:public VertexShader {

	Graphics& gfx;
	RectI screen;

	StdVertexShader(Graphics& gfx):gfx(gfx) {
		screen = gfx.GetScreenRect();
	}

	void operator() (V3f& vertex) {
		vertex.x /= vertex.z;
		vertex.y /= vertex.z;

		vertex.x = (vertex.x + 1) * (screen.GetWidth() / 2);
		vertex.y = (-vertex.y + 1) * (screen.GetHeight() / 2);
	}
};

struct TexturedShader:public FragmentShader {

	Surface* texture;

	TexturedShader(Surface* texture) {
		this->texture = texture;
	}

	Color operator() (int i, V2i& pos, EdgeWalker& walker, V3f& normal, Graphics& gfx) {
		return Color(255, 0, 0);
	}
};