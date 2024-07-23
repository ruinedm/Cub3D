/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:25:15 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/23 20:04:05 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

int	parser(t_cub3d *program, char *map_name)
{
	int i;

	i = 0;
	(void)program;
	while (map_name[i] && map_name[i] != '.')
		i++;
	if(ft_strcmp(&map_name[i], ".cub"))
		return (print_parsing_error("Invalid map name"), 0);
	return (1);
}