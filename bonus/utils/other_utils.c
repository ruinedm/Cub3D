/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 01:10:18 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/31 08:48:45 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

bool	is_a_player(int mode)
{
	return (mode == PLAYER_E || mode == PLAYER_W
		|| mode == PLAYER_N || mode == PLAYER_S);
}

int	create_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}
