#define tag
#ifdef tag1
// ==================================== SUN AND TEXT ====================================
#include <iostream>
#include <fstream>
#include <cmath>
#include <glut.h>
#include <stack>
#include <cstdio>
#include <string>
#include "GLTools\GLTools.h"
#include "GLTools\OpenGLSB.h"

using namespace std;

#define FRAME_W 800
#define FRAME_H 800
const int xc = FRAME_W / 2;
const int yc = FRAME_H / 2;


struct color
{
	float red;
	float green;
	float blue;
};


void DrawPoint(int x, int y, color c = { 1, 1, 1 })
{
	glBegin(GL_POINTS);
	glColor3f(c.red, c.green, c.blue);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}


void DrawLine(int x1, int y1, int x2, int y2)
{
	int x, y;
	x = x1; y = y1;
	DrawPoint(x1, y1);
	while (x <= x2)
	{
		if ((y2 - y1) * (x + 1) - (x2 - x1) * (y + 0.5) - (x1*(y2 - y1) - y1*(x2 - x1)) > 0)
		{
			y = y + 1;
		}
		x = x + 1;
		DrawPoint(x, y);
	}
}


void DrawCircle(int x0, int y0, int r)
{
	int dx = x0 - xc, dy = y0 - yc;
	int x = xc, y = yc + r;
	DrawPoint(xc - r + dx, yc + dy);
	DrawPoint(xc + r + dx, yc + dy);
	DrawPoint(xc + dx, yc - r + dy);
	DrawPoint(xc + dx, yc + r + dy);
	while (x <= y)
	{
		if (((x + 1 - xc)*(x + 1 - xc) + (y - 0.5 - yc)*(y - 0.5 - yc)) > r*r)
		{
			y = y - 1;
		}
		x = x + 1;
		DrawPoint(x + dx, y + dy);
		DrawPoint(2 * xc - x + dx, y + dy);
		DrawPoint(x + dx, 2 * yc - y + dy);
		DrawPoint(2 * xc - x + dx, 2 * yc - y + dy);
		DrawPoint(y + dx, x + dy);
		DrawPoint(2 * yc - y + dx, x + dy);
		DrawPoint(y + dx, 2 * xc - x + dy);
		DrawPoint(2 * yc - y + dx, 2 * xc - x + dy);
	}
}


void DrawCircleFilled(int x0, int y0, int r)
{
	for (int i = y0 + r; i >= y0 - r; i = i - 1)
	for (int j = x0 - r; j <= x0 + r; j = j + 1)
	{
		if ((j - x0)*(j - x0) + (i - y0)*(i - y0) < r*r - 1) DrawPoint(j, i);
	}
}


void FloodFill(int x, int y, color OldColor, color NewColor)
{
	color BackColor;
	struct coordinate { int x, y; };
	stack <coordinate> filler;
	filler.push({ x, y });

	while (!filler.empty())
	{
		coordinate top = filler.top();
		filler.pop();

		glReadPixels(top.x, top.y, 1, 1, GL_RGB, GL_FLOAT, &BackColor);

		if ((BackColor.red == OldColor.red) && (BackColor.green == OldColor.green) && (BackColor.blue == OldColor.blue))
		{
			DrawPoint(top.x, top.y, NewColor);
			filler.push({ top.x + 1, top.y });
			filler.push({ top.x, top.y + 1 });
			filler.push({ top.x - 1, top.y });
			filler.push({ top.x, top.y - 1 });
		}
	}
}


void PrintString(string str, GLint x0, GLint y0)
{
	unsigned char fnt[2048];
	FILE *f;
#pragma pack(1)
	struct
	{
		GLubyte first;
		unsigned short count;
		GLubyte w;
		GLubyte h;
		GLubyte bpl;
	} head;

	//f = fopen("08X14.GSF", "rb");
	fopen_s(&f, "08X14.GSF", "rb");
	fread(&head, 6, 1, f);
	fread(fnt, 1, head.count * head.w, f);

	int k = 0, t;

	for (unsigned int p = 0; p < str.length(); p++)
	{
		t = str[p] * head.h;
		for (int i = 0; i < head.h; i++)
		{
			for (int j = head.w - 1; j >= 0; j--)
			{
				if (((fnt[t + i] >> j) & 1) != 0)
					DrawPoint(x0 + head.w - j, y0 - i);
			}
		}
		x0 += head.w;
	}

	fclose(f);
}


void SetupRC()
{
	//glClearColor(0.3, 0.5, 0.7, 1);
	glClearColor(0, 0, 0, 1);

	glEnable(GL_MULTISAMPLE_ARB);

	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glEnable(GL_BLEND);
	//glEnable(GL_POINT_SMOOTH);
	//glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	//glEnable(GL_LINE_SMOOTH);
	//glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	//glEnable(GL_POLYGON_SMOOTH);
	//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
}


void Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	DrawCircle(700, 600, 30);
	/*
	color oldcolor, newcolor = { 1.0, 1.0, 0.0 };
	glReadPixels(700, 600, 1, 1, GL_RGB, GL_FLOAT, &oldcolor);
	FloodFill(700, 600, oldcolor, newcolor);
	PrintString("TEST ONE TWO THREE", 400, 400);
	*/

	glColor3f(1, 1, 1);
	glLineWidth(3.5);
	glBegin(GL_LINE_STRIP);
	glVertex2f(0.0f, 25.0f);
	glVertex2f(50.0f, 100.0f);
	glVertex2f(100.0f, 25.0f);
	glVertex2f(225.0f, 125.0f);
	glVertex2f(300.0f, 50.0f);
	glVertex2f(375.0f, 100.0f);
	glVertex2f(460.0f, 25.0f);
	glVertex2f(525.0f, 100.0f);
	glVertex2f(600.0f, 20.0f);
	glVertex2f(675.0f, 70.0f);
	glVertex2f(750.0f, 25.0f);
	glVertex2f(800.0f, 90.0f);
	glEnd();

	glutSwapBuffers();
}


void Keyboard(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
}


int main()
{
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowPosition(400, 150);
	glutInitWindowSize(FRAME_H, FRAME_W);
	glutCreateWindow("Application_1 Window");
	gluOrtho2D(0, FRAME_H, 0, FRAME_W);
	glutDisplayFunc(Display);
	SetupRC();
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

#endif


