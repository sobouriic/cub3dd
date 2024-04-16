/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 02:07:25 by khammadi          #+#    #+#             */
/*   Updated: 2024/02/29 15:05:29 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

int	check_map_edges(t_cub3d *cub)
{
	int	j;
	int	y;

	j = -1;
	y = 0;
	if (cub->parse.map[0] == NULL)
		return (exit_game(cub, ERROR, "Parse Fail"));
	while (cub->parse.map[y])
		y++;
	y -= 1;
	j = -1;
	while (cub->parse.map[y][++j])
		if (cub->parse.map[y][j] != '1' && cub->parse.map[y][j] != ' ')
			return (exit_game(cub, ERROR, "Parse Fail"));
	j = -1;
	while (cub->parse.map[0][++j])
		if (cub->parse.map[0][j] != '1' && cub->parse.map[0][j] != ' ')
			return (exit_game(cub, ERROR, "Parse Fail"));
	j = -1;
	while (cub->parse.map[++j])
		if (cub->parse.map[j][0] != '1' && cub->parse.map[j][0] != ' ')
			return (exit_game(cub, ERROR, "Parse Fail"));
	return (0);
}

int	parse_arguments( int ac, char **av)
{
	if (ac != 2)
		ft_error("Wrong Argument\n");
	if (check_ext(av[1]))
		ft_error("Wrong file Path\n");
	return (0);
}

int	main(int ac, char **av)
{
	t_cub3d	*cub;

	cub = ft_calloc(1, sizeof(t_cub3d));
	if (parse_arguments(ac, av) == ERROR)
		return (exit_game(cub, ERROR, "Wrong parse"));
	init_f_c(cub);
	if (parsing(cub, av[1]) == ERROR)
		return (exit_game(cub, ERROR, "Wrong parse"));
	if (check_map_edges(cub) == ERROR)
		return (exit_game(cub, ERROR, "Parse Fail"));
	if (final_parsing(cub) == ERROR)
		return (exit_game(cub, ERROR, "Parse Fail"));
	main_init(cub);
	init_mlx(cub);
	apload_texture(cub);
	init_mlx_loop(cub);
	mlx_loop(cub->mlx);
	text_free(cub);
	return (0);
}
