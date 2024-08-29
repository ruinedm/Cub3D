/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:25:15 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/29 05:40:59 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

bool	is_valid_fl(char *line)
{
	while (*line)
	{
		if (*line != ' ' && *line != '1')
			return (false);
		line++;
	}
	line--;
	return (true);
}

void	ws_to_one(char *str)
{
	while (*str)
	{
		if (is_ws(*str))
			*str = '1';
		str++;
	}
}

t_map	*get_map(char *first_line, t_cub3d *cube)
{
	char	*line;
	t_map	*map;
	t_map	*current;

	ws_to_one(first_line);
	if (!is_valid_fl(first_line))
		return (NULL);
	map = ft_lstnew_mapline(ft_strdup(first_line));
	line = get_next_line(cube->map_fd);
	while (line)
	{
		ws_to_one(line);
		current = ft_lstnew_mapline(line);
		ft_lstaddback_mapline(&map, current);
		line = get_next_line(cube->map_fd);
	}
	if (!is_valid_inner(map->next, &(cube->player_direction))
		|| !is_valid_fl(current->current_line))
		return (NULL);
	return (map);
}

int	parse_line(char *str, t_cub3d *cube)
{
	int	i;

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
		cube->map = get_map(str, cube);
		if (!cube->map)
			return (print_err("Invalid map"), 0);
	}
	else
		return (print_err("Invalid config file"), 0);
	return (1);
}

int	parser(t_cub3d *cube, char *map_name)
{
	int		i;
	int		fd;
	char	*line;

	line = NULL;
	i = 0;
	while (map_name[i] && map_name[i] != '.')
		i++;
	if (ft_strcmp(&map_name[i], ".cub"))
		return (print_err("Invalid map name"), 0);
	fd = open(map_name, O_RDONLY, 0644);
	if (fd == -1)
		return (print_err(NULL), 0);
	cube->map_fd = fd;
	line = get_next_line(fd);
	while (line)
	{
		if (*line && !parse_line(line, cube))
			return (free(line), 0);
		free(line);
		line = get_next_line(fd);
	}
	if (!do_i_exist(cube, F) || !do_i_exist(cube, C)
		|| !do_i_exist(cube, EA) || !do_i_exist(cube, SO)
		|| !do_i_exist(cube, WE) || !do_i_exist(cube, NO))
		return (print_err("Missing parameter"), 0);
	if (!cube->map)
		return (print_err("Invalid or missing map"), 0);
	cube->x_len = max_len(cube->map);
	cube->y_len = ft_lstsize_mapline(cube->map);
	cube->width = cube->x_len * TILE_SIZE;
	cube->height = cube->y_len * TILE_SIZE;
	cube->player.tiled_pp_dist = cube->width / (2 * tan(FOV_ANGLE / 2)) * TILE_SIZE; // MULTIPLYING BY TILE_SIZE IS JUST FOR OPTIMIZATION PURPOSES FOR PP DISTANCE AND IS NOT A PART OF THE FORMULA
	close(fd);
	return (1);
}
