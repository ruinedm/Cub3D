/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/23 16:27:59 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/29 05:40:59 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

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
