/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_map.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:29:33 by aboukdid          #+#    #+#             */
/*   Updated: 2024/10/23 22:59:32 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

t_map	*read_map_file(char *first_line, t_cub3d *cube, int *line_count)
{
	t_map	*map;
	t_map	*current;
	char	*line;

	map = NULL;
	current = NULL;
	*line_count = 1;
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
		(*line_count)++;
	}
	return (map);
}

char	**convert_and_validate_map(t_map *map, int line_count, t_cub3d *cube)
{
	t_map	*current;
	int		i;
	bool	exist;

	i = 0;
	exist = false;
	if (!validate_map(map, cube))
		return (NULL);
	cube->map_array = malloc(sizeof(char *) * (line_count + 1));
	if (!cube->map_array)
		return (NULL);
	current = map;
	while (current)
	{
		cube->map_array[i] = current->current_line;
		map = current;
		current = current->next;
		free(map);
		i++;
	}
	return (cube->map_array[i] = NULL, cube->map_array);
}

char	**get_map(char *first_line, t_cub3d *cube)
{
	t_map	*map;
	int		line_count;
	char	**res;

	map = read_map_file(first_line, cube, &line_count);
	if (!map)
		return (NULL);
	res = convert_and_validate_map(map, line_count, cube);
	if (!res)
		return (ft_lstclear_mapline(map), NULL);
	return (res);
}

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
