/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:30 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/28 23:23:28 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"

void	initialize_mlx(t_cub3d *cube)
{
	if(cube->player_direction == PLAYER_S)
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
	cube->max_render_distance = sqrt((cube->width - 1) * (cube->width - 1) + (cube->height - 1) * (cube->height - 1));
}
// AHSAN FUNCTION F DNYA LAY3TIK SE7A
void    draw_line(t_cub3d *cube, double x0, double y0, double x1, double y1, uint32_t color)
{
	x0 *= MINI_MAP;
	y0 *= MINI_MAP;
	x1 *= MINI_MAP;
	y1 *= MINI_MAP;
    double delta_x = x1 - x0;
    double delta_y = y1 - y0;
    int pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
    double pixel_x = x0;
    double pixel_y = y0;

    double x_inc = delta_x / pixels;
    double y_inc = delta_y / pixels;

    for (int i = 0; i < pixels; i++)
    {
        mlx_put_pixel(cube->image, (int)pixel_x, (int)pixel_y, color);
        pixel_x += x_inc;
        pixel_y += y_inc;
    }
}
void	draw_circle(t_cub3d *cube)
{
	t_circle	cir;

	cir.cx = cube->player.x * MINI_MAP;
	cir.cy = cube->player.y * MINI_MAP;
	cir.r = 2;
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
	int final_x = cube->player.x + cos(cube->player.rotation_angle) * 60;
	int final_y = cube->player.y + sin(cube->player.rotation_angle) * 60;
	draw_line(cube, cube->player.x, cube->player.y, final_x, final_y, RED);
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

	scaled_x = x * TILE_SIZE * MINI_MAP;
	scaled_y = y * TILE_SIZE * MINI_MAP;
	color = WHITE;
	if (mode == WALL)
		color = BLACK;
	i = 0;
	while (i < TILE_SIZE * MINI_MAP)
	{
		j = 0;
		while (j < TILE_SIZE * MINI_MAP)
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
	ray.x_step = TILE_SIZE / tan(ray_angle);
	if (ray.facing_up)
	{
		ray.y_step *= -1;
		ray.x_step *= -1;
	}
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
	ray.y_step = TILE_SIZE * tan(ray_angle);
	if (ray.facing_left)
	{
		ray.x_step *= -1;
		ray.y_step *= -1;
	}
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

void	draw_rectangle(int start_x, int start_y, int width, int height, t_cub3d *cube)
{
	int real_height;
	int real_width;
	int x;
	int y;

	real_height = start_y + height;
	real_width = start_x + width;
	x = start_x;
	y = start_y;
	while(y < real_height)
	{
		if (y < 0 || y >= cube->height)
			return;
		x = start_x;
		while(x < real_width)
		{
			if (x < 0 || x >= cube->width)
				return;
			mlx_put_pixel(cube->image, x, y, cube->strip_color);
			x++;
		}
		y++;
	}
}
int	create_trgb(unsigned char t, unsigned char r, unsigned char g,
		unsigned char b)
{
	unsigned char	trgb[4];

	trgb[0] = t;
	trgb[1] = r;
	trgb[2] = g;
	trgb[3] = b;
	return (*(int *)trgb);
}
void	render_ray(t_cub3d *cube, double ray_angle, int i)
{
	t_ray	ray;
	int		wall_strip_height;

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
		ray.ray_distance = ray.ho_distance;
	else
		ray.ray_distance = ray.ve_distance;
	double ray_angle_diff = normalize_angle(ray_angle - cube->player.rotation_angle);
    double corrected_distance = ray.ray_distance * cos(ray_angle_diff);
	wall_strip_height = TILE_SIZE * cube->player.projection_plane_distance / (corrected_distance);
    double transparency = 25000.0 / corrected_distance;
    transparency = fmin(fmax(transparency, 0.0), 255.0);
    cube->strip_color = create_trgb((unsigned char)transparency, 255, 255, 255);
	draw_rectangle(i, cube->height / 2 - wall_strip_height / 2, 1, wall_strip_height, cube);
}

void	ray_casting(t_cub3d *cube)
{
	double	ray_angle;
	int		i;

	ray_angle = cube->player.rotation_angle - (FOV_ANGLE / 2);
	i = 0;
	while (i < cube->width)
	{
		render_ray(cube, ray_angle, i);
		ray_angle += FOV_ANGLE / cube->width;
		i++;
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
	int color = CYAN;
	while (y < cube->height)
	{
		if (y > cube->height / 2)
			color = LIGHT_GREEN;
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
