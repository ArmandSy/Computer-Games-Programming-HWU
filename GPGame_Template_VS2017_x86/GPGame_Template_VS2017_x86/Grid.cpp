#include "grid.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "shapes.h"
#include "graphics.h"
#include <time.h>
#include <stack> 
#include <array> 



Grid::Grid(int grid_height, int grid_width, glm::vec3 grid_begin, Particle player_shape)
{
	this->grid_height = grid_height;
	this->grid_width = grid_width;
	this->grid_begin = grid_begin;
	this->player_shape = player_shape;
};

Grid::Grid()
{
}


Grid::~Grid()
{
}

void Grid::FillGrid()
{
	srand(time(0));
	int i;
	int j;

	int h;
	int k;

	

	GridComponent emptyGrid = GridComponent(glm::vec3(0,0,0), 0);
	vector<GridComponent> iVec;

	for (i = 0; i < this->grid_height; i++)
	{
		for (j = 0; j < this->grid_width; j++)
		{
			GridComponent tmpGridComponent = GridComponent(glm::vec3(this->grid_begin[0] + i, 0, this->grid_begin[2] + j), (rand()%3)%2 - rand() % 2);
			tmpGridComponent.cout_f = FLT_MAX;
			tmpGridComponent.cout_g = FLT_MAX;
			tmpGridComponent.cout_h = FLT_MAX;
			tmpGridComponent.parentx = -1;
			tmpGridComponent.parenty = -1;
			this->closedlist[i][j] = false;

			tmpGridComponent.Load();
			this->shapes_vector[i][j] = tmpGridComponent;
		}
	}
}

void Grid::Spawn_Player()
{
	this->player_shape.position = glm::vec3(this->grid_begin[0] + this->start.position[0], 1, this->grid_begin[0] + this->start.position[2]);
	this->player_shape.lineColor = glm::vec4(0, 0, 0, 0);
	this->player_shape.fillColor = glm::vec4(0.4, 0, 0.7, 1); //purple
}

void Grid::DefineStartAndEnd()
{	
	
	this->start = this->shapes_vector[rand()%this->grid_width][rand()%this->grid_height];
	this->end = this->shapes_vector[rand() % this->grid_width][rand() % this->grid_height];
	while (this->start.obstacle == 1)
	{
		this->start = this->shapes_vector[1 + (rand() % this->grid_width - 1)][ 1 + (rand() % this->grid_height - 1)];
	}
	while (this->end.obstacle == 1)
	{
		this->end = this->shapes_vector[rand() % this->grid_width][rand() % this->grid_height];
	}

	//this->end.obstacle = 2;
	//this->start.obstacle = 2;
}

void Grid::DrawGrid(Graphics myGraphics)
{
	for (int i = 0; i < this->grid_width; ++i)
	{
		for (int j = 0; j < this->grid_width; ++j)
		{
			// iterator sur son shapes_vector et Draw() chacun des elements
			if (this->shapes_vector[i][j].obstacle == 1)
			{
				this->shapes_vector[i][j].fillColor = glm::vec4(1, 0, 0, 1);
				this->shapes_vector[i][j].Render(myGraphics);
			}
			else if (this->shapes_vector[i][j].position[0] == this->start.position[0] && this->shapes_vector[i][j].position[2] == this->start.position[2]) // Draw start
			{
				this->shapes_vector[i][j].fillColor = glm::vec4(0, 0, 1, 1);
				this->shapes_vector[i][j].Render(myGraphics);
			}
			else if (this->shapes_vector[i][j].position[0] == this->end.position[0] && this->shapes_vector[i][j].position[2] == this->end.position[2]) // Draw end
			{
				this->shapes_vector[i][j].fillColor = glm::vec4(0, 1, 1, 1);
				this->shapes_vector[i][j].Render(myGraphics);
			
			}
			else if (this->shapes_vector[i][j].obstacle == 2 ) // Draw solution except on start and end
			{
				this->shapes_vector[i][j].fillColor = glm::vec4(0, 1, 0, 1);
				this->shapes_vector[i][j].Render(myGraphics);
			}
			
			else // Draw cases normales
			{
				this->shapes_vector[i][j].fillColor = glm::vec4(1, 1, 1, 1);
				this->shapes_vector[i][j].Render(myGraphics);
			}
		}

	}
}


void Grid::DrawSolution(vector<GridComponent> myvec)
{
	vector<GridComponent>::iterator k;
	for (k = myvec.begin(); k != myvec.end(); ++k)
	{
		this->shapes_vector[(*k).position[0]][(*k).position[2]].obstacle = 2;
	}
}



float Grid::Distance(int x1, int y1, int x2, int y2)
{
	return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));
}

bool Grid::isDestination(GridComponent G1)
{
	if (G1.position[0] == this->end.position[0] && G1.position[2] == this->end.position[2])
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Grid::isValid(int x, int y)
{
	if (x > -1 && y > -1 && x < 25 && y < 25)
	{
		return true;
	}
	else
	{
		return false;
	}
}

vector<GridComponent> Grid::Astar()
{
	int x = this->start.position[0];
	int y = this->start.position[2];
	this->shapes_vector[x][y].cout_f = 0.0;
	this->shapes_vector[x][y].cout_g = 0.0;
	this->shapes_vector[x][y].cout_h = 0.0;
	this->shapes_vector[x][y].parentx = x;
	this->shapes_vector[x][y].parenty = y;

	vector<GridComponent> nada;

	if ( !this->isValid(this->start.position[0], this->start.position[2]))
	{
		return nada;
		printf("start fail");
	}

	vector<GridComponent> openlist;
	openlist.emplace_back(this->shapes_vector[x][y]);


	while (!openlist.empty() && openlist.size() < 25*this->grid_height*this->grid_width)
	{
		GridComponent node;
		do
		{
			float temp = FLT_MAX;
			vector<GridComponent>::iterator itNode;
			for (auto it = openlist.begin(); it != openlist.end(); ++it)
			{
				GridComponent n = *it;

				if (n.cout_f < temp)
				{
					temp = n.cout_f;
					itNode = it;
				}
			}

			node = *itNode;
			openlist.erase(itNode);
			/*
			for (int h = 0; h < openlist.size(); h++)
			{
				if (openlist[h].position[0] == tempGridComponent.position[0] && openlist[h].position[2] == tempGridComponent.position[2])
				{
					openlist.erase(openlist.begin() + h);
					printf("hello");

				}

			} */
		

		} while ( (this->isValid(node.position[0], node.position[2]) == false || node.obstacle == 1));

			x = node.position[0];
			y = node.position[2];
			closedlist[x][y] = true;

			for (int posx = -1; posx <= 1; posx++)
			{
				for (int posy = -1; posy <= 1; posy++)
				{
					float g, h, f;
					if (this->isValid(x + posx, y + posy) && this->shapes_vector[x + posx][y + posy].obstacle == 0)
					{
						if (this->shapes_vector[x + posx][y + posy].position[0] == this->end.position[0] && this->shapes_vector[x + posx][y + posy].position[2] == this->end.position[2])
						{
							this->shapes_vector[x + posx][y + posy].parentx = x;
							this->shapes_vector[x + posx][y + posy].parenty = y;
							printf("solution trouvee");
							vector<GridComponent> final_path = this->make_final_path(this->shapes_vector);
							return final_path;
						}
						else if (closedlist[x + posx][y + posy] == false)
						{
							g = node.cout_g + 1.0f;
							h = this->Distance(x + posx, y + posy, this->end.position[0], this->end.position[2]);
							f = g + h;

							if (this->shapes_vector[x + posx][y + posy].cout_f == FLT_MAX || this->shapes_vector[x + posx][y + posy].cout_f > f)
							{
								this->shapes_vector[x + posx][y + posy].cout_f = f;
								this->shapes_vector[x + posx][y + posy].cout_g = g;
								this->shapes_vector[x + posx][y + posy].cout_h = h;
								this->shapes_vector[x + posx][y + posy].parentx = x;
								this->shapes_vector[x + posx][y + posy].parenty = y;
								openlist.emplace_back(this->shapes_vector[x + posx][y + posy]);
							}
						}
					}
				}
			}
	}
}

vector<GridComponent> Grid::make_final_path(array<array < GridComponent, 25>, 25 > shapes_vector)
{
		int x = this->end.position[0];
		int y = this->end.position[2];

		stack<GridComponent> path;
		vector<GridComponent> final_path;

		while (!(this->shapes_vector[x][y].parentx == x && this->shapes_vector[x][y].parenty == y && this->shapes_vector[x][y].position[0] !=-1 && this->shapes_vector[x][y].position[2] != -1))
		{
			path.push(this->shapes_vector[x][y]);
			int tmpx = this->shapes_vector[x][y].parentx;
			int tmpy = this->shapes_vector[x][y].parenty;
			x = tmpx;
			y = tmpy;
		}
		path.push(this->shapes_vector[x][y]);
		while (!path.empty())
		{
			GridComponent Stacktop = path.top();
			path.pop();
			final_path.emplace_back(Stacktop);
		}
		
		printf("%d\n", final_path.size());
		return final_path;
}

