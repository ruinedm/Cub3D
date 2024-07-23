/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/22 17:27:10 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/23 20:04:13 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

#include <mlx.h>
# include <fcntl.h>
# include <limits.h>
# include <stdlib.h>
# include <unistd.h>


typedef struct	s_cub3d
{
	char	*no_path;
	char	*so_path;
	char	*we_path;
	char	*ea_path;
	int		floor_r;
	int		floor_g;
	int		floor_b;
} t_cub3d;


char	*get_next_line(int fd);


// PARSING
int		parser(t_cub3d *program, char *map_name);
void	print_parsing_error(char *str);


// UTILS
size_t	ft_strlen(const char *s);
void	ft_putstr_fd(int fd, char *str);
char	*ft_strchr(char *s, int c);
char	*ft_strdup(char *s1);
char	*ft_substr(const char *s, unsigned int start, size_t len);
char	*ft_strjoin(char *s1, char *s2);
int		ft_strcmp(const char *s1, const char *s2);

#endif