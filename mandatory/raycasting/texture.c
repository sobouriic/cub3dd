/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khammadi <khammadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 23:59:04 by khammadi          #+#    #+#             */
/*   Updated: 2024/02/28 10:57:03 by khammadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	*image_allocation_texture(t_cub3d *cub, char *path, t_mlx_image *img, int i)
{
	int	colum;
	int	row;
	int	*value;

	img->ptr = mlx_xpm_file_to_image(cub->mlx, path, &img->img_w, &img->img_h);
	if (!img->ptr)
		exit_game(cub, ERROR, "Wrong Texture");
	cub->parse.texture[i].w = img->img_w;
	cub->parse.texture[i].h = img->img_h;
	img->data = (int *)mlx_get_data_addr(img->ptr, &img->bpp,
			&img->line_size, &img->endian);
	value = (int *)malloc(sizeof(int) * (img->img_w * img->img_h));
	row = -1;
	while (++row < img->img_h)
	{
		colum = -1;
		while (++colum < img->img_w)
			value[img->img_w * row + colum]
				= img->data[img->img_w * row + colum];
	}
	mlx_destroy_image(cub->mlx, img->ptr);
	return (value);
}

void	apload_texture(t_cub3d *cub)
{
	int			i;
	t_mlx_image	img;

	i = -1;
	while (++i < COUNT)
	{
		cub->parse.texture[i].texture
			= image_allocation_texture(cub, cub->parse.texture[i].path,
				&img, i);
		arr_free(cub->parse.texture[i].path);
	}
}

int	drawing_sky(t_cub3d *cub, int n_r, int wa_tp, int collor)
{
	int	x;
	int	y;

	y = -1;
	while (++y < wa_tp)
	{
		x = -1;
		while (++x < WALL_STRIP)
			if (cub->img.data[cub->map.win_width * y
					+ (x + n_r * WALL_STRIP)] == COLOR)
				cub->img.data[cub->map.win_width * y
					+ (x + n_r * WALL_STRIP)] = collor;
	}
	return (0);
}

int	drawing_bottom(t_cub3d *cub, int n_r, int wa_bot, int collor)
{
	int	x;
	int	y;

	y = wa_bot - 1;
	while (++y < cub->map.win_height)
	{
		x = -1;
		while (++x < WALL_STRIP)
			if (cub->img.data[cub->map.win_width * y
					+ (x + n_r * WALL_STRIP)] == COLOR)
				cub->img.data[cub->map.win_width * y
					+ (x + n_r * WALL_STRIP)] = collor;
	}
	return (0);
}

void	walls_3d_projection(t_cub3d *cub, int i)
{
	t_treed	v;
	int		r;
	int		x;
	int		y;

	init_threed(cub, &v, i);
	r = 0;
	y = v.corr_top - 1;
	while (++y < v.corr_bott)
	{
		x = -1;
		while (++x < WALL_STRIP)
			if (cub->img.data[cub->map.win_width * y
					+ (x + i * WALL_STRIP)] == COLOR)
				cub->img.data[cub->map.win_width * y
					+ (x + i * WALL_STRIP)] = set_color_wall(cub, &v, r, i);
		r++;
	}
	drawing_sky(cub, i, v.corr_top, cub->parse.ceiling);
	drawing_bottom(cub, i, v.corr_bott, cub->parse.floor);
}
