/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:29:14 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/29 11:15:57 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

static int	ft_srch(const char *str, char c)
{
	while (*str != '\0')
	{
		if (*str == c)
			return (TRUE);
		str++;
	}
	return (FALSE);
}

int	check_map_char(char *str)
{
	int	i;
	int	found_non_space;

	i = 0;
	found_non_space = FALSE;
	while (str[i])
	{
		if (!found_non_space && str[i] == ' ')
		{
			i++;
			continue ;
		}
		found_non_space = TRUE;
		if (ft_srch("01NSEW ", str[i]) == FALSE)
		{
			ft_error("Wrong char in map\n");
			return (FALSE);
		}
		i++;
	}
	return (TRUE);
}

int	texture_type(char *line)
{
	int	i;

	i = 0;
	if (!line)
		return (0);
	while (line[i] == ' ')
		i++;
	if (line[i] == 'N' && line[i + 1] == 'O' && line[i + 2] == ' ')
		return (NORTH);
	else if (line[i] == 'S' && line[i + 1] == 'O' && line[i + 2] == ' ')
		return (SOUTH);
	else if (line[i] == 'W' && line[i + 1] == 'E' && line[i + 2] == ' ')
		return (WEST);
	else if (line[i] == 'E' && line[i + 1] == 'A' && line[i + 2] == ' ')
		return (EAST);
	else if (line[i] == 'F' && line[i + 1] == ' ')
		return (FLOOR);
	else if (line[i] == 'C' && line[i + 1] == ' ')
		return (CIEL);
	else if (check_map_char(line + i))
		return (MAP);
	return (ERR);
}

int	final_map(t_parse *parse)
{
	size_t	column;
	int		row;
	char	**temp_map;

	temp_map = ft_split(parse->temp_map, '\n');
	if (temp_map == NULL)
		return (arr_free_return(parse->temp_map, SUCCESS));
	column = 0;
	row = -1;
	while (temp_map[++row] != NULL)
	{
		if (ft_strlen(temp_map[row]) > column)
			column = ft_strlen(temp_map[row]);
	}
	arr_free(parse->map);
	parse->map = temp_map;
	parse->row = row;
	parse->column = column;
	return (arr_free_return(parse->temp_map, SUCCESS));
}

int	check_map(t_parse *parse, int get_value, int texture, char *line)
{
	if (texture >= NORTH && texture <= EAST)
		return (texture_path(parse, texture, line));
	else if (texture == CIEL || texture == FLOOR)
		return (sky_ground(parse, texture, line));
	else
		return (handle_parsing(parse, get_value, line));
}
