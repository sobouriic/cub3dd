/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_mlx_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sobouric <sobouric@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/28 15:15:34 by sobouric          #+#    #+#             */
/*   Updated: 2024/02/28 16:03:29 by sobouric         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3d_bonus.h"

void	init_player(t_player *player)
{
	player->point = PLAYER_POINT;
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

int	game_loop(t_cub3d *cub)
{
	main_update(cub);
	update_3d_value(cub);
	update_multiple_rays(cub);
	map_rendering(cub);
	render_ray_mini(cub);
	walls_3d_projection_render(cub);
	player_render(cub);
	mlx_put_image_to_window(cub->mlx, cub->win, cub->img.ptr, 0, 0);
	return (0);
}
