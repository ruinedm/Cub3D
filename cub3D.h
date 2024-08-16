/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:10 by mboukour          #+#    #+#             */
/*   Updated: 2024/08/16 21:36:34 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include <mlx.h>
# include <fcntl.h>
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include <stdio.h>
# include <limits.h>

# define NONE -1

enum e_map_options
{
	C,
	NO,
	SO,
	WE,
	EA,
	F,
	MAP
};

typedef struct	s_map
{
	char			*current_line;
	int				line_len;
	struct s_map	*prev;
	struct s_map	*next;
} t_map;

typedef struct	s_cub3d
{
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
	t_map	*map;
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
#endif