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
		colors.push_back(Color(255, 0, 0));
		colors.push_back(Colors::Blue);
		colors.push_back(Colors::Cyan);
		colors.push_back(Colors::Gray);
		colors.push_back(Colors::Green);
		colors.push_back(Colors::Magenta);
		colors.push_back(Colors::White);
		colors.push_back(Colors::Yellow);
		colors.push_back(Color(255, 255, 0));
		colors.push_back(Color(255, 0, 255));
		colors.push_back(Color(0, 255, 255));
		colors.push_back(Color(255, 0, 125));
	}

	Color operator() (int i, V2i& pos, EdgeWalker& walker, V3f& normal, Graphics& gfx) {
		return colors[i / 3 % colors.size()];
	}
};