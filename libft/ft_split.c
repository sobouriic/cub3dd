/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/20 07:25:12 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/28 14:21:25 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_str_len(char const *s, char c)
{
	int	i;

	i = 0;
	while (s[i] == c)
		i++;
	while (s[i] && s[i] != c)
	{
		i++;
	}
	return (i);
}

static int	ft_str_count(char const	*s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (!s)
		return (0);
	while (s[i])
	{
		if (s[i] != c)
		{
			count++;
			while (s[i] != c && s[i])
				i++;
			while (s[i] == c && s[i])
				i++;
		}
		else
			i++;
	}
	return (count);
}

static char	**ft_free(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return (NULL);
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
	return (NULL);
}

char	**ft_split(const char *s, char c)
{
	char		**arr;
	int			i;
	int			len;
	int			wordl;

	len = ft_str_count(s, c);
	i = 0;
	arr = (char **)malloc((len + 1) * sizeof(char *));
	if (!arr)
		return (NULL);
	while (i < len)
	{
		while (*s == c)
			s++;
		wordl = ft_str_len(s, c);
		arr[i] = malloc((wordl + 1) * sizeof(char));
		if (!arr[i])
			return (ft_free(arr));
		ft_strlcpy(arr[i], s, wordl + 1);
		arr[i][wordl] = '\0';
		s += wordl;
		i++;
	}
	arr[i] = NULL;
	return (arr);
}
