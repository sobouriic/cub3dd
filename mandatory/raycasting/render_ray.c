/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_ray.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khammadi <khammadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 23:58:55 by khammadi          #+#    #+#             */
/*   Updated: 2024/02/28 02:33:40 by khammadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

double	regularize_angle(double ang)
{
	if (ang >= 0)
	{
		while (ang >= DOUBLE_PI)
			ang -= DOUBLE_PI;
	}
	else
	{
		while (ang <= 0)
			ang += DOUBLE_PI;
	}
	return (ang);
}

void	init_rays(double rayAng, t_ray *ray)
{
	ray->ray_ang = regularize_angle(rayAng);
	ray->faci_down = ray->ray_ang > 0 && ray->ray_ang < PI;
	ray->faci_up = !ray->faci_down;
	ray->faci_right = ray->ray_ang < 0.5 * PI || ray->ray_ang > 1.5 * PI;
	ray->faci_left = !ray->faci_right;
}

void	calcul_distance(t_cub3d *cub, t_data_ray *ray)
{
	if (ray->f_w_hit == TRUE)
		ray->distance = distance_entre_points(cub->player.x, cub->player.y,
				ray->w_hitx, ray->w_hity);
	else
		ray->distance = 111111;
}

void	calcule_ray(t_cub3d *cub, t_data_ray *hv, int a, int b)
{
	double	next_tx;
	double	next_ty;

	next_tx = hv->x_inter;
	next_ty = hv->y_inter;
	while (next_tx >= 0 && next_tx <= cub->parse.column * SIZE_W
		&& next_ty >= 0 && next_ty <= cub->parse.row * SIZE_W)
	{
		if (the_wall(cub, next_tx + a, next_ty + b))
		{
			hv->f_w_hit = TRUE;
			hv->w_hitx = next_tx;
			hv->w_hity = next_ty;
			break ;
		}
		next_tx += hv->x_step;
		next_ty += hv->y_step;
	}
	calcul_distance(cub, hv);
}

void	calcule_horiz_rays(t_cub3d *cub, t_data_ray *hor)
{
	hor->f_w_hit = FALSE;
	hor->w_hitx = 0;
	hor->w_hity = 0;
	hor->y_inter = floor(cub->player.y / SIZE_W) * SIZE_W;
	hor->y_inter += conditional_operator(cub->ray.faci_down, SIZE_W, 0);
	hor->x_inter = cub->player.x + (hor->y_inter - cub->player.y)
		/ tan(cub->ray.ray_ang);
	hor->y_step = SIZE_W;
	hor->y_step *= conditional_operator(cub->ray.faci_up, -1, 1);
	hor->x_step = SIZE_W / tan(cub->ray.ray_ang);
	hor->x_step *= conditional_operator(cub->ray.faci_left
			&& hor->x_step > 0, -1, 1);
	hor->x_step *= conditional_operator(cub->ray.faci_right
			&& hor->x_step < 0, -1, 1);
	calcule_ray(cub, hor, 0, conditional_operator(cub->ray.faci_up, -1, 0));
}
