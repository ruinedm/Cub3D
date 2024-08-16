/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:25:15 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/16 18:42:03 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

bool	is_good_color(char *str)
{
	int i;
	int count;

	i = 0;
	count = 0;
	if(str[0] == ',')
		return (false);
	while(str[i])
	{		
		while(str[i] && str[i] != ',')
		{
			if (!is_digit(str[i]))
				return (false);
			i++;
		}
		if (!str[i])
		{
			if(count != 2)
				return (false);
			return (true);
		}
		else if (str[i] == ',' && !str[i + 1])
			return (false);
		count++;
		i++;
		while(str[i] && is_ws(str[i]))
			i++;
	}
	return (true);
}

bool	do_i_exist(t_cub3d *cube, int type)
{
	if (type == F)
		return (cube->floor_r);
	else if (type == C)
		return (cube->ceiling_r);
	else if (type == NO)
		return (cube->no_path);
	else if (type == SO)
		return (cube->so_path);
	else if (type == WE)
		return (cube->we_path);
	else if (type == EA)
		return (cube->ea_path);
	return (false);
}

bool	set_color(char *str, t_cub3d *cube, int c_type)
{
	int r;
	int g;
	int b;
	if (do_i_exist(cube, c_type))
		return (print_parsing_error("Duplicate paramaters are not accepted!"), false);
	str++;
	while (*str && is_ws(*str))
		str++;
	if(!is_good_color(str))
	{
		print_parsing_error("Syntax error in color line.");
		return (false);
	}
	while (*str && is_ws(*str))
		str++;
	if(!is_good_color(str))
		return (print_parsing_error("Invalid color format."), 0);
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
		return (print_parsing_error("Invalid color range"), false);
	if (c_type == F)
	{
		cube->floor_r = r;
		cube->floor_g = g;
		cube->floor_b = b;
	}
	else
	{
		cube->ceiling_r = r;
		cube->ceiling_g = g;
		cube->ceiling_b = b;
	}
	return (true);
}

bool	set_textures(char *str, t_cub3d *cube, int texture)
{
	if(do_i_exist(cube, texture))
		return (print_parsing_error("Duplicate paramaters are not accepted!"), false);
	str++;
	str++;
	while(*str && is_ws(*(str)))
		str++;
	if(!*str)
		return (print_parsing_error("Invalid north texture"), false);
	if (texture == NO)
		cube->no_path = ft_strdup(str);
	else if (texture == SO)
		cube->so_path = ft_strdup(str);
	else if (texture == WE)
		cube->we_path = ft_strdup(str);
	else if (texture == EA)
		cube->ea_path = ft_strdup(str);
	return (true);
}

t_map *get_map(char *first_line, t_cub3d *cube)
{
	char *line;
	t_map *map;
	t_map *current;

	map = ft_lstnew_mapline(first_line);
	while ((line = get_next_line(cube->map_fd)))
	{
		current = ft_lstnew_mapline(line);
		ft_lstaddback_mapline(&map, current);
		free(line);
	}
	return (map);
}


int	parse_line(char *str, t_cub3d *cube)
{
	int 	i;

	i = 0;
	while (*str && is_ws(*str))
		str++;
	if (*str == 'F' && is_ws(*(str + 1)))
		return (set_color(str, cube, F));
	else if (*str == 'C' && is_ws(*(str + 1)))
		return (set_color(str, cube, C));
	else if (*str == 'N' && *(str + 1) == 'O')
		return (set_textures(str, cube, NO));
	else if (*str == 'S' && *(str + 1) == 'O')
		return (set_textures(str, cube, SO));
	else if (*str == 'W' && *(str + 1) == 'E')
		return (set_textures(str, cube, WE));
	else if (*str == 'E' && *(str + 1) == 'A')
		return (set_textures(str, cube, EA));
	else if (*str == '1' || *str == 'S' || *str == '0')
	{
		if (*str != '1')
			return (print_parsing_error("Invalid map"), 0);
		cube->map = get_map(str, cube);
	}
	else
		return (print_parsing_error("Invalid config file"), 0);
	return (1);
}


int	parser(t_cub3d *cube, char *map_name)
{
	int i;
	int fd;
	char *line;

	line = NULL;
	i = 0;
	while (map_name[i] && map_name[i] != '.')
		i++;
	if(ft_strcmp(&map_name[i], ".cub"))
		return (print_parsing_error("Invalid map name"), 0);
	fd = open(map_name, O_RDONLY, 0644);
	if (fd == -1)
		return (print_parsing_error(NULL), 0);
	cube->map_fd = fd;
	while((line = get_next_line(fd)))
	{
		if (*line && !parse_line(line, cube))
			return (free(line), 0);
		free(line);
	}
	if (!cube->map)
		return (print_parsing_error("Couldn't find the map"), 0);
	printf("R: %i // G: %i // B: %i\n", cube->floor_r, cube->floor_g, cube->floor_b);
	printf("R: %i // G: %i // B: %i\n", cube->ceiling_r, cube->ceiling_g, cube->ceiling_b);
	printf("NO: %s\nEA: %s\nSO: %s\nWE: %s\n", cube->no_path, cube->ea_path, cube->so_path, cube->we_path);
	ft_lstiter_mapline(cube->map);
	return (1);
}
