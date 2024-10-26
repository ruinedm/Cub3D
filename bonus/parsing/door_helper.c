/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   door_helper.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:24:13 by aboukdid          #+#    #+#             */
/*   Updated: 2024/10/24 13:45:55 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

int	player_valid_doors(t_map *map, int i)
{
	t_map	*prev;
	t_map	*next;
	char	*line;

	prev = map->prev;
	next = map->next;
	line = map->current_line;
	if (is_player_direction(line[i - 1])
		&& line[i + 1] == '0' && prev->current_line[i] == '1'
		&& next->current_line[i] == '1')
		return (1);
	if (line[i - 1] == '0' && is_player_direction(line[i + 1])
		&& prev->current_line[i] == '1' && next->current_line[i] == '1')
		return (1);
	if (line[i - 1] == '1' && line[i + 1] == '1'
		&& is_player_direction(prev->current_line[i])
		&& next->current_line[i] == '0')
		return (1);
	if (line[i - 1] == '1' && line[i + 1] == '1'
		&& prev->current_line[i] == '0'
		&& is_player_direction(next->current_line[i]))
		return (1);
	return (0);
}

int	is_door_valid(t_map *map, int i)
{
	t_map	*prev;
	t_map	*next;
	char	*line;

	prev = map->prev;
	next = map->next;
	line = map->current_line;
	if ((line[i - 1] == '1' && line[i + 1] == '1'
			&& prev->current_line[i] == '0'
			&& next->current_line[i] == '0')
		|| (line[i - 1] == '0' && line[i + 1] == '0'
			&& prev->current_line[i] == '1'
			&& next->current_line[i] == '1'))
		return (1);
	if (player_valid_doors(map, i))
		return (1);
	return (0);
}

int	validate_doors(t_map *map)
{
	t_map	*tmp;
	char	*line;
	int		i;

	tmp = map;
	while (tmp)
	{
		line = tmp->current_line;
		i = 0;
		while (line[i])
		{
			if (line[i] == 'D')
			{
				if (!is_door_valid(tmp, i))
					return (0);
			}
			i++;
		}
		tmp = tmp->next;
	}
	return (1);
}

int	get_color(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

bool	set_textures(char *str, t_cub3d *cube, int texture)
{
	mlx_texture_t	*texture_ptr;

	if (do_i_exist(cube, texture))
		return (print_err("Duplicate paramaters are not accepted!"), false);
	str++;
	str++;
	while (*str && is_ws(*(str)))
		str++;
	if (!*str)
		return (print_err("Invalid texture input"), false);
	texture_ptr = mlx_load_png(str);
	if (!texture_ptr)
		return (print_err("Error loading texture, check the path"), false);
	if (texture == NO)
		cube->textures.no_texture = texture_ptr;
	else if (texture == SO)
		cube->textures.so_texture = texture_ptr;
	else if (texture == WE)
		cube->textures.we_texture = texture_ptr;
	else if (texture == EA)
		cube->textures.ea_texture = texture_ptr;
	else if (texture == DO)
		cube->textures.d_texture = texture_ptr;
	return (true);
}
