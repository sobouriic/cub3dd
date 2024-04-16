/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khammadi <khammadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 23:59:20 by khammadi          #+#    #+#             */
/*   Updated: 2024/02/28 00:11:01 by khammadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	game_loop(t_cub3d *cub)
{
	main_update(cub);
	update_3d_value(cub);
	update_multiple_rays(cub);
	init_screen(cub);
	walls_3d_projection_render(cub);
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img.ptr, 0, 0);
	return (0);
}

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
