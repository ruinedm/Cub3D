/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:27:59 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/23 19:59:47 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"


void	print_parsing_error(char *str)
{
	ft_putstr_fd(2, "Invalid input: ");
	ft_putstr_fd(2, str);
	ft_putstr_fd(2, "\nExpected input:\n./cub3D {map_name}.cub\n");
	ft_putstr_fd(2, "The map should be valid according to the ");
	ft_putstr_fd(2, "subject's rules.\n");
}