/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/12 15:16:42 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/29 10:53:43 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static	int	ft_find(char *s, int c )
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == c)
			return (1);
		i++;
	}
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	char	*str;
	size_t	s;
	size_t	e;
	size_t	i;
	char	*ss1;

	ss1 = (char *)s1;
	s = 0;
	if (!s1 || !set)
		return (NULL);
	while (ss1[s] != '\0' && ft_find((char *)set, ss1[s]))
		s++;
	e = ft_strlen(ss1);
	while (ss1[e - 1] && ft_find((char *) set, ss1[e - 1]) && e > s)
		e--;
	str = (char *)malloc(sizeof(char) * e - s + 1);
	if (str == NULL)
		return (NULL);
	i = 0;
	while (e > s)
	{
		str[i++] = ss1[s++];
	}
	str[i] = '\0';
	return (str);
}
