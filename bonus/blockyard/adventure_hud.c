#include "../includes/blockyard.h"

int vx_map_color(int tile)
{
	if (tile == '~') return (0x438da9);
	if (tile == 'b') return (0xcda66c);
	if (tile == '4') return (0x6be2d4);
	if (tile == 'T') return (0xca9bef);
	if (tile == '3') return (0xb38e61);
	if (tile == '2') return (0x819753);
	if (tile == '1') return (0x9baba0);
	if (tile == 'D') return (0xc39854);
	return (0x52674b);
}

void vx_block_icon(int x, int y, int size, int material)
{
	int xx, yy, c;
	vx_rect(x-2,y-2,size+4,size+4,0x263a31);
	for (yy = 0; yy < size; yy++)
		for (xx = 0; xx < size; xx++)
		{
			c = vx_material('1'+material,(double)xx/size,(double)yy/size,0,0);
			if (xx > size*0.72) c = vx_shade(c,0.68);
			if (yy < size*0.14) c = vx_shade(c,1.18);
			vx_pixel(x+xx,y+yy,c);
		}
}

static void map_overview(void)
{
	int x, y, s, ox, oy;
	t_cub3d *c = vx()->cub;
	s = (int)fmax(1,fmin(6,fmin(390.0/c->parse.column,205.0/c->parse.row)));
	ox = VX_W/2-c->parse.column*s/2;
	oy = 77;
	vx_rect(ox-10,oy-26,c->parse.column*s+20,c->parse.row*s+58,0x21352f);
	vx_text(ox,oy-17,"ATLAS / CYAN CRYSTALS / PURPLE BEACON",0xf0dfb9,1);
	for (y = 0; y < c->parse.row; y++)
		for (x = 0; x < c->parse.column; x++)
			vx_rect(ox+x*s,oy+y*s,s-1,s-1,vx_map_color(vx_tile(x,y)));
	vx_rect(ox+vx()->x*s-2,oy+vx()->y*s-2,5,5,0xfff8d2);
	vx_text(ox,oy+c->parse.row*s+10,"M TO CLOSE / BUILD BRIDGES OVER BLUE WATER",0xc9d8b6,1);
}

void vx_adventure_hud(void)
{
	char text[96];
	int width;
	if (vx()->crystal_goal)
	{
		vx_rect(181,12,274,27,0x21352f);
		snprintf(text,sizeof(text),"RESTORE THE BEACON / CRYSTALS %d-%d",vx()->crystals,vx()->crystal_goal);
		vx_text(189,18,text,0xf1dfb2,1);
		vx_text(189,29,vx()->crystals < vx()->crystal_goal
			? "MINE CYAN ORE / M FOR THE ISLAND ATLAS"
			: "ALL SHARDS FOUND / E AT THE PURPLE BEACON",0x95cbb4,1);
	}
	if (vx()->message_until > vx()->time)
	{
		width = (int)ft_strlen(vx()->message)*6+16;
		vx_rect(VX_W/2-width/2,251,width,17,0x21352f);
		vx_text(VX_W/2-width/2+8,256,vx()->message,0xf3db9f,1);
	}
	if (vx_portal_near())
	{
		vx_rect(181,218,198,21,0x332f49);
		vx_text(191,225,vx()->crystals >= vx()->crystal_goal
			? "E / RESTORE THE ANCIENT BEACON" : "BEACON LOCKED / FIND ALL SHARDS",0xe2c8f7,1);
	}
	if (vx()->won)
	{
		vx_rect(177,45,282,32,0x21352f);
		vx_text(188,52,"BEACON RESTORED!",0xe8e8b8,2);
		vx_text(188,69,"THE ISLAND IS YOURS - KEEP BUILDING",0x95cbb4,1);
	}
	if (vx()->map_on == 2) map_overview();
}

static void edge(double x1, double y1, double x2, double y2, int color)
{
	int i, steps = (int)fmin(5000,fmax(fabs(x2-x1),fabs(y2-y1))+1);
	for (i = 0; i <= steps; i++)
		vx_pixel(x1+(x2-x1)*i/steps,y1+(y2-y1)*i/steps,color);
}

void vx_build_preview(void)
{
	int x, y, i, c;
	double px[4], py[4], dx, dy, z, side;
	int cx[4] = {0,1,1,0}, cy[4] = {0,0,1,1};
	if (!vx()->build_mode || !vx_build_target(&x,&y)) return;
	for (i = 0; i < 4; i++)
	{
		dx = x+cx[i]-vx()->x;
		dy = y+cy[i]-vx()->y;
		z = dx*cos(vx()->angle)+dy*sin(vx()->angle);
		side = -dx*sin(vx()->angle)+dy*cos(vx()->angle);
		if (z < 0.08) return;
		px[i] = VX_W/2+side*485/z;
		py[i] = VX_H/2+vx()->pitch+242.5/z;
	}
	c = ft_strchr("0~b",vx_tile(x,y)) ? 0xf4e5ac : 0xe89080;
	for (i = 0; i < 4; i++) edge(px[i],py[i],px[(i+1)%4],py[(i+1)%4],c);
}
