#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <time.h>
#include <math.h>
#include <Eigen/Dense>

#include "Particles.h"
#include "Spring.h"
#include "Cloth.h"

using namespace std;
using namespace Eigen;

Vector3f gravity(0.0f, -0.98f, 0.0f);

//number of particles in a grid
//const int gridSize = 13;
int gridWidth, gridHeight;

//Spring properties
int numS;
float springConst = 10.0f;
float restLength = 1.0f;
float damper = 0.8f;
//Spring *springs = NULL;
vector<Spring> springs;

//Particle properties
int numP;
float mass = 0.01f;
Particles *p1=NULL;
Particles *p2=NULL;
Particles *currentP=NULL;
Particles *nextP=NULL;
vector<Particles> p;





//****************************************************
// Some Classes
//****************************************************
class Viewport;

class Viewport {
  public:
    int w, h; // width and height
};

//****************************************************
// Global Variables
//****************************************************
Viewport	viewport;

//****************************************************
// Simple init function
//****************************************************
void initScene(){
	/*
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0); 
	GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	*/
  // Nothing to do here for this simple example.

  p1 = new Particles[gridWidth*gridHeight];
  p2 = new Particles[gridWidth*gridHeight];
  //springs = new Spring[]

}


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.w = w;
  viewport.h = h;
  
  if(h == 0) { h = 1; }
  float ratio = 1.0* w / h;

  glViewport (0,0,viewport.w,viewport.h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  

}


//****************************************************
// A routine to set a pixel by drawing a GL point.  This is not a
// general purpose routine as it assumes a lot of stuff specific to
// this example.
//****************************************************

void setPixel(int x, int y, GLfloat r, GLfloat g, GLfloat b) {
  glColor3f(r, g, b);
  glVertex2f(x + 0.5, y + 0.5);   // The 0.5 is to target pixel
  // centers 
  // Note: Need to check for gap
  // bug on inst machines.
}

//****************************************************
// Cloth Methods
//****************************************************
void drawTriangle(Particles p1, Particles p2, Particles p3){

  //p1
  glNormal3fv((GLfloat *) &(p1.normal));
  glVertex3fv((GLfloat *) &(p1.position));

  //p2
  glNormal3fv((GLfloat *) &(p2.normal));
  glVertex3fv((GLfloat *) &(p2.position));

  //p3
  glNormal3fv((GLfloat *) &(p3.normal));
  glVertex3fv((GLfloat *) &(p3.position));

}

Vector3f triangleNorm(Particles p1, Particles p2, Particles p3){
  Vector3f v1 = p2.position - p1.position;
  Vector3f v2 = p3.position - p1.position;

  return v1.cross(v2);
}

vector<Particles> drawCloth(float width, float height, int gridWidth, int gridHeight){
  //cloth structure as grid of particles
  p.resize(gridWidth*gridHeight);
  for(int i=0; i<gridWidth; i++){
    for(int j=0; j<gridHeight; j++){
      Particles n;
      n.position = Vector3f(width * (i/(float)gridWidth), -height * (j/(float)gridHeight), 0);
      p[j*gridWidth+i]= n;
    }
  }

  //connecting immediate particles with springs
  for(int i=0; i<gridWidth; i++){
    for(int j=0; j<gridHeight; j++){

      if(i<gridWidth-1){
        springs.push_back(Spring(p[j*gridWidth+i],p[j*gridWidth+(i+1)]));
      }
      if(j<gridHeight-1){
        springs.push_back(Spring(p[j*gridWidth+i],p[(j+1)*gridWidth+i]));
      } 
      if(i<gridWidth-1 && j<gridHeight-1){
        springs.push_back(Spring(p[j*gridWidth+i],p[(j+1)*gridWidth+(i+1)]));
      }
      if(i<gridWidth-1 && j<gridHeight-1){
        springs.push_back(Spring(p[j*gridWidth+(i+1)],p[(j+1)*gridWidth+i]));
      }
    }
  }

  // Connecting secondary neighbors with springs (distance 2 and sqrt(4) in the grid)
    for(int i=0; i<gridWidth; i++)
    {
      for(int j=0; j<gridHeight; j++)
      {
        if(i<gridWidth-2){
          springs.push_back(Spring(p[j*gridWidth+i],p[j*gridWidth+(i+2)]));
        }
        if(j<gridHeight-2){
          springs.push_back(Spring(p[j*gridWidth+i],p[(j+2)*gridWidth+i]));
        }
        if(i<gridWidth-2 && j<gridHeight-2){
          springs.push_back(Spring(p[j*gridWidth+i],p[(j+2)*gridWidth+(i+2)]));
        }
        if(i<gridWidth-2 && j<gridHeight-2){
          springs.push_back(Spring(p[j*gridWidth+(i+2)],p[(j+2)*gridWidth+i]));
        }
      }
    }

}

void shader(){

  for(int i = 0; i<gridWidth-1; i++){
    for(int j=0; j<gridHeight-1; j++){
      Vector3f n1 = triangleNorm(p[j*gridWidth+(i+1)],p[j*gridWidth+i],p[(j+1)*gridWidth+i]);
      p[j*gridWidth+(i+1)].normal += n1;
      p[j*gridWidth+i].normal += n1;
      p[(j+1)*gridWidth+i].normal += n1;

      Vector3f n2 = triangleNorm(p[(j+1)*gridWidth+(i+1)],p[j*gridWidth+(i+1)],p[(j+1)*gridWidth+i]);
      p[(j+1)*gridWidth+(i+1)].normal += n2;
      p[j*gridWidth+(i+1)].normal += n2;
      p[(j+1)*gridWidth+i].normal += n2;
    }
  }

  glBegin(GL_TRIANGLES);
  glColor3f(1.0f,1.0f,1.0f);
  for(int i = 0; i<gridWidth-1; i++){
    for(int j=0; j<gridHeight-1; j++){
      
      drawTriangle(p[j*gridWidth+(i+1)],p[j*gridWidth+i],p[(j+1)*gridWidth+i]);
      drawTriangle(p[(j+1)*gridWidth+(i+1)],p[j*gridWidth+(i+1)],p[(j+1)*gridWidth+i]);
    }
  }
  glEnd();
}

vector<Particles> cloth1 = drawCloth(14,10,55,45);

//****************************************************
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);             // indicate we are specifying camera transformations
	glLoadIdentity();
  shader();
	
	glPopMatrix();
	
	glFlush();
  glutSwapBuffers();          // swap buffers (we earlier set double buffer)
  glutPostRedisplay();
}


int main(int argc, char** argv) {
	
	
	//This initializes glut
  	glutInit(&argc, argv);
  	
	  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  	// Initalize theviewport size
  	viewport.w = 400;
  	viewport.h = 400;

  	//The size and position of the window
  	glutInitWindowSize(viewport.w, viewport.h);
  	glutInitWindowPosition(0,0);
  	glutCreateWindow(argv[0]);
  
  	initScene();							// quick function to set up scene
    
    glutReshapeFunc(myReshape);        // function to run when the window gets resized
  	glutDisplayFunc(myDisplay);				// function to run when its time to draw somethin
  	
  	glutMainLoop();							// infinite loop that will keep drawing and resizing
  	// and whatever else
  	
	return 0;

}


