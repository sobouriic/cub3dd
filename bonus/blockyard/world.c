#include "../includes/blockyard.h"

static t_voxel g_world;

t_voxel *vx(void) { return (&g_world); }

int vx_tile(int x, int y)
{
	t_cub3d *c = vx()->cub;
	if (x < 0 || y < 0 || y >= c->parse.row || x >= c->parse.column)
		return ('1');
	if (!c->map.map[y][x] || c->map.map[y][x] == ' ')
		return ('1');
	return (c->map.map[y][x]);
}

t_vdoor *vx_door(int x, int y)
{
	int i;
	for (i = 0; i < vx()->door_count; i++)
		if (vx()->doors[i].x == x && vx()->doors[i].y == y)
			return (&vx()->doors[i]);
	return (NULL);
}

static int solid(int x, int y)
{
	int t = vx_tile(x, y);
	return (t >= '1' && t <= '4');
}

int vx_clear(double x, double y, double r)
{
	int xx, yy;
	t_vdoor *d;
	for (yy = (int)floor(y-r); yy <= (int)floor(y+r); yy++)
		for (xx = (int)floor(x-r); xx <= (int)floor(x+r); xx++)
		{
			if (solid(xx, yy) || vx_tile(xx, yy) == '~' || vx_tile(xx, yy) == 'T')
				return (0);
			d = vx_door(xx, yy);
			if (d && d->open < 0.98)
				return (0);
		}
	return (1);
}

void vx_init(t_cub3d *cub)
{
	int x, y, portal = 0;
	t_vdoor *d;
	t_vnpc *n;
	ft_memset(vx(), 0, sizeof(*vx()));
	vx()->cub = cub;
	vx()->x = cub->player.x / SIZE_W;
	vx()->y = cub->player.y / SIZE_W;
	vx()->angle = cub->player.r_angle;
	vx()->map_on = 1;
	vx()->help = 1;
	vx()->weapon = 1;
	vx()->inventory[0] = 8;
	vx()->inventory[1] = 8;
	vx()->inventory[2] = 16;
	vx()->mine_x = -1;
	vx()->mine_y = -1;
	for (y = 0; y < cub->parse.row; y++)
		for (x = 0; x < cub->parse.column; x++)
		{
			if (vx_tile(x, y) == '4') vx()->crystal_goal++;
			if (vx_tile(x, y) == 'T') portal = 1;
			if (vx_tile(x, y) == 'D')
			{
				if (vx()->door_count == VX_MAX_DOORS)
					ft_error("Too many doors (maximum 128)\n");
				d = &vx()->doors[vx()->door_count++];
				d->x = x;
				d->y = y;
				d->axis = !(solid(x, y-1) && solid(x, y+1));
				if (d->axis && !(solid(x-1, y) && solid(x+1, y)))
					ft_error("Doors need walls on two opposite sides\n");
			}
			if (vx_tile(x, y) == 'P')
			{
				if (vx()->npc_count == VX_MAX_NPCS)
					ft_error("Too many characters (maximum 64)\n");
				n = &vx()->npcs[vx()->npc_count++];
				n->x = x + 0.5;
				n->y = y + 0.5;
				n->home_x = n->x;
				n->home_y = n->y;
				n->angle = vx()->npc_count * 2.399;
				n->hp = 3;
			}
		}
	if (!portal) vx()->crystal_goal = 0;
}

int vx_near_door(void)
{
	int i, best = -1;
	double dx, dy, dist, near = 1.7;
	t_vhit hit;
	for (i = 0; i < vx()->door_count; i++)
	{
		dx = vx()->doors[i].x + 0.5 - vx()->x;
		dy = vx()->doors[i].y + 0.5 - vx()->y;
		dist = hypot(dx, dy);
		if (dist < near && (dist < 0.4
				|| (dx*cos(vx()->angle)+dy*sin(vx()->angle))/dist > 0.55))
		{
			hit = vx_cast(vx()->x, vx()->y, dx, dy);
			if (hit.distance >= 0.99 || (hit.tile == 'D'
					&& hit.x == vx()->doors[i].x && hit.y == vx()->doors[i].y))
			{
				best = i;
				near = dist;
			}
		}
	}
	return (best);
}

void vx_interact(void)
{
	int i = vx_near_door();
	if (vx_portal_near())
	{
		if (vx()->crystals >= vx()->crystal_goal)
		{
			vx()->won = 1;
			vx_notice("BEACON RESTORED! KEEP EXPLORING AND BUILDING");
		}
		else vx_notice("MINE ALL THE CYAN CRYSTALS TO POWER THE BEACON");
		return;
	}
	if (i >= 0)
		vx()->doors[i].target = !vx()->doors[i].target;
}

static void doors_update(double dt)
{
	int i, j, occupied;
	t_vdoor *d;
	for (i = 0; i < vx()->door_count; i++)
	{
		d = &vx()->doors[i];
		occupied = fabs(vx()->x-d->x-0.5) < 0.8
			&& fabs(vx()->y-d->y-0.5) < 0.8;
		for (j = 0; j < vx()->npc_count; j++)
			if (!vx()->npcs[j].respawn
				&& fabs(vx()->npcs[j].x-d->x-0.5) < 0.8
				&& fabs(vx()->npcs[j].y-d->y-0.5) < 0.8)
				occupied = 1;
		if (occupied && !d->target)
			d->target = 1;
		d->open = fmax(0, fmin(1, d->open + (d->target*2-1)*dt*1.8));
	}
}

void vx_update(double dt)
{
	double f, s, dx, dy, len;
	vx()->dt = fmin(0.05, fmax(0, dt));
	dt = vx()->dt;
	vx()->time += dt;
	vx()->angle += (vx()->keys[KEY_RIGHT]-vx()->keys[KEY_LEFT])*dt*1.9;
	vx()->angle = remainder(vx()->angle, DOUBLE_PI);
	vx()->pitch += (vx()->keys[KEY_DOWN]-vx()->keys[KEY_UP])*dt*90;
	vx()->pitch = fmax(-95, fmin(95, vx()->pitch));
	f = vx()->keys[KEY_W]-vx()->keys[KEY_S];
	s = vx()->keys[KEY_D]-vx()->keys[KEY_A];
	len = fmax(1, hypot(f, s));
	dx = (cos(vx()->angle)*f-sin(vx()->angle)*s)*dt*2.4/len;
	dy = (sin(vx()->angle)*f+cos(vx()->angle)*s)*dt*2.4/len;
	if (vx_clear(vx()->x+dx, vx()->y, 0.19)) vx()->x += dx;
	if (vx_clear(vx()->x, vx()->y+dy, 0.19)) vx()->y += dy;
	doors_update(dt);
	vx_npcs_update(dt);
	vx()->attack = fmax(0, vx()->attack-dt);
	vx()->hit = fmax(0, vx()->hit-dt);
	if (vx()->time > vx()->mine_until) vx()->mine_hits = 0;
	if (vx()->keys[49]) vx_action();
}
