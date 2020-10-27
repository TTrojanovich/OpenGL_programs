#define tag
#ifdef tag2
// ==================================== IMAGE EFFECTS ====================================
#include <iostream>
#include <fstream>
#include <cmath>
#include <glut.h>
#include <stack>
#include <string>

using namespace std;

#define FRAME_W 800
#define FRAME_H 800
const int xc = FRAME_W / 2;
const int yc = FRAME_H / 2;

void Keyboard(unsigned char key, int x, int y)
{
	if (key == 27) exit(0);
}

typedef struct
{
	GLbyte	identsize;
	GLbyte	colorMapType;
	GLbyte	imageType;
	GLbyte a, b, c, d, e;
	unsigned short	xstart;
	unsigned short	ystart;
	unsigned short	width;
	unsigned short	height;
	GLbyte	bits;
	GLbyte	descriptor;
} TGAHEADER;


GLubyte *gltLoadTGA(const char *szFileName, GLint *iWidth, GLint *iHeight, GLint *depth, GLint *iComponents, GLenum *eFormat)
{
	FILE *pFile;
	TGAHEADER tgaHeader;
	unsigned long lImageSize;
	short sDepth;
	GLubyte	*pBits = nullptr;

	*iWidth = 0;
	*iHeight = 0;
	*eFormat = GL_BGR_EXT;
	*iComponents = GL_RGB8;

	pFile = fopen(szFileName, "rb");
	if (pFile == nullptr)
		return nullptr;

	fread(&tgaHeader, 18, 1, pFile);

	*iWidth = tgaHeader.width;
	*iHeight = tgaHeader.height;
	sDepth = tgaHeader.bits / 8;
	*depth = sDepth;
	
	if (tgaHeader.bits != 8 && tgaHeader.bits != 24 && tgaHeader.bits != 32)
		return nullptr;

	lImageSize = tgaHeader.width * tgaHeader.height * sDepth;

	pBits = (GLubyte*)malloc(lImageSize * sizeof(GLubyte));
	if (pBits == nullptr)
		return nullptr;

	if (fread(pBits, lImageSize, 1, pFile) != 1)
	{
		free(pBits);
		return nullptr;
	}

	switch (sDepth)
	{
	case 3:
		*eFormat = GL_BGR_EXT;
		*iComponents = GL_RGB8;
		break;
	case 4:
		*eFormat = GL_BGRA_EXT;
		*iComponents = GL_RGBA8;
		break;
	case 1:
		*eFormat = GL_LUMINANCE;
		*iComponents = GL_LUMINANCE8;
		break;
	};

	fclose(pFile);
	return pBits;
}


void OrderedDith(GLubyte *prt, GLint width, GLint height, GLint depth)
{
	int arr[2][2] =
	{
		{ 0, 2 },
		{ 3, 1 }
	};

	int x = 0, y = 0;
	
	for (int i = 0; i < width*height * depth; i = i + depth)
	{
		if ((prt[i] * 5 / 256) > arr[y % 2][x % 2])
		{
			prt[i] = 255;
			prt[i + 1] = 255;
			prt[i + 2] = 255;
		}
		else
		{
			prt[i] = 0;
			prt[i + 1] = 0;
			prt[i + 2] = 0;
		}
		x++;
		if (i % width * depth == 0) { x = 0; y++; }
	}
}


void ErrorDiff(GLubyte *prt, GLint width, GLint height, GLint depth)
{
	GLubyte err = 0;
	int size = width * height * depth;
		 
	for (int i = 0; i < size; i = i + depth)
	{
		if (prt[i] > 128)
		{
			err = prt[i] - 0;
			prt[i] = 0;
			prt[i + 1] = 0;
			prt[i + 2] = 0;
		}
		else
		{
			err = prt[i] - 255;
			prt[i] = 255;
			prt[i + 1] = 255;
			prt[i + 2] = 255;
		}

		if (i + depth <= size - 1) prt[i + depth] += err / 16 * 7;
		if (i + depth + width * depth < size - 1) prt[i + depth + width * depth] += err / 16 * 1;
		if (i + 0 + width * depth < size - 1) prt[i + 0 + width * depth] += err / 16 * 5;
		if (i - depth + width * depth < size - 1) prt[i - depth + width * depth] += err / 16 * 3;

		if ((i % width*depth == 0) & (i > 0)) err = 0;
	}
}


void Display(void)
{
	glClearColor(0.3, 0.5, 0.7, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	GLubyte *prt;
	GLint width, height, depth, comp;
	GLenum format;
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	prt = gltLoadTGA("pics/build.tga", &width, &height, &depth, &comp, &format);

	//OrderedDith(prt, width, height, depth);
	ErrorDiff(prt, width, height, depth);
	
	glRasterPos2i(50, 50);	
	glDrawPixels(width, height, format, GL_UNSIGNED_BYTE, prt);
	free(prt);

	glutSwapBuffers();
}


int main(int ArgC, char *ArgV[])
{
	glutInit(&ArgC, ArgV);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(400, 150);
	glutInitWindowSize(FRAME_H, FRAME_W);
	glutCreateWindow("Application_2 Window");
	gluOrtho2D(0, FRAME_H, 0, FRAME_W);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Keyboard);
	glutMainLoop();
}

#endif
