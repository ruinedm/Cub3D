/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 04:37:23 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/30 04:37:35 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void    render_ray1(t_cub3d *cube, double ray_angle, int i)
{
    t_ray    ray;

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
        draw_line(cube, cube->player.x, cube->player.y, ray.ho_wall_hit_x, ray.ho_wall_hit_y, RED);
    else
        draw_line(cube, cube->player.x, cube->player.y, ray.ve_wall_hit_x, ray.ve_wall_hit_y, RED);
}

void	raycast1(t_cub3d *cube)
{
	double	ray_angle;
	int		i;

	ray_angle = cube->player.rotation_angle - (FOV_ANGLE / 2);
	i = 0;
	while (i < cube->width)
	{
		render_ray1(cube, ray_angle, i);
		ray_angle += FOV_ANGLE / cube->width;
		i++;
	}
}