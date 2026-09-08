#include "../includes/blockyard.h"

/* Camera rays have forward component 1: their t is perpendicular depth. */
t_vhit vx_cast(double x, double y, double dx, double dy)
{
	t_vhit h = {1000, 0, '1', 0, (int)floor(x), (int)floor(y)};
	double ax = fabs(dx) < 1e-10 ? 1e20 : fabs(1/dx);
	double ay = fabs(dy) < 1e-10 ? 1e20 : fabs(1/dy);
	int sx = dx < 0 ? -1 : 1, sy = dy < 0 ? -1 : 1, step;
	double tx = (dx < 0 ? x-h.x : h.x+1-x)*ax;
	double ty = (dy < 0 ? y-h.y : h.y+1-y)*ay;
	double t, u, coord;
	t_vdoor *d;
	for (step = 0; step < 2048; step++)
	{
		if (tx < ty)
		{ t = tx; tx += ax; h.x += sx; h.side = 0; }
		else
		{ t = ty; ty += ay; h.y += sy; h.side = 1; }
		h.tile = vx_tile(h.x, h.y);
		if (h.tile == 'D')
		{
			d = vx_door(h.x, h.y);
			if (!d || d->open >= 0.999) continue;
			if ((!d->axis && fabs(dx) < 1e-10)
				|| (d->axis && fabs(dy) < 1e-10)) continue;
			t = d->axis ? (h.y+0.5-y)/dy : (h.x+0.5-x)/dx;
			coord = d->axis ? x+t*dx-h.x : y+t*dy-h.y;
			if (t <= 0 || coord < d->open || coord >= 1) continue;
			h.side = d->axis;
			h.u = coord-d->open;
		}
		else if ((h.tile >= '1' && h.tile <= '4') || h.tile == 'T')
		{
			u = h.side ? x+t*dx : y+t*dy;
			h.u = u-floor(u);
		}
		else continue;
		h.distance = fmax(0.001, t);
		return (h);
	}
	return (h);
}

void vx_pixel(int x, int y, int color)
{
	if (x >= 0 && x < VX_W && y >= 0 && y < VX_H)
		vx()->frame[y*VX_W+x] = color;
}

void vx_rect(int x, int y, int w, int h, int color)
{
	int xx, yy;
	for (yy = (int)fmax(0, y); yy < (int)fmin(VX_H, y+h); yy++)
		for (xx = (int)fmax(0, x); xx < (int)fmin(VX_W, x+w); xx++)
			vx_pixel(xx, yy, color);
}

static int sky_mix(int a, int b, double t)
{
	int r, g, blue;
	t = fmax(0, fmin(1, t));
	r = ((a>>16)&255)*(1-t)+((b>>16)&255)*t;
	g = ((a>>8)&255)*(1-t)+((b>>8)&255)*t;
	blue = (a&255)*(1-t)+(b&255)*t;
	return ((r<<16)|(g<<8)|blue);
}

static int sunset(int x, int y, double horizon)
{
	double projection = VX_W/2.0/tan(FOV_ANGLE/2);
	double angle = vx()->angle+atan((x-VX_W/2.0)/projection);
	double longitude, delta, sun_x, height = horizon-y;
	int c, cell, band;

	c = sky_mix(0x777fae, 0xf6b88f, 1-height/230);
	delta = remainder(0.20-vx()->angle, DOUBLE_PI);
	if (fabs(delta) < HALF_PI)
	{
		sun_x = VX_W/2.0+tan(delta)*projection;
		if (fabs(x-sun_x) < 27 && fabs(height-107) < 27)
			c = sky_mix(c, 0xffdba0, 0.30);
		if (fabs(x-sun_x) < 18 && fabs(height-107) < 18)
			c = 0xffe8ac;
		if (fabs(x-sun_x) < 14 && fabs(height-107) < 14)
			c = 0xfff5ce;
	}
	longitude = fmod(angle/DOUBLE_PI*1024+vx()->time*1.5, 1024);
	if (longitude < 0) longitude += 1024;
	cell = (int)longitude%128;
	band = ((int)longitude/128)%3;
	if (cell > 12 && cell < 70 && height > 153+band*6
		&& height < 164+band*6)
		c = 0xe8c1bd;
	if (cell > 25 && cell < 57 && height >= 164+band*6
		&& height < 172+band*6)
		c = 0xf5d9ce;
	if (cell > 83 && cell < 117 && height > 65+band*5
		&& height < 72+band*5)
		c = 0xf7c9a8;
	return (c);
}

static void landscape(void)
{
	int x, y, color;
	double horizon = VX_H/2 + vx()->pitch;
	double dirx = cos(vx()->angle), diry = sin(vx()->angle);
	double rayx, rayy, dist, wx, wy;
	for (y = 0; y < VX_H; y++)
		for (x = 0; x < VX_W; x++)
		{
			if (y < horizon)
			{
				color = sunset(x, y, horizon);
			}
			else
			{
				dist = (VX_W/2.0/tan(FOV_ANGLE/2))*0.5/fmax(0.5,y-horizon);
				rayx = dirx - diry*(2.0*x/VX_W-1)*tan(FOV_ANGLE/2);
				rayy = diry + dirx*(2.0*x/VX_W-1)*tan(FOV_ANGLE/2);
				wx = vx()->x+rayx*dist;
				wy = vx()->y+rayy*dist;
				color = vx_material(vx_tile((int)floor(wx),(int)floor(wy)) == '~'
					? '~' : (vx_tile((int)floor(wx),(int)floor(wy)) == 'b' ? 'b' : 'G'),
					wx-floor(wx), wy-floor(wy),
					(int)floor(wx), (int)floor(wy));
				color = vx_shade(color, fmax(0.40, 1.0-dist*0.028));
			}
			vx_pixel(x, y, color);
		}
}

void vx_render(void)
{
	int x, y, top, bottom, color;
	double dx = cos(vx()->angle), dy = sin(vx()->angle);
	double plane, height, start, horizon = VX_H/2+vx()->pitch;
	t_vhit h;
	landscape();
	for (x = 0; x < VX_W; x++)
	{
		plane = (2.0*(x+0.5)/VX_W-1)*tan(FOV_ANGLE/2);
		h = vx_cast(vx()->x, vx()->y, dx-dy*plane, dy+dx*plane);
		vx()->depth[x] = h.distance;
		height = (VX_W/2.0/tan(FOV_ANGLE/2))/h.distance;
		start = horizon-height/2;
		top = (int)fmax(0, ceil(start));
		bottom = (int)fmin(VX_H-1, start+height);
		for (y = top; y <= bottom; y++)
		{
			color = vx_material(h.tile, h.u, (y-start)/height, h.x, h.y);
			if (h.x == vx()->mine_x && h.y == vx()->mine_y && vx()->mine_hits > 0
				&& abs(((int)(h.u*32)+(int)((y-start)/height*48))%16-8)
				< vx()->mine_hits)
				color = 0x29372f;
			color = vx_shade(color, fmax(0.30, (h.side ? 0.78 : 1.0)
				- h.distance*0.024));
			vx_pixel(x, y, color);
		}
	}
	vx_npcs_render();
	vx_build_preview();
	vx_weapon();
	vx_hud();
}

void vx_capture(const char *path)
{
	FILE *f = fopen(path, "wb");
	int i, c;
	if (!f) ft_error("Cannot write capture\n");
	fprintf(f, "P6\n%d %d\n255\n", VX_W, VX_H);
	for (i = 0; i < VX_W*VX_H; i++)
	{
		c = vx()->frame[i];
		fputc((c>>16)&255, f); fputc((c>>8)&255, f); fputc(c&255, f);
	}
	fclose(f);
}
