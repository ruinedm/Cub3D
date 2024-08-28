/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 00:25:11 by aboukdid          #+#    #+#             */
/*   Updated: 2024/08/29 00:27:04 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

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

bool	is_a_player(int mode)
{
	return (mode == PLAYER_E || mode == PLAYER_W
		|| mode == PLAYER_N || mode == PLAYER_S);
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
