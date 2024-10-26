/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   collisions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:25:46 by aboukdid          #+#    #+#             */
/*   Updated: 2024/10/23 21:39:44 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

bool	does_player_collide(t_cub3d *cube, int new_x,
	int new_y, int radius)
{
	int		angle;
	int		offset_x;
	int		offset_y;
	t_door	*door;

	angle = 0;
	while (angle < 360)
	{
		offset_x = new_x + radius * cos(angle * M_PI / 180);
		offset_y = new_y + radius * sin(angle * M_PI / 180);
		door = get_door(cube->door, offset_x / TILE_SIZE, offset_y / TILE_SIZE);
		angle += 45;
		if (door && door->is_open)
			continue ;
		if (collides_with_wall(cube, offset_x, offset_y))
			return (true);
	}
	return (false);
}

void	move_player(t_cub3d *cube, t_move move)
{
	if (cube->player.x == move.new_x && cube->player.y == move.new_y)
		return ;
	if (!does_player_collide(cube, move.new_x, move.new_y, 5))
	{
		sprite(cube, false);
		cube->player.x = move.new_x;
		cube->player.y = move.new_y;
	}
}
