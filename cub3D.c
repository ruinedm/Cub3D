/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:30 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/16 17:31:19 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

void	initialize_cube(t_cub3d *cube)
{
	cube->map_fd = -1;
	cube->ceiling_b = 0;
	cube->ceiling_r = 0;
	cube->ceiling_g = 0;
	cube->floor_b = 0;
	cube->floor_r = 0;
	cube->floor_b = 0;
	cube->ea_path = NULL;
	cube->map = NULL;
	cube->no_path = NULL;
	cube->so_path = NULL;
	cube->we_path = NULL;
}

int main(int ac, char **av)
{
	t_cub3d	cube;

	if (ac != 2)
	{
		print_parsing_error("Invalid number of arguments");
		return (1);
	}
	initialize_cube(&cube);
	if (!parser(&cube, av[1]))
		return (1);
}
