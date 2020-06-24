#define tag
#ifdef tag6
// ==================================== SPIRAL MOTION BLUR ====================================
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
float AmbLight[4] = { 0.2, 0.2, 0.2, 1 };
float DiffLight[4] = { 1, 1, 1, 1 };
float BlackLight[4] = { 0, 0, 0, 1 };
float fogColor[4] = { 0.3, 0.5, 0.7, 1.0 };
GLTMatrix ShadowMatrix;


void SetupRC()
{
	glEnable(GL_MULTISAMPLE);

	glClearColor(0.25, 0.25, 0.25, 1.0);
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

	gltMakeShadowMatrix(PlanePoints, LightSrcPos, ShadowMatrix);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	gltMoveFrameForward(&frameCamera, -100);
}

void DrawGround()
{
	float fExtent = 405;
	float fStep = 20;
	float y = -10;
	float fColor, iStrip, iRun;
	int iBounce = 0;

	for (iStrip = -fExtent; iStrip <= fExtent; iStrip += fStep)
	{
		glBegin(GL_QUADS);
		for (iRun = fExtent; iRun >= -fExtent; iRun -= fStep)
		{
			if ((iBounce % 2) == 0)
				fColor = 1.0f;
			else
				fColor = 0.0f;

			glColor4f(fColor, fColor, fColor, 1);
			glVertex3f(iStrip, y, iRun);
			glVertex3f(iStrip + fStep, y, iRun);
			glVertex3f(iStrip + fStep, y, iRun - fStep);
			glVertex3f(iStrip, y, iRun - fStep);

			iBounce++;
		}
		glEnd();
	}
}


void DrawObjects()
{
	static int y = 25, sRot = 0;
	glTranslatef(0, y, 0);
	glColor3f(1,0,0);
	for (int sRot = 0; sRot <= 180; sRot += 30)
	{
		glPushMatrix();
		glRotatef(sRot, 0, 0, 1);
		glTranslatef(30, 0, 0);
		glutSolidSphere(5, 50, 50);
		glPopMatrix();
		if (sRot == 0) glAccum(GL_LOAD, 0.9);
		else glAccum(GL_LOAD, 0.5 * (1.0 / sRot));
		glColor3f(0, 1, 0);
	}
	
	//if (sRot > 360) sRot = 0;
	
}


void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	gltApplyCameraTransform(&frameCamera);


	DrawGround();
	DrawObjects();

	glAccum(GL_RETURN, 1);

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
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(FRAME_W * 2, FRAME_H * 1.2);
	glutCreateWindow("Application_6 Window");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	SetupRC();
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(1000, TimerFunc, 1);
	glutMainLoop();
}

#endif