/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_raycasting_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:19:11 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/28 15:23:08 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	pair_col_row(int *n1, int *n2)
{
	if (*n1 % 2 != 0)
		*n1 = *n1 + 1;
	if (*n2 % 2 != 0)
		*n2 = *n2 + 1;
}

void	map_initialisation(t_cub3d *cub)
{
	int	i;
	int	colum;
	int	row;

	cub->map.map = (char **)malloc(sizeof(char *) * cub->map.map_row);
	i = -1;
	while (++i < cub->map.map_row)
		cub->map.map[i] = malloc(sizeof(char) * (cub->map.map_col + 1));
	row = -1;
	while (++row < cub->parse.row)
	{
		colum = -1;
		while (cub->parse.map[row][++colum] != '\0')
			cub->map.map[row][colum] = cub->parse.map[row][colum];
		colum--;
		while (++colum < cub->map.map_col)
			cub->map.map[row][colum] = ' ';
		cub->map.map[row][colum] = '\0';
	}
	ft_free((void **)cub->parse.map, cub->parse.row);
}

void	main_init(t_cub3d *cub)
{
	cub->map.map_row = cub->parse.row;
	cub->map.map_col = cub->parse.column;
	pair_col_row(&cub->map.map_row, &cub->map.map_col);
	cub->map.win_height = W_H * SIZE_W;
	cub->map.win_width = W_W * SIZE_W;
	cub->map.r_count = cub->map.win_width / WALL_STRIP;
	map_initialisation(cub);
	cub->rray = (t_rrray *)malloc(sizeof(t_rrray) * (cub->map.r_count));
}

void	init_mlx(t_cub3d *cub)
{
	cub->mlx = mlx_init();
	cub->win = mlx_new_window(cub->mlx, cub->map.win_width,
			cub->map.win_height, "CUB3D");
	cub->img.ptr = mlx_new_image(cub->mlx,
			cub->map.win_width, cub->map.win_height);
	cub->img.data = (int *)mlx_get_data_addr(cub->img.ptr, &(cub->img.bpp),
			&(cub->img.line_size), &(cub->img.endian));
}

void	walls_3d_projection_render(t_cub3d *cub)
{
	int	i;

	i = -1;
	while (++i < cub->map.r_count)
		walls_3d_projection(cub, i);
}
