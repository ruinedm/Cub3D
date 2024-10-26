/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:30 by mboukour          #+#    #+#             */
/*   Updated: 2024/10/26 20:29:33 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	initialize_mlx(t_cub3d *cube)
{
	if (cube->player_direction == PLAYER_S)
		cube->player.rotation_angle = M_PI_2;
	else if (cube->player_direction == PLAYER_N)
		cube->player.rotation_angle = 3 * M_PI / 2;
	else if (cube->player_direction == PLAYER_W)
		cube->player.rotation_angle = M_PI;
	else if (cube->player_direction == PLAYER_E)
		cube->player.rotation_angle = 0;
	cube->mlx = mlx_init(WIDTH, HEIGHT, "Ruined CUB3D\n", false);
	if (!cube->mlx)
	{
		print_err("Mlx init error\n");
		exit(EXIT_FAILURE);
	}
	cube->image = mlx_new_image(cube->mlx, WIDTH, HEIGHT);
	if (!cube->image)
	{
		print_err("Mlx new image error\n");
		mlx_terminate(cube->mlx);
		exit(EXIT_FAILURE);
	}
	mlx_image_to_window(cube->mlx, cube->image, 0, 0);
}

void	initialize_cube(t_cub3d *cube)
{
	cube->map_fd = NONE;
	cube->ceiling_b = NONE;
	cube->ceiling_r = NONE;
	cube->ceiling_g = NONE;
	cube->floor_b = NONE;
	cube->floor_r = NONE;
	cube->floor_b = NONE;
	cube->map_array = NULL;
	cube->map = NULL;
	cube->textures.ea_texture = NULL;
	cube->textures.no_texture = NULL;
	cube->textures.so_texture = NULL;
	cube->textures.we_texture = NULL;
	cube->fov_angle = 60 * (M_PI / 180);
	cube->player.angle = 0;
	cube->player.turn_direction = 0;
	cube->player.walk_direction = 0;
	cube->player.strafe_direction = 0;
	cube->player.movement_speed = 7;
	cube->player.rotation_speed = 4 * (M_PI / 180);
	cube->player.mouse_rotation_speed = 7 * (M_PI / 180);
	cube->player_direction = NONE;
	cube->center_factor = 2;
}

void	render_map(t_cub3d *cube)
{
	set_flo_ce(cube);
	ray_casting(cube);
}

void	set_player_position(t_cub3d *cube)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (cube->map_array[y])
	{
		x = 0;
		while (cube->map_array[y][x])
		{
			if (is_a_player(cube->map_array[y][x]))
			{
				cube->player.x = x * TILE_SIZE + TILE_SIZE / 2;
				cube->player.y = y * TILE_SIZE + TILE_SIZE / 2;
			}
			x++;
		}
		y++;
	}
}

int	main(int ac, char **av)
{
	t_cub3d	cube;

	if (ac != 2)
	{
		print_err("Invalid number of arguments");
		return (1);
	}
	initialize_cube(&cube);
	if (!parser(&cube, av[1]))
		return (1);
	initialize_mlx(&cube);
	set_player_position(&cube);
	render_map(&cube);
	mlx_loop_hook(cube.mlx, player_move_hook, &cube);
	mlx_key_hook(cube.mlx, key_hook, &cube);
	mlx_close_hook(cube.mlx, close_hook, &cube);
	mlx_loop(cube.mlx);
}
