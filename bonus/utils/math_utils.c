/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 00:25:11 by aboukdid          #+#    #+#             */
/*   Updated: 2024/10/23 21:39:06 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

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
	int	tile_x;
	int	tile_y;

	tile_x = new_x / TILE_SIZE;
	tile_y = new_y / TILE_SIZE;
	if (new_x < 0 || new_x >= cube->width || new_y < 0 || new_y >= cube->height)
		return (true);
	if (tile_y < 0 || tile_y >= cube->y_len
		|| tile_x < 0 || tile_x >= cube->x_len)
		return (true);
	if (cube->map_array[tile_y][tile_x] == '1')
		return (true);
	return (false);
}

void	calculate_distances(t_cub3d *cube, t_ray *ray)
{
	if (ray->hit_ho)
		ray->ho_distance = distance(cube, ray->ho_wall_hit_x,
				ray->ho_wall_hit_y);
	else
		ray->ho_distance = INT_MAX;
	if (ray->hit_ver)
		ray->ve_distance = distance(cube, ray->ve_wall_hit_x,
				ray->ve_wall_hit_y);
	else
		ray->ve_distance = INT_MAX;
}

int	max_len(char **array)
{
	int	i;
	int	max;

	i = 0;
	max = 0;
	while (array[i])
	{
		if (ft_strlen(array[i]) > (size_t)max)
			max = ft_strlen(array[i]);
		i++;
	}
	return (max);
}
