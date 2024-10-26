/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:32:40 by aboukdid          #+#    #+#             */
/*   Updated: 2024/10/26 17:07:05 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

int	array_size(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

bool	is_a_door(int tile_x, int tile_y, t_door *door)
{
	while (door)
	{
		if (door->door_x == tile_x && door->door_y == tile_y)
			return (true);
		door = door->next;
	}
	return (false);
}

bool	validate_map(t_map *map, t_cub3d *cube)
{
	if (!is_valid_inner(map, &(cube->player_direction)))
		return (false);
	if (!is_valid_fl(ft_lstlast_mapline(map)->current_line))
		return (false);
	if (!validate_doors(map))
		return (false);
	return (true);
}

void	set_flo_ce(t_cub3d *cube)
{
	int		x;
	int		y;

	x = 0;
	y = 0;
	while (x < WIDTH)
	{
		y = 0;
		while (y < HEIGHT)
		{
			if (y > (int)(HEIGHT / cube->center_factor))
				mlx_put_pixel(cube->image, x, y, cube->floor);
			else
				mlx_put_pixel(cube->image, x, y, cube->ceiling);
			y++;
		}
		x++;
	}
}

int	set_color_textures(char **str, t_cub3d *cube)
{
	while (**str && is_ws(**str))
		(*str)++;
	if (**str == 'F' && is_ws(*(*str + 1)))
		return (set_color(*str, cube, F));
	else if (**str == 'C' && is_ws(*(*str + 1)))
		return (set_color(*str, cube, C));
	else if (**str == 'N' && *(*str + 1) == 'O' && is_ws(*(*str + 2)))
		return (set_textures(*str, cube, NO));
	else if (**str == 'S' && *(*str + 1) == 'O' && is_ws(*(*str + 2)))
		return (set_textures(*str, cube, SO));
	else if (**str == 'W' && *(*str + 1) == 'E' && is_ws(*(*str + 2)))
		return (set_textures(*str, cube, WE));
	else if (**str == 'E' && *(*str + 1) == 'A' && is_ws(*(*str + 2)))
		return (set_textures(*str, cube, EA));
	else if (**str == 'D' && *(*str + 1) == 'O' && is_ws(*(*str + 2)))
		return (set_textures(*str, cube, DO));
	return (2);
}
