#define tag
#ifdef tag5
// ==================================== SPHERE WORLD APPLICATION WITH MIRROR ====================================
#include <iostream>
#include <glut.h>
#include <string>
#include <time.h>
#include <math.h>
#include "GLTools\GLTools.h"
#include "GLTools\OpenGLSB.h"

using namespace std;


#define FRAME_W 800
#define FRAME_H 800
#define FRAME_Z 800

GLTFrame spheres[50];
GLTFrame frameCamera;
static float xRot = 0, yRot = 0, TorRot = 0;
//static int x = -700, y = -700;
//GRASS = 0, 0.6, 0.2 ; SPHERE = 0.6, 0.6, 0.2 ; TORUS = 1, 0.8, 0 ; GROUND = 0.6, 0.4, 0.2
GLTVector3 PlanePoints[3] = { { 100, -11, 100 }, { 100, -11, -100 }, { -100, -11, 100 } };
float LightSrcPos[4] = { -200, 150, 200, 1 };
float LightSrcPosMirr[4] = { -200, -150, 200, 1 };
float AmbLight[4] = { 0.2, 0.2, 0.2, 1 };
float DiffLight[4] = { 1, 1, 1, 1 };


void SetupRC()
{
	glClearColor(0.3, 0.5, 0.7, 1.0);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	gltInitFrame(&frameCamera);

	for (int i = 0; i < 50; i++)
	{
		gltInitFrame(&spheres[i]);
		spheres[i].vLocation[0] = (float)((rand() % 800) - 400);
		spheres[i].vLocation[1] = 0;
		spheres[i].vLocation[2] = (float)((rand() % 800) - 400);
	}

	glLightfv(GL_LIGHT0, GL_AMBIENT, AmbLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, DiffLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, DiffLight);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
	glMateriali(GL_FRONT, GL_SHININESS, 128);
	glMaterialfv(GL_FRONT, GL_SPECULAR, DiffLight);

	gltMoveFrameForward(&frameCamera, -300);
	gltMoveFrameUp(&frameCamera, 10);
}

void DrawGround()
{
	int yPlane2 = 0;

	glColor4f(1, 1, 1, 0.5);
	glBegin(GL_QUADS);
	glVertex3f(405, yPlane2, -405);
	glVertex3f(-405, yPlane2, -405);

	glVertex3f(-405, yPlane2, 405);
	glVertex3f(405, yPlane2, 405);
	glEnd();
}


void DrawObjects()
{
	glPushMatrix();
	glTranslatef(0, 50, 0);
	glRotatef(-TorRot * 3, 0, 1, 0);
	glTranslatef(100, 0, 0);
	glColor3f(1, 1, 0);
	glutSolidSphere(10, 50, 50);
	glPopMatrix();

	glPushMatrix();
	TorRot += 1;
	glTranslatef(0, 45, 0);
	glRotatef(TorRot, 0, 1, 0);
	glColor3f(1, 0, 0);
	glutSolidTorus(15, 30, 60, 60);
	glPopMatrix();
}


void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gltApplyCameraTransform(&frameCamera);


	glLightfv(GL_LIGHT0, GL_POSITION, LightSrcPosMirr);
	glPushMatrix();
	glFrontFace(GL_CW);
	glScalef(1, -1, 1);
	DrawObjects();
	glFrontFace(GL_CCW);
	glPopMatrix();


	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	DrawGround();
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);


	glLightfv(GL_LIGHT0, GL_POSITION, LightSrcPos);
	DrawObjects();


	glPopMatrix();
	glutSwapBuffers();
}


void TimerFunc(int value)
{
	glutPostRedisplay();
	glutTimerFunc(20, TimerFunc, 1);
}


void ChangeSize(int w, int h)
{
	float fAspect;

	if (h == 0)	h = 1;

	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	fAspect = (float)w / (float)h;
	gluPerspective(60.0, fAspect, 1.0, FRAME_Z);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}
void Keyboard(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
	if (key == 'w') gltMoveFrameForward(&frameCamera, 10);
	if (key == 's') gltMoveFrameForward(&frameCamera, -10);
	if (key == 'a') gltRotateFrameLocalY(&frameCamera, 0.1);
	if (key == 'd') gltRotateFrameLocalY(&frameCamera, -0.1);

	//if (key == GLUT_KEY_UP)
	//if (key == GLUT_KEY_DOWN)
	//if (key == GLUT_KEY_LEFT)
	//if (key == GLUT_KEY_RIGHT)

	glutPostRedisplay();
}


int main()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(FRAME_W * 2, FRAME_H * 1.2);
	glutCreateWindow("Application_5 Window");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	SetupRC();
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(1000, TimerFunc, 1);
	glutMainLoop();
}

#endif