#include "../includes/blockyard.h"

int vx_key_down(int key, void *unused)
{
	(void)unused;
	if (key == KEY_ESC) return (ft_exit());
	if (key < 0 || key >= 128) return (0);
	if (!vx()->keys[key])
	{
		if (key == 14) vx_interact();
		if (key >= 18 && key <= 20)
		{
			if (vx()->build_mode) vx()->block = key-18;
			else vx()->weapon = key-18;
		}
		if (key == 11)
		{
			vx()->build_mode = !vx()->build_mode;
			vx_notice(vx()->build_mode ? "BUILD MODE: 1-3 MATERIAL / RIGHT CLICK PLACE"
				: "TOOLS: 2 PICKAXE / CLICK OR SPACE TO MINE");
		}
		if (key == 15) vx_place();
		if (key == 46) vx()->map_on = (vx()->map_on+1)%3;
		if (key == 4) vx()->help = !vx()->help;
	}
	vx()->keys[key] = 1;
	return (0);
}

int vx_key_up(int key, void *unused)
{
	(void)unused;
	if (key >= 0 && key < 128) vx()->keys[key] = 0;
	return (0);
}

int vx_mouse(int button, int x, int y, void *unused)
{
	(void)x; (void)y; (void)unused;
	if (button == 1) vx_action();
	if (button == 2) vx_place();
	if (button == 4 || button == 5)
	{
		if (vx()->build_mode) vx()->block = (vx()->block+(button == 4 ? 1 : 2))%3;
		else vx()->weapon = (vx()->weapon+(button == 4 ? 1 : 2))%3;
	}
	return (0);
}

int vx_loop(t_cub3d *cub)
{
	struct timespec ts;
	double now;
	int x, y, color, row;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	now = ts.tv_sec + ts.tv_nsec/1e9;
	if (vx()->last && now-vx()->last < 1.0/60)
	{
		usleep(1000);
		return (0);
	}
	vx_update(vx()->last ? now-vx()->last : 1.0/60);
	vx()->last = now;
	vx_render();
	for (y = 0; y < cub->map.win_height; y++)
	{
		row = y*cub->img.line_size/4;
		for (x = 0; x < cub->map.win_width; x++)
		{
			color = vx()->frame[(y*VX_H/cub->map.win_height)*VX_W
				+x*VX_W/cub->map.win_width];
			cub->img.data[row+x] = color;
		}
	}
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img.ptr, 0, 0);
	return (0);
}
