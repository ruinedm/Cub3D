/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:30 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/25 01:16:49 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"
#include "MLX42/include/MLX42/MLX42.h"

void	initialize_mlx(t_cub3d *cube)
{
	cube->mlx = mlx_init(cube->width, cube->height, "Ruined CUB3D\n", false);
	if (!cube->mlx)
		exit(EXIT_FAILURE);
	cube->image = mlx_new_image(cube->mlx, cube->width, cube->height);
	if(!cube->image)
		exit(EXIT_FAILURE);
	mlx_image_to_window(cube->mlx, cube->image, 0, 0);
}

void	initialize_cube(t_cub3d *cube)
{
	cube->map_fd = NONE;
	cube->ceiling_b = NONE;
	cube->ceiling_r = NONE;
	cube->ceiling_g = NONE;
	cube->floor_b = NONE;
	cube->floor_r = NONE;
	cube->floor_b = NONE;
	cube->ea_path = NULL;
	cube->map = NULL;
	cube->no_path = NULL;
	cube->so_path = NULL;
	cube->we_path = NULL;
    cube->player.x = 0;
    cube->player.y = 0;
    cube->player.angle = 0;
    cube->player.turn_direction = 0;
    cube->player.walk_direction = 0;
    cube->player.rotation = PI / 2;
    cube->player.speed = 3;
    cube->player.turn_speed = 3 * (PI / 180);
    
}




void draw_circle(int cx, int cy, int r, void *img)
{
    int x;
    int y = cy - r;
    int l_x;
    int l_y;

    while(y < cy + r)
    {
        x = cx - r;
        while(x < cx + r)
        {
            if ((x - cx) * (x - cx) + (y - cy) * (y - cy) < r * r)
                mlx_put_pixel(img, x, y, RED);
            x++;
        }
        y++;
    }
    // LINE FACING UP -- FOR TESTING PURPOSES
    l_x = cx;
    l_y = cy - r;
    while(l_y > cy - r - 100)
    {
        mlx_put_pixel(img, l_x, l_y, RED);
        l_y--;
    }
}

bool is_a_player(int mode)
{
    return (mode == PLAYER_E || mode == PLAYER_W || mode == PLAYER_N || mode == PLAYER_S);
}

void draw_tile(t_cub3d *cube, int x, int y, int mode)
{
    int i;
    int j;
    int scaled_x;
    int scaled_y;
    int color;
    scaled_x = x * TILE_SIZE;
    scaled_y = y * TILE_SIZE;

    color = WHITE;
    if (mode == WALL)
        color = BLACK;
    i = 0;
    while (i < TILE_SIZE)
    {
        j = 0;
        while (j < TILE_SIZE)
        {
            if (color == (int)WHITE && (!i || !j))
                mlx_put_pixel(cube->image, scaled_x + i, scaled_y + j, BLACK);
            else
                mlx_put_pixel(cube->image, scaled_x + i, scaled_y + j, color);
            j++;
        }
        i++;
    }
}

void render_map(t_cub3d *cube)
{
    t_map *map = cube->map;
    int x = 0;
    int y = 0;

    while (map)
    {
        x = 0;
        while (map->current_line[x])
        {
			draw_tile(cube, x, y, map->current_line[x]);
            if(!cube->player.x && is_a_player(map->current_line[x]))
            {
                cube->player.x = x;
                cube->player.y = y;
            }
            x++;
        }
        y++;
        map = map->next;
    }
    draw_circle(cube->player.x * TILE_SIZE + TILE_SIZE / 2, cube->player.y * TILE_SIZE + TILE_SIZE / 2, 5, cube->image);
}

void key_hook(mlx_key_data_t key_data, void *param)
{
    t_cub3d *cube = (t_cub3d *)param;
    
    if (key_data.action == MLX_PRESS || key_data.action == MLX_REPEAT)
    {
        if (key_data.key == MLX_KEY_W)
            cube->player.walk_direction = 1;
        else if (key_data.key == MLX_KEY_S)
            cube->player.walk_direction = -1;
        else if (key_data.key == MLX_KEY_A)
            cube->player.turn_direction = -1;
        else if (key_data.key == MLX_KEY_D)
            cube->player.turn_direction = 1;
        else if (key_data.key == MLX_KEY_ESCAPE)
            exit(0);
    }
    if (key_data.action == MLX_RELEASE)
    {
        if (key_data.key == MLX_KEY_W || key_data.key == MLX_KEY_S)
            cube->player.walk_direction = 0;
        if (key_data.key == MLX_KEY_A || key_data.key == MLX_KEY_D)
            cube->player.turn_direction = 0;
    }
}

void update_player_position(t_cub3d *cube)
{
    double move_step = cube->player.walk_direction * cube->player.speed;
    double new_x = cube->player.x + cos(cube->player.angle) * move_step;
    double new_y = cube->player.y + sin(cube->player.angle) * move_step;
    cube->player.x = new_x;
    cube->player.y = new_y;
    cube->player.angle += cube->player.turn_direction * cube->player.turn_speed;
}

void    loop_hook(void *param)
{
    t_cub3d *cube = (t_cub3d *)param;
    update_player_position(cube);
    render_map(cube);
}
int main(int ac, char **av)
{
	(void)av;
	t_cub3d	cube;
	if (ac != 2)
	{
		print_parsing_error("Invalid number of arguments");
		return (1);
	}
	initialize_cube(&cube);
	if (!parser(&cube, av[1]))
		return (1);
	initialize_mlx(&cube);
	render_map(&cube);
    mlx_key_hook(cube.mlx, key_hook, &cube);
    mlx_loop_hook(cube.mlx, loop_hook, &cube);
	mlx_loop(cube.mlx);
}
