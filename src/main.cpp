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
#include "Constraints.h"

using namespace std;
using namespace Eigen;

Vector3f gravity(0.0f, -0.98f, 0.0f);

//Spring properties
int springNum;
float springConst = 10.0f;
float restLength = 1.0f;
float damper = 0.8f;


//Particle properties
int particleNum;
float mass = 0.01f;

//number of particles grid
const int gridSize = 13;

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
// function that does the actual drawing of stuff
//***************************************************
void myDisplay() {

	glMatrixMode(GL_MODELVIEW);             // indicate we are specifying camera transformations
	glLoadIdentity();

  //draw cloth
  glEnable(GL_LIGHTING);
  glMaterialfv(GL_FRONT, GL_AMBIENT, COLOR(0.8f, 0.0f, 1.0f));  //set material
  glMaterialfv(GL_FRONT, GL_DIFFUSE, COLOR(0.8f, 0.0f, 1.0f));
  glMaterialfv(GL_BACK, GL_AMBIENT, COLOR(1.0f, 1.0f, 0.0f));
  glMaterialfv(GL_BACK, GL_DIFFUSE, COLOR(1.0f, 1.0f, 0.0f));
  glBegin(GL_TRIANGLES);
  {
      for(int i=0; i<gridSize-1; ++i)
      {
        for(int j=0; j<gridSize-1; ++j)
        {
          glNormal3fv(currentBalls[i*gridSize+j].normal);
          glVertex3fv(currentBalls[i*gridSize+j].position);
          glNormal3fv(currentBalls[i*gridSize+j+1].normal);
          glVertex3fv(currentBalls[i*gridSize+j+1].position);
          glNormal3fv(currentBalls[(i+1)*gridSize+j].normal);
          glVertex3fv(currentBalls[(i+1)*gridSize+j].position);

          glNormal3fv(currentBalls[(i+1)*gridSize+j].normal);
          glVertex3fv(currentBalls[(i+1)*gridSize+j].position);
          glNormal3fv(currentBalls[i*gridSize+j+1].normal);
          glVertex3fv(currentBalls[i*gridSize+j+1].position);
          glNormal3fv(currentBalls[(i+1)*gridSize+j+1].normal);
          glVertex3fv(currentBalls[(i+1)*gridSize+j+1].position);
        }
      }
    }
    glEnd();
    glDisable(GL_LIGHTING);
  }
	
	glPopMatrix();
	
	glFlush();
  	glutSwapBuffers();          // swap buffers (we earlier set double buffer)
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


