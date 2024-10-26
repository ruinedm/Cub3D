/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_hooks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 12:33:34 by mboukour          #+#    #+#             */
/*   Updated: 2024/10/23 21:40:28 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

void	mouse_hook(enum mouse_key key, enum action action
	, enum modifier_key mod_ke, void *param)
{
	t_cub3d	*cube;

	(void)key;
	(void)action;
	(void)mod_ke;
	cube = param;
	mlx_get_mouse_pos(cube->mlx, &cube->mouse_x, &cube->mouse_y);
}

void	mouse_pos_hook(void *param)
{
	int		x;
	int		y;
	t_cub3d	*cube;

	cube = param;
	mlx_get_mouse_pos(cube->mlx, &x, &y);
	if (x == cube->mouse_x)
	{
		player_movement(cube);
		render_map(cube);
		return ;
	}
	if (x < cube->mouse_x)
		cube->player.rotation_angle -= cube->player.mouse_rotation_speed;
	else if (x > cube->mouse_x)
		cube->player.rotation_angle += cube->player.mouse_rotation_speed;
	cube->mouse_x = x;
	player_movement(cube);
	render_map(cube);
}
