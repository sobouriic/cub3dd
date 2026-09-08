/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coma_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 01:50:45 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/29 10:54:01 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

int	count_commas(const char *line)
{
	int	count;

	count = 0;
	while (*line)
	{
		if (*line == ',')
			count++;
		line++;
	}
	return (count);
}

char	**parse_rgb_values(const char *line)
{
	char	**palet;

	palet = ft_split(line, ',');
	return (palet);
}

int	color_parsing(int *x, char *line)
{
	char	**palet;
	int		count;
	int		result;

	if (!x || !line)
		return (ERROR);
	while (is_upper(*line) == TRUE)
		line++;
	while (is_space(*line) == TRUE)
		line++;
	if (count_commas(line) != 2)
		return (ERROR);
	palet = parse_rgb_values(line);
	if (!palet)
		return (ERROR);
	count = 0;
	while (palet[count])
		count++;
	result = ERROR;
	if (count == 3)
		result = check_rgb(x, palet);
	ft_free((void **)palet, count);
	return (result);
}

int	check_spaces(t_cub3d *cub, int row, int colum)
{
	int	dx[4] = {-1, 1, 0, 0};
	int	dy[4] = {0, 0, -1, 1};
	int	i;
	int	x;
	int	y;

	i = -1;
	while (++i < 4)
	{
		x = colum + dx[i];
		y = row + dy[i];
		if (y < 0 || y >= cub->parse.row || x < 0
			|| x >= (int)ft_strlen(cub->parse.map[y])
			|| cub->parse.map[y][x] == ' ')
			return (ERROR);
	}
	return (SUCCESS);
}

char	*map_update(char *str1, char *str2)
{
	char	*new;
	char	*result;

	if (!str1)
		str1 = ft_strdup("");
	new = ft_strjoin(str1, "\n");
	result = ft_strjoin(new, str2);
	arr_free(str1);
	arr_free(new);
	return (result);
}
