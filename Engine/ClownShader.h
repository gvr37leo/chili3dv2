#pragma once
#include "Vector.h"
#include "Colors.h"
#include "Edgewalker.h"
#include "Surface.h"
#include "Mesh.h"
#include "RectI.h"
#include "Graphics.h"

struct ClownShader :public FragmentShader {

	std::vector<Color> colors;

	ClownShader() {
		colors.push_back(Color(255, 155, 0));
		colors.push_back(Color(0, 0, 255));
		colors.push_back(Color(0, 255, 0));
		colors.push_back(Color(0, 255, 255));
		colors.push_back(Color(255, 0, 0));
		colors.push_back(Color(255, 0, 255));
		colors.push_back(Color(255, 255, 0));
		colors.push_back(Color(255, 255, 255));

		colors.push_back(Color(155, 0, 255));
		colors.push_back(Color(0, 155, 255));
		colors.push_back(Color(0, 255, 155));
		colors.push_back(Color(155, 255, 0));
		colors.push_back(Color(255, 155, 0));
		colors.push_back(Color(255, 0, 155));

	}

	Color operator() (int i, V2i& pos, EdgeWalker& walker, V3f& normal, Graphics& gfx) {
		return colors[i / 3 % colors.size()];
	}
};