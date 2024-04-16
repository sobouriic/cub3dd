/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/17 12:08:46 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/28 15:13:46 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	ft_free_str(char *str)
{
	free(str);
	str = NULL;
}

void	arr_free(void *arr)
{
	if (arr != NULL)
	{
		free(arr);
		arr = NULL;
	}
}

int	arr_free_return(void *arr, int value)
{
	arr_free(arr);
	return (value);
}

void	text_free(t_cub3d *cub)
{
	int	i;

	i = 0;
	while (i < COUNT)
	{
		arr_free(cub->parse.texture[i].texture);
		i++;
	}
}

void	ft_free(void **arr, int i)
{
	if (arr == NULL)
		return ;
	while (--i >= 0)
		arr_free(arr[i]);
	arr_free(arr);
	arr = NULL;
}
