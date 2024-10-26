/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inner.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:25:52 by aboukdid          #+#    #+#             */
/*   Updated: 2024/10/22 20:27:10 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

bool	checking_lines(char above_char, char next_char)
{
	if ((above_char != '1' && above_char != '0' && above_char != ' ')
		|| (next_char != '1' && next_char != '0' && next_char != ' '))
		return (false);
	return (true);
}

bool	checking_zero(char current_char, char adjacent_char)
{
	if (current_char == '0')
	{
		if (adjacent_char != '1' && adjacent_char != '0'
			&& adjacent_char != ' ' && adjacent_char != 'D'
			&& adjacent_char != 'S' && adjacent_char != 'W'
			&& adjacent_char != 'E' && adjacent_char != 'N')
			return (false);
	}
	return (true);
}

bool	adjacent_init(t_map *map, int i, char *above_char, char *next_char)
{
	if (map->prev && map->prev->current_line)
		*above_char = map->prev->current_line[i];
	else
		*above_char = ' ';
	if (map->next && map->next->current_line)
		*next_char = map->next->current_line[i];
	else
		*next_char = ' ';
	return (true);
}

bool	validate_line(t_map *map, char *line, int *player_direction)
{
	char	above_char;
	char	next_char;
	int		i;

	i = 0;
	while (line[i])
	{
		adjacent_init(map, i, &above_char, &next_char);
		if (line[i] == 'S' || line[i] == 'W'
			|| line[i] == 'E' || line[i] == 'N')
		{
			if (!checking_lines(above_char, next_char)
				|| !checking_direction(player_direction, line[i]))
				return (false);
		}
		else if (line[i] != '1' && line[i] != '0'
			&& line[i] != ' ' && line[i] != 'D')
			return (false);
		if (!checking_zero(line[i], above_char)
			|| !checking_zero(line[i], next_char))
			return (false);
		i++;
	}
	return (true);
}

bool	is_valid_inner(t_map *map, int *player_direction)
{
	char	*line;

	while (map && map->next)
	{
		line = map->current_line;
		if (!validate_wall_boundary(line) || !validate_doors(map))
			return (false);
		if (!validate_line(map, line, player_direction))
			return (false);
		map = map->next;
	}
	return (*player_direction != NONE);
}
