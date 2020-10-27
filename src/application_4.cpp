#define tag4
#ifdef tag4
// ==================================== SPHERE WORLD APPLICATION WITH LIGHTING AND FOG ====================================
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
GLTVector3 PlanePoints[3] = { { 100, -10, 100 }, { 100, -10, -100 }, { -100, -10, 100 } };
float LightSrcPos[4] = { -200, 150, 200, 1 };
float AmbLight[4] = { 0.2, 0.2, 0.2, 1 };
float DiffLight[4] = { 1, 1, 1, 1 };
float BlackLight[4] = { 0, 0, 0, 1 };
float fogColor[4] = { 0.3, 0.5, 0.7, 1.0 };
GLTMatrix ShadowMatrix;


void SetupRC()
{
	glEnable(GL_MULTISAMPLE);

	glClearColor(0.3, 0.5, 0.7, 1.0);

	glStencilOp(GL_INCR, GL_INCR, GL_INCR);
	glClearStencil(0);
	glStencilFunc(GL_EQUAL, 0x0, 0x01);

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	gltInitFrame(&frameCamera);

	glEnable(GL_FOG);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_START, 50);
	glFogf(GL_FOG_END, 500);
	glFogi(GL_FOG_MODE, GL_LINEAR);

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
	glMateriali(GL_FRONT, GL_SHININESS, 128);

	gltMoveFrameForward(&frameCamera, -300);
}

void DrawGround()
{
	float fExtent = 405;
	float fStep = 20;
	float y = -10;
	float fColor, iStrip, iRun;
	int iBounce = 0;

	//glShadeModel(GL_FLAT);
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
	//glShadeModel(GL_SMOOTH);
}


void DrawObjects(bool ShadowToggle)
{
	if (ShadowToggle == 1) glColor4f(0, 0, 0, 0.7);

	for (int i = 0; i < 50; i++)
	{
		glPushMatrix();
		gltApplyActorTransform(&spheres[i]);
		if (ShadowToggle == 0) glColor3f(0, 1, 0);
		glutSolidSphere(5, 50, 50);
		glPopMatrix();
	}

	glPushMatrix();
	glTranslatef(0, 50, 0);
	glRotatef(-TorRot * 3, 0, 1, 0);
	glTranslatef(100, 0, 0);
	if (ShadowToggle == 0) glColor3f(1, 1, 0);
	glutSolidSphere(10, 50, 50);
	glPopMatrix();

	glPushMatrix();
	TorRot += 1;
	glTranslatef(0, 38, 0);
	glRotatef(TorRot, 0, 1, 0);
	if (ShadowToggle == 0) glColor3f(1, 0, 0);
	glutSolidTorus(15, 30, 60, 60);
	glPopMatrix();

	glPushMatrix();
	if (ShadowToggle == 0) glColor3f(0, 0.4, 1);
	glTranslatef(150, 30, 50);
	glutSolidSphere(30, 50, 50);
	glPopMatrix();

	glPushMatrix();
	if (ShadowToggle == 0) glColor3f(0.8, 0.2, 0.4);
	glTranslatef(150, 30, 100);
	glutSolidCone(4, 6, 15, 15);
	if (ShadowToggle == 0) glColor3f(1, 1, 0);
	glutSolidSphere(3, 50, 50);
	glPopMatrix();
}


void RenderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	
	glPushMatrix();

	gltApplyCameraTransform(&frameCamera);

	glLightfv(GL_LIGHT0, GL_POSITION, LightSrcPos);

	DrawGround();

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_STENCIL_TEST);

	glPushMatrix();
	glMultMatrixf(ShadowMatrix);
	DrawObjects(1);
	glPopMatrix();

	glPushMatrix();
	glColor3f(1, 1, 0);
	glTranslatef(LightSrcPos[0], LightSrcPos[1], LightSrcPos[2]);
	glutSolidSphere(50, 50, 50);
	glPopMatrix();

	glDisable(GL_STENCIL_TEST);
	glDisable(GL_BLEND);

	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);

	glMaterialfv(GL_FRONT, GL_SPECULAR, DiffLight);

	DrawObjects(0);

	glMaterialfv(GL_FRONT, GL_SPECULAR, BlackLight);

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

	if (key == 'e') gltMoveFrameUp(&frameCamera, 5);
	if (key == 'q') gltMoveFrameUp(&frameCamera, -5);
	//if (key == GLUT_KEY_LEFT)
	//if (key == GLUT_KEY_RIGHT)

	glutPostRedisplay();
} 


int main()
{
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH | GLUT_STENCIL | GLUT_MULTISAMPLE);
	glutInitWindowPosition(300, 50);
	glutInitWindowSize(FRAME_W * 2, FRAME_H * 1.2);
	glutCreateWindow("Application_4 Window");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	SetupRC();
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(1000, TimerFunc, 1);
	glutMainLoop();
}

#endif