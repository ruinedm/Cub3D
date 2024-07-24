/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:25:15 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/24 01:26:00 by mboukour         ###   ########.fr       */
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
	}
	return (true);
}

int	check_type(char *str, t_cub3d *prgoram)
{
	int i;

	i = 0;
	while (*str && is_ws(*str))
		str++;
	if(*str == 'C' && is_ws(*(str + 1)))
	{
		str++;
		while (*str && is_ws(*str))
			str++;
		if(!is_good_color(str))
			return (print_parsing_error("Invalid color format."), exit(EXIT_FAILURE), 0);
	}
	return (1);
}

int	parser(t_cub3d *program, char *map_name)
{
	int i;
	int fd;
	char **str;
	char *line;

	line = NULL;
	(void)program;
	while (map_name[i] && map_name[i] != '.')
		i++;
	if(ft_strcmp(&map_name[i], ".cub"))
		return (print_parsing_error("Invalid map name"), 0);
	fd = open(map_name, O_RDONLY, 0644);
	if (fd == -1)
		return (print_parsing_error(NULL), 0);
	while((line = get_next_line(fd)))
	{
		printf("%s\n", line);
		check_type(line, program);
		free(line);
	}
	return (1);
}