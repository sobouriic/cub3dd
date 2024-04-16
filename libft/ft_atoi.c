/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/07 11:36:29 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/28 14:12:47 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *str)
{
	int					i;
	int					signe;
	unsigned long long	result;

	i = 0;
	result = 0;
	signe = 1;
	while ((str[i] == 32) || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			signe = signe * (-1);
		i++;
	}
	while (result < 9223372036854775807
		&& str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i++] - 48;
		if (result > 9223372036854775807 && signe == 1)
			return (-1);
		if (result >= 9223372036854775807 && signe == -1)
			return (0);
	}
	return (result * signe);
}
