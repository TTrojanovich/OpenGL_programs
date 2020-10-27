#define tag
#ifdef tag3
// ==================================== ROTATING CUBE ====================================
#include <iostream>
#include <fstream>
#include <cmath>
#include <glut.h>
#include <stack>
#include <cstdio>
#include <string>
#include <time.h>
#include "CGmath.h"

using namespace std;

#define FRAME_W 800
#define FRAME_H 800
#define FRAME_Z 800
const int xc = FRAME_W / 2;
const int yc = FRAME_H / 2;
const int zc = FRAME_Z / 2;


void Display(void)
{
	glEnable(GL_DEPTH_TEST);
	glClearColor(0.3, 0.5, 0.7, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glColor3d(1, 1, 1);
	
	double t = clock() / (double)CLOCKS_PER_SEC;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	


	MATR m1, m2, m3, m4;
	VEC v1, v = { 1, 4, 2 };

	//m1 = cmlMatrTranslate(1, 2, 3);
	//m2 = cmlMatrTranspos(m1);

	MATR arr;
	arr = cmlMatrRotate(t, 0, 1, 0);
	


	glRotated(30, 1, 0, 0);
	glRotated(30, 0, 1, 0);

	glColor3d(255, 255, 0); 
	glBegin(GL_LINES);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 1, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(1, 0, 0);
	glVertex3d(0, 0, 0);
	glVertex3d(0, 0, 1);
	glEnd();
	
	glMultMatrixd(arr.m);
	
	glColor3d(255, 255, 255);
	glutWireCube(1);
	
	glutSwapBuffers();
	glutPostRedisplay();
}




void Keyboard(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
}
int main(int ArgC, char *ArgV[])
{
	glutInit(&ArgC, ArgV);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(400, 150);
	glutInitWindowSize(FRAME_H, FRAME_W);
	glutCreateWindow("Application_3 Window");
	//glOrtho(0, FRAME_H, 0, FRAME_W, 0, FRAME_Z);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

#endif


