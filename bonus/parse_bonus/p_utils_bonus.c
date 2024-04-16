/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   p_utils_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 09:00:37 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/29 10:54:01 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

int	handle_parsing(t_parse *parse, int get_value, char *line)
{
	if (!is_space(line[0]) && ft_isdigit(line[0]) == FALSE && line[0] != '\0')
		return (ERROR);
	if (add_line_size_node(&(parse->pa.line_sizes), ft_strlen(line)) == ERROR)
	{
		free(line);
		return (ERROR);
	}
	parse->temp_map = map_update(parse->temp_map, line);
	if (get_value == 0 && final_map(parse) == ERROR)
	{
		free(line);
		return (ERROR);
	}
	free(line);
	return (SUCCESS);
}

int	parsing(t_cub3d *cub, const char *file)
{
	t_do_parsing		pa;
	t_line_size_node	*current;

	pa.fd = open_file(cub, file);
	if (pa.fd < 0)
		return (ERROR);
	if (parse_lines(cub, &pa) == ERROR)
		return (ERROR);
	current = cub->parse.pa.line_sizes;
	while (current != NULL)
	{
		if (current->size != 0 && current->next != NULL
			&& current->next->size == 0)
			return (ERROR);
		current = current->next;
	}
	close(pa.fd);
	return (SUCCESS);
}

int	add_line_size_node(t_line_size_node **head, size_t size)
{
	t_line_size_node	*new_node;
	t_line_size_node	*current;

	new_node = create_line_size_node(size);
	if (!new_node)
		return (ERROR);
	if (*head == NULL)
	{
		*head = new_node;
	}
	else
	{
		current = *head;
		while (current->next != NULL)
		{
			current = current->next;
		}
		current->next = new_node;
	}
	return (SUCCESS);
}

void	free_parsed_lines(t_line *parsed_lines)
{
	t_line	*current_line;
	t_line	*next_line;	

	current_line = parsed_lines;
	while (current_line != NULL)
	{
		next_line = current_line->next;
		free(current_line);
		current_line = next_line;
	}
}
