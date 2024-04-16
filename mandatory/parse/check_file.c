/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/18 20:46:02 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/29 10:54:01 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	check_ext(const char *file)
{
	int	i;

	i = ft_strlen(file) - 4;
	return (ft_strncmp(file + i, ".cub", 4));
}

int	empty_line(char *line)
{
	int	i;

	i = 0;
	if (line[0] == '\0')
		return (1);
	i = 0;
	while (line[i])
	{
		if (!is_space(line[i]))
			return (FALSE);
		i++;
	}
	return (TRUE);
}

int	open_file(t_cub3d *cub, const char *file)
{
	t_do_parsing	pa;

	pa.fd = open(file, O_RDONLY);
	if (pa.fd < 0)
	{
		exit_game(cub, ERROR, "cannot open file");
		return (-1);
	}
	return (pa.fd);
}

char	*trim_line(char *line)
{
	char	*trimmed;

	trimmed = ft_strtrim(line, " ");
	free(line);
	return (trimmed);
}

int	process_parsed_lines(t_line *parsed_lines)
{
	t_line	*current_line;
	int		found_012345;
	int		found_6_after_012345;

	current_line = parsed_lines;
	found_012345 = 0;
	found_6_after_012345 = 0;
	while (current_line != NULL)
	{
		if (current_line->type >= 0 && current_line->type <= 5)
		{
			if (found_6_after_012345)
				ft_error("wrong parse\n");
			found_012345 = 1;
		}
		else if (current_line->type == 6 && found_012345)
		{
			found_6_after_012345 = 1;
		}
		current_line = current_line->next;
	}
	free_parsed_lines(parsed_lines);
	return (1);
}
