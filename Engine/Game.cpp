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

Surface sayan("Images\\sayan.bmp");
TexturedShader* textureShader;
StdVertexShader* stdVertexShader;
Game::Game( MainWindow& wnd ):wnd( wnd ),gfx( wnd ){
	textureShader = new TexturedShader(&sayan);
	stdVertexShader = new StdVertexShader(gfx);

}

void Game::Go()
{
	gfx.BeginFrame();	
	UpdateModel();
	ComposeFrame();
	gfx.EndFrame();
}

void Game::UpdateModel()
{
	
	ft.Mark();
}

int count = 0;
void Game::ComposeFrame()
{
	Mesh mesh = Mesh::quad();
	mesh.shader.fragmentShader = textureShader;
	mesh.shader.vertexShader = stdVertexShader;
	for (int i = 0; i < mesh.vertices.size(); i++) {
		mesh.vertices[i].add(V3f(0, 0, 8));
	}


	Camera camera;
	camera.draw(mesh, gfx);
	OutputDebugString(L"frame\n");
}
