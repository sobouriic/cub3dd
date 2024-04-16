/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coma_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 01:50:45 by khammadi          #+#    #+#             */
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
	int		count;
	char	**palet;

	*x = 0;
	count = 0;
	while (is_upper(*line) == TRUE)
		line++;
	while (is_space(*line) == TRUE)
		line++;
	if (count_commas(line) != 2)
		return (ERROR);
	palet = parse_rgb_values(line);
	if (check_rgb(x, palet) == ERROR)
	{
		ft_free((void **)palet, 3);
		return (ERROR);
	}
	ft_free((void **)palet, 3);
	return (0);
}

int	check_spaces(t_cub3d *cub, int row, int colum)
{
	if (row < 0 || row >= cub->parse.row
		|| colum < 0 || colum >= cub->parse.column)
		return (ERROR);
	if ((colum + 1 < cub->parse.column
			&& cub->parse.map[row][colum + 1] == ' ') ||
		(cub->parse.map[row][colum + 1] == '\0') ||
		(colum - 1 >= 0 && cub->parse.map[row][colum - 1] == ' ') ||
		(row + 1 < cub->parse.row && cub->parse.map[row + 1][colum] == ' ') ||
		(cub->parse.map[row + 1][colum] == '\0') ||
		(row - 1 >= 0 && cub->parse.map[row - 1][colum] == ' ') ||
		(colum >= (int)ft_strlen(cub->parse.map[row - 1])) ||
		(colum >= (int)ft_strlen(cub->parse.map[row + 1])))
	{
		if ((colum + 1 < cub->parse.column
				&& cub->parse.map[row][colum + 1] != ' ') ||
			(colum - 1 >= 0 && cub->parse.map[row][colum - 1] != ' ') ||
			(row - 1 >= 0 && cub->parse.map[row - 1][colum] != ' ') ||
			(row + 1 < cub->parse.row && cub->parse.map[row + 1][colum] != ' '))
			return (ERROR);
		return (SUCCESS);
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
