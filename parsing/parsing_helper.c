/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 00:45:55 by aboukdid          #+#    #+#             */
/*   Updated: 2024/08/29 03:22:44 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"
bool	is_good_color(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (str[0] == ',')
		return (false);
	while (str[i])
	{
		while (str[i] && str[i] != ',')
		{
			if (!is_digit(str[i]))
				return (false);
			i++;
		}
		if (!str[i])
		{
			if (count != 2)
				return (false);
			return (true);
		}
		else if (str[i] == ',' && !str[i + 1])
			return (false);
		count++;
		i++;
		while (str[i] && is_ws(str[i]))
			i++;
	}
	return (true);
}

int	get_color(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

bool	set_color(char *str, t_cub3d *cube, int c_type)
{
	int	r;
	int	g;
	int	b;

	if (do_i_exist(cube, c_type))
		return (prin_err("Duplicate paramaters are not accepted!"), false);
	str++;
	while (*str && is_ws(*str))
		str++;
	if (!is_good_color(str))
	{
		prin_err("Syntax error in color line.");
		return (false);
	}
	while (*str && is_ws(*str))
		str++;
	if (!is_good_color(str))
		return (prin_err("Invalid color format."), 0);
	r = ft_atoi(str);
	while (*str && is_digit(*str))
		str++;
	str++;
	g = ft_atoi(str);
	while (*str && is_ws(*str))
		str++;
	while (*str && is_digit(*str))
		str++;
	str++;
	b = ft_atoi(str);
	if ((r < 0 || r > 255) || (g < 0 || g > 255) || (b < 0 || b > 255))
		return (prin_err("Invalid color range"), false);
	if (c_type == F)
	{
		cube->floor_r = r;
		cube->floor_g = g;
		cube->floor_b = b;
		cube->floor = get_color(r, g, b, 255);
	}
	else
	{
		cube->ceiling_r = r;
		cube->ceiling_g = g;
		cube->ceiling_b = b;
		cube->ceiling = get_color(r, g, b, 255);
	}
	return (true);
}

bool	set_textures(char *str, t_cub3d *cube, int texture)
{
	void *texture_ptr;

	if (do_i_exist(cube, texture))
		return (prin_err("Duplicate paramaters are not accepted!"), false);
	str++;
	str++;
	while (*str && is_ws(*(str)))
		str++;
	if (!*str)
		return (prin_err("Invalid north texture"), false);
	texture_ptr = mlx_load_png(str);
	if(!texture)
		return (false);
	if (texture == NO)
		cube->no_texture = texture_ptr;
	else if (texture == SO)
		cube->so_texture = texture_ptr;
	else if (texture == WE)
		cube->we_texture = texture_ptr;
	else if (texture == EA)
		cube->ea_texture = texture_ptr;
	return (true);
}

bool	is_valid_inner(t_map *map, int *player_direction)
{
	char	*line;
	int		i;

	while (map && map->next)
	{
		line = map->current_line;
		i = 0;
		if (*line != '1' || line[ft_strlen(line) - 1] != '1')
			return (false);
		while (line[i])
		{
			if (line[i] == 'S' || line[i] == 'W'
				|| line[i] == 'E' || line[i] == 'N')
			{
				if (*player_direction != NONE)
					return (false);
				*player_direction = line[i];
			}
			else if (line[i] != '1' && line[i] != '0')
				return (false);
			i++;
		}
		map = map->next;
	}
	if (*player_direction == NONE)
		return (false);
	return (true);
}

bool	do_i_exist(t_cub3d *cube, int type)
{
	if (type == F)
		return (cube->floor_r != NONE);
	else if (type == C)
		return (cube->ceiling_r != NONE);
	else if (type == NO)
		return (cube->no_texture);
	else if (type == SO)
		return (cube->so_texture);
	else if (type == WE)
		return (cube->we_texture);
	else if (type == EA)
		return (cube->ea_texture);
	return (false);
}
