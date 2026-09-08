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

int vx_material(int tile, double u, double v, int x, int y)
{
	int a = (int)(u*32)&31, b = (int)(v*32)&31;
	int c, n = noise(a, b, x*31+y) % 5;
	if (tile == '~')
	{
		c = 0x368baf;
		if ((b+(int)(vx()->time*3)+x*3)%12 < 2) c = 0x70b7c6;
		return (vx_shade(c,0.92+n*0.03));
	}
	if (tile == 'b')
	{
		c = (b%8 == 0) ? 0x5d422c : 0xbb945a;
		if ((a == 3 || a == 28) && b%8 == 3) c = 0x594e3e;
		return (vx_shade(c,0.95+n*0.02));
	}
	if (tile == 'T')
	{
		if (a < 4 || a > 27 || b < 4 || b > 28)
			return ((a+b)%5 ? 0x3a344f : 0x756585);
		c = vx()->crystals >= vx()->crystal_goal ? 0x72e9c5 : 0x9b6fcb;
		return (vx_shade(c,0.65+0.30*sin(a*0.4+b*0.25+vx()->time*2)));
	}
	if (tile == 'G')
	{
		c = 0x679344;
		if ((x % 7+7)%7 == 3 || (y % 7+7)%7 == 3) c = 0xb7a27a;
		if (n == 0) c = vx_shade(c, 0.83);
		if (n == 1) c = vx_shade(c, 1.12);
		return (c);
	}
	if (tile == 'D')
	{
		c = 0x986333;
		if (a%8 == 0) c = 0x583b26;
		if (b < 2 || b > 29 || b == 14 || b == 15) c = 0x523c2c;
		if (b > 4 && b < 12 && a > 3 && a < 28)
			c = (a%8 < 2) ? 0x513b27 : 0x80b7b0;
		if (a >= 24 && a < 28 && b >= 19 && b < 22) c = 0xf2cd65;
		return (vx_shade(c, 0.94+n*0.025));
	}
	if (tile == '2')
	{
		c = 0x876044;
		if (b < 5+(int)(noise(a/3,0,1)%4)) c = 0x648f3b;
		return (vx_shade(c, 0.80+n*0.08));
	}
	if (tile == '3')
	{
		c = 0xb18a50;
		if (b%8 == 0 || (a+(b/8%2)*16)%32 == 0) c = 0x725333;
		if (n == 0 && b%8 == 5) c = 0x896638;
		return (vx_shade(c, 0.93+n*0.025));
	}
	c = 0x909b9a;
	if (b%8 == 0 || (a+(b/8%2)*8)%16 == 0) c = 0x566565;
	if (b%8 == 1) c = 0xa9b3ab;
	if (tile == '4' && n == 1 && a%8 > 3 && b%8 > 2) c = 0x45cabb;
	return (vx_shade(c, 0.88+n*0.04));
}
