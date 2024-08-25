/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:10 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/25 04:56:56 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include <fcntl.h>
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h> 
# include <limits.h>
# include <math.h>
#include <stdio.h>

# define NONE -1
# define WIDTH 1280
# define HEIGHT 920
# define TILE_SIZE 32
# define BLACK 0x000000FF
# define WHITE 0xFFFFFFFF
# define RED 0xFF0000FF
#define PI 3.14159265

enum e_parse_options
{
    C,
    NO,
    SO,
    WE,
    EA,
    F
};


enum e_map_params
{
    VOID = '0',
    WALL = '1',
    PLAYER_E = 'E',
    PLAYER_W = 'W',
    PLAYER_N = 'N',
    PLAYER_S = 'S'
};


typedef struct	s_player
{
	int x;
	int y;
	int angle;
	int turn_direction;
	int walk_direction;
	double rotation_angle;
	int movement_speed;
	double rotation_speed;
} t_player;

typedef struct	s_map
{
	char			*current_line;
	int				line_len;
	struct s_map	*prev;
	struct s_map	*next;
} t_map;

typedef struct	s_cub3d
{
	void	*mlx;
	void*	image;
	void*	context;
	int		width;
	int		height;
	double	delta_time;
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	int		floor_r;
	int		floor_g;
	int		floor_b;
	int		ceiling_r;
	int		ceiling_g;
	int		ceiling_b;
	int		map_fd;
	int		x_len;
	int		y_len;
	bool	initial;
	t_map	*map;
	t_player player;
} t_cub3d;



// PARSING
int		parser(t_cub3d *cube, char *map_name);
void	print_parsing_error(char *str);
char	*get_next_line(int fd);


// UTILS
size_t	ft_strlen(const char *s);
void	ft_putstr_fd(int fd, char *str);
char	*ft_strchr(char *s, int c);
char	*ft_strdup(char *s1);
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strjoin(char *s1, char *s2);
int		ft_strcmp(const char *s1, const char *s2);
bool	is_ws(char c);
int		is_digit(int c);
int		ft_atoi(const char *str);
t_map	*ft_lstnew_mapline(char *line);
t_map	*ft_lstlast_mapline(t_map *lst);
void	ft_lstaddback_mapline(t_map **head, t_map *new);
void	ft_lstiter_mapline(t_map *head);
int		ft_lstsize_mapline(t_map *map);
int		max_len(t_map *map);
#endif