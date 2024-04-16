/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   update_3d_value_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:56:44 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/28 16:01:01 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	update_3d_value(t_cub3d *cub)
{
	cub->player.up_si += 20 * (cub->key.updown_sight);
	if (cub->player.up_si > cub->map.win_height / 2)
		cub->player.up_si = cub->map.win_height / 2;
	if (cub->player.up_si < -cub->map.win_height / 2)
		cub->player.up_si = -cub->map.win_height / 2;
}

void	init_screen(t_cub3d *cub)
{
	int	i;
	int	j;

	j = -1;
	while (++j < cub->map.win_height)
	{
		i = -1;
		while (++i < cub->map.win_width)
			cub->img.data[cub->map.win_width * j + i] = COLOR;
	}
}

void	set_wall_dir(t_cub3d *cub, int i)
{
	if (cub->rray[i].hit_ver == TRUE
		&& (cub->player.x - cub->rray[i].w_hitx) > 0)
		cub->ray.wal_pap = WEST;
	else if (cub->rray[i].hit_ver == TRUE
		&& (cub->player.x - cub->rray[i].w_hitx) < 0)
		cub->ray.wal_pap = EAST;
	else if (cub->rray[i].hit_ver == FALSE
		&& (cub->player.y - cub->rray[i].wall_hity) > 0)
		cub->ray.wal_pap = NORTH;
	else
		cub->ray.wal_pap = SOUTH;
}

int	set_color_wall(t_cub3d *cub, t_treed *m, int re, int i)
{
	int	col;
	int	row;
	int	paper;

	set_wall_dir(cub, i);
	paper = cub->ray.wal_pap;
	if (paper == WEST || paper == EAST)
		col = ((int)cub->rray[i].wall_hity % SIZE_W)
			* (cub->parse.texture[paper].w / SIZE_W);
	else
		col = ((int)cub->rray[i].w_hitx % SIZE_W)
			* (cub->parse.texture[paper].w / SIZE_W);
	row = (((m->corr_top - m->top + re)
				* cub->parse.texture[paper].h) / m->eight);
	return (cub->parse.texture[paper].texture[(int)cub->parse.texture[paper].w
			* row + col]);
}

void	init_threed(t_cub3d *cub, t_treed *t, int i)
{
	if (cub->rray[i].distance == 0)
		cub->rray[i].distance = 0.1;
	t->corr_distance = cub->rray[i].distance
		* cos(cub->rray[i].angle - cub->player.r_angle);
	t->dis_p = (cub->map.win_width / 2)
		/ tan(FOV_ANGLE / 2);
	t->p_hei = (int)((SIZE_W / t->corr_distance) * t->dis_p);
	t->top = (cub->map.win_height / 2)
		- (t->p_hei / 2) - cub->player.up_si;
	t->corr_top = conditional_operator(t->top < 0, 1, t->top);
	t->bottom = (cub->map.win_height / 2) + (t->p_hei / 2)
		- cub->player.up_si;
	t->corr_bott = conditional_operator(t->bottom > cub->map.win_height,
			cub->map.win_height, t->bottom);
	t->eight = t->bottom - t->top;
}
