/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 10:56:05 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/29 10:54:01 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

static char	*ft_strndup(const char *str, ssize_t n)
{
	char	*result;
	ssize_t	i;

	i = 0;
	result = (char *)malloc(sizeof(char) * (n + 1));
	if (!result)
		return (0);
	while (*str != '\0' && i < n)
	{
		*result++ = *str++;
		i++;
	}
	*result = '\0';
	return (result - i);
}

static ssize_t	newline_check(char *str)
{
	ssize_t	i;

	i = 0;
	while (str != NULL && *str != '\0')
	{
		if (*str == '\n')
			return (i);
		str++;
		i++;
	}
	return (-1);
}

static ssize_t	ft_line(char **line, char **str, ssize_t i)
{
	char	*temp;
	char	*n_line;

	if (i >= 0)
	{
		*line = ft_strndup(*str, i);
		n_line = *str + i + 1;
		temp = ft_strndup(n_line, ft_strlen(n_line));
		ft_free_str(*str);
		*str = temp;
		return (1);
	}
	if (*str == NULL)
		*line = (ft_strndup("", 0));
	else
	{
		*line = *str;
		*str = NULL;
	}
	return (0);
}

void	get_uils(char **str, t_get_line *line, int fd)
{
	while (line->i == -1 && line->ofset > 0)
	{
		line->buff[line->ofset] = '\0';
		if (*str == NULL)
			line->tmp = ft_strndup(line->buff, line->ofset);
		else
			line->tmp = ft_strjoin(*str, line->buff);
		ft_free_str(*str);
		*str = line->tmp;
		line->i = newline_check(*str);
		if (line->i == -1)
			line->ofset = read(fd, line->buff, 1);
	}
}

int	get_next_line(int fd, char **line)
{
	t_get_line	gnl;
	static char	*str[10240];

	if (fd < 0 || fd > 10240 || line == NULL)
		return (-1);
	gnl.buff = (char *)malloc(sizeof(char) * (2));
	if (gnl.buff == NULL)
		return (-1);
	gnl.i = -1;
	gnl.ofset = read(fd, gnl.buff, 1);
	get_uils(&str[fd], &gnl, fd);
	ft_free_str(gnl.buff);
	if (gnl.ofset < 0)
		return (-1);
	return (ft_line(line, &str[fd], gnl.i));
}
