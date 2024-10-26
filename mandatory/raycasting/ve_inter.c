/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ve_inter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:17:24 by aboukdid          #+#    #+#             */
/*   Updated: 2024/10/23 21:37:11 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

bool	ve_inter_loop(t_cub3d *cube, t_ve_ray *ray
	, double *wall_hit_x, double *wall_hit_y)
{
	bool	collision;

	while (ray->next_ve_x >= 0 && ray->next_ve_x < cube->width
		&& ray->next_ve_y >= 0 && ray->next_ve_y <= cube->height)
	{
		collision = collides_with_wall(cube, ray->next_ve_x
				- ray->decrement, ray->next_ve_y);
		if (collision)
		{
			*wall_hit_x = ray->next_ve_x;
			*wall_hit_y = ray->next_ve_y;
			ray->collision_type = true;
			return (true);
		}
		ray->next_ve_x += ray->x_step;
		ray->next_ve_y += ray->y_step;
	}
	return (false);
}

t_ve_ray	ve_inter(t_cub3d *cube, double ray_angle
		, double *wall_hit_x, double *wall_hit_y)
{
	t_ve_ray			ray;

	ray.facing_right = ray_angle < M_PI_2 || ray_angle > 3 * M_PI_2;
	ray.facing_left = !ray.facing_right;
	ray.facing_up = ray_angle > M_PI && ray_angle < 2 * M_PI;
	ray.next_ve_x = floor(cube->player.x / TILE_SIZE) * TILE_SIZE;
	if (ray.facing_right)
		ray.next_ve_x += TILE_SIZE;
	ray.next_ve_y = cube->player.y
		+ (ray.next_ve_x - cube->player.x) * tan(ray_angle);
	ray.x_step = TILE_SIZE;
	ray.y_step = TILE_SIZE * tan(ray_angle);
	ray.decrement = 0;
	if (ray.facing_left)
	{
		ray.x_step *= -1;
		ray.y_step *= -1;
		ray.decrement = 1;
	}
	if (ve_inter_loop(cube, &ray, wall_hit_x, wall_hit_y))
		return (ray);
	ray.collision_type = 0;
	return (ray);
}

t_texture_ren	set_ve_ray_attributes(t_cub3d *cube
		, t_ve_ray *ve_ray, t_ray *ray)
{
	t_texture_ren	ren_info;

	ren_info.offset = fmod(ray->ve_wall_hit_y, TILE_SIZE);
	if (ve_ray->facing_right)
		ren_info.texture = cube->textures.ea_texture;
	else
		ren_info.texture = cube->textures.we_texture;
	return (ren_info);
}
