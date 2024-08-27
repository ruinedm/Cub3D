/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:30 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/27 15:24:55 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"
#include <limits.h>
#include <string.h>

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

double	normalize_angle(double angle)
{
	angle = fmod(angle, 2 * M_PI);
	if (angle < 0)
		angle += 2 * M_PI;
	return (angle);
}

double	distance(t_cub3d *cube, double x, double y)
{
	return (sqrt((cube->player.x - x) * (cube->player.x - x)
			+ (cube->player.y - y) * (cube->player.y - y)));
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
    if (new_x < 0 || new_x >= cube->width || new_y < 0 || new_y >= cube->height)
		return (true);
	while (map && i < tile_y)
	{
		i++;
		map = map->next;
	}
	if (map->current_line[tile_x] == '1')
		return (true);
	return (false);
}

bool	ho_inter(t_cub3d *cube, double ray_angle, double *wall_hit_x, double *wall_hit_y)
{
	t_ho_ray	ray;

	ray.facing_down = ray_angle > 0 && ray_angle < M_PI;
	ray.facing_up = !ray.facing_down;
	ray.facing_left = ray_angle > M_PI_2 && ray_angle < 3 * M_PI_2;
	ray.y_intercept = floor(cube->player.y / TILE_SIZE) * TILE_SIZE;
	if (ray.facing_down)
		ray.y_intercept += TILE_SIZE;
	ray.x_intercept = cube->player.x + (ray.y_intercept - cube->player.y) / tan(ray_angle);
	ray.y_step = TILE_SIZE;
	if (ray.facing_up)
		ray.y_step *= -1;
	ray.x_step = TILE_SIZE / tan(ray_angle);
	if ((ray.facing_left && ray.x_step > 0) || (!ray.facing_left && ray.x_step < 0))
		ray.x_step *= -1;
	ray.next_ho_x = ray.x_intercept;
	ray.next_ho_y = ray.y_intercept;
	while (ray.next_ho_x >= 0 && ray.next_ho_x < cube->width && ray.next_ho_y >= 0 && ray.next_ho_y <= cube->height)
	{
		if (collides_with_wall(cube, ray.next_ho_x, ray.next_ho_y - (ray.facing_up ? 1 : 0)))
		{
			*wall_hit_x = ray.next_ho_x;
			*wall_hit_y = ray.next_ho_y;
			return (true);
		}
		ray.next_ho_x += ray.x_step;
		ray.next_ho_y += ray.y_step;
	}
	return (false);
}

bool	ve_inter(t_cub3d *cube, double ray_angle, double *wall_hit_x, double *wall_hit_y)
{
	t_ve_ray	ray;

	ray.facing_right = ray_angle < M_PI_2 || ray_angle > 3 * M_PI_2;
	ray.facing_left = !ray.facing_right;
	ray.facing_up = ray_angle > M_PI && ray_angle < 2 * M_PI;
	ray.x_intercept = floor(cube->player.x / TILE_SIZE) * TILE_SIZE;
	if (ray.facing_right)
		ray.x_intercept += TILE_SIZE;
	ray.y_intercept = cube->player.y + (ray.x_intercept - cube->player.x) * tan(ray_angle);
	ray.x_step = TILE_SIZE;
	if (ray.facing_left)
		ray.x_step *= -1;
	ray.y_step = TILE_SIZE * tan(ray_angle);
	if ((ray.facing_up && ray.y_step > 0) || (!ray.facing_up && ray.y_step < 0))
		ray.y_step *= -1;
	ray.next_ve_x = ray.x_intercept;
	ray.next_ve_y = ray.y_intercept;
	while (ray.next_ve_x >= 0 && ray.next_ve_x < cube->width && ray.next_ve_y >= 0 && ray.next_ve_y <= cube->height)
	{
		if (collides_with_wall(cube, ray.next_ve_x - (ray.facing_left ? 1 : 0), ray.next_ve_y))
		{
			*wall_hit_x = ray.next_ve_x;
			*wall_hit_y = ray.next_ve_y;
			return (true);
		}
		ray.next_ve_x += ray.x_step;
		ray.next_ve_y += ray.y_step;
	}
	return (false);
}

void	render_ray(t_cub3d *cube, double ray_angle)
{
	t_ray	ray;

	ray_angle = normalize_angle(ray_angle);
	ray.hit_ho = ho_inter(cube, ray_angle, &ray.ho_wall_hit_x, &ray.ho_wall_hit_y);
	ray.hit_ver = ve_inter(cube, ray_angle, &ray.ve_wall_hit_x, &ray.ve_wall_hit_y);
	if (ray.hit_ho)
		ray.ho_distance = distance(cube, ray.ho_wall_hit_x, ray.ho_wall_hit_y);
	else
		ray.ho_distance = INT_MAX;
	if (ray.hit_ver)
		ray.ve_distance = distance(cube, ray.ve_wall_hit_x, ray.ve_wall_hit_y);
	else
		ray.ve_distance = INT_MAX;
	if (ray.ho_distance < ray.ve_distance)
		draw_line(cube->player.x, cube->player.y, ray.ho_wall_hit_x, ray.ho_wall_hit_y, cube->image);
	else
		draw_line(cube->player.x, cube->player.y, ray.ve_wall_hit_x, ray.ve_wall_hit_y, cube->image);
}

void	ray_casting(t_cub3d *cube)
{
	double	ray_angle;
	int		i;

	ray_angle = cube->player.rotation_angle - (FOV_ANGLE / 2);
	ray_angle = normalize_angle(cube->player.rotation_angle - (FOV_ANGLE / 2));
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
