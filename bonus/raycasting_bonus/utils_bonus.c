/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 23:59:20 by khammadi          #+#    #+#             */
/*   Updated: 2024/02/28 16:03:14 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	update_multiple_rays(t_cub3d *cub)
{
	double	angle;
	int		i;

	i = 0;
	angle = cub->player.r_angle - (RANGE_RAY / 2.0);
	while (i < cub->map.r_count)
	{
		update_rays(cub, angle, i);
		angle += RANGE_RAY / cub->map.r_count;
		i++;
	}
}

void	init_mlx_loop(t_cub3d *cub)
{
	init_player(&cub->player);
	mlx_hook(cub->win, 2, 1L << 0, &key_press, &cub->key);
	mlx_hook(cub->win, 3, 1L << 1, &keyrel, &cub->key);
	mlx_hook(cub->win, 17, 0, &ft_exit, cub);
	mlx_loop_hook(cub->mlx, game_loop, cub);
}
