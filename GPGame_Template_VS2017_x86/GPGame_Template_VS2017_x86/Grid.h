#pragma once

#include <iostream>
#include <vector>
using namespace std;
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "shapes.h"
#include "graphics.h"
#include <array> 

class Grid
{
public:
	Grid(int grid_height, int grid_width, glm::vec3 grid_begin, Particle player_shape);
	Grid();
	~Grid();
	

	int grid_height;
	int grid_width;
	glm::vec3 grid_begin;
	array<array < GridComponent,25>, 25 > shapes_vector;
	vector <GridComponent> final_path;
	Particle player_shape;
	GridComponent start;
	GridComponent end;
	bool closedlist[25][25];

	void FillGrid();
	void DrawGrid(Graphics myGraphics);
	//void DrawSolution();
	void DefineStartAndEnd();
	void Spawn_Player();
	float Distance(int x1, int y1, int x2, int y2);
	bool isDestination(GridComponent G1);
	void DrawSolution(vector<GridComponent> myvec);
	bool isValid(int x, int y);
	vector<GridComponent> Astar();
	vector<GridComponent> make_final_path(array<array < GridComponent, 25>, 25 > shapes_vector);
};


