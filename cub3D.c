/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:30 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/23 20:39:30 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"


int main(int ac, char **av)
{
	t_cub3d	program;

	if (ac != 2)
	{
		print_parsing_error("Invalid number of arguments");
		return (1);
	}
	if (!parser(&program, av[1]))
		return (1);
}
