/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map3_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 17:21:46 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/29 15:17:29 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

int	check_spaces_and_player(t_cub3d *cub, int row, int colum)
{
	if (check_spaces(cub, row, colum) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

int	angle_setting(t_cub3d *cub, int row, int colum)
{
	char	player;

	if (cub->player.x != 0 || cub->player.y != 0)
		return (exit_game(cub, ERROR, "Multiple Player"));
	player = cub->parse.map[row][colum];
	if (player == 'E')
		cub->player.r_angle = HALF_PI * 0;
	else if (player == 'S')
		cub->player.r_angle = HALF_PI * 1;
	else if (player == 'W')
		cub->player.r_angle = HALF_PI * 2;
	else if (player == 'N')
		cub->player.r_angle = HALF_PI * 3;
	cub->player.x = ((double)colum + 0.5) * SIZE_W;
	cub->player.y = ((double)row + 0.5) * SIZE_W;
	return (0);
}

int	checker2(t_cub3d *cub, int row, int colum, int i)
{
	if (ft_strchr("0", i) != NULL)
	{
		if (check_spaces_and_player(cub, row, colum) == ERROR)
			return (exit_game(cub, ERROR, "space next to 0!"));
	}
	return (0);
}

int	checker(t_cub3d *cub, int row, int colum)
{
	char	str;

	str = cub->parse.map[row][colum];
	if (ft_strchr("NSEW", str) != NULL)
	{
		if (check_spaces_and_player(cub, row, colum) == ERROR)
			return (ERROR);
		if (angle_setting(cub, row, colum) == ERROR)
			return (ERROR);
		cub->parse.d = TRUE;
	}
	else if (ft_strchr("0", str) != NULL)
	{
		if (checker2(cub, row, colum, str) == ERROR)
			return (ERROR);
	}
	return (SUCCESS);
}

int	check_map3(t_cub3d *cub)
{
	int	colum;
	int	row;

	row = -1;
	while (cub->parse.map[++row] != NULL)
	{
		colum = -1;
		while (cub->parse.map[row][++colum] != '\0')
		{
			if (checker(cub, row, colum) == ERROR)
				return (ERROR);
		}
	}
	if (cub->parse.d != TRUE)
		return (exit_game(cub, ERROR, "No player!"));
	return (0);
}
