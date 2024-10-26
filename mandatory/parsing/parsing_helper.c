/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 00:45:55 by aboukdid          #+#    #+#             */
/*   Updated: 2024/10/26 16:52:40 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

bool	is_player_direction(char c)
{
	return (c == 'N' || c == 'E' || c == 'W' || c == 'S');
}

bool	validate_wall_boundary(char *line)
{
	if (*line != '1' || line[ft_strlen(line) - 1] != '1')
		return (false);
	return (true);
}

bool	checking_direction(int *player_direction, char current_char)
{
	if (*player_direction != NONE)
		return (false);
	*player_direction = current_char;
	return (true);
}

bool	do_i_exist(t_cub3d *cube, int type)
{
	if (type == F)
		return (cube->floor_r != NONE);
	else if (type == C)
		return (cube->ceiling_r != NONE);
	else if (type == NO)
		return (cube->textures.no_texture);
	else if (type == SO)
		return (cube->textures.so_texture);
	else if (type == WE)
		return (cube->textures.we_texture);
	else if (type == EA)
		return (cube->textures.ea_texture);
	return (false);
}

void	print_err(char *str)
{
	ft_putstr_fd(2, "Error\n");
	if (str)
	{
		ft_putstr_fd(2, str);
		ft_putstr_fd(2, "\n");
	}
	else
		perror("");
	ft_putstr_fd(2, "Expected input:\n./cub3D {config_file}.cub\n");
	ft_putstr_fd(2, "Make sure the file ");
	ft_putstr_fd(2, "exists and you have read permissions.\n");
	ft_putstr_fd(2, "The config should be valid according to the ");
	ft_putstr_fd(2, "subject's rules.\n");
}
