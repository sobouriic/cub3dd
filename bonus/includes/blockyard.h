#ifndef BLOCKYARD_H
# define BLOCKYARD_H
# include "cub3d_bonus.h"
# include <time.h>
# define VX_W 560
# define VX_H 350
# define VX_MAX_DOORS 128
# define VX_MAX_NPCS 64

typedef struct s_vdoor
{
	int x, y, axis, target;
	double open;
} t_vdoor;

typedef struct s_vnpc
{
	double x, y, home_x, home_y, angle, turn, flash, respawn;
	int hp;
} t_vnpc;

typedef struct s_vhit
{
	double distance, u;
	int tile, side, x, y;
} t_vhit;

typedef struct s_voxel
{
	t_cub3d *cub;
	double x, y, angle, pitch, time, last, dt, attack, hit;
	int keys[128], weapon, score, map_on, help, door_count, npc_count;
	int build_mode, block, inventory[3], crystals, crystal_goal, won;
	int mine_x, mine_y, mine_hits, placed, mined;
	double message_until, mine_until;
	char message[80];
	int frame[VX_W * VX_H];
	double depth[VX_W];
	t_vdoor doors[VX_MAX_DOORS];
	t_vnpc npcs[VX_MAX_NPCS];
} t_voxel;

t_voxel *vx(void);
void vx_init(t_cub3d *cub);
void vx_update(double dt);
void vx_render(void);
int vx_loop(t_cub3d *cub);
int vx_key_down(int key, void *unused);
int vx_key_up(int key, void *unused);
int vx_mouse(int button, int x, int y, void *unused);
int vx_tile(int x, int y);
t_vdoor *vx_door(int x, int y);
int vx_clear(double x, double y, double radius);
t_vhit vx_cast(double x, double y, double dx, double dy);
void vx_interact(void);
int vx_near_door(void);
void vx_attack(void);
void vx_npcs_update(double dt);
void vx_npcs_render(void);
int vx_npc_pixel(int x, int y, double phase, int variant);
int vx_material(int tile, double u, double v, int x, int y);
int vx_shade(int color, double light);
void vx_pixel(int x, int y, int color);
void vx_rect(int x, int y, int w, int h, int color);
void vx_text(int x, int y, const char *text, int color, int scale);
void vx_hud(void);
void vx_weapon(void);
void vx_capture(const char *path);
void vx_action(void);
void vx_place(void);
void vx_mine(void);
void vx_notice(const char *text);
int vx_build_target(int *x, int *y);
int vx_portal_near(void);
void vx_adventure_hud(void);
int vx_map_color(int tile);
void vx_block_icon(int x, int y, int size, int material);
void vx_build_preview(void);
#endif
