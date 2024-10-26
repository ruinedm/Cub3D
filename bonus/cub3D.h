/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:10 by mboukour          #+#    #+#             */
/*   Updated: 2024/10/26 17:14:40 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <fcntl.h>
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h> 
# include <math.h>
# include <stdio.h>
# include "MLX42.h"

# define NONE -1
# define WIDTH 1280
# define HEIGHT 920
# define TILE_SIZE 64
# define BLACK 0x000000FF
# define WHITE 0xFFFFFFFF
# define RED 0xFF0000FF
# define CYAN 0x00FFFFFF
# define LIGHT_GREEN 0x90EE90FF
# define HORIZONTAL 0
# define VERTICAL 1
# define SMALL_VALUE 0.0001
# define MINI_MAP_WH 200
# define OPEN_DOOR 1
# define CLOSED_DOOR 2

enum e_parse_options
{
	C,
	NO,
	SO,
	WE,
	EA,
	F,
	DO
};

enum e_map_params
{
	VOID = '0',
	WALL = '1',
	PLAYER_E = 'E',
	PLAYER_W = 'W',
	PLAYER_N = 'N',
	PLAYER_S = 'S',
	DOOR = 'D',
};

typedef struct s_player
{
	int		x;
	int		y;
	int		angle;
	int		turn_direction;
	int		walk_direction;
	double	rotation_angle;
	int		strafe_direction;
	int		movement_speed;
	double	rotation_speed;
	double	mouse_rotation_speed;
	int		tiled_pp_dist;
	int		real_pp_dist;
}	t_player;

typedef struct s_map
{
	char			*current_line;
	int				line_len;
	struct s_map	*prev;
	struct s_map	*next;
}	t_map;

typedef struct s_ho_ray
{
	int		decrement;
	double	x_intercept;
	double	y_intercept;
	double	x_step;
	double	y_step;
	double	next_ho_x;
	double	next_ho_y;
	int		facing_down;
	int		facing_up;
	int		facing_left;
	int		collision_type;
}	t_ho_ray;

typedef struct s_ve_ray
{
	int		decrement;
	double	x_intercept;
	double	y_intercept;
	double	x_step;
	double	y_step;
	double	next_ve_x;
	double	next_ve_y;
	int		facing_right;
	int		facing_left;
	int		facing_up;
	int		collision_type;
}	t_ve_ray;

typedef struct s_ray
{
	double		ho_wall_hit_x;
	double		ho_wall_hit_y;
	double		ve_wall_hit_x;
	double		ve_wall_hit_y;
	int			hit_ho;
	int			hit_ver;
	double		ho_distance;
	double		ve_distance;
	double		ray_distance;
	int			ray_type;
}	t_ray;

typedef struct s_move
{
	int		step;
	int		strafe;
	int		new_x;
	int		new_y;
	double	strafe_angle;
}	t_move;

typedef struct s_textures
{
	mlx_texture_t		*no_texture;
	mlx_texture_t		*so_texture;
	mlx_texture_t		*we_texture;
	mlx_texture_t		*ea_texture;
	mlx_texture_t		*d_texture;

}	t_textures;

typedef struct s_sprite
{
	mlx_texture_t	*textures[13];
	mlx_image_t		*img;
}	t_sprite;

typedef struct s_door
{
	int				door_x;
	int				door_y;
	bool			is_open;
	bool			is_in_fov;
	struct s_door	*next;
	struct s_door	*prev;
}	t_door;

typedef struct s_collision_info
{
	int		collision_type;
	t_door	*current_door;
}	t_collision_info;

typedef struct s_texture_ren
{
	mlx_texture_t	*texture;
	double			offset;
	int				wall_strip_height;
	int				strip_x;
	int				start_y;
	int				end_y;
	double			step;
}	t_texture_ren;

typedef struct s_cub3d
{
	void				*mlx;
	void				*image;
	int					width;
	int					height;
	int					floor_r;
	int					floor_g;
	int					floor_b;
	int					floor;
	int					ceiling_r;
	int					ceiling_g;
	int					ceiling_b;
	int					ceiling;
	int					map_fd;
	int					x_len;
	int					y_len;
	int					strip_color;
	int					max_render_distance;
	int					player_direction;
	double				center_factor;
	int					mouse_x;
	int					mouse_y;
	int					center;
	double				fov_angle;
	char				**map_array;
	t_textures			textures;
	t_map				*map;
	t_player			player;
	t_sprite			sprites;
	t_door				*door;
}	t_cub3d;

int					parser(t_cub3d *cube, char *map_name);
void				print_err(char *str);
char				*get_next_line(int fd);
char				**get_map(char *first_line, t_cub3d *cube);
int					array_size(char **array);
int					get_color(int r, int g, int b, int a);
bool				is_player_direction(char c);
bool				checking_direction(int *player_direction,
						char current_char);
bool				validate_wall_boundary(char *line);
void				ws_to_one(char *str);
void				set_flo_ce(t_cub3d *cube);
bool				is_valid_fl(char *line);
t_door				*manage_doors(t_map *map, bool *exist, int x, int y);
void				clean_wall_textures(t_cub3d *cube);
void				clean_all_textures(t_cub3d *cube);
bool				is_valid_inner(t_map *map, int *player_direction);
bool				set_color(char *str, t_cub3d *cube, int c_type);
bool				set_textures(char *str, t_cub3d *cube, int texture);
bool				do_i_exist(t_cub3d *cube, int type);
size_t				ft_strlen(const char *s);
void				ft_putstr_fd(int fd, char *str);
void				mouse_pos_hook(void *param);
char				*ft_strchr(char *s, int c);
char				*ft_strdup(char *s1);
int					set_color_textures(char **str, t_cub3d *cube);
char				*ft_substr(const char *s, unsigned int start, size_t len);
char				*ft_strjoin(char *s1, char *s2);
int					ft_strcmp(const char *s1, const char *s2);
bool				is_ws(char c);
int					is_digit(int c);
int					ft_atoi(const char *str);
t_map				*ft_lstnew_mapline(char *line);
void				ft_lstaddback_mapline(t_map **head, t_map *new);
void				ft_lstclear_mapline(t_map *map);
int					max_len(char **array);
int					create_rgba( int r, int g, int b, int a);
void				ray_casting(t_cub3d *cube);
void				render_ray(t_cub3d *cube, double ray_angle, int i);
double				normalize_angle(double angle);
double				distance(t_cub3d *cube, double x, double y);
bool				collides_with_wall(t_cub3d *cube, int new_x, int new_y);
bool				is_a_player(int mode);
void				render_map(t_cub3d *cube);
void				player_movement(t_cub3d *cube);
void				key_hook(mlx_key_data_t key_data, void *param);
void				sprite(t_cub3d *cube, bool first);
t_door				*ft_lstnew_door(int door_x, int door_y);
void				ft_lstaddback_door(t_door **head, t_door *new);
void				ft_lstclear_door(t_door *door);
bool				is_a_door(int tile_x, int tile_y, t_door *door);
int					validate_doors(t_map *map);
t_map				*ft_lstlast_mapline(t_map *lst);
void				ray_casting(t_cub3d *cube);
void				render_ray(t_cub3d *cube, double ray_angle, int i);
void				set_real_ray(t_cub3d *cube, t_ho_ray *ho_ray,
						t_ve_ray *ve_ray, t_ray *ray);
t_ve_ray			ve_inter(t_cub3d *cube, double ray_angle,
						double *wall_hit_x, double *wall_hit_y);
bool				ve_inter_loop(t_cub3d *cube, t_ve_ray *ray,
						double *wall_hit_x, double *wall_hit_y);
bool				ho_inter_loop(t_cub3d *cube, t_ho_ray *ray,
						double *wall_hit_x, double *wall_hit_y);
t_ho_ray			ho_inter(t_cub3d *cube, double ray_angle,
						double *wall_hit_x, double *wall_hit_y);
t_texture_ren		set_ho_ray_attributes(t_cub3d *cube,
						t_ho_ray *ho_ray, t_ray *ray);
t_texture_ren		set_ve_ray_attributes(t_cub3d *cube,
						t_ve_ray *ve_ray, t_ray *ray);
t_collision_info	collides_with_wall_with_type(t_cub3d *cube,
						int new_x, int new_y);
void				render_texture(t_cub3d *cube, t_texture_ren *ren_info,
						double y_pos, mlx_texture_t *texture);
void				toggle_door(t_cub3d *cube);
t_door				*get_door(t_door *door, int tile_x, int tile_y);
void				render_minimap(t_cub3d *cube);
void				move_player(t_cub3d *cube, t_move move);
void				sprite(t_cub3d *cube, bool first);
void				init_sprites(t_cub3d *cube);
bool				validate_map(t_map *map, t_cub3d *cube);

#endif 