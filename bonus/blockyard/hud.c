#include "../includes/blockyard.h"

/* Original 5x7 bitmap lettering, rendered into the image with the scene. */
static const unsigned char g_font[][7] = {
{14,17,17,31,17,17,17},{30,17,17,30,17,17,30},{14,17,16,16,16,17,14},
{30,17,17,17,17,17,30},{31,16,16,30,16,16,31},{31,16,16,30,16,16,16},
{14,17,16,23,17,17,15},{17,17,17,31,17,17,17},{31,4,4,4,4,4,31},
{7,2,2,2,18,18,12},{17,18,20,24,20,18,17},{16,16,16,16,16,16,31},
{17,27,21,21,17,17,17},{17,25,21,19,17,17,17},{14,17,17,17,17,17,14},
{30,17,17,30,16,16,16},{14,17,17,17,21,18,13},{30,17,17,30,20,18,17},
{15,16,16,14,1,1,30},{31,4,4,4,4,4,4},{17,17,17,17,17,17,14},
{17,17,17,17,17,10,4},{17,17,17,21,21,21,10},{17,17,10,4,10,17,17},
{17,17,10,4,4,4,4},{31,1,2,4,8,16,31},
{14,17,19,21,25,17,14},{4,12,4,4,4,4,14},{14,17,1,2,4,8,31},
{30,1,1,14,1,1,30},{2,6,10,18,31,2,2},{31,16,16,30,1,1,30},
{14,16,16,30,17,17,14},{31,1,2,4,8,8,8},{14,17,17,14,17,17,14},
{14,17,17,15,1,1,14},{0,0,0,31,0,0,0},{0,4,0,0,4,0,0},
{0,1,2,4,8,16,0},{4,4,4,4,4,0,4}
};

void vx_text(int x, int y, const char *text, int color, int scale)
{
	const char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-:/!";
	const char *p;
	int i, row, col;
	for (i = 0; text[i]; i++)
	{
		p = ft_strchr(chars, text[i]);
		if (!p) continue;
		for (row = 0; row < 7; row++)
			for (col = 0; col < 5; col++)
				if (g_font[p-chars][row] & (1<<(4-col)))
					vx_rect(x+i*6*scale+col*scale,y+row*scale,scale,scale,color);
	}
}

static int tool_pixel(int x, int y, int kind)
{
	if (kind == 2)
	{
		if (x >= 20 && x <= 28 && y >= 15 && y < 57)
			return (x < 23 ? 0xd4ae6b : 0x795733);
		if (y >= 21 && y < 29 && x >= 3 && x < 46)
			return (y < 24 ? 0xc49253 : 0x6a4930);
		if ((x == 4 || x == 44) && y >= 28 && y <= 38) return (0xd6e5df);
		if (y == 38 && x > 4 && x < 44) return (0xd6e5df);
		if (x >= 23 && x <= 25 && y >= 5 && y < 40) return (0xa8dfd2);
		if (abs(x-24)+(y-3) <= 5 && y >= 3) return (0xe4fff1);
		return (-1);
	}
	if (x >= 21 && x <= 26 && y >= 40 && y < 58)
		return (x < 24 ? 0xba8a50 : 0x65482f);
	if (kind == 0)
	{
		if (x >= 13 && x < 35 && y >= 37 && y < 42) return (0xe2b459);
		if (x >= 20 && x <= 27 && y >= 9 && y < 37)
			return (x < 23 ? 0xd6fff0 : (x < 26 ? 0x60d3c2 : 0x2d8f92));
		if (y >= 4 && y < 9 && abs(x-24) <= y-4) return (0xd6fff0);
	}
	else
	{
		if (x >= 21 && x <= 26 && y >= 14 && y < 40) return (0xb48c53);
		if (y >= 9 && y < 16 && x >= 10 && x < 39) return (0x8dd5d0);
		if (y >= 16 && y < 27 && x >= 32 && x < 39) return (0x439698);
		if (y >= 16 && y < 21 && x >= 6 && x < 14) return (0xc9f4dc);
	}
	return (-1);
}

void vx_weapon(void)
{
	int x, y, tx, ty, c;
	double duration = vx()->weapon == 1 ? 0.52 : 0.34;
	double swing = vx()->attack ? sin(vx()->attack/duration*PI) : 0;
	double angle = -0.35+swing*1.05;
	double bob = (vx()->keys[KEY_W] || vx()->keys[KEY_S]
		|| vx()->keys[KEY_A] || vx()->keys[KEY_D]) ? sin(vx()->time*10)*3 : 0;
	double px = 419-swing*65, py = 322+bob;
	if (vx()->build_mode)
	{
		vx_block_icon(393-swing*12,228+bob,76,vx()->block);
		vx_rect(410,301+bob,27,35,0xc69a72);
		return;
	}
	/* Inverse mapping gives solid, nearest-neighbour rotated pixel art. */
	for (y = 80; y < VX_H; y++)
		for (x = 270; x < VX_W; x++)
		{
			tx = (int)floor(((x-px)*cos(angle)+(y-py)*sin(angle))/3)+24;
			ty = (int)floor((-(x-px)*sin(angle)+(y-py)*cos(angle))/3)+57;
			c = tool_pixel(tx,ty,vx()->weapon);
			if (c >= 0) vx_pixel(x,y,c);
			else if (tool_pixel(tx-1,ty,vx()->weapon) >= 0
				|| tool_pixel(tx+1,ty,vx()->weapon) >= 0
				|| tool_pixel(tx,ty-1,vx()->weapon) >= 0
				|| tool_pixel(tx,ty+1,vx()->weapon) >= 0) vx_pixel(x,y,0x293d39);
		}
	vx_rect((int)px-8,(int)py-12,24,35,0xb78861);
	vx_rect((int)px-8,(int)py-12,8,30,0xd0a17a);
}

static void panel(int x, int y, int w, int h)
{
	vx_rect(x+2,y+3,w,h,0x172724);
	vx_rect(x,y,w,h,0x344640);
	vx_rect(x+1,y+1,w-2,h-2,0x21352f);
	vx_rect(x+1,y+1,w-2,1,0x6f8170);
}

static void minimap(void)
{
	int x, y, t, c, xx, yy, i;
	panel(466,12,82,91);
	vx_text(475,18,"LOCAL MAP",0xb5c6aa,1);
	for (y = -4; y <= 4; y++)
		for (x = -4; x <= 4; x++)
		{
			xx = (int)floor(vx()->x)+x;
			yy = (int)floor(vx()->y)+y;
			t = vx_tile(xx,yy);
			c = vx_map_color(t);
			if (t == 'D') c = vx_door(xx,yy)->open > 0.98 ? 0x7bae6a : 0xc39854;
			vx_rect(475+(x+4)*7,32+(y+4)*7,6,6,c);
		}
	for (i = 0; i < vx()->npc_count; i++)
	{
		x = (int)floor(vx()->npcs[i].x)-(int)floor(vx()->x);
		y = (int)floor(vx()->npcs[i].y)-(int)floor(vx()->y);
		if (abs(x) <= 4 && abs(y) <= 4 && !vx()->npcs[i].respawn)
			vx_rect(477+(x+4)*7,34+(y+4)*7,3,3,0xe5b872);
	}
	vx_rect(504,61,5,5,0xf5f3d0);
	vx_rect(505+cos(vx()->angle)*5,62+sin(vx()->angle)*5,2,2,0xf5f3d0);
}

void vx_hud(void)
{
	int i, x, y, c, near;
	char label[64];
	const char *tools[] = {"DIAMOND SWORD", "MINER PICKAXE", "OAK CROSSBOW"};
	const char *blocks[] = {"STONE BLOCK", "GRASS BLOCK", "TIMBER BLOCK"};
	const char **names = vx()->build_mode ? blocks : tools;
	int selected = vx()->build_mode ? vx()->block : vx()->weapon;
	panel(12,12,160,39);
	vx_text(21,19,"BLOCKYARD",0xf0eacb,2);
	vx_text(22,38,"THE SHATTERED ISLAND",0xa3b69e,1);
	if (vx()->help)
	{
		panel(12,60,172,65);
		vx_text(20,68,"WASD MOVE / ARROWS LOOK",0xc1cbb5,1);
		vx_text(20,79,"E INTERACT / B BUILD MODE",0xc1cbb5,1);
		vx_text(20,90,"1-3 SELECT / CLICK OR SPACE",0xc1cbb5,1);
		vx_text(20,101,"RIGHT CLICK OR R TO PLACE",0x92a78f,1);
		vx_text(20,112,"M ATLAS / H HELP / ESC EXIT",0x92a78f,1);
	}
	if (vx()->map_on == 1) minimap();
	c = vx()->hit > 0 ? 0xffce78 : 0xf7f2d9;
	vx_rect(VX_W/2-5,VX_H/2,11,1,0x253c34);
	vx_rect(VX_W/2,VX_H/2-5,1,11,0x253c34);
	vx_rect(VX_W/2-4,VX_H/2,9,1,c);
	vx_rect(VX_W/2,VX_H/2-4,1,9,c);
	near = vx_near_door();
	if (near >= 0)
	{
		panel(214,224,132,20);
		vx_text(224,231,vx()->doors[near].target ? "E / CLOSE WOOD DOOR"
			: "E / OPEN WOOD DOOR",0xf5d898,1);
	}
	panel(199,288,162,51);
	for (i = 0; i < 3; i++)
	{
		vx_rect(204+i*52,293,48,41,i == selected ? 0xddc383 : 0x526353);
		vx_rect(206+i*52,295,44,37,i == selected ? 0x4d6555 : 0x2b4036);
		for (y = 0; y < 32 && !vx()->build_mode; y++)
			for (x = 0; x < 24; x++)
			{
				c = tool_pixel(x*2,y*2,i);
				if (c >= 0) vx_pixel(218+i*52+x,296+y,c);
			}
		if (vx()->build_mode)
		{
			vx_block_icon(220+i*52,299,22,i);
			snprintf(label,sizeof(label),"%d",vx()->inventory[i]);
			vx_text(224+i*52,324,label,0xffe5a5,1);
		}
		label[0] = '1'+i; label[1] = 0;
		vx_text(210+i*52,298,label,0xf7edcd,1);
	}
	vx_text(VX_W/2-(int)ft_strlen(names[selected])*3,275,
		names[selected],0xfff3ca,1);
	panel(12,312,150,26);
	snprintf(label,sizeof(label),"%s / B TO SWITCH",vx()->build_mode ? "BUILD" : "TOOLS");
	vx_text(20,321,label,0xc9d8b6,1);
	vx_adventure_hud();
}
