/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_hook.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/01 12:33:34 by mboukour          #+#    #+#             */
/*   Updated: 2024/10/24 13:24:37 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

void	player_move_hook(void *param)
{
	t_cub3d	*cube;

	cube = param;
	player_movement(cube);
	render_map(cube);
}
