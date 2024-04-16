/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khammadi <khammadi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 23:58:18 by khammadi          #+#    #+#             */
/*   Updated: 2024/02/28 02:50:03 by khammadi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d.h"

void	init_player(t_player *player)
{
	player->w_sped = W_SPEED;
	player->t_sped = T_SPEED;
}

int	key_press(int keycode, t_keyboard *key)
{
	if (keycode == KEY_S)
		key->down = 1;
	if (keycode == KEY_W)
		key->up = 1;
	if (keycode == KEY_A)
		key->left = 1;
	if (keycode == KEY_D)
		key->right = 1;
	if (keycode == KEY_RIGHT)
		key->right_rotat = 1;
	if (keycode == KEY_LEFT)
		key->left_rotat = 1;
	if (keycode == KEY_UP)
		key->updown_sight = -1;
	if (keycode == KEY_DOWN)
		key->updown_sight = 1;
	if (keycode == KEY_ESC)
		exit(0);
	return (0);
}

int	keyrel(int keycode, t_keyboard *key)
{
	if (keycode == KEY_S)
		key->down = 0;
	if (keycode == KEY_W)
		key->up = 0;
	if (keycode == KEY_A)
		key->left = 0;
	if (keycode == KEY_D)
		key->right = 0;
	if (keycode == KEY_RIGHT)
		key->right_rotat = 0;
	if (keycode == KEY_LEFT)
		key->left_rotat = 0;
	if (keycode == KEY_UP)
		key->updown_sight = 0;
	if (keycode == KEY_DOWN)
		key->updown_sight = 0;
	if (keycode == KEY_ESC)
		exit(0);
	return (0);
}

int	ft_exit(void)
{
	exit(0);
	return (0);
}

void	init_mlx_loop(t_cub3d *cub)
{
	init_player(&cub->player);
	mlx_hook(cub->win, 2, 1L << 0, &key_press, &cub->key);
	mlx_hook(cub->win, 3, 1L << 1, &keyrel, &cub->key);
	mlx_hook(cub->win, 17, 0, &ft_exit, cub);
	mlx_loop_hook(cub->mlx, game_loop, cub);
}
