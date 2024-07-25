/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:25:15 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/25 02:04:53 by mboukour         ###   ########.fr       */
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

bool	set_color(char *str, t_cub3d *program, int c_type)
{
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
	if (c_type == F)
		program->floor_r = ft_atoi(str);
	else if (c_type == C)
		program->ceiling_r = ft_atoi(str);
	while (*str && is_digit(*str))
		str++;
	str++;
	if (c_type == F)
		program->floor_g = ft_atoi(str);
	else if (c_type == C)
		program->ceiling_g = ft_atoi(str);
		
	while (*str && is_ws(*str))
		str++;
	while (*str && is_digit(*str))
		str++;
	str++;
	if (c_type == F)
		program->floor_b = ft_atoi(str);
	else if (c_type == C)
		program->ceiling_b = ft_atoi(str);
	return (true);
}

bool	set_textures(char *str, t_cub3d *program, int texture)
{
	str++;
	str++;
	while(*str && is_ws(*(str)))
		str++;
	if(!*str)
		return (print_parsing_error("Invalid north texture"), false);
	if (texture == NO)
		program->no_path = ft_strdup(str);
	else if (texture == SO)
		program->so_path = ft_strdup(str);
	else if (texture == WE)
		program->we_path = ft_strdup(str);
	else if (texture == EA)
		program->ea_path = ft_strdup(str);
	return (true);
}

t_map *get_map(char *first_line, t_cub3d *program)
{
	char *line;
	t_map *map;
	t_map *current;

	map = ft_lstnew_mapline(first_line);
	while ((line = get_next_line(program->map_fd)))
	{
		current = ft_lstnew_mapline(line);
		ft_lstaddback_mapline(&map, current);
		free(line);
	}
	return (map);
}

int	parse_line(char *str, t_cub3d *program, bool *found_map)
{
	int 	i;

	i = 0;
	while (*str && is_ws(*str))
		str++;
	if (*str == 'F' && is_ws(*(str + 1)))
		return (set_color(str, program, F));
	else if (*str == 'C' && is_ws(*(str + 1)))
		return (set_color(str, program, C));
	else if (*str == 'N' && *(str + 1) == 'O')
		return (set_textures(str, program, NO));
	else if (*str == 'S' && *(str + 1) == 'O')
		return (set_textures(str, program, SO));
	else if (*str == 'W' && *(str + 1) == 'E')
		return (set_textures(str, program, WE));
	else if (*str == 'E' && *(str + 1) == 'A')
		return (set_textures(str, program, EA));
	else if (*str == '1' || *str == 'S' || *str == '0')
	{
		*found_map = true;
		if (*str != '1')
			return (print_parsing_error("Invalide map"), 0);
		program->map = get_map(str, program);
		
	}
	return (1);
}

int	parser(t_cub3d *program, char *map_name)
{
	int i;
	int fd;
	char *line;
	bool found_map;

	line = NULL;
	i = 0;
	found_map = false;
	while (map_name[i] && map_name[i] != '.')
		i++;
	if(ft_strcmp(&map_name[i], ".cub"))
		return (print_parsing_error("Invalid map name"), 0);
	fd = open(map_name, O_RDONLY, 0644);
	if (fd == -1)
		return (print_parsing_error(NULL), 0);
	program->map_fd = fd;
	while((line = get_next_line(fd)))
	{
		if (*line && !parse_line(line, program, &found_map))
			return (free(line), 0);
		free(line);
	}
	if (!found_map)
		return (print_parsing_error("Couldn't find the map"), 0);
	printf("R: %i // G: %i // B: %i\n", program->floor_r, program->floor_g, program->floor_b);
	printf("R: %i // G: %i // B: %i\n", program->ceiling_r, program->ceiling_g, program->ceiling_b);
	printf("NO: %s\nEA: %s\nSO: %s\nWE: %s\n", program->no_path, program->ea_path, program->so_path, program->we_path);
	ft_lstiter_mapline(program->map);
	return (1);
}
