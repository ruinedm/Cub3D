/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:30 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/30 04:38:46 by mboukour         ###   ########.fr       */
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
	cube->map = NULL;
	cube->textures.ea_texture = NULL;
	cube->textures.no_texture = NULL;
	cube->textures.so_texture = NULL;
	cube->textures.we_texture = NULL;
	cube->initial = false;
	cube->player.x = 0;
	cube->player.y = 0;
	cube->player.angle = 0;
	cube->player.turn_direction = 0;
	cube->player.walk_direction = 0;
	cube->player.strafe_direction = 0;
	cube->player.movement_speed = 9;
	cube->player.rotation_speed = 9 * (M_PI / 180);
	cube->player_direction = NONE;
	cube->max_render_distance = sqrt((cube->width - 1) * (cube->width - 1)
			+ (cube->height - 1) * (cube->height - 1));
}




void    minimap(t_cub3d *cube)
{
    t_map    *map;
    int        x;
    int        y;

    map = cube->map;
    x = 0;
    y = 0;
    while (map)
    {
        x = 0;
        while (map->current_line[x])
        {
            draw_tile(cube, x, y, map->current_line[x]);
            x++;
        }
        y++;
        map = map->next;
    }
    draw_mini_player(cube);
	// raycast1(cube);
}

void    set_flo_ce(t_cub3d *cube)
{
    t_map    *map;
    int        x;
    int        y;

    map = cube->map;
    x = 0;
    y = 0;
    while (x < cube->width)
    {
        y = 0;
        while (y < cube->height)
        {
            if (y > cube->height / 2)
                mlx_put_pixel(cube->image, x, y, cube->floor);
            else
                mlx_put_pixel(cube->image, x, y, cube->ceiling);
            y++;
        }
        x++;
    }
}

void	render_map(t_cub3d *cube)
{
	t_map	*map;
	int		x;
	int		y;

	map = cube->map;
	x = 0;
	y = 0;
	set_flo_ce(cube);
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
	minimap(cube); // MINI_MAP ==> FOR DEBUGGING
}


void	loop_hook(mlx_key_data_t key_data, void *param)
{
	t_cub3d	*cube;
	int		step;
	int		strafe;
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
			cube->player.strafe_direction = -1;
		else if (key_data.key == MLX_KEY_D)
			cube->player.strafe_direction = 1;
		else if (key_data.key == MLX_KEY_LEFT)
			cube->player.turn_direction = -1;
		else if (key_data.key == MLX_KEY_RIGHT)
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
			cube->player.strafe_direction = 0;
		else if (key_data.key == MLX_KEY_LEFT || key_data.key == MLX_KEY_RIGHT)
			cube->player.turn_direction = 0;
		else
			return ;
	}
	strafe = cube->player.strafe_direction * cube->player.movement_speed;
	if(!strafe)
		step = cube->player.walk_direction * cube->player.movement_speed;
	else
		step = cube->player.walk_direction;
	new_x = cube->player.x + step * cos(cube->player.rotation_angle) + strafe * cos(cube->player.rotation_angle + M_PI_2);
	new_y = cube->player.y + step * sin(cube->player.rotation_angle) + strafe * sin(cube->player.rotation_angle + M_PI_2);
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

	if (ac != 2)
	{
		print_err("Invalid number of arguments");
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
