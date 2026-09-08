#include "../includes/blockyard.h"

int vx_shade(int color, double light)
{
	int r, g, b;
	light = fmax(0, fmin(1.3, light));
	r = (int)fmin(255, ((color>>16)&255)*light);
	g = (int)fmin(255, ((color>>8)&255)*light);
	b = (int)fmin(255, (color&255)*light);
	return ((r<<16)|(g<<8)|b);
}

static unsigned int noise(int x, int y, int seed)
{
	unsigned int v = (unsigned int)x*374761393u
		+ (unsigned int)y*668265263u + (unsigned int)seed*1274126177u;
	v = (v^(v>>13))*1274126177u;
	return (v^(v>>16));
}

static int timber(int a, int b, int seed)
{
	int row = b/16, seam = (a+(row%2)*32)%64;
	int grain = (int)(noise(a/8,b/2,seed)%5);
	int c = 0xaf8350;
	if (b%16 == 0 || seam == 0) return (0x563e2b);
	if (b%16 == 1) return (0xd2ad76);
	if (b%16 == 15) return (0x795231);
	if ((b+grain)%7 == 0) c = 0x93683d;
	if ((a+row*13)%31 < 5 && b%16 > 5 && b%16 < 10) c = 0x785436;
	return (vx_shade(c,0.95+(noise(a/2,b/2,seed)%6)*0.02));
}

static int masonry(int a, int b, int seed, int ore)
{
	int xx = (a+(b/16%2)*16)%32, yy = b%16;
	int c = 0x859397;
	int patch = noise(a/4,b/3,seed)%7;
	if (xx < 2 || yy < 2) return (0x445356);
	if (xx == 2 || yy == 2) c = 0xb0bab5;
	else if (xx > 29 || yy > 13) c = 0x626f73;
	else c = vx_shade(c,0.87+patch*0.035);
	if (ore && xx > 5 && xx < 26 && yy > 3 && yy < 13)
	{
		int vein = (a/8*3+b/8*5+seed)%7;
		if (vein < 3)
			c = (a%8 < 3 || b%8 < 3) ? 0xb2f8e5 : 0x36a9aa;
		if (vein < 3 && a%8 > 4 && b%8 > 4) c = 0x257580;
	}
	return (vx_shade(c,0.97+(noise(a,b,seed)%4)*0.015));
}

int vx_material(int tile, double u, double v, int x, int y)
{
	int a = (int)(u*64)&63, b = (int)(v*64)&63;
	int seed = x*31+y, c, n = noise(a/2,b/2,seed)%7;
	if (tile == '3' || tile == 'b')
	{
		c = timber(a,b,seed);
		if (tile == 'b' && (a == 6 || a == 57) && b%16 == 6) c = 0x3b4443;
		return (c);
	}
	if (tile == '~')
	{
		c = 0x327f9f;
		if ((b+(int)(vx()->time*5)+a/12+x*3)%24 < 3) c = 0x7dbec6;
		return (vx_shade(c,0.95+n*0.012));
	}
	if (tile == 'T')
	{
		if (a < 8 || a > 55 || b < 8 || b > 56)
			return (vx_shade(masonry(a,b,seed,0),0.6));
		c = vx()->crystals >= vx()->crystal_goal ? 0x72e9c5 : 0x9b6fcb;
		return (vx_shade(c,0.65+0.30*sin(a*0.2+b*0.125+vx()->time*2)));
	}
	if (tile == 'G')
	{
		c = 0x64863e;
		if ((x%7+7)%7 == 3 || (y%7+7)%7 == 3)
		{
			c = 0xb19c77;
			if (noise(a/3,b/3,seed)%13 == 0) c = 0x8e826d;
		}
		else if ((a+b*3)%17 < 2 && n < 3) c = 0x99ae53;
		return (vx_shade(c,0.91+n*0.025));
	}
	if (tile == 'D')
	{
		c = timber(b,a,seed);
		if (b < 4 || b > 59 || b == 28 || b == 29) c = 0x4f3c2e;
		if (b > 8 && b < 24 && a > 6 && a < 56)
			c = a%16 < 3 ? 0x503a29 : (a%16 < 6 ? 0xc2e3d4 : 0x70a4a6);
		if (a >= 48 && a < 56 && b >= 38 && b < 44)
			c = b < 40 ? 0xffdc83 : 0xb38941;
		return (c);
	}
	if (tile == '2')
	{
		int grass = 9+(int)(noise(a/4,0,seed)%7);
		c = 0x866143;
		if (noise(a/4,b/3,seed)%9 == 0) c = 0xa28968;
		if (b < grass) c = b < 3 ? 0x9bb75b : 0x63863b;
		if (b == grass) c = 0x41562e;
		return (vx_shade(c,0.87+n*0.035));
	}
	return (masonry(a,b,seed,tile == '4'));
}
