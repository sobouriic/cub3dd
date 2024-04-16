/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 12:39:13 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/29 10:54:01 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	ft_error(char *err)
{
	write(2, "Error : \n", 10);
	ft_putstr(err);
	exit(1);
}

int	ft_destroy(t_cub3d *cub, int ret)
{
	if (cub->img.ptr)
		mlx_destroy_image(cub->mlx, cub->img.ptr);
	if (cub->win)
		mlx_destroy_window(cub->mlx, cub->win);
	if (cub)
		exit(ret);
	else
		exit(0);
	return (ret);
}

int	exit_game(t_cub3d *cub, int ret, const char *err)
{
	write(2, "Error\n", 6);
	if (err)
	{
		write(2, err, ft_strlen(err));
		write(2, "\n", 1);
	}
	ft_destroy(cub, ret);
	return (ret);
}

int	the_wall(t_cub3d *cub, double x, double y)
{
	int	i;
	int	j;

	i = (int)floor(x / SIZE_W);
	j = (int)floor(y / SIZE_W);
	return (ft_strchr("0NSEW", cub->map.map[j][i]) == NULL);
}
