/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:25:15 by mboukour          #+#    #+#             */
/*   Updated: 2024/10/24 15:46:26 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

int	parse_line(char *str, t_cub3d *cube)
{
	char	*original;
	int		color_res;

	original = str;
	color_res = set_color_textures(&str, cube);
	if (!color_res)
		return (0);
	if (color_res == 1)
		return (1);
	if (*str == '1' || *str == 'S' || *str == '0')
	{
		cube->map_array = get_map(original, cube);
		if (!cube->map_array)
			return (print_err("Invalid map"), 0);
	}
	else
		return (print_err("Invalid config file"), 0);
	return (1);
}

int	check_existance(t_cub3d *cube)
{
	int	i;

	i = 0;
	if (!do_i_exist(cube, F) || !do_i_exist(cube, C)
		|| !do_i_exist(cube, EA) || !do_i_exist(cube, SO)
		|| !do_i_exist(cube, WE) || !do_i_exist(cube, NO))
	{
		if (cube->map_array)
		{
			while (cube->map_array[i])
				free(cube->map_array[i++]);
			free(cube->map_array);
		}
		return (clean_wall_textures(cube), close(cube->map_fd),
			print_err("Missing parameter"), 0);
	}
	return (1);
}

int	parse_file(t_cub3d *cube, char *map_name)
{
	int		fd;
	char	*line;

	fd = open(map_name, O_RDONLY);
	if (fd == -1)
		return (print_err(NULL), 0);
	cube->map_fd = fd;
	while (1)
	{
		line = get_next_line(fd);
		if (!line)
			break ;
		if (*line && !parse_line(line, cube))
			return (clean_wall_textures(cube), close(fd), free(line), 0);
		free(line);
	}
	if (!check_existance(cube))
		return (0);
	close(fd);
	return (1);
}

int	parser(t_cub3d *cube, char *map_name)
{
	int	i;

	i = 0;
	while (map_name[i] && map_name[i] != '.')
		i++;
	if (ft_strcmp(&map_name[i], ".cub"))
		return (print_err("Invalid map name"), 0);
	if (!parse_file(cube, map_name))
		return (0);
	if (!cube->map_array)
		return (clean_wall_textures(cube),
			print_err("Invalid or missing map"), 0);
	cube->x_len = max_len(cube->map_array);
	cube->y_len = array_size(cube->map_array);
	cube->width = cube->x_len * TILE_SIZE;
	cube->height = cube->y_len * TILE_SIZE;
	cube->player.real_pp_dist = WIDTH / (2 * tan(cube->fov_angle / 2));
	cube->player.tiled_pp_dist = cube->player.real_pp_dist * TILE_SIZE;
	cube->mouse_x = WIDTH / 2;
	return (1);
}
