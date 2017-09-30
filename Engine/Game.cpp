/****************************************************************************************** 
 *	Chili DirectX Framework Version 16.07.20											  *	
 *	Game.cpp																			  *
 *	Copyright 2016 PlanetChili.net <http://www.planetchili.net>							  *
 *																						  *
 *	This file is part of The Chili DirectX Framework.									  *
 *																						  *
 *	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
 *	it under the terms of the GNU General Public License as published by				  *
 *	the Free Software Foundation, either version 3 of the License, or					  *
 *	(at your option) any later version.													  *
 *																						  *
 *	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
 *	GNU General Public License for more details.										  *
 *																						  *
 *	You should have received a copy of the GNU General Public License					  *
 *	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
 ******************************************************************************************/
#include "MainWindow.h"
#include "Game.h"
#include "Vector.h"
#include "Mesh.h"
#include "Camera.h"
#include "TexturedShader.h"
#include "ClownShader.h"
#include "UvShader.h"
const float PI = 3.1415927;

Surface sayan("Images\\sayan.bmp");
TexturedShader* textureShader;
StdVertexShader* stdVertexShader;
ClownShader* clownShader;
UvShader* uvShader;
Game::Game( MainWindow& wnd ):wnd( wnd ),gfx( wnd ){
	textureShader = new TexturedShader(&sayan);
	stdVertexShader = new StdVertexShader(gfx);
	clownShader = new ClownShader();
	uvShader = new UvShader();
}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}
float mytime = 0;
void Game::UpdateModel()
{
	mytime += ft.Mark();
	
}

void Game::ComposeFrame(){
	Mesh mesh = Mesh::quad();
	

	mesh.shader.fragmentShader = textureShader;
	mesh.shader.vertexShader = stdVertexShader;
	for (int i = 0; i < mesh.vertices.size(); i++) {
		mesh.vertices[i].rotY(mytime);
	}
	for (int i = 0; i < mesh.vertices.size(); i++) {
		mesh.vertices[i].add(V3f(0, 0, 4));
	}


	Camera camera;
	camera.draw(mesh, gfx);
}
