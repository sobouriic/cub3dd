#include "../includes/blockyard.h"

void vx_npcs_update(double dt)
{
	int i;
	double nx, ny;
	t_vnpc *n;
	for (i = 0; i < vx()->npc_count; i++)
	{
		n = &vx()->npcs[i];
		n->flash = fmax(0, n->flash-dt);
		if (n->respawn > 0)
		{
			n->respawn = fmax(0, n->respawn-dt);
			if (!n->respawn) { n->hp = 3; n->x = n->home_x; n->y = n->home_y; }
			continue;
		}
		n->turn -= dt;
		if (n->turn <= 0)
		{
			n->angle += 0.8+sin(vx()->time+i)*1.5;
			n->turn = 1.5+(i%3)*0.6;
		}
		nx = n->x+cos(n->angle)*dt*0.48;
		ny = n->y+sin(n->angle)*dt*0.48;
		if (vx_clear(nx, ny, 0.22) && hypot(nx-vx()->x,ny-vx()->y) > 0.65)
		{ n->x = nx; n->y = ny; }
		else n->angle += dt*4.0;
	}
}

void vx_attack(void)
{
	int i, target = -1;
	double range = vx()->weapon == 2 ? 12.0 : 1.65;
	double dx, dy, dist, dot, side;
	t_vhit hit;
	if (vx()->attack > 0) return;
	vx()->attack = vx()->weapon == 1 ? 0.52 : 0.34;
	for (i = 0; i < vx()->npc_count; i++)
	{
		if (vx()->npcs[i].respawn) continue;
		dx = vx()->npcs[i].x-vx()->x;
		dy = vx()->npcs[i].y-vx()->y;
		dist = hypot(dx, dy);
		dot = dx*cos(vx()->angle)+dy*sin(vx()->angle);
		side = fabs(-dx*sin(vx()->angle)+dy*cos(vx()->angle));
		if (dot <= 0 || side > 0.27 || dist >= range) continue;
		if (fabs(vx()->pitch + 0.075*485/dot) > 0.425*485/dot) continue;
		hit = vx_cast(vx()->x, vx()->y, dx, dy);
		if (hit.distance < 1) continue;
		range = dist;
		target = i;
	}
	if (target < 0) return;
	vx()->npcs[target].hp -= vx()->weapon == 1 ? 2 : 1;
	vx()->npcs[target].flash = 0.22;
	vx()->hit = 0.18;
	if (vx()->npcs[target].hp <= 0)
	{
		vx()->npcs[target].respawn = 5;
		vx()->score++;
	}
}

int vx_npc_pixel(int x, int y, double phase, int variant)
{
	int step = (int)(sin(phase)*2);
	int shirt = variant%2 ? 0x539b9c : 0xa97d4e;
	if (x >= 8 && x < 24 && y >= 1 && y < 16)
	{
		if (y < 4 || x < 10 || x >= 22) return (0x634533);
		if (y >= 7 && y < 10 && ((x >= 11 && x < 14) || (x >= 18 && x < 21)))
			return (0x273d40);
		if (x >= 14 && x < 18 && y >= 9 && y < 13) return (0x9c6745);
		if (y >= 13 && x >= 12 && x < 21) return (0x694934);
		return (0xcda77b);
	}
	if (y >= 16 && y < 32 && x >= 8 && x < 24)
	{
		if (x == 15 || x == 16) return (0x334744);
		if (y >= 29) return (0x594839);
		return (shirt);
	}
	if (x >= 3 && x < 8 && y >= 17+step && y < 33+step)
		return (y > 28+step ? 0xcda77b : vx_shade(shirt,0.8));
	if (x >= 24 && x < 29 && y >= 17-step && y < 33-step)
		return (y > 28-step ? 0xcda77b : vx_shade(shirt,0.8));
	if (x >= 9 && x < 15 && y >= 32 && y < 46+step)
		return (y > 42+step ? 0x343633 : 0x465b62);
	if (x >= 17 && x < 23 && y >= 32 && y < 46-step)
		return (y > 42-step ? 0x343633 : 0x465b62);
	return (-1);
}

static void draw_npc(int i)
{
	t_vnpc *n = &vx()->npcs[i];
	double dx = n->x-vx()->x, dy = n->y-vx()->y;
	double z = dx*cos(vx()->angle)+dy*sin(vx()->angle);
	double side = -dx*sin(vx()->angle)+dy*cos(vx()->angle);
	double proj = VX_W/2.0/tan(FOV_ANGLE/2);
	double height, width, left, top;
	int x, y, c;
	if (z < 0.12 || n->respawn) return;
	height = proj*0.85/z;
	width = height*32/48;
	left = VX_W/2+side*proj/z-width/2;
	top = VX_H/2+vx()->pitch+proj*0.5/z-height;
	for (x = (int)fmax(0,ceil(left)); x < fmin(VX_W,left+width); x++)
	{
		if (z >= vx()->depth[x]) continue;
		for (y = (int)fmax(0,ceil(top)); y < fmin(VX_H,top+height); y++)
		{
			c = vx_npc_pixel((x-left)*32/width, (y-top)*48/height,
				vx()->time*5+i, i);
			if (c < 0) continue;
			if (n->flash) c = 0xffb694;
			vx_pixel(x,y,vx_shade(c,fmax(0.45,1-z*0.025)));
		}
		if (n->hp < 3 && top > 6 && top < VX_H)
		{
			c = x < left+width*n->hp/3 ? 0x9cdf78 : 0x503c38;
			for (y = top-5; y < top-3; y++) vx_pixel(x,y,c);
		}
	}
}

void vx_npcs_render(void)
{
	int order[VX_MAX_NPCS], i, j, tmp;
	double a, b;
	for (i = 0; i < vx()->npc_count; i++) order[i] = i;
	for (i = 0; i < vx()->npc_count; i++)
		for (j = i+1; j < vx()->npc_count; j++)
		{
			a = hypot(vx()->npcs[order[i]].x-vx()->x,vx()->npcs[order[i]].y-vx()->y);
			b = hypot(vx()->npcs[order[j]].x-vx()->x,vx()->npcs[order[j]].y-vx()->y);
			if (b > a) { tmp = order[i]; order[i] = order[j]; order[j] = tmp; }
		}
	for (i = 0; i < vx()->npc_count; i++) draw_npc(order[i]);
}
