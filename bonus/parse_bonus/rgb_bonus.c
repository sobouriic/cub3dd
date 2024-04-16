/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rgb_bonus.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 15:01:54 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/28 12:50:21 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

int	check_rgb(int *x, char **palet)
{
	int	i;
	int	j;
	int	clr;

	i = -1;
	while (++i < 3)
	{
		if (palet[i] == NULL)
			return (ERROR);
		*x = *x * 256;
		j = -1;
		clr = 0;
		palet[i] = trim_line(palet[i]);
		while (palet[i][++j] != '\0')
		{
			if (ft_isdigit(palet[i][j]) == FALSE)
				return (ERROR);
			clr = clr * 10 + palet[i][j] - '0';
		}
		if (ft_atoi(palet[i]) > 255 || ft_atoi(palet[i]) < 0)
			return (ERROR);
		*x = *x + clr;
	}
	return (0);
}

int	is_color(int texture, t_parse *parse, char *line)
{
	if (texture == FLOOR)
	{
		if (parse->f == TRUE || color_parsing(&parse->floor, line) == ERROR
			|| parse->flag == 1)
			return (ERROR);
		parse->f = TRUE;
	}
	else if (texture == CIEL)
	{
		if (parse->c == TRUE || color_parsing(&parse->ceiling, line) == ERROR
			|| parse->flag == 1)
		{
			return (ERROR);
			parse->c = TRUE;
		}
	}
	return (0);
}

int	check_edge_direction(t_cub3d *cub, t_init_edge param)
{
	return ((ft_strchr("0NSEW",
				cub->map.map[param.y1 - param.y][param.x1]) == NULL)
		&& (ft_strchr("0NSEW",
		cub->map.map[param.y1][param.x1 - param.x]) == NULL));
}

int	check_edge(t_cub3d *cub, const t_point *first, const t_point *last)
{
	t_init_edge	param;

	param.x = (int)(first->x / SIZE_W) - (int)(last->x / SIZE_W);
	param.y = (int)(first->y / SIZE_W) - (int)(last->y / SIZE_W);
	param.x1 = (int)(first->x / SIZE_W);
	param.y1 = (int)(first->y / SIZE_W);
	if (param.x == 1 && param.y == 1)
		return (check_edge_direction(cub, param));
	if (param.x == 1 && param.y == -1)
		return (check_edge_direction(cub, param));
	if (param.x == -1 && param.y == 1)
		return (check_edge_direction(cub, param));
	if (param.x == -1 && param.y == -1)
		return (check_edge_direction(cub, param));
	return (FALSE);
}

void	init_f_c(t_cub3d *cub)
{
	cub->parse.floor = COLOR;
	cub->parse.ceiling = COLOR;
}
