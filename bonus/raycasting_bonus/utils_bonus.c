/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 23:59:20 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/28 16:03:14 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/blockyard.h"

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
	mlx_hook(cub->win, 2, 1L << 0, &vx_key_down, cub);
	mlx_hook(cub->win, 3, 1L << 1, &vx_key_up, cub);
	mlx_hook(cub->win, 17, 0, &ft_exit, cub);
	mlx_mouse_hook(cub->win, &vx_mouse, cub);
	mlx_do_key_autorepeatoff(cub->mlx);
	mlx_loop_hook(cub->mlx, vx_loop, cub);
}
