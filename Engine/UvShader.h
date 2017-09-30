#pragma once
#include "Vector.h"
#include "Colors.h"
#include "Edgewalker.h"
#include "Surface.h"
#include "Mesh.h"
#include "RectI.h"
#include "Graphics.h"

struct UvShader :public FragmentShader {


	UvShader() {
	}

	Color operator() (int i, V2i& pos, EdgeWalker& walker, V3f& normal, Graphics& gfx) {
		return Color(walker.get(EdgeWalkerCode::uvx) * 255, walker.get(EdgeWalkerCode::uvy) * 255, 0);
	}
};