/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_player_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:38:25 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/29 15:12:10 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	player_render(t_cub3d *cub)
{
	int		x;
	int		y;
	int		colum;
	int		row;
	t_point	point;

	determine_point(&point, cub->player.x, cub->player.y);
	mini_map_location(&cub->parse, &x, &y, &point);
	row = -(cub->player.point) / 2 - 1;
	while (++row <= (cub->player.point) / 2)
	{
		colum = -(cub->player.point) / 2 - 1;
		while (++colum <= (cub->player.point) / 2)
			cub->img.data[cub->map.win_width
				* (y + row) + (x + colum)] = 0x0000FF;
	}
}

void	i_ray_cal(t_cub3d *cub, t_calcule_ray *calcul, double *x, double *y)
{
	calcul->r_x = cub->player.x;
	calcul->r_y = cub->player.y;
	calcul->maximum = fmax(fabs(*x), fabs(*y));
	if (calcul->maximum == 0)
		return ;
	*x = *x / calcul->maximum;
	*y = *y / calcul->maximum;
}

void	line(t_cub3d *cub, double x, double y)
{
	t_calcule_ray	calcul;
	t_point			point1;
	t_point			point2;

	i_ray_cal(cub, &calcul, &x, &y);
	while (1)
	{
		determine_point(&point1, calcul.r_x, calcul.r_y);
		determine_point(&point2, calcul.r_x + x, calcul.r_y + y);
		if (!check_edge(cub, &point1, &point2)
			&& !the_wall(cub, calcul.r_x + x, calcul.r_y + y))
		{
			calcul.r_x += x;
			calcul.r_y += y;
		}
		else
			break ;
		mini_map_location(&cub->parse, &calcul.x, &calcul.y, &point1);
		cub->img.data[cub->map.win_width * calcul.y + calcul.x] = 0xFF0000;
	}
}

void	render_ray_mini(t_cub3d *cub)
{
	int	i;

	i = 0;
	while (i < cub->map.r_count)
	{
		line(cub, cub->rray[i].w_hitx - cub->player.x,
			cub->rray[i].wall_hity - cub->player.y);
		i++;
	}
}
