/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   colors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 18:22:00 by aboukdid          #+#    #+#             */
/*   Updated: 2024/10/23 21:02:55 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

bool	is_valid_number(char *str, int *i)
{
	if (str[*i] == '+' || str[*i] == '-')
		(*i)++;
	if (!is_digit(str[*i]))
		return (false);
	while (str[*i] && str[*i] != ',')
	{
		if (!is_digit(str[*i]))
			return (false);
		(*i)++;
	}
	return (true);
}

bool	is_good_color(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (str[0] == ',')
		return (false);
	while (str[i])
	{
		if (!is_valid_number(str, &i))
			return (false);
		if (str[i] == ',')
		{
			count++;
			i++;
			if (str[i] == ',' || str[i] == '\0')
				return (false);
		}
	}
	if (count != 2)
		return (false);
	return (true);
}

bool	parse_color(char *str, int *r, int *g, int *b)
{
	while (*str && is_ws(*str))
		str++;
	if (!is_good_color(str))
		return (print_err("Invalid color format."), false);
	*r = ft_atoi(str);
	while (*str && is_digit(*str))
		str++;
	str++;
	*g = ft_atoi(str);
	while (*str && is_digit(*str))
		str++;
	str++;
	*b = ft_atoi(str);
	if ((*r < 0 || *r > 255) || (*g < 0 || *g > 255) || (*b < 0 || *b > 255))
		return (print_err("Invalid color range"), false);
	return (true);
}

void	set_cub_color(t_cub3d *cube, int color[3], int c_type)
{
	if (c_type == F)
	{
		cube->floor_r = color[0];
		cube->floor_g = color[1];
		cube->floor_b = color[2];
		cube->floor = get_color(color[0], color[1], color[2], 255);
	}
	else
	{
		cube->ceiling_r = color[0];
		cube->ceiling_g = color[1];
		cube->ceiling_b = color[2];
		cube->ceiling = get_color(color[0], color[1], color[2], 255);
	}
}

bool	set_color(char *str, t_cub3d *cube, int c_type)
{
	int	color[3];

	if (do_i_exist(cube, c_type))
		return (print_err("Duplicate parameters are not accepted!"), false);
	str++;
	while (*str && is_ws(*str))
		str++;
	if (!parse_color(str, &color[0], &color[1], &color[2]))
		return (false);
	set_cub_color(cube, color, c_type);
	return (true);
}
