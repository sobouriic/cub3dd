/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 23:26:09 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/29 11:16:11 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

char	*path_parsing(char *line)
{
	char	*tmp_line;

	if (!line)
		return (NULL);
	while (is_upper(*line) == TRUE)
		line++;
	while (is_space(*line) == TRUE)
		line++;
	tmp_line = ft_strdup(line);
	if (!tmp_line)
		return (NULL);
	return (tmp_line);
}

int	texture_path(t_parse *parse, int texture, char *line)
{
	line = trim_line(line);
	if (parse->texture[texture].path)
		return (arr_free_return(line, ERROR));
	parse->texture[texture].path = path_parsing(line);
	return (arr_free_return(line, SUCCESS));
}

int	sky_ground(t_parse *parse, int texture, char *line)
{
	line = trim_line(line);
	if ((is_color(texture, parse, line)))
		return (arr_free_return(line, ERROR));
	return (arr_free_return(line, SUCCESS));
}

t_line_size_node	*create_line_size_node(size_t size)
{
	t_line_size_node	*new_node;

	new_node = malloc(sizeof(t_line_size_node));
	if (new_node)
	{
		new_node->size = size;
		new_node->next = NULL;
	}
	return (new_node);
}

void	free_line_size_list(t_line_size_node *head)
{
	t_line_size_node	*current;
	t_line_size_node	*temp;

	current = head;
	while (current != NULL)
	{
		temp = current;
		current = current->next;
		free(temp);
	}
}
