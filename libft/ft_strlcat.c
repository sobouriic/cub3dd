/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 08:57:05 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/29 10:53:43 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	l;
	size_t	y;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	l = ft_strlen(src);
	if (dstsize == 0 && !dst)
		return (l);
	while (dst[j] != '\0')
		j++;
	y = ft_strlen(dst);
	if (dstsize == 0 || dstsize <= y)
		return (dstsize + l);
	while (src[i] != '\0' && i < dstsize - y - 1)
	{
		dst[j] = src[i];
		j++;
		i++;
	}
	dst[j] = '\0';
	return (y + l);
}
