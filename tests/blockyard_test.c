#include "../bonus/includes/blockyard.h"
#include <assert.h>
#include <string.h>

static t_cub3d cub;
static char grid[9][15];
static char *rows[9];

static void setup(void)
{
	int x, y;
	memset(&cub,0,sizeof(cub));
	for (y = 0; y < 9; y++)
	{
		rows[y] = grid[y];
		for (x = 0; x < 14; x++)
			grid[y][x] = (x == 0 || x == 13 || y == 0 || y == 8 || x == 7) ? '1' : '0';
		grid[y][14] = 0;
	}
	grid[4][7] = 'D';
	grid[4][10] = 'P';
	cub.parse.row = 9;
	cub.parse.column = 14;
	cub.map.map = rows;
	cub.player.x = 5.5*SIZE_W;
	cub.player.y = 4.5*SIZE_W;
	vx_init(&cub);
}

static void doors(void)
{
	t_vhit h;
	int i;
	setup();
	assert(vx()->door_count == 1 && vx()->npc_count == 1);
	assert(!vx_clear(7.5,4.5,0.19));
	h = vx_cast(5.5,4.5,1,0);
	assert(h.tile == 'D' && fabs(h.distance-2) < 1e-6);
	vx()->x = 6.0;
	assert(vx_near_door() == 0);
	vx_interact();
	for (i = 0; i < 40; i++) vx_update(1.0/60);
	assert(vx()->doors[0].open == 1);
	assert(vx_clear(7.5,4.5,0.19));
	h = vx_cast(5.5,4.5,1,0);
	assert(h.tile == '1' && h.distance > 7);
	vx()->x = 7.5;
	vx()->doors[0].target = 0;
	vx_update(0.05);
	assert(vx()->doors[0].open == 1 && vx()->doors[0].target == 1);
	vx()->x = 5.5;
	vx()->doors[0].target = 0;
	for (i = 0; i < 40; i++) vx_update(1.0/60);
	assert(vx()->doors[0].open == 0);
	assert(!vx_clear(-1,2,0.2) && !vx_clear(100,100,0.2));
	puts("PASS: doors animate, block rays/movement, and cannot close on player");
}

static void timing(void)
{
	int i;
	double a;
	setup(); vx()->x = 2; vx()->y = 2;
	vx()->keys[KEY_W] = 1;
	for (i = 0; i < 60; i++) vx_update(1.0/60);
	a = vx()->x;
	setup(); vx()->x = 2; vx()->y = 2;
	vx()->keys[KEY_W] = 1;
	for (i = 0; i < 120; i++) vx_update(1.0/120);
	assert(fabs(a-vx()->x) < 1e-8 && fabs(a-4.4) < 1e-8);
	for (i = 0; i < 180; i++) vx_update(1.0/60);
	assert(vx()->x < 6.82);
	puts("PASS: movement is frame-rate independent and walls stop the player");
}

static void combat(void)
{
	int i;
	setup(); vx()->weapon = 2;
	vx_attack();
	assert(vx()->npcs[0].hp == 3);
	vx()->doors[0].open = 1;
	vx()->doors[0].target = 1;
	for (i = 0; i < 3; i++) { vx()->attack = 0; vx_attack(); }
	assert(vx()->npcs[0].respawn == 5 && vx()->score == 1);
	for (i = 0; i < 102; i++) vx_update(0.05);
	assert(vx()->npcs[0].hp == 3 && !vx()->npcs[0].respawn);
	vx_key_down(18,NULL); assert(vx()->weapon == 0);
	vx_key_down(19,NULL); assert(vx()->weapon == 1);
	vx_key_down(20,NULL); assert(vx()->weapon == 2);
	puts("PASS: attacks respect closed doors, targets respawn, all weapons equip");
}

static void rendering(void)
{
	static int hidden[VX_W*VX_H];
	int i;
	double x;
	setup(); vx()->map_on = 0;
	vx_render();
	memcpy(hidden,vx()->frame,sizeof(hidden));
	vx()->npc_count = 0;
	vx_render();
	assert(memcmp(hidden,vx()->frame,sizeof(hidden)) == 0);
	setup();
	x = vx()->npcs[0].x;
	for (i = 0; i < 600; i++)
	{
		vx_update(1.0/60);
		assert(vx_clear(vx()->npcs[0].x,vx()->npcs[0].y,0.21));
	}
	assert(fabs(x-vx()->npcs[0].x) > 0.01);
	for (i = 0; i < 64; i++)
	{
		vx()->angle = i*0.1;
		vx()->pitch = i%2 ? -95 : 95;
		vx_render();
	}
	puts("PASS: walls occlude characters; wandering and extreme camera angles are safe");
}

static void building(void)
{
	int i, before;
	setup();
	vx()->x = 2.5; vx()->y = 4.5;
	vx()->build_mode = 1; vx()->block = 2;
	before = vx()->inventory[2];
	vx_place();
	assert(grid[4][4] == '3' && vx()->inventory[2] == before-1);
	for (i = 0; i < 2; i++) { vx()->attack = 0; vx_mine(); }
	assert(grid[4][4] == '0' && vx()->inventory[2] == before);
	vx()->attack = 0; vx()->inventory[2] = 0;
	vx_place();
	assert(grid[4][4] == '0' && vx()->inventory[2] == 0);
	vx()->inventory[2] = before;
	grid[4][4] = '~';
	assert(!vx_clear(4.5,4.5,0.19));
	vx_place();
	assert(grid[4][4] == 'b' && vx_clear(4.5,4.5,0.19));
	assert(vx()->inventory[2] == before-1);
	/* Character homes remain free even while their character is elsewhere. */
	vx()->attack = 0; vx()->x = 8.9;
	vx_place();
	assert(grid[4][10] == 'P');
	/* Cannot mine the boundary or the structural frame of a door. */
	vx()->x = 1.5; vx()->y = 2.5; vx()->angle = PI;
	for (i = 0; i < 4; i++) { vx()->attack = 0; vx_mine(); }
	assert(grid[2][0] == '1');
	vx()->x = 5.5; vx()->y = 3.5; vx()->angle = 0;
	for (i = 0; i < 4; i++) { vx()->attack = 0; vx_mine(); }
	assert(grid[3][7] == '1');
	puts("PASS: mining, inventory, bridges, empty stock, and protected spaces");
}

static void quest(void)
{
	int i;
	setup();
	grid[2][4] = '4'; grid[6][11] = 'T';
	vx_init(&cub);
	assert(vx()->crystal_goal == 1);
	vx()->x = 10.0; vx()->y = 6.5;
	vx_interact();
	assert(!vx()->won);
	vx()->x = 2.5; vx()->y = 2.5;
	for (i = 0; i < 3; i++) { vx()->attack = 0; vx_mine(); }
	assert(vx()->crystals == 1 && grid[2][4] == '0');
	vx()->attack = 0; vx_mine();
	assert(vx()->crystals == 1);
	vx()->x = 10.0; vx()->y = 6.5;
	assert(vx_portal_near());
	vx_interact();
	assert(vx()->won);
	vx()->build_mode = 1;
	for (i = 0; i < 3; i++)
	{
		vx()->block = i; vx()->map_on = i;
		vx_render();
	}
	puts("PASS: crystals collect once and the beacon unlocks only on completion");
}

int main(void)
{
	doors(); timing(); combat(); rendering(); building(); quest();
	puts("All Blockyard checks passed.");
	return (0);
}
