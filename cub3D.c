/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:30 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/26 15:36:44 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"
#include <limits.h>
void	initialize_mlx(t_cub3d *cube)
{
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
	cube->player.rotation_angle = M_PI / 2;
	cube->player.movement_speed = 9;
	cube->player.rotation_speed = 9 * (M_PI / 180);
}

void	draw_line(int start_x, int start_y, int end_x, int end_y, void *img)
{
	t_line	line;
	int		i;

	i = 0;
	line.dx = end_x - start_x;
	line.dy = end_y - start_y;
	if (fabs(line.dx) > fabs(line.dy))
		line.steps = fabs(line.dx);
	else
		line.steps = fabs(line.dy);
	line.x_inc = line.dx / line.steps;
	line.y_inc = line.dy / line.steps;
	line.x = start_x;
	line.y = start_y;
	while (i <= line.steps)
	{
		mlx_put_pixel(img, (int)line.x, (int)line.y, RED);
		line.x += line.x_inc;
		line.y += line.y_inc;
		i++;
	}
}

void	draw_circle(t_cub3d *cube)
{
	t_circle	cir;

	cir.cx = cube->player.x;
	cir.cy = cube->player.y;
	cir.r = 5;
	cir.y = cir.cy - cir.r;
	cir.r_squared = cir.r * cir.r;
	while (cir.y < cir.cy + cir.r)
	{
		cir.x = cir.cx - cir.r;
		while (cir.x < cir.cx + cir.r)
		{
			if ((cir.x - cir.cx) * (cir.x - cir.cx) + (cir.y - cir.cy)
				* (cir.y - cir.cy) < cir.r_squared)
				mlx_put_pixel(cube->image, cir.x, cir.y, RED);
			cir.x++;
		}
		cir.y++;
	}
	cir.final_x = cir.cx + cos(cube->player.rotation_angle) * 30;
	cir.final_y = cir.cy + sin(cube->player.rotation_angle) * 30;
	draw_line(cir.cx, cir.cy, cir.final_x, cir.final_y, cube->image);
}

bool	is_a_player(int mode)
{
	return (mode == PLAYER_E || mode == PLAYER_W
		|| mode == PLAYER_N || mode == PLAYER_S);
}

void	draw_tile(t_cub3d *cube, int x, int y, int mode)
{
	int	i;
	int	j;
	int	scaled_x;
	int	scaled_y;
	int	color;

	scaled_x = x * TILE_SIZE;
	scaled_y = y * TILE_SIZE;
	color = WHITE;
	if (mode == WALL)
		color = BLACK;
	i = 0;
	while (i < TILE_SIZE)
	{
		j = 0;
		while (j < TILE_SIZE)
		{
			if (color == (int)WHITE && (!i || !j))
				mlx_put_pixel(cube->image, scaled_x + i, scaled_y + j, BLACK);
			else
				mlx_put_pixel(cube->image, scaled_x + i, scaled_y + j, color);
			j++;
		}
		i++;
	}
}

bool	collides_with_wall(t_cub3d *cube, int new_x, int new_y)
{
	int		tile_x;
	int		tile_y;
	t_map	*map;
	int		i;

	tile_x = new_x / TILE_SIZE;
	tile_y = new_y / TILE_SIZE;
	map = cube->map;
	i = 0;
	while (map && i < tile_y)
	{
		i++;
		map = map->next;
	}
	if (map->current_line[tile_x] == '1')
		return (true);
	return (false);
}

double normalize_angle(double angle)
{
    // Keep the angle in the range of 0 to 2 * M_PI
    angle = fmod(angle, 2 * M_PI);
    if (angle < 0)
    {
        angle += 2 * M_PI;
    }
    return angle;
}

void	render_ray(t_cub3d *cube, double ray_angle)
{
	ray_angle = normalize_angle(ray_angle);
	double 	x_step;
	double 	y_step;
	double	x_intercept;
	double	y_intercept;
	int 	facing_down = ray_angle > 0 && ray_angle < M_PI;
	int 	facing_up = !facing_down;
	int 	facing_right = ray_angle < M_PI_2 || ray_angle > 3 * M_PI_2;
	int 	facing_left = !facing_right;
	double	next_ho_x;
	double	next_ho_y;
	bool	hit_wall = false;
	double		wall_hit_x = 0;
	double		wall_hit_y = 0;
	
	y_intercept = floor(cube->player.y / TILE_SIZE) * TILE_SIZE;
	if (facing_down)
		y_intercept += TILE_SIZE;
	x_intercept = cube->player.x + (y_intercept - cube->player.y) / tan(ray_angle);
	y_step = TILE_SIZE;
	if (facing_up)
		y_step *= -1;
	x_step = TILE_SIZE / tan(ray_angle);
	if ((facing_left && x_step > 0) || (facing_right && x_step < 0))
		x_step *= -1;
	next_ho_x = x_intercept;
	next_ho_y = y_intercept;
	if (facing_up)
		next_ho_y--;
	while (next_ho_x >= 0 && next_ho_x < cube->width && next_ho_y >= 0 && next_ho_y < cube->height)
	{
		if (collides_with_wall(cube, next_ho_x, next_ho_y))
		{
			hit_wall = true;
			wall_hit_x = next_ho_x;
			wall_hit_y = next_ho_y;
			draw_line(cube->player.x, cube->player.y, wall_hit_x, wall_hit_y, cube->image);
			break;
		}
		else
		{
			next_ho_x += x_step;
			next_ho_y += y_step;
		}
	}
}

void	ray_casting(t_cub3d *cube)
{
	double	ray_angle;
	int		i;

	ray_angle = cube->player.rotation_angle - (FOV_ANGLE / 2);
	i = -1;
	while (++i < NUM_RAYS)
	{
		render_ray(cube, ray_angle);
		ray_angle += FOV_ANGLE / NUM_RAYS;
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
	while (map)
	{
		x = 0;
		while (map->current_line[x])
		{
			draw_tile(cube, x, y, map->current_line[x]);
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
	draw_circle(cube);
	// ray_casting(cube);
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
		print_parsing_error("Invalid number of arguments");
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
