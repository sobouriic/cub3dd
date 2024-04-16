/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_file_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:32:54 by khammadi          #+#    #+#             */
/*   Updated: 2024/02/29 09:15:53 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	create_new_line(t_line **p_line, t_line **c_line, int *count, int type)
{
	t_line	*new_line;

	new_line = malloc(sizeof(t_line));
	if (!new_line)
		return (ERROR);
	new_line->type = type;
	new_line->next = NULL;
	if (*p_line == NULL)
	{
		*p_line = new_line;
		*c_line = *p_line;
	}
	else
	{
		(*c_line)->next = new_line;
		*c_line = (*c_line)->next;
	}
	if (type == CIEL || type == FLOOR)
		(*count)++;
	return ((SUCCESS));
}

int	check_empty_line(t_do_parsing *parse)
{
	parse->parse = texture_type(parse->line);
	if (parse->line[0] == '\0')
		parse->parse = 7;
	return (SUCCESS);
}

int	update_parse_and_free_line(t_do_parsing *parse)
{
	parse->parse = texture_type(parse->line);
	if (parse->parse == ERR)
	{
		free(parse->line);
		return (ERROR);
	}
	return (SUCCESS);
}

void	init_variable(t_norm_init *var, t_do_parsing *parse)
{
	var->count = 0;
	var->parsed_lines = NULL;
	var->current_line = NULL;
	parse->get_value = get_next_line(parse->fd, &parse->line);
}

int	parse_lines(t_cub3d *cub, t_do_parsing *parse)
{
	t_norm_init	var;

	init_variable(&var, parse);
	while (parse->get_value > 0)
	{
		check_empty_line(parse);
		if (create_new_line(&var.parsed_lines,
				&var.current_line, &var.count, parse->parse) == ERROR)
			return (ERROR);
		if (parse->parse == ERR)
			free(parse->line);
		else if (parse->parse == ERR && empty_line(parse->line))
			return (exit_game(cub, ERROR, "wrong parsing"));
		else if (check_map(&cub->parse, parse->get_value,
				parse->parse, parse->line) == ERROR)
			return (ERROR);
		parse->get_value = get_next_line(parse->fd, &parse->line);
	}
	if (update_parse_and_free_line(parse) == ERROR)
		return (ERROR);
	check_map(&cub->parse, parse->get_value, parse->parse, parse->line);
	if (var.count != 2)
		return (ERROR);
	process_parsed_lines(var.parsed_lines);
	return (SUCCESS);
}
