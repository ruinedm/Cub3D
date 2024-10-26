/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ho_inter.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:16:30 by aboukdid          #+#    #+#             */
/*   Updated: 2024/10/23 21:37:01 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

t_ho_ray	ho_inter(t_cub3d *cube, double ray_angle
		, double *wall_hit_x, double *wall_hit_y)
{
	t_ho_ray			ray;

	ray.facing_down = ray_angle > 0 && ray_angle < M_PI;
	ray.facing_up = !ray.facing_down;
	ray.facing_left = ray_angle > M_PI_2 && ray_angle < 3 * M_PI_2;
	ray.next_ho_y = floor(cube->player.y / TILE_SIZE) * TILE_SIZE;
	if (ray.facing_down)
		ray.next_ho_y += TILE_SIZE;
	ray.next_ho_x = cube->player.x
		+ (ray.next_ho_y - cube->player.y) / tan(ray_angle);
	ray.y_step = TILE_SIZE;
	ray.x_step = TILE_SIZE / tan(ray_angle);
	ray.decrement = 0;
	if (ray.facing_up)
	{
		ray.y_step *= -1;
		ray.x_step *= -1;
		ray.decrement = 1;
	}
	if (ho_inter_loop(cube, &ray, wall_hit_x, wall_hit_y))
		return (ray);
	ray.collision_type = 0;
	return (ray);
}

bool	ho_inter_loop(t_cub3d *cube, t_ho_ray *ray,
		double *wall_hit_x, double *wall_hit_y)
{
	bool	collision;

	while (ray->next_ho_x >= 0 && ray->next_ho_x < cube->width
		&& ray->next_ho_y >= 0 && ray->next_ho_y <= cube->height)
	{
		collision = collides_with_wall(cube, ray->next_ho_x,
				ray->next_ho_y - ray->decrement);
		if (collision)
		{
			*wall_hit_x = ray->next_ho_x;
			*wall_hit_y = ray->next_ho_y;
			ray->collision_type = true;
			return (true);
		}
		ray->next_ho_x += ray->x_step;
		ray->next_ho_y += ray->y_step;
	}
	return (false);
}

t_texture_ren	set_ho_ray_attributes(t_cub3d *cube
		, t_ho_ray *ho_ray, t_ray *ray)
{
	t_texture_ren	ren_info;

	ren_info.offset = fmod(ray->ho_wall_hit_x, TILE_SIZE);
	if (ho_ray->facing_up)
		ren_info.texture = cube->textures.no_texture;
	else
		ren_info.texture = cube->textures.so_texture;
	return (ren_info);
}
