/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_map_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/21 11:44:45 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/29 09:53:41 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	map_init(t_cub3d *cub)
{
	int	i;
	int	j;

	j = -1;
	while (++j < cub->map.win_height)
	{
		i = -1;
		while (++i < cub->map.win_width)
		{
			cub->img.data[cub->map.win_width * j + i] = COLOR;
		}
	}
}

void	mini_map_location(t_parse *parse, int *x, int *y, t_point *point)
{
	if (LOCATION == LEFTUP_MAP)
	{
		*x = (int)(MAP_SCALE * point->x);
		*y = (int)(MAP_SCALE * point->y);
	}
	else if (LOCATION == LEFTDOWN_MAP)
	{
		*x = (int)(MAP_SCALE * point->x);
		*y = (int)((W_H - MAP_SCALE * parse->row)
				* SIZE_W + MAP_SCALE * point->y);
	}
	else if (LOCATION == RIGHTUP_MAP)
	{
		*x = (int)((W_W - MAP_SCALE * parse->column)
				* SIZE_W + MAP_SCALE * point->x);
		*y = (int)(MAP_SCALE * point->y);
	}
	else if (LOCATION == RIGHTDOWN_MAP)
	{
		*x = (int)((W_W - MAP_SCALE * parse->column)
				* SIZE_W + MAP_SCALE * point->x);
		*y = (int)((W_H - MAP_SCALE * parse->row)
				* SIZE_W + MAP_SCALE * point->y);
	}
}

void	put_squares(t_cub3d *cub, int x, int y, int collor)
{
	int		i;
	int		j;	
	t_point	point;

	determine_point(&point, x, y);
	mini_map_location(&cub->parse, &x, &y, &point);
	j = -1;
	while (++j <= (int)(MAP_SCALE * SIZE_W))
	{
		i = 0;
		while (i <= (int)(MAP_SCALE * SIZE_W))
		{
			cub->img.data[cub->map.win_width * (y + j) + (x + i)] = collor;
			i++;
		}
	}
}

void	put_color(t_cub3d *cub, int row, int colum)
{
	if (cub->map.map[row][colum] == '1')
		put_squares(cub, (int)(SIZE_W * colum), (int)(SIZE_W * row), 0x8A2BE2);
	else
		put_squares(cub, (int)(SIZE_W * colum), (int)(SIZE_W * row), 0xE6E6FA);
}

void	map_rendering(t_cub3d *cub)
{
	int	colum;
	int	row;

	map_init(cub);
	row = -1;
	while (++row < cub->parse.row)
	{
		colum = -1;
		while (++colum < cub->parse.column)
			put_color(cub, row, colum);
	}
}
