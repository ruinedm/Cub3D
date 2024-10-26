/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:23:01 by aboukdid          #+#    #+#             */
/*   Updated: 2024/10/23 21:37:41 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

int	get_door_mode(int tile_x, int tile_y, t_door *door)
{
	while (door)
	{
		if (door->door_x == tile_x && door->door_y == tile_y)
		{
			if (door->is_open)
				return (OPEN_DOOR);
			return (CLOSED_DOOR);
		}
		door = door->next;
	}
	return (0);
}

void	draw_circle(int cx, int cy, int r, void *img)
{
	int	x;
	int	y;

	y = cy - r;
	while (y < cy + r)
	{
		x = cx - r;
		while (x <= cx + r)
		{
			if ((x - cx) * (x - cx) + (y - cy) * (y - cy) <= r * r)
				mlx_put_pixel(img, x, y, RED);
			x++;
		}
		y++;
	}
}

int	get_tile_mode(t_cub3d *cube, int tile_x, int tile_y)
{
	int	mode;

	if (tile_x < 0 || tile_y < 0 || tile_x * TILE_SIZE >= cube->width
		|| tile_y * TILE_SIZE >= cube->height)
		return (WALL);
	if ((size_t)tile_x > ft_strlen(cube->map_array[tile_y]))
		return (WALL);
	mode = get_door_mode(tile_x, tile_y, cube->door);
	if (mode)
		return (mode);
	return (cube->map_array[tile_y][tile_x]);
}

void	render_tile(t_cub3d *cube, int x, int y)
{
	int	color;
	int	mode;
	int	color_x;
	int	color_y;

	color_x = cube->player.x - (MINI_MAP_WH / 2) / 0.25 + (x / 0.25);
	color_y = cube->player.y - (MINI_MAP_WH / 2) / 0.25 + (y / 0.25);
	mode = get_tile_mode(cube, color_x / TILE_SIZE, color_y / TILE_SIZE);
	if (mode == OPEN_DOOR)
		color = LIGHT_GREEN;
	else if (mode == CLOSED_DOOR)
		color = CYAN;
	else if (mode == PLAYER_E || mode == PLAYER_N
		|| mode == PLAYER_S || mode == PLAYER_W || mode == VOID)
		color = WHITE;
	else
		color = BLACK;
	mlx_put_pixel(cube->image, x, y, color);
}

void	render_minimap(t_cub3d *cube)
{
	int	x;
	int	y;

	y = 0;
	while (y < MINI_MAP_WH)
	{
		x = 0;
		while (x < MINI_MAP_WH)
		{
			render_tile(cube, x, y);
			x++;
		}
		y++;
	}
	draw_circle(MINI_MAP_WH / 2, MINI_MAP_WH / 2, 5, cube->image);
}
