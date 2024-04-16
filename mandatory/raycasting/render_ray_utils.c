/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ray_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khammadi <khammadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 23:58:47 by khammadi          #+#    #+#             */
/*   Updated: 2024/02/27 23:58:50 by khammadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

double	distance_entre_points(double x1, double y1, double x2, double y2)
{
	return (sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1)));
}

int	conditional_operator(int p, int true, int false)
{
	if (p == TRUE)
		return (true);
	return (false);
}

int	conditional_operator2(double p, double true, double false)
{
	if (p == TRUE)
		return (true);
	return (false);
}

void	calcule_vert_rays(t_cub3d *cub, t_data_ray *vert)
{
	vert->f_w_hit = FALSE;
	vert->w_hitx = 0;
	vert->w_hity = 0;
	vert->x_inter = floor(cub->player.x / SIZE_W) * SIZE_W;
	vert->x_inter += conditional_operator(cub->ray.faci_right, SIZE_W, 0);
	vert->y_inter = cub->player.y + (vert->x_inter - cub->player.x)
		* tan(cub->ray.ray_ang);
	vert->x_step = SIZE_W;
	vert->x_step *= conditional_operator(cub->ray.faci_left, -1, 1);
	vert->y_step = SIZE_W * tan(cub->ray.ray_ang);
	vert->y_step *= conditional_operator(cub->ray.faci_up
			&& vert->y_step > 0, -1, 1);
	vert->y_step *= conditional_operator(cub->ray.faci_down
			&& vert->y_step < 0, -1, 1);
	calcule_ray(cub, vert, conditional_operator(cub->ray.faci_left, -1, 0), 0);
}

void	update_rays(t_cub3d *cub, double ang, int i)
{
	t_data_ray	hor;
	t_data_ray	vert;

	init_rays(ang, &cub->ray);
	calcule_horiz_rays(cub, &hor);
	calcule_vert_rays(cub, &vert);
	if (vert.distance < hor.distance)
	{
		cub->rray[i].w_hitx = vert.w_hitx;
		cub->rray[i].wall_hity = vert.w_hity;
		cub->rray[i].distance = vert.distance;
		cub->rray[i].hit_ver = TRUE;
	}
	else
	{
		cub->rray[i].w_hitx = hor.w_hitx;
		cub->rray[i].wall_hity = hor.w_hity;
		cub->rray[i].distance = hor.distance;
		cub->rray[i].hit_ver = FALSE;
	}
	cub->rray[i].angle = cub->ray.ray_ang;
}
