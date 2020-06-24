#define tag
#ifdef tag21
// ==================================== IMAGE FILTERS ====================================
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


void Negative(GLubyte *prt, GLint width, GLint height, GLint depth)
{
	int LUT[256];
	for (int i = 0; i < 256; i++) LUT[i] = 255 - i;
	int size = width*height * depth;
	for (int i = 0; i < size; i++)
	{
		prt[i] = LUT[prt[i]];
	}
}


void ChangeBright(GLubyte *prt, GLint width, GLint height, GLint depth, GLint d)
{
	int q, LUT[256];
	for (int i = 0; i < 256; i++)
	{
		q = i + d;
		if (q < 0) q = 0;
		else if (q > 255) q = 255;
		LUT[i] = q;
	}

	int size = width*height * depth;
	for (int i = 0; i < size; i++)
	{
		prt[i] = LUT[prt[i]];
	}
}


void ChangeCont(GLubyte *prt, GLint width, GLint height, GLint depth, GLint d)
{
	int q, LUT[256];
	//int a = 0, b = 255;
	double ContLevel = double(100 + d) / 100;

	for (int i = 0; i < 256; i++)
	{
		//q = (i - a) / (b - a) * 255;
		q = (((double)i / 255 - 0.5) * ContLevel + 0.5) * 255;

		if (q < 0) q = 0;
		else if (q > 255) q = 255;

		LUT[i] = q;
	}

	int size = width*height * depth;
	for (int i = 0; i < size; i++)
	{
		prt[i] = LUT[prt[i]];
	}
}


void GammaCorr(GLubyte *prt, GLint width, GLint height, GLint depth)
{
	int q, LUT[256];
	for (int i = 0; i < 256; i++)
	{
		q = pow(double(i) / 255.0, 2.2) * 255;
		LUT[i] = q;
	}

	int size = width*height * depth;
	for (int i = 0; i < size; i++)
	{
		prt[i] = LUT[prt[i]];
	}
}


void RGB2HSV(GLubyte *prt, GLint width, GLint height, GLint depth)
{
	double r, g, b, min, max, h, s, v, delta;

	for (int i = 0; i < width*height * depth; i = i + depth)
	{
		r = prt[i + 2] / 255.0;
		g = prt[i + 1] / 255.0;
		b = prt[i + 0] / 255.0;

		h = 0.0;
		s = 0.0;
		v = 0.0;

		min = r < g ? r : g;
		min = min < b ? min : b;
		max = r > g ? r : g;
		max = max > b ? max : b;

		v = max;

		delta = max - min;

		if (delta < 0.00001)
		{
			s = 0;
			h = 0;
		}
		else
		{
			if (max > 0.0)
				s = delta / max;
			else
			{
				s = 0.0;
				h = NAN;
				//cout << "ERROR" << endl;
			}
			if (max > 0.0)
			{
				if (r >= max)
					h = (g - b) / delta;
				else if (g >= max)
					h = 2.0 + (b - r) / delta;
				else
					h = 4.0 + (r - g) / delta;

				h = h * 60.0;

				if (h < 0.0)
					h += 360.0;
			}
		}

		v = round(v * 100);
		s = round(s * 100);

		prt[i + 0] = v;
		prt[i + 1] = s;
		prt[i + 2] = h;
	}
}


void HSV2RGB(GLubyte *prt, GLint width, GLint height, GLint depth)
{
	double n, h, s, v, r, g, b, hh, p, q, t, ff;

	for (int i = 0; i < width*height * depth; i = i + depth)
	{
		v = prt[i + 0] / 255.0;
		s = prt[i + 1] / 255.0;
		h = prt[i + 2];

		if (s <= 0.0)
		{
			r = v;
			g = v;
			b = v;
			//cout << "WARNING" << endl;
		}
		else
		{
			hh = h;
			if (hh >= 360.0) hh = 0.0;
			hh /= 60.0;
			n = (long)hh;
			ff = hh - n;
			p = v * (1.0 - s);
			q = v * (1.0 - (s * ff));
			t = v * (1.0 - (s * (1.0 - ff)));

			switch (i) {
			case 0:
				r = v;
				g = t;
				b = p;
				break;
			case 1:
				r = q;
				g = v;
				b = p;
				break;
			case 2:
				r = p;
				g = v;
				b = t;
				break;
			case 3:
				r = p;
				g = q;
				b = v;
				break;
			case 4:
				r = t;
				g = p;
				b = v;
				break;
			case 5:
			default:
				r = v;
				g = p;
				b = q;
				break;
			}
		}

		prt[i + 0] = b * 255;
		prt[i + 1] = g * 255;
		prt[i + 2] = r * 255;
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

	//RGB2HSV(prt, width, height, depth);

	//Negative(prt, width, height, depth);
	//ChangeBright(prt, width, height, depth, 20);
	//ChangeCont(prt, width, height, depth, 50);
	//GammaCorr(prt, width, height, depth);

	//HSV2RGB(prt, width, height, depth);

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
