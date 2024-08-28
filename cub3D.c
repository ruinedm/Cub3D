/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:30 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/29 00:54:49 by aboukdid         ###   ########.fr       */
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
	cube->mlx = mlx_init(cube->width, cube->height, "Ruined CUB3D\n", false);
	if (!cube->mlx)
		exit(EXIT_FAILURE);
	cube->image = mlx_new_image(cube->mlx, cube->width, cube->height);
	if (!cube->image)
		exit(EXIT_FAILURE);
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
	cube->ea_path = NULL;
	cube->map = NULL;
	cube->no_path = NULL;
	cube->so_path = NULL;
	cube->we_path = NULL;
	cube->initial = false;
	cube->player.x = 0;
	cube->player.y = 0;
	cube->player.angle = 0;
	cube->player.turn_direction = 0;
	cube->player.walk_direction = 0;
	cube->player.movement_speed = 9;
	cube->player.rotation_speed = 9 * (M_PI / 180);
	cube->player_direction = NONE;
	cube->strip_color = RED;
	cube->max_render_distance = sqrt((cube->width - 1) * (cube->width - 1)
			+ (cube->height - 1) * (cube->height - 1));
}

void	render_map(t_cub3d *cube)
{
	t_map	*map;
	int		x;
	int		y;
	int		color;

	map = cube->map;
	x = 0;
	y = 0;
	color = create_trgb(0, cube->ceiling_r, cube->ceiling_g, cube->ceiling_b);
	while (y < cube->height)
	{
		if (y > cube->height / 2)
			color = cube->floor_r << 16 | cube->floor_g << 8 | cube->floor_b;
		x = 0;
		while (x < cube->width)
		{
			mlx_put_pixel(cube->image, x, y, color);
			x++;
		}
		y++;
	}
	x = 0;
	y = 0;
	while (map)
	{
		x = 0;
		while (map->current_line[x])
		{
			if (!cube->initial && is_a_player(map->current_line[x]))
			{
				cube->player.x = x * TILE_SIZE + TILE_SIZE / 2;
				cube->player.y = y * TILE_SIZE + TILE_SIZE / 2;
				cube->initial = true;
			}
			x++;
		}
		y++;
		map = map->next;
	}
	ray_casting(cube);
}

void	loop_hook(mlx_key_data_t key_data, void *param)
{
	t_cub3d	*cube;
	int		step;
	int		new_x;
	int		new_y;

	cube = (t_cub3d *)param;
	if (key_data.action == MLX_PRESS || key_data.action == MLX_REPEAT)
	{
		if (key_data.key == MLX_KEY_W)
			cube->player.walk_direction = 1;
		else if (key_data.key == MLX_KEY_S)
			cube->player.walk_direction = -1;
		else if (key_data.key == MLX_KEY_A)
			cube->player.turn_direction = -1;
		else if (key_data.key == MLX_KEY_D)
			cube->player.turn_direction = 1;
		else if (key_data.key == MLX_KEY_ESCAPE)
			exit(0);
		else
			return ;
	}
	else if (key_data.action == MLX_RELEASE)
	{
		if (key_data.key == MLX_KEY_W || key_data.key == MLX_KEY_S)
			cube->player.walk_direction = 0;
		else if (key_data.key == MLX_KEY_A || key_data.key == MLX_KEY_D)
			cube->player.turn_direction = 0;
		else
			return ;
	}
	step = cube->player.walk_direction * cube->player.movement_speed;
	new_x = cube->player.x + step * cos(cube->player.rotation_angle);
	new_y = cube->player.y + step * sin(cube->player.rotation_angle);
	if (!collides_with_wall(cube, new_x, new_y))
	{
		cube->player.x = new_x;
		cube->player.y = new_y;
	}
	cube->player.rotation_angle += cube->player.turn_direction
		* cube->player.rotation_speed;
	render_map(cube);
}

int	main(int ac, char **av)
{
	t_cub3d	cube;

	(void)av;
	if (ac != 2)
	{
		prin_err("Invalid number of arguments");
		return (1);
	}
	initialize_cube(&cube);
	if (!parser(&cube, av[1]))
		return (1);
	initialize_mlx(&cube);
	render_map(&cube);
	mlx_key_hook(cube.mlx, loop_hook, &cube);
	mlx_loop(cube.mlx);
}
