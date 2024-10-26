/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprite.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 16:34:26 by aboukdid          #+#    #+#             */
/*   Updated: 2024/10/24 13:53:14 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

void	sprite(t_cub3d *cube, bool first)
{
	static int	i;
	static int	frame_skip;
	int			img_width;
	int			img_height;

	frame_skip++;
	if (!first && frame_skip < 3)
		return ;
	frame_skip = 0;
	if (cube->sprites.img)
		mlx_delete_image(cube->mlx, cube->sprites.img);
	img_width = cube->sprites.textures[i]->width;
	img_height = cube->sprites.textures[i]->height;
	cube->sprites.img = mlx_texture_to_image(cube->mlx,
			cube->sprites.textures[i]);
	mlx_image_to_window(cube->mlx, cube->sprites.img,
		(WIDTH - img_width) / 2, HEIGHT - img_height);
	i++;
	if (i == 13)
		i = 0;
}

void	set_the_sprite_protected(t_cub3d *cube, int current, char *str)
{
	int		i;

	i = 0;
	cube->sprites.textures[current] = mlx_load_png(str);
	if (cube->sprites.textures[current])
		return ;
	while (i < current)
	{
		mlx_delete_texture(cube->sprites.textures[i]);
		i++;
	}
	mlx_delete_texture(cube->textures.ea_texture);
	mlx_delete_texture(cube->textures.so_texture);
	mlx_delete_texture(cube->textures.we_texture);
	mlx_delete_texture(cube->textures.no_texture);
	if (cube->textures.d_texture)
		mlx_delete_texture(cube->textures.d_texture);
	mlx_delete_image(cube->mlx, cube->image);
	mlx_terminate(cube->mlx);
	print_err("Error loading sprite textures");
	exit(EXIT_FAILURE);
}

void	init_sprites(t_cub3d *cube)
{
	set_the_sprite_protected(cube, 0, "bonus/sprites/1.png");
	set_the_sprite_protected(cube, 1, "bonus/sprites/2.png");
	set_the_sprite_protected(cube, 2, "bonus/sprites/3.png");
	set_the_sprite_protected(cube, 3, "bonus/sprites/4.png");
	set_the_sprite_protected(cube, 4, "bonus/sprites/5.png");
	set_the_sprite_protected(cube, 5, "bonus/sprites/6.png");
	set_the_sprite_protected(cube, 6, "bonus/sprites/7.png");
	set_the_sprite_protected(cube, 7, "bonus/sprites/8.png");
	set_the_sprite_protected(cube, 8, "bonus/sprites/9.png");
	set_the_sprite_protected(cube, 9, "bonus/sprites/10.png");
	set_the_sprite_protected(cube, 10, "bonus/sprites/11.png");
	set_the_sprite_protected(cube, 11, "bonus/sprites/12.png");
	set_the_sprite_protected(cube, 12, "bonus/sprites/13.png");
	cube->sprites.img = NULL;
}
