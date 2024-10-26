/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:09:12 by aboukdid          #+#    #+#             */
/*   Updated: 2024/10/23 16:15:25 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

void	ray_casting(t_cub3d *cube)
{
	double	ray_angle;
	int		i;

	ray_angle = cube->player.rotation_angle - (cube->fov_angle / 2);
	i = 0;
	while (i < WIDTH)
	{
		render_ray(cube, ray_angle, i);
		ray_angle += cube->fov_angle / WIDTH;
		i++;
	}
}

void	render_ray(t_cub3d *cube, double ray_angle, int i)
{
	t_ray			ray;
	double			ray_angle_diff;
	t_ho_ray		ho_ray;
	t_ve_ray		ve_ray;
	t_texture_ren	ren_info;

	ray_angle = normalize_angle(ray_angle);
	ho_ray = ho_inter(cube, ray_angle,
			&ray.ho_wall_hit_x, &ray.ho_wall_hit_y);
	ve_ray = ve_inter(cube, ray_angle,
			&ray.ve_wall_hit_x, &ray.ve_wall_hit_y);
	set_real_ray(cube, &ho_ray, &ve_ray, &ray);
	if (ray.ray_type == HORIZONTAL)
		ren_info = set_ho_ray_attributes(cube, &ho_ray, &ray);
	else
		ren_info = set_ve_ray_attributes(cube, &ve_ray, &ray);
	ren_info.offset /= TILE_SIZE;
	if (ray.ray_distance < SMALL_VALUE)
		ray.ray_distance = SMALL_VALUE;
	ray_angle_diff = normalize_angle(ray_angle - cube->player.rotation_angle);
	ren_info.wall_strip_height = cube->player.tiled_pp_dist
		/ (ray.ray_distance * cos(ray_angle_diff));
	ren_info.strip_x = i;
	render_texture(cube, &ren_info, 0, ren_info.texture);
}

void	set_real_ray(t_cub3d *cube, t_ho_ray *ho_ray
		, t_ve_ray *ve_ray, t_ray *ray)
{
	if (!ho_ray->collision_type)
	{
		ray->ray_distance = distance(cube, ray->ve_wall_hit_x,
				ray->ve_wall_hit_y);
		ray->ray_type = VERTICAL;
		return ;
	}
	if (!ve_ray->collision_type)
	{
		ray->ray_distance = distance(cube, ray->ho_wall_hit_x,
				ray->ho_wall_hit_y);
		ray->ray_type = HORIZONTAL;
		return ;
	}
	ray->ho_distance = distance(cube, ray->ho_wall_hit_x, ray->ho_wall_hit_y);
	ray->ve_distance = distance(cube, ray->ve_wall_hit_x, ray->ve_wall_hit_y);
	if (ray->ho_distance < ray->ve_distance)
	{
		ray->ray_distance = ray->ho_distance;
		ray->ray_type = HORIZONTAL;
		return ;
	}
	ray->ray_distance = ray->ve_distance;
	ray->ray_type = VERTICAL;
}

t_collision_info	collides_with_wall_with_type(t_cub3d *cube
	, int new_x, int new_y)
{
	t_collision_info	info;

	info.collision_type = 0;
	if (new_x < 0 || new_x >= cube->width || new_y < 0 || new_y >= cube->height)
		return (info.collision_type = WALL, info);
	if (cube->map_array[new_y / TILE_SIZE][new_x / TILE_SIZE] == '1')
		info.collision_type = WALL;
	return (info);
}

void	render_texture(t_cub3d *cube, t_texture_ren *ren_info
		, double y_pos, mlx_texture_t *texture)
{
	int		texture_x;
	int		y;
	int		color;
	uint8_t	*pixel;

	texture_x = (int)(ren_info->offset * texture->width);
	ren_info->start_y = HEIGHT / 2 - ren_info->wall_strip_height / 2;
	if (ren_info->start_y < 0)
		ren_info->start_y = 0;
	y = ren_info->start_y;
	ren_info->end_y = y + ren_info->wall_strip_height;
	if (ren_info->end_y > HEIGHT)
		ren_info->end_y = HEIGHT;
	ren_info->step = (double)texture->height / ren_info->wall_strip_height;
	y_pos = (ren_info->start_y - HEIGHT / 2 + ren_info->wall_strip_height / 2)
		* ren_info->step;
	while (y < ren_info->end_y)
	{
		pixel = texture->pixels + (texture_x * texture->bytes_per_pixel)
			+ ((int)y_pos * texture->width * texture->bytes_per_pixel);
		color = create_rgba(pixel[0], pixel[1], pixel[2], pixel[3]);
		mlx_put_pixel(cube->image, ren_info->strip_x, y, color);
		y_pos += ren_info->step;
		y++;
	}
}
