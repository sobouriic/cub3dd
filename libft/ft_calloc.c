/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/21 13:35:30 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/28 14:19:17 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t count, size_t size)
{
	char	*str;
	size_t	i;

	i = 0;
	if (count != 0 && size > SIZE_MAX / count)
		return (NULL);
	str = malloc(count * size);
	if (str == NULL)
		return (NULL);
	while (i < (count * size))
	{
		str[i] = 0;
		i++;
	}
	return (str);
}
