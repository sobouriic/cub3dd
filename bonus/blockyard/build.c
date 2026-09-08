#include "../includes/blockyard.h"

void vx_notice(const char *text)
{
	snprintf(vx()->message,sizeof(vx()->message),"%s",text);
	vx()->message_until = vx()->time+3.0;
}

static int interior(int x, int y)
{
	t_cub3d *c = vx()->cub;
	if (x <= 0 || y <= 0 || x >= c->parse.column-1 || y >= c->parse.row-1)
		return (0);
	return (c->map.map[y][x] != ' ' && c->map.map[y-1][x] != ' '
		&& c->map.map[y+1][x] != ' ' && c->map.map[y][x-1] != ' '
		&& c->map.map[y][x+1] != ' ');
}

int vx_build_target(int *x, int *y)
{
	double dx = cos(vx()->angle), dy = sin(vx()->angle), dist = 1.6;
	t_vhit h = vx_cast(vx()->x,vx()->y,dx,dy);
	double step;
	/* Extend bridges from the near bank instead of skipping a water tile. */
	for (step = 0.25; step < fmin(2.8,h.distance); step += 0.1)
	{
		*x = (int)floor(vx()->x+dx*step);
		*y = (int)floor(vx()->y+dy*step);
		if (vx_tile(*x,*y) == '~') return (interior(*x,*y));
	}
	if (h.distance <= 2.8 && fabs(vx()->pitch) < 242.5/h.distance)
	{
		*x = h.x-(h.side == 0 ? (dx > 0 ? 1 : -1) : 0);
		*y = h.y-(h.side == 1 ? (dy > 0 ? 1 : -1) : 0);
	}
	else
	{
		if (vx()->pitch < -20) dist = fmin(2.8,242.5/-vx()->pitch);
		*x = (int)floor(vx()->x+dx*dist);
		*y = (int)floor(vx()->y+dy*dist);
		/* A low aiming angle must not place blocks through a nearer wall. */
		if (h.distance < dist) return (0);
	}
	return (interior(*x,*y));
}

static int occupied(int x, int y)
{
	int i;
	t_vnpc *n;
	if (fabs(vx()->x-x-0.5) < 0.71 && fabs(vx()->y-y-0.5) < 0.71)
		return (1);
	for (i = 0; i < vx()->npc_count; i++)
	{
		n = &vx()->npcs[i];
		if ((!n->respawn && fabs(n->x-x-0.5) < 0.73 && fabs(n->y-y-0.5) < 0.73)
			|| ((int)n->home_x == x && (int)n->home_y == y))
			return (1);
	}
	return (0);
}

void vx_place(void)
{
	int x, y, tile, cost;
	if (!vx()->build_mode) { vx_notice("PRESS B TO ENTER BUILD MODE"); return; }
	if (vx()->attack > 0) return;
	if (!vx_build_target(&x,&y)) { vx_notice("OUT OF REACH"); return; }
	tile = vx_tile(x,y);
	if (!ft_strchr("0NSEW~b",tile) || occupied(x,y))
	{ vx_notice("THAT SPACE IS OCCUPIED - STEP BACK"); return; }
	cost = tile == '~' ? 2 : vx()->block;
	if (vx()->inventory[cost] <= 0)
	{ vx_notice(tile == '~' ? "MINE TIMBER FOR BRIDGE PLANKS" : "NO MATERIAL - MINE MORE BLOCKS"); return; }
	vx()->cub->map.map[y][x] = tile == '~' ? 'b' : '1'+vx()->block;
	vx()->inventory[cost]--;
	vx()->placed++;
	vx()->attack = 0.22;
	vx_notice(tile == '~' ? "BRIDGE BUILT - YOU CAN WALK ACROSS" : "BLOCK PLACED");
}

static int protected(int x, int y)
{
	if (!interior(x,y)) return (1);
	return (vx_tile(x-1,y) == 'D' || vx_tile(x+1,y) == 'D'
		|| vx_tile(x,y-1) == 'D' || vx_tile(x,y+1) == 'D');
}

void vx_mine(void)
{
	t_vhit h;
	int hardness, material;
	char text[80];
	if (vx()->attack > 0) return;
	vx()->attack = 0.28;
	h = vx_cast(vx()->x,vx()->y,cos(vx()->angle),sin(vx()->angle));
	if (h.distance > 2.8 || fabs(vx()->pitch) > 242.5/h.distance)
	{ vx_notice("MOVE CLOSER AND AIM AT A BLOCK"); return; }
	if (h.tile < '1' || h.tile > '4' || protected(h.x,h.y))
	{ vx_notice("BOUNDARIES AND DOOR FRAMES ARE PROTECTED"); return; }
	if (vx()->mine_x != h.x || vx()->mine_y != h.y)
		vx()->mine_hits = 0;
	vx()->mine_x = h.x;
	vx()->mine_y = h.y;
	vx()->mine_until = vx()->time+2;
	vx()->mine_hits++;
	hardness = h.tile == '4' ? 3 : (h.tile == '2' ? 1 : 2);
	if (vx()->mine_hits < hardness)
	{ vx_notice("MINING - KEEP HOLDING SPACE"); return; }
	material = h.tile == '4' ? 0 : h.tile-'1';
	vx()->inventory[material] = (int)fmin(999,vx()->inventory[material]+1);
	vx()->cub->map.map[h.y][h.x] = '0';
	vx()->mine_hits = 0;
	vx()->mined++;
	if (h.tile == '4')
	{
		vx()->crystals++;
		snprintf(text,sizeof(text),"CRYSTAL FOUND! %d / %d - %s",vx()->crystals,
			vx()->crystal_goal,vx()->crystals == vx()->crystal_goal ? "RETURN TO THE BEACON" : "KEEP EXPLORING");
		vx_notice(vx()->crystal_goal ? text : "CRYSTAL COLLECTED");
	}
	else vx_notice("MATERIAL COLLECTED - B TO BUILD WITH IT");
}

void vx_action(void)
{
	t_vhit h = vx_cast(vx()->x,vx()->y,cos(vx()->angle),sin(vx()->angle));
	if (vx()->build_mode || (vx()->weapon == 1 && h.distance < 2.8))
		vx_mine();
	else vx_attack();
}

int vx_portal_near(void)
{
	t_vhit h = vx_cast(vx()->x,vx()->y,cos(vx()->angle),sin(vx()->angle));
	return (h.tile == 'T' && h.distance < 2.0);
}
