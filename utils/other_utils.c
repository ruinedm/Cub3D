/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   other_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 01:10:18 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/29 01:10:45 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

bool	is_a_player(int mode)
{
	return (mode == PLAYER_E || mode == PLAYER_W
		|| mode == PLAYER_N || mode == PLAYER_S);
}


int	create_trgb(unsigned char t, unsigned char r, unsigned char g,
		unsigned char b)
{
	unsigned char	trgb[4];

	trgb[0] = t;
	trgb[1] = r;
	trgb[2] = g;
	trgb[3] = b;
	return (*(int *)trgb);
}
