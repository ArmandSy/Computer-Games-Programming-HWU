#pragma once

#include <iostream>
#include <vector>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "graphics.h"

class Shapes {

public:
	Shapes(glm::vec3 position, float mass, float radius, glm::vec3 speed, glm::vec3 acceleration);
	Shapes();
	~Shapes();

	float mass; 
	float radius; 
	glm::vec3 speed;
	glm::vec3 acceleration;
	glm::vec3 position;
	glm::vec3 rotation;

	void Load(const char* filename, vector<glm::vec4> &vertices, vector<glm::vec3> &normals, vector<GLushort> &elements);
	void Load();
	void Draw();
	void Render(Graphics myGraphics);
	int SlowMove(glm::vec3 target);
	void ApplyGravity(float delta_time);
	void UpdateVector(float delta_time);
	void VectorDiff(glm::vec3 VecteurCible);
	bool Dist(glm::vec3 position);
	void setposition(glm::vec3 position);
	void RenderBoids(Graphics myGraphics, glm::vec3 rotvec);

	void checkErrorShader(GLuint shader);

	vector<GLfloat> vertexPositions;

	GLuint          program;
	GLuint          vao;
	GLuint          buffer;
	GLint           mv_location;
	GLint           proj_location;
	GLint           color_location;
	glm::mat4		proj_matrix = glm::mat4(1.0f);
	glm::mat4		mv_matrix = glm::mat4(1.0f);

	glm::vec4		fillColor = glm::vec4(1.0, 0.0, 0.0, 1.0);
	glm::vec4		lineColor = glm::vec4(0.0, 0.0, 0.0, 1.0);
	float			lineWidth = 2.0f;


protected:
	string rawData;			// Import obj file from Blender (note: no textures or UVs).
	void LoadObj();
};

class Cube : public Shapes {
public:
	Cube(glm::vec3 position, float mass, float radius, glm::vec3 speed, glm::vec3 acceleration);
	Cube();
	~Cube();
};

class Particle : public Shapes {
public:
	int lifespan;
	glm::vec3 rotationVector;
	float rotationFactor;
	Particle(glm::vec3 position, glm::vec3 speed, glm::vec3 acceleration, glm::vec3 rotationVector, float rotationFactor, float mass, int lifespan);
	Particle();
	~Particle();
};

class Meneur : public Shapes {
public:
	glm::vec3 position;
	Meneur(glm::vec3 position);
	~Meneur();
};

class Suiveur : public Shapes {
public:
	glm::vec3 position;
	Suiveur(glm::vec3 position);
	~Suiveur();
};


class Sniper : public Shapes {
public:
	glm::vec3 position;
	Sniper(const char* filename, vector<glm::vec4> sniper_vertices, vector<glm::vec3> sniper_normals, vector<GLushort> sniper_elements, glm::vec3 position);
	~Sniper();
};

class GridComponent : public Shapes {
	public:
		int obstacle;
		float cout_g;
		float cout_h;
		float cout_f;
		int parentx;
		int parenty;
		GridComponent(glm::vec3 position, int obstacle);
		GridComponent();
		~GridComponent();
		bool isVoisin(GridComponent G1);
		
};

class Boids : public Shapes {
public:
	int isMeneur;
	Boids(glm::vec3 position);
	Boids();
	~Boids();
};