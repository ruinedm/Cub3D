/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dooors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:20:25 by aboukdid          #+#    #+#             */
/*   Updated: 2024/10/23 16:32:24 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

void	toggle_door(t_cub3d *cube)
{
	int		player_tile_x;
	int		player_tile_y;
	t_door	*door;

	player_tile_x = cube->player.x / TILE_SIZE;
	player_tile_y = cube->player.y / TILE_SIZE;
	door = cube->door;
	while (door)
	{
		if (door->is_in_fov)
		{
			if (door->door_x == player_tile_x
				&& (door->door_y == player_tile_y + 1
					|| door->door_y == player_tile_y - 1))
				door->is_open = !door->is_open;
			else if (door->door_y == player_tile_y
				&& (door->door_x == player_tile_x + 1
					|| door->door_x == player_tile_x - 1))
				door->is_open = !door->is_open;
		}
		door = door->next;
	}
}

t_door	*get_door(t_door *door, int tile_x, int tile_y)
{
	while (door)
	{
		if (door->door_x == tile_x && door->door_y == tile_y)
			return (door);
		door = door->next;
	}
	return (NULL);
}
