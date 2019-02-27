// Simplified Renderer application for GP course
// Features:
// Reduced OpenGL version from 4.5 to 3.3 to allow it to render in older laptops.
// Added Shapes library for rendering cubes, spheres and vectors.
// Added examples of matrix multiplication on Update.
// Added resize screen and keyboard callbacks.
// Added FPS camera functionality
// Update 2019/01 updated libraries and created project for VS2017 including directory dependant links to libraries.

// Suggestions or extra help please do email me at S.Padilla@hw.ac.uk

// Standard C++ libraries
#include <iostream>
#include <vector>
using namespace std;

// Helper graphic libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>
#include "graphics.h"
#include "shapes.h"
#include "grid.h"
#include "stdlib.h"
#include "time.h"

// MAIN FUNCTIONS
void startup();
void updateCamera();
void updateSceneElements();
void renderScene();

// CALLBACK FUNCTIONS
void onResizeCallback(GLFWwindow* window, int w, int h);	
void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void onMouseMoveCallback(GLFWwindow* window, double x, double y);
void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset);

// VARIABLES
bool		quit = false;
float		deltaTime = 0.0f;	// Keep track of time per frame.
float		lastTime = 0.0f;	// variable to keep overall time.
bool		keyStatus[1024];	// Hold key status.
int			maxBalles = 10;
int			maxParticles = 360;
int			maxSuiveur = 30;
int MeneusePasDansVec = 1;

// MAIN GRAPHICS OBJECT
Graphics	myGraphics;		// Runing all the graphics in this object


// DEMO OBJECTS
Cube		myCube = Cube(glm::vec3(0.0f, 50.0f, 0.0f), 1.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Cube        myFloor = Cube(glm::vec3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Cube        myFloor2 = Cube(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Cube        myFloor3 = Cube(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Cube        myFloor4 = Cube(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
Cube        myFloor5 = Cube(glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
vector<Particle> ParticleVector;
vector<Boids> Essaim;

glm::vec3 vecnull = glm::vec3(0, 0, 0);
vector<Particle> Balls;
int maxBalls = 10;



//Particle Particle1 = Particle(glm::vec3(5.0f, 2.0f, 0.0f), vecnull, vecnull, vecnull, 0, 0, 100000);

Boids myBoidsMeneuse = Boids(vecnull);

vector<GridComponent> shapes_vector;
vector<GridComponent> solution_vector;
Particle player_shape = Particle(vecnull, vecnull, vecnull, vecnull, 0, 0, 100000);
Grid myGrid = Grid(25, 25, glm::vec3(0.0f, 0.0f, 0.0f), player_shape);

vector<GridComponent> myvec;

bool Astar = false;
int playerposition = 0;



/*Suiveur mySuiveur1 = Suiveur(glm::vec3(0.0f, 0.5f, 0.2f));

Suiveur mySuiveur2 = Suiveur(glm::vec3(5.0f, 2.0f, 0.0f));
Suiveur mySuiveur3 = Suiveur(glm::vec3(3.0f, 0.0f, 0.0f));
Suiveur mySuiveur4 = Suiveur(glm::vec3(0.0f, 1.5f, 0.0f));*/

//vector<glm::vec4> sniper_vertices;
//vector<glm::vec3> sniper_normals;
//vector<GLushort> sniper_elements;

//Sniper mySniper = Sniper("Ball OBJ.obj",sniper_vertices, sniper_normals, sniper_elements, glm::vec3(0.0f, 2.0f, 0.0f));





// Some global variable to do the animation.
float t = 0.001f;			// Global variable for animation


int main()
{
	int errorGraphics = myGraphics.Init();		// Launch window and graphics context
	if (errorGraphics) return 0;				// Close if something went wrong...

	startup();									// Setup all necessary information for startup (aka. load texture, shaders, models, etc).



	// MAIN LOOP run until the window is closed
	while (!quit){

		// Update the camera tranform based on interactive inputs or by following a predifined path.
		updateCamera();

		// Update position, orientations and any other relevant visual state of any dynamic elements in the scene.
		updateSceneElements();

		// Render a still frame into an off-screen frame buffer known as the backbuffer.
		renderScene();

		// Swap the back buffer with the front buffer, making the most recently rendered image visible on-screen.
		glfwSwapBuffers(myGraphics.window);		// swap buffers (avoid flickering and tearing)
	
	} 


	myGraphics.endProgram();			// Close and clean everything up...

	cout << "\nPress any key to continue...\n";
	cin.ignore(); cin.get(); // delay closing console to read debugging errors.

	return 0;
}

glm::vec3 normalize(glm::vec3 vector)
{
	float norme = vector[0] + vector[1] + vector[2];
	return glm::vec3(vector[0] / norme, vector[1] / norme, vector[2] / norme);
}

void startup() {
	// Keep track of the running time
	GLfloat currentTime = (GLfloat)glfwGetTime();	// retrieve timelapse
	deltaTime = currentTime;						// start delta time
	lastTime = currentTime;							// Save for next frame calculations.

	// Callback graphics and key update functions - declared in main to avoid scoping complexity.
	// More information here : https://www.glfw.org/docs/latest/input_guide.html
	glfwSetWindowSizeCallback(myGraphics.window, onResizeCallback);			// Set callback for resize
	glfwSetKeyCallback(myGraphics.window, onKeyCallback);					// Set Callback for keys
	glfwSetMouseButtonCallback(myGraphics.window, onMouseButtonCallback);	// Set callback for mouse click
	glfwSetCursorPosCallback(myGraphics.window, onMouseMoveCallback);		// Set callback for mouse move
	glfwSetScrollCallback(myGraphics.window, onMouseWheelCallback);			// Set callback for mouse wheel.

	// Calculate proj_matrix for the first time.
	myGraphics.aspect = (float)myGraphics.windowWidth / (float)myGraphics.windowHeight;
	myGraphics.proj_matrix = glm::perspective(glm::radians(50.0f), myGraphics.aspect, 0.1f, 1000.0f);

	// Load Geometry examples
	myCube.Load();

	myGrid.FillGrid();
	myGrid.DefineStartAndEnd();
	myGrid.Spawn_Player();
	myGrid.player_shape.Load();
	//myGrid.Astar();


	srand(time(0));
	

	myFloor.Load();
	myFloor2.Load();
	myFloor3.Load();
	myFloor4.Load();
	myFloor5.Load();

	myFloor.fillColor = glm::vec4(130.0f / 255.0f, 96.0f / 255.0f, 61.0f / 255.0f, 1.0f);    // Sand Colour
	myFloor.lineColor = glm::vec4(130.0f / 255.0f, 96.0f / 255.0f, 61.0f / 255.0f, 1.0f);    // Sand again

	myFloor2.fillColor = glm::vec4(0.5, 0.5, 0.5, 1.0f);    
	myFloor2.lineColor = glm::vec4(0,0,0, 1.0f);    

	myFloor3.fillColor = glm::vec4(1, 0.5, 0.5, 1.0f);
	myFloor3.lineColor = glm::vec4(0, 0, 0, 1.0f);

	myFloor4.fillColor = glm::vec4(0, 0.5, 0.5, 1.0f);
	myFloor4.lineColor = glm::vec4(0, 0, 0, 1.0f);

	myFloor5.fillColor = glm::vec4(0.5, 0.5, 0, 1.0f);
	myFloor5.lineColor = glm::vec4(0, 0, 0, 1.0f);
	//mySniper.fillColor = glm::vec4(1.0f, 1.0f, 1.0f, 125.0f);

	

	

	// Optimised Graphics
	myGraphics.SetOptimisations();		// Cull and depth testing
	
}

void updateCamera() {

	// calculate movement for FPS camera
	GLfloat xoffset = myGraphics.mouseX - myGraphics.cameraLastX;	
	GLfloat yoffset = myGraphics.cameraLastY - myGraphics.mouseY;	// Reversed mouse movement
	myGraphics.cameraLastX = (GLfloat)myGraphics.mouseX; 
	myGraphics.cameraLastY = (GLfloat)myGraphics.mouseY;

	GLfloat sensitivity = 0.05f;
	xoffset *= sensitivity; 
	yoffset *= sensitivity;

	myGraphics.cameraYaw += xoffset; 
	myGraphics.cameraPitch += yoffset;

	// check for pitch out of bounds otherwise screen gets flipped
	if (myGraphics.cameraPitch > 89.0f) myGraphics.cameraPitch = 89.0f;
	if (myGraphics.cameraPitch < -89.0f) myGraphics.cameraPitch = -89.0f;

	
	// Calculating FPS camera movement (See 'Additional Reading: Yaw and Pitch to Vector Calculations' in VISION) 
	glm::vec3 front;
	front.x = cos(glm::radians(myGraphics.cameraYaw)) * cos(glm::radians(myGraphics.cameraPitch));
	front.y = sin(glm::radians(myGraphics.cameraPitch));
	front.z = sin(glm::radians(myGraphics.cameraYaw)) * cos(glm::radians(myGraphics.cameraPitch));

	myGraphics.cameraFront = glm::normalize(front);

	// Update movement using the keys
	GLfloat cameraSpeed = 1.0f * deltaTime;
	if (keyStatus[GLFW_KEY_W]) myGraphics.cameraPosition += cameraSpeed * myGraphics.cameraFront;
	if (keyStatus[GLFW_KEY_S]) myGraphics.cameraPosition -= cameraSpeed * myGraphics.cameraFront;
	if (keyStatus[GLFW_KEY_A]) myGraphics.cameraPosition -= glm::normalize(glm::cross(myGraphics.cameraFront, myGraphics.cameraUp)) * cameraSpeed;
	if (keyStatus[GLFW_KEY_D]) myGraphics.cameraPosition += glm::normalize(glm::cross(myGraphics.cameraFront, myGraphics.cameraUp)) * cameraSpeed;
	if (keyStatus[GLFW_KEY_J]) myGraphics.cameraPosition = glm::vec3(13,22,10); // camera on A* visualization
	
	if (keyStatus[GLFW_KEY_P]) myGraphics.cameraPosition = glm::vec3(1, 10, 40); // camera on particles
	if (keyStatus[GLFW_KEY_N]) myGraphics.cameraPosition = glm::vec3(0, 10, -40); // Cam sur 

	//if (keyStatus[GLFW_KEY_G]) myGrid.player_shape.position = myvec[++playerposition].position + glm::vec3(0, 1, 0);

	//if(keyStatus[GLFW_KEY_U])  myGrid.Astar(myGraphics);
	
	//if (keyStatus[GLFW_KEY_L]) myGrid.player_shape.SlowMove(myGrid.player_shape.position + glm::vec3(1,0,0));
																				// IMPORTANT PART
	// Calculate my view matrix using the lookAt helper function
	myGraphics.viewMatrix = glm::lookAt(myGraphics.cameraPosition,		// eye
		myGraphics.cameraPosition + myGraphics.cameraFront,				// centre
		myGraphics.cameraUp);											// up

}

void updateSceneElements() {

	glfwPollEvents();								// poll callbacks

	// Calculate frame time/period -- used for all (physics, animation, logic, etc).
	GLfloat currentTime = (GLfloat)glfwGetTime();	// retrieve timelapse
	deltaTime = currentTime - lastTime;				// Calculate delta time
	lastTime = currentTime;							// Save for next frame calculations.


	
	if (!Astar)
	{
		vector<GridComponent> myvec = myGrid.Astar();
		myGrid.DrawSolution(myvec);
		Astar = true;
	}
	
	

	// Do not forget your ( T * R * S ) http://www.opengl-tutorial.org/beginners-tutorials/tutorial-3-matrices/

	if (Balls.size() < maxBalls && t > 45)
	{
		if ((int)(10 * t) % 15 == 0)
		{
			Particle myBall = Particle(glm::vec3(sin(t)*3.0f, 10.0f, -30.0F), glm::vec3( 4 + 8*sin(t*(rand()%5))*sin(rand()%5), 8.0f, 4 + 8*cos(t*(rand() % 5))*sin(rand() % 5)), vecnull, vecnull, 3.0f, 3.0f, 1000);
			myBall.lineColor = glm::vec4(0, 0, 0, 0);
			myBall.Load();
			myBall.fillColor = glm::vec4(sin(t), cos(t), sin(t)*cos(t), 1); // R,G,B,T
			Balls.push_back(myBall);
		}

	}
	vector<Particle>::iterator k;
	for (k = Balls.begin(); k != Balls.end(); ++k)
	{
		if ((*k).position[2] > -27)
		{
			(*k).position[2] = -27;
			(*k).speed[2] = -(*k).speed[2] / (*k).mass;
		}
		if ((*k).position[1] < 1)
		{
			(*k).position[1] = 1;
			(*k).speed[1] = -(*k).speed[1] / (*k).mass;
			(*k).speed[2] -= (*k).speed[2] / 10;
			(*k).speed[0] -= (*k).speed[0] / 10;
		}
		if ((*k).position[0] > 4)
		{
			(*k).position[0] = 4;
			(*k).speed[0] = -(*k).speed[0] / (*k).mass;
		}
		if ((*k).position[0] < -4)
		{
			(*k).position[0] = -4;
			(*k).speed[0] = -(*k).speed[0] / (*k).mass;
		}
		
		vector<Particle>::iterator f;
		for (f = Balls.begin(); f != Balls.end(); ++f)
		{
			if (length(((*k).position - (*f).position)) < 1)
			{
				(*k).speed += ((*k).position - (*f).position);
			}
		}
		(*k).ApplyGravity(0.05f);
	}

	if (ParticleVector.size() < maxParticles)
	{
		if ((int)(10*t) % 3 == 0)
		{
			Particle myParticle = Particle(glm::vec3(cos(rand()%360), 2.0f, 65 + cos(rand() % 360)), glm::vec3(sin(rand()%10)*2*sin(rand() % 10), 10 + cos(t), cos(rand() % 10)* 2 * sin(rand() % 10)), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(rand()%10, rand() % 10, rand() % 10), rand()%1 + 1, 1.5 + (rand() % 100) / 50, 100);
			myParticle.lineColor = glm::vec4(0, 0, 0, 0);
			myParticle.rotationVector = glm::cross(myParticle.acceleration, myParticle.speed);
			myParticle.Load();
			myParticle.fillColor = glm::vec4(0.5, 0.5, 0.5, 1); // R,G,B,T
			ParticleVector.push_back(myParticle);
		}
		
	}

	vector<Particle>::iterator v;
	for (v = ParticleVector.begin(); v != ParticleVector.end(); ++v)
	{
		// If the particle time to live is more than zero...
		if ((*v).lifespan > 0)
		{
			// ...update the particle position position and draw it.
			//(*k).Render(myGraphics);
			(*v).ApplyGravity(0.05f);
			//(*i).fillColor = glm::vec4(sin(0.1*t), cos(0.2*t), sin(0.3*t), 0.5 + 0.5*sin(t)); // R,G,B,T

			(*v).lifespan = (*v).lifespan - 1;

			(*v).fillColor = glm::vec4(0.5, 0.5, 0.5, ((float)(*v).lifespan / 100));

			if ((*v).position[1] < 1) // if the ball touches the ground, it bounces (accordingly to its mass)
			{
				(*v).position[1] = 1;
				(*v).speed[1] = -(*v).speed[1] / (*v).mass;
				(*v).speed[0] = (*v).speed[0] - (*v).speed[0] / 30;
				(*v).speed[2] = (*v).speed[2] - (*v).speed[2] / 30;
			}

		}
		else // // If it's time to destroy the particle...
		{
			// ...then remove it from the vector...
			v = ParticleVector.erase(v);
		}

	} // End of iterator loop

	if (MeneusePasDansVec)
	{
		myBoidsMeneuse.isMeneur = 1;
		myBoidsMeneuse.fillColor = glm::vec4(0, 0, 0, 1);
		myBoidsMeneuse.Load();
		Essaim.push_back(myBoidsMeneuse);
		MeneusePasDansVec = 0;
	}

	

	myBoidsMeneuse.fillColor = glm::vec4(sin(0.1*t), cos(0.2*t), sin(0.3*t), 0.5 + 0.5*sin(t));

	//myBoidsMeneuse.RenderBoids(myGraphics);

	myBoidsMeneuse.setposition(glm::vec3(
		7 * (sin(0.1*t*2.0f) + 2 * sin(0.2*t*2.0f)),
		7 * (cos(0.1*t*2.0f) - 2 * cos(0.2*t*2.0f)) + 30,
		7 * (-sin(0.3*t*2.0f)
	)));

	if (Essaim.size() < maxSuiveur)
	{
		if ((int)(10 * t) % 40 == 0)
		{
			Boids myBoids = Boids(glm::vec3(rand() % 30, rand() % 30 + 30, rand() % 30));
			myBoids.lineColor = glm::vec4(0, 0, 0, 0);
			myBoids.Load();
			
			//myParticle.fillColor = glm::vec4(sin(0.1*t), cos(0.2*t), sin(0.3*t), 0.5 + 0.5*sin(t)); // R,G,B,T
			Essaim.push_back(myBoids);
		}
	}

	vector<Boids>::iterator r;
	for (r = Essaim.begin(); r != Essaim.end(); ++r)
	{
			// ...update the particle position position and draw it.

			glm::vec3 position_moyenne_voisins = glm::vec3(0.0f, 0.0f, 0.0f); // temp vector for rule 1
			glm::vec3 separation_vector = glm::vec3(0.0f, 0.0f, 0.0f); // temp vector for rule 2
			glm::vec3 velocity_match = glm::vec3(0.0f, 0.0f, 0.0f); // temp vector for rule 3

			vector<Boids>::iterator g;
			for (g = Essaim.begin(); g != Essaim.end(); ++g) // COHESION RULE
			{
				if ((*r).position != (*g).position && (*r).isMeneur == 0)
				{
					position_moyenne_voisins[0] = position_moyenne_voisins[0] + (*g).position[0];
					position_moyenne_voisins[1] = position_moyenne_voisins[1] + (*g).position[1];
					position_moyenne_voisins[2] = position_moyenne_voisins[2] + (*g).position[2];
				}
					
				if ((*r).Dist((*g).position) && (*r).isMeneur == 0) // SEPARATION RULE
				{
					separation_vector = separation_vector - 1.3f*((*g).position - (*r).position);
				}

				if ((*r).position != (*g).position && (*r).isMeneur == 0) // ALIGNEMENT RULE
				{
					velocity_match += glm::vec3((*g).speed[0]/8, (*g).speed[1]/8, (*g).speed[2]/8);
				}

			}

			position_moyenne_voisins = glm::vec3(position_moyenne_voisins[0] / (Essaim.size() - 1), position_moyenne_voisins[1] / (Essaim.size() - 1), position_moyenne_voisins[2] / (Essaim.size() - 1));
			glm::vec3 posi_cal = (position_moyenne_voisins - (*r).position)*0.01f; 
			
			velocity_match = glm::vec3(velocity_match[0] / (Essaim.size() - 1), velocity_match[1] / (Essaim.size() - 1), velocity_match[2] /(Essaim.size() - 1));
			glm::vec3 velocity_cal = (velocity_match - (*r).speed);
			(*r).speed = (*r).speed + posi_cal + velocity_cal + separation_vector + (myBoidsMeneuse.position - (*r).position);
			(*r).UpdateVector(0.015f);			
	
			//(*i).fillColor = glm::vec4(sin(0.1*t), cos(0.2*t), sin(0.3*t), 0.5 + 0.5*sin(t)); // R,G,B,T
	}
// Disable 2D textures and point sprites
	glDisable(GL_POINT_SPRITE);
	glDisable(GL_TEXTURE_2D);

	// ----- Stop Drawing Stuff! ------

	//glfwSwapBuffers(); // Swap the buffers to display the scene (so we don't have to watch it being drawn!)

	// Calculate Particles position
	
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_POINT_SPRITE);

	// Calculate floor position and resize
	myFloor.mv_matrix = myGraphics.viewMatrix *
		glm::translate(glm::vec3(0.0f, 0.0f, 0.0f)) *
		glm::scale(glm::vec3(1000.0f, 0.001f, 1000.0f)) *
		glm::mat4(1.0f);
	myFloor.proj_matrix = myGraphics.proj_matrix;

	myFloor2.mv_matrix = myGraphics.viewMatrix *
		glm::translate(glm::vec3(0.0f, 0.0f, -30.0f)) *
		glm::scale(glm::vec3(30.0f, 1.0f, 30.0f)) *
		glm::mat4(1.0f);
	myFloor2.proj_matrix = myGraphics.proj_matrix;

	myFloor3.mv_matrix = myGraphics.viewMatrix *
		glm::translate(glm::vec3(0.0f, 5.0f, -25.0f)) *
		glm::scale(glm::vec3(20.0f, 20.0f, 1.0f)) *
		glm::mat4(1.0f);
	myFloor3.proj_matrix = myGraphics.proj_matrix;

	myFloor4.mv_matrix = myGraphics.viewMatrix *
		glm::translate(glm::vec3(5.0f, 5.0f, -30.0f)) *
		glm::scale(glm::vec3(1.0f, 20.0f, 20.0f)) *
		glm::mat4(1.0f);
	myFloor4.proj_matrix = myGraphics.proj_matrix;

	myFloor5.mv_matrix = myGraphics.viewMatrix *
		glm::translate(glm::vec3(-5.0f, 5.0f, -30.0f)) *
		glm::scale(glm::vec3(1.0f, 20.0f, 20.0f)) *
		glm::mat4(1.0f);
	myFloor5.proj_matrix = myGraphics.proj_matrix;

	t += 0.1f; // increment movement variable

	if (glfwWindowShouldClose(myGraphics.window) == GL_TRUE) quit = true; // If quit by pressing x on window.

}

void renderScene() 
{
	// Clear viewport - start a new frame.
	myGraphics.ClearViewport();

	// Draw objects in screen
	myFloor.Draw();
	myFloor2.Draw();
	myFloor3.Draw();
	myFloor4.Draw();
	myFloor5.Draw();
	//myBall.Draw();

	for (int i = 0; i < ParticleVector.size(); i++)
	{
		ParticleVector[i].Render(myGraphics);
	}
	for (int i = 0; i < Balls.size(); i++)
	{
		Balls[i].Render(myGraphics);
	}
	for (int i = 0; i < Essaim.size(); i++)
	{
		Essaim[i].RenderBoids(myGraphics, -1.0f*(myBoidsMeneuse.position - Essaim[i].position));
	}
	//myBoidsMeneuse.RenderBoids(myGraphics);
	
	//if (keyStatus[GLFW_KEY_U]) myGrid.DrawSolution();

	myGrid.DrawGrid(myGraphics);
	
	//Particle1.Draw();
	myGrid.player_shape.Render(myGraphics);

}


// CallBack functions low level functionality.
void onResizeCallback(GLFWwindow* window, int w, int h) {	// call everytime the window is resized
	myGraphics.windowWidth = w;
	myGraphics.windowHeight = h;

	myGraphics.aspect = (float)w / (float)h;
	myGraphics.proj_matrix = glm::perspective(glm::radians(50.0f), myGraphics.aspect, 0.1f, 1000.0f);
}

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) { // called everytime a key is pressed
	if (action == GLFW_PRESS) keyStatus[key] = true;
	else if (action == GLFW_RELEASE) keyStatus[key] = false;

	// toggle showing mouse.
	if (keyStatus[GLFW_KEY_M]) myGraphics.ToggleMouse();

	// If exit key pressed.
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

}

void onMouseMoveCallback(GLFWwindow* window, double x, double y) {
	int mouseX = static_cast<int>(x);
	int mouseY = static_cast<int>(y);

	myGraphics.mouseX = mouseX;
	myGraphics.mouseY = mouseY;

	// helper variables for FPS camera
	if (myGraphics.cameraFirstMouse) {
		myGraphics.cameraLastX = (GLfloat)myGraphics.mouseX; myGraphics.cameraLastY = (GLfloat)myGraphics.mouseY; myGraphics.cameraFirstMouse = false;
	}
}

static void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset) {
	int yoffsetInt = static_cast<int>(yoffset);
}
