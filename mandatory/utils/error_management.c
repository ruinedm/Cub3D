/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_management.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 19:38:56 by mboukour          #+#    #+#             */
/*   Updated: 2024/10/24 01:27:59 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

void	clean_wall_textures(t_cub3d *cube)
{
	if (cube->textures.no_texture)
		mlx_delete_texture(cube->textures.no_texture);
	if (cube->textures.we_texture)
		mlx_delete_texture(cube->textures.we_texture);
	if (cube->textures.ea_texture)
		mlx_delete_texture(cube->textures.ea_texture);
	if (cube->textures.so_texture)
		mlx_delete_texture(cube->textures.so_texture);
}
