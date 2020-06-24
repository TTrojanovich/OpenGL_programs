#include <iostream>
#include <cmath>
using namespace std;

#pragma once


class VEC
{
public:
	double m[4];

	VEC()
	{
		m[0] = 0; m[1] = 0; m[2] = 0; m[3] = 1;
	}

	VEC(double x, double y, double z)
	{
		m[0] = x; m[1] = y; m[2] = z; m[3] = 1;
	}

	void operator = (const VEC &n)
	{
		for (int i = 0; i < 3; i++) m[i] = n.m[i];
	}

	VEC operator + (const VEC &n)
	{
		VEC t;
		t.m[0] = m[0] + n.m[0];
		t.m[1] = m[1] + n.m[1];
		t.m[2] = m[2] + n.m[2];
		return t;
	}

	VEC operator - (const VEC &n)
	{
		VEC t;
		t.m[0] = m[0] - n.m[0];
		t.m[1] = m[1] - n.m[1];
		t.m[2] = m[2] - n.m[2];
		return t;
	}

	VEC cmlVecProd(VEC &n)
	{
		VEC t;
		t.m[0] = m[1] * n.m[2] - m[2] * n.m[1];
		t.m[1] = (m[0] * n.m[2] - m[2] * n.m[0]) * -1;
		t.m[2] = m[0] * n.m[1] - m[1] * n.m[0];
		return t;
	}

	double cmlScalProd(VEC &n)
	{
		return m[0] * n.m[0] + m[1] * n.m[1] + m[2] * n.m[2];
	}
};


class MATR
{
public:
	double m[16];
	void operator = (double* n)
	{
		for (int i = 0; i < 16; i++) m[i] = *(n + i);
	}

	MATR operator + (const MATR &n)
	{
		MATR t;
		for (int i = 0; i < 16; i++)
		{
			t.m[i] = m[i] + n.m[i];
		}
		return t;
	}

	MATR operator - (const MATR &n)
	{
		MATR t;
		for (int i = 0; i < 16; i++)
		{
			t.m[i] = m[i] - n.m[i];
		}
		return t;
	}

	MATR operator * (const MATR &n)
	{
		MATR t;
		for (int i = 0; i < 16; i++) t.m[i] = 0;

		for (int i = 0; i < 16; i++)
		{
			// j - stroka, q - stolbec
			int k1 = i / 4, k2 = i % 4;
			
			for (int j = k1 * 4, q = k2; j < 4 + k1 * 4, q <= 12 + k2; j++, q += 4)
				t.m[i] += m[j] * n.m[q];
		}
		return t;
	}

	VEC operator * (const VEC &n)
	{
		VEC t;
		for (int i = 0; i < 4; i++) t.m[i] = 0;

		for (int i = 0; i < 4; i++)
		{
			for (int j = i * 4, q = 0; j < 4 + i * 4, q < 4; j++, q++)
				t.m[i] += m[j] * n.m[q];
		}
		return t;
	}
};





double* cmlMatrTranslate(double x, double y, double z)
{
	static double m[] = // translation
	{
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		x, y, z, 1
	};
	m[12] = x;
	m[13] = y;
	m[14] = z;
	return &m[0];
}


double* cmlMatrShear(double x, double y, double z)
{
	static double m[] = // shear
	{
		1, 0, 0, x,
		0, 1, 0, y,
		0, 0, 1, z,
		0, 0, 0, 1
	};
	m[3] = x;
	m[7] = y;
	m[11] = z;
	return &m[0];
}


double* cmlMatrScale(double x, double y, double z)
{
	static double m[] = // scale
	{
		x, 0, 0, 0,
		0, y, 0, 0,
		0, 0, z, 0,
		0, 0, 0, 1
	};
	m[0] = x;
	m[5] = y;
	m[10] = z;
	return &m[0];
}


double* cmlMatrRotate(double a, double x, double y, double z)
{
	static double m[16]; // rotate
	m[0] = cos(a) + x*x*(1 - cos(a));
	m[1] = x*y*(1 - cos(a)) - z*sin(a);
	m[2] = x*z*(1 - cos(a)) + y*sin(a);
	m[3] = 0;
	m[4] = y*x*(1 - cos(a)) + z*sin(a);
	m[5] = cos(a) + y*y*(1 - cos(a));
	m[6] = y*z*(1 - cos(a)) - x*sin(a);
	m[7] = 0;
	m[8] = z*x*(1 - cos(a)) - y*sin(a);
	m[9] = z*y*(1 - cos(a)) + x*sin(a);
	m[10] = cos(a) + z*z*(1 - cos(a));
	m[11] = 0;
	m[12] = 0;
	m[13] = 0;
	m[14] = 0;
	m[15] = 1;

	return &m[0];
}


double* cmlMatrTranspos(MATR &n)
{
	static double m[16];
	int k1, k2;
	for (int i = 0; i < 16; i++)
	{
		k1 = i / 4; k2 = i % 4;
		m[k2 * 4 + k1] = n.m[i];
	}
	return &m[0];
}


double* cmlMatrInverse(MATR &n)
{
	static double m[16];

	// the cake is a lie

	return &m[0];
}


void cmlMatrPrint(MATR &n)
{
	for (int i = 0; i < 4; i++)
	{
		cout << endl;
		for (int j = 0; j < 4; j++)
			cout << n.m[i * 4 + j] << " ";
	}
	cout << endl;
}


void cmlVecPrint(VEC &n)
{
	for (int i = 0; i < 4; i++)
		cout << n.m[i] << " ";
	cout << endl;
}


