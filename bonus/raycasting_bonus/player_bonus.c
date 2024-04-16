/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:30:16 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/28 15:37:42 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	determine_point(t_point *point, double x, double y)
{
	point->x = x;
	point->y = y;
}

void	update_pl22(t_cub3d *cub, t_init_var *var)
{
	if (cub->key.left_rotat == 1)
		var->t_dir = -1.0;
	if (cub->key.right_rotat == 1)
		var->t_dir = 1.0;
	if (cub->key.up == 1 && cub->key.down == 0)
		var->w_dir = 1.0;
	if (cub->key.down == 1 && cub->key.up == 0)
		var->w_dir = -1.0;
}

void	update_pl2(t_cub3d *cub, t_init_var *var)
{
	if (cub->key.left == 1 && cub->key.right == 0)
	{
		var->w_dir = 1.0;
		var->m_side = PI / 2.0;
		if (cub->key.up == 1 && cub->key.down == 0)
			var->m_side = HALF_PI / 2;
		if (cub->key.down == 1 && cub->key.up == 0)
		{
			var->w_dir = -1.0;
			var->m_side = -HALF_PI / 2;
		}
	}
	update_pl22(cub, var);
}

void	update_pl3(t_cub3d *cub, t_init_var *var)
{
	if (cub->key.right == 1 && cub->key.left == 0)
	{
		var->w_dir = -1.0;
		var->m_side = HALF_PI;
		if (cub->key.up == 1 && cub->key.down == 0)
		{
			var->w_dir = 1.0;
			var->m_side = -HALF_PI / 2;
		}
		if (cub->key.down == 1 && cub->key.up == 0)
			var->m_side = HALF_PI / 2;
	}
	cub->player.r_angle += var->t_dir * cub->player.t_sped;
}

void	main_update(t_cub3d *cub)
{
	t_init_var	var;
	t_point		p1;
	t_point		p2;	

	ft_memset(&var, 0, sizeof(t_init_var));
	update_pl2(cub, &var);
	update_pl3(cub, &var);
	var.m_step = var.w_dir * cub->player.w_sped;
	var.n_x = cub->player.x + var.m_step
		* cos(cub->player.r_angle - var.m_side);
	var.n_y = cub->player.y + var.m_step
		* sin(cub->player.r_angle - var.m_side);
	determine_point(&p1, cub->player.x, cub->player.y);
	determine_point(&p2, var.n_x, var.n_y);
	if (!the_wall(cub, var.n_x, var.n_y) && !check_edge(cub, &p1, &p2))
	{
		cub->player.x = var.n_x;
		cub->player.y = var.n_y;
	}
}
