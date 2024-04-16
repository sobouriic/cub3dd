/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 16:50:25 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/29 10:54:01 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	texture_check(t_cub3d *cub)
{
	int	i;
	int	j;

	i = 0;
	while (i <= WEST)
	{
		j = i;
		while (++j <= EAST)
		{
			if (ft_strncmp(cub->parse.texture[i].path,
					cub->parse.texture[j].path,
					ft_strlen(cub->parse.texture[i].path) == 0))
				return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

int	check_colors_and_textures(t_cub3d *cub)
{
	int	i;

	if ((cub->parse.ceiling == COLOR) || (cub->parse.floor == COLOR))
		return (exit_game(cub, ERROR, "parse Ceiling and Floor"));
	if (texture_check(cub) == ERROR)
		return (exit_game(cub, ERROR, "Texture's name should be diffrent"));
	i = 0;
	while (i < COUNT)
	{
		if (cub->parse.texture[i].path == NULL)
			return (exit_game(cub, ERROR, "Textures needed"));
		++i;
	}
	return (SUCCESS);
}

int	check_map_dimensions(t_cub3d *cub)
{
	if (!(cub->parse.row) || !(cub->parse.column))
		return (exit_game(cub, ERROR, "Error in map"));
	return (SUCCESS);
}

int	check_map2(t_cub3d *cub)
{
	if (check_map_dimensions(cub) == ERROR)
		return (ERROR);
	if (check_colors_and_textures(cub) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

int	final_parsing(t_cub3d *cub)
{
	if (check_map2(cub) == ERROR)
		return (exit_game(cub, ERROR, "wrong Texture type"));
	if (check_map3(cub) == ERROR)
		return (exit_game(cub, ERROR, "wrong Map"));
	return (SUCCESS);
}
