/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 00:21:41 by aboukdid          #+#    #+#             */
/*   Updated: 2024/08/30 10:35:50 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void render_texture(t_cub3d *cube, double offset, int wall_strip_height, int x)
{
    int texture_x;
    int texture_y;
    int end_y;
    int y;
    int start_y;
    int color;
	uint8_t *pixel;
    double percentage;
    mlx_texture_t *texture = cube->textures.no_texture;

    texture_x = (int)(offset * texture->width);
    start_y = cube->height / 2 - wall_strip_height / 2;
    if (start_y < 0)
        start_y = 0;
    y = start_y;
    end_y = y + wall_strip_height;
    if (end_y > cube->height)
        end_y = cube->height;

    while (y < end_y)
    {
        percentage = (double)(y - start_y) / wall_strip_height;
        texture_y = (int)(percentage * texture->height);
        if (texture_y < 0)
            texture_y = 0;
        if (texture_y >= texture->height)
            texture_y = texture->height - 1;

        pixel = texture->pixels + (texture_x * texture->bytes_per_pixel) + (texture_y * texture->width * texture->bytes_per_pixel);
       	uint8_t r = pixel[0];
        uint8_t g = pixel[1];
        uint8_t b = pixel[2];
        uint8_t a = pixel[3];

        color = create_rgba(r, g ,b, a);
        mlx_put_pixel(cube->image, x, y, color);
        y++;
    }
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

void	render_ray(t_cub3d *cube, double ray_angle, int i)
{
	t_ray	ray;
	int		wall_strip_height;
	double	ray_angle_diff;
	double	corrected_distance;
	double	transparency;
	double	offset;

	ray_angle = normalize_angle(ray_angle);
	ray.hit_ho = ho_inter(cube, ray_angle,
			&ray.ho_wall_hit_x, &ray.ho_wall_hit_y);
	ray.hit_ver = ve_inter(cube, ray_angle,
			&ray.ve_wall_hit_x, &ray.ve_wall_hit_y);
	if (ray.hit_ho)
		ray.ho_distance = distance(cube, ray.ho_wall_hit_x, ray.ho_wall_hit_y);
	else
		ray.ho_distance = INT_MAX;
	if (ray.hit_ver)
		ray.ve_distance = distance(cube, ray.ve_wall_hit_x, ray.ve_wall_hit_y);
	else
		ray.ve_distance = INT_MAX;
	if (ray.ho_distance < ray.ve_distance)
	{
		ray.ray_distance = ray.ho_distance;
		ray.ray_type = HORIZONTAL;
		offset = fmod(ray.ho_wall_hit_x, TILE_SIZE);
	}
	else
	{
		ray.ray_distance = ray.ve_distance;
		ray.ray_type = VERTICAL;
		offset = fmod(ray.ve_wall_hit_y, TILE_SIZE);
	}
	offset /= TILE_SIZE;
	if (ray.ray_distance < SMALL_VALUE)
		ray.ray_distance = SMALL_VALUE;
	ray_angle_diff = normalize_angle(ray_angle - cube->player.rotation_angle);
	corrected_distance = ray.ray_distance * cos(ray_angle_diff);
	wall_strip_height = cube->player.tiled_pp_dist / corrected_distance;
	render_texture(cube, offset, wall_strip_height, i);
}

bool	ve_inter(t_cub3d *cube, double ray_angle
		, double *wall_hit_x, double *wall_hit_y)
{
	t_ve_ray	ray;

	ray.facing_right = ray_angle < M_PI_2 || ray_angle > 3 * M_PI_2;
	ray.facing_left = !ray.facing_right;
	ray.facing_up = ray_angle > M_PI && ray_angle < 2 * M_PI;
	ray.x_intercept = floor(cube->player.x / TILE_SIZE) * TILE_SIZE;
	if (ray.facing_right)
		ray.x_intercept += TILE_SIZE;
	ray.y_intercept = cube->player.y
		+ (ray.x_intercept - cube->player.x) * tan(ray_angle);
	ray.x_step = TILE_SIZE;
	ray.y_step = TILE_SIZE * tan(ray_angle);
	if (ray.facing_left)
	{
		ray.x_step *= -1;
		ray.y_step *= -1;
	}
	ray.next_ve_x = ray.x_intercept;
	ray.next_ve_y = ray.y_intercept;
	while (ray.next_ve_x >= 0 && ray.next_ve_x < cube->width
		&& ray.next_ve_y >= 0 && ray.next_ve_y <= cube->height)
	{
		if (collides_with_wall(cube, ray.next_ve_x
				- (ray.facing_left ? 1 : 0), ray.next_ve_y))
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

bool	ho_inter(t_cub3d *cube, double ray_angle
		, double *wall_hit_x, double *wall_hit_y)
{
	t_ho_ray	ray;

	ray.facing_down = ray_angle > 0 && ray_angle < M_PI;
	ray.facing_up = !ray.facing_down;
	ray.facing_left = ray_angle > M_PI_2 && ray_angle < 3 * M_PI_2;
	ray.y_intercept = floor(cube->player.y / TILE_SIZE) * TILE_SIZE;
	if (ray.facing_down)
		ray.y_intercept += TILE_SIZE;
	ray.x_intercept = cube->player.x
		+ (ray.y_intercept - cube->player.y) / tan(ray_angle);
	ray.y_step = TILE_SIZE;
	ray.x_step = TILE_SIZE / tan(ray_angle);
	if (ray.facing_up)
	{
		ray.y_step *= -1;
		ray.x_step *= -1;
	}
	ray.next_ho_x = ray.x_intercept;
	ray.next_ho_y = ray.y_intercept;
	while (ray.next_ho_x >= 0 && ray.next_ho_x < cube->width
		&& ray.next_ho_y >= 0 && ray.next_ho_y <= cube->height)
	{
		if (collides_with_wall(cube, ray.next_ho_x, ray.next_ho_y
				- (ray.facing_up ? 1 : 0)))
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
