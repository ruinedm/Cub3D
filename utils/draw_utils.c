/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 00:16:57 by aboukdid          #+#    #+#             */
/*   Updated: 2024/08/29 01:13:19 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

void	draw_line(t_cub3d *cube, double x0, double y0
	, double x1, double y1, uint32_t color)
{
	t_line	line;
	int		i;

	i = 0;
	x0 *= MINI_MAP;
	y0 *= MINI_MAP;
	x1 *= MINI_MAP;
	y1 *= MINI_MAP;
	line.delta_x = x1 - x0;
	line.delta_y = y1 - y0;
	line.pixels = sqrt((line.delta_x * line.delta_x)
			+ (line.delta_y * line.delta_y));
	line.pixel_x = x0;
	line.pixel_y = y0;
	line.x_inc = line.delta_x / line.pixels;
	line.y_inc = line.delta_y / line.pixels;
	while (i < line.pixels)
	{
		mlx_put_pixel(cube->image, (int)line.pixel_x, (int)line.pixel_y, color);
		line.pixel_x += line.x_inc;
		line.pixel_y += line.y_inc;
		i++;
	}
}

void	draw_circle(t_cub3d *cube)
{
	t_circle	cir;
	int			final_x;
	int			final_y;

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
	final_x = cube->player.x + cos(cube->player.rotation_angle) * 60;
	final_y = cube->player.y + sin(cube->player.rotation_angle) * 60;
	draw_line(cube, cube->player.x, cube->player.y, final_x, final_y, RED);
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

void	draw_rectangle(int start_x, int start_y,
	int width, int height, t_cub3d *cube)
{
	int	real_height;
	int	real_width;
	int	x;
	int	y;

	real_height = start_y + height;
	real_width = start_x + width;
	x = start_x;
	y = start_y;
	while (y < real_height)
	{
		if (y < 0 || y >= cube->height)
			return ;
		x = start_x;
		while (x < real_width)
		{
			if (x < 0 || x >= cube->width)
				return ;
			mlx_put_pixel(cube->image, x, y, cube->strip_color);
			x++;
		}
		y++;
	}
}
