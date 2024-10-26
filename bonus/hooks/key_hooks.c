/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   key_hooks.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/31 07:31:13 by mboukour          #+#    #+#             */
/*   Updated: 2024/10/26 17:17:20 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

bool	handle_key_press(int key, t_cub3d *cube)
{
	if (key == MLX_KEY_W)
		cube->player.walk_direction = 1;
	else if (key == MLX_KEY_S)
		cube->player.walk_direction = -1;
	else if (key == MLX_KEY_A)
		cube->player.strafe_direction = -1;
	else if (key == MLX_KEY_D)
		cube->player.strafe_direction = 1;
	else if (key == MLX_KEY_LEFT)
		cube->player.turn_direction = -1;
	else if (key == MLX_KEY_RIGHT)
		cube->player.turn_direction = 1;
	else if (key == MLX_KEY_O)
		toggle_door(cube);
	else if (key == MLX_KEY_ESCAPE)
		exit(0);
	else
		return (false);
	return (true);
}

bool	handle_key_release(int key, t_cub3d *cube)
{
	if (key == MLX_KEY_W || key == MLX_KEY_S)
		cube->player.walk_direction = 0;
	else if (key == MLX_KEY_A || key == MLX_KEY_D)
		cube->player.strafe_direction = 0;
	else if (key == MLX_KEY_LEFT || key == MLX_KEY_RIGHT)
		cube->player.turn_direction = 0;
	else
		return (false);
	return (true);
}

void	player_movement(t_cub3d *cube)
{
	t_move	move;

	move.strafe = cube->player.strafe_direction * 3;
	move.strafe_angle = cube->player.rotation_angle + M_PI_2;
	if (!move.strafe)
		move.step = cube->player.walk_direction * cube->player.movement_speed;
	else
		move.step = cube->player.walk_direction;
	cube->player.rotation_angle = normalize_angle(cube->player.rotation_angle);
	move.new_x = roundf(cube->player.x + move.step * cos(cube->player.rotation_angle)
		+ move.strafe * cos(move.strafe_angle));
	move.new_y = roundf(cube->player.y + move.step * sin(cube->player.rotation_angle)
		+ move.strafe * sin(move.strafe_angle));
	move_player(cube, move);
	cube->player.rotation_angle += cube->player.turn_direction
		* cube->player.rotation_speed;
}

void	key_hook(mlx_key_data_t key_data, void *param)
{
	t_cub3d	*cube;

	cube = (t_cub3d *)param;
	if ((key_data.action == MLX_PRESS || key_data.action == MLX_REPEAT)
		&& !handle_key_press(key_data.key, cube))
		return ;
	else if (key_data.action == MLX_RELEASE
		&& !handle_key_release(key_data.key, cube))
		return ;
}
