/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ll_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:14:31 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/25 02:17:38 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

static char	*line_with_no_ws(char *str)
{
	int i;
	int real_size;
	char *real;

	i = 0;
	real_size = 0;
	while(str[i])
	{
		if(!is_ws(str[i]))
			real_size++;
		i++;
	}
	real = malloc(real_size + 1);
	i = 0;
	real_size = 0;
	while(str[i])
	{
		if(!is_ws(str[i]))
		{
			real[real_size] = str[i];
			real_size++;
		}
		i++;
	}
	real[real_size] = '\0';
	return (real);
}

t_map	*ft_lstnew_mapline(char *line)
{
	t_map	*new_node;

	new_node = malloc(sizeof(t_map));
	if (!new_node)
		return (NULL);
	new_node->current_line = line_with_no_ws(line);
	new_node->line_len = ft_strlen(line);
	new_node->next = NULL;
	new_node->next = NULL;
	return (new_node);
}

t_map	*ft_lstlast_mapline(t_map *lst)
{
	t_map	*temp;

	if (lst == NULL)
		return (NULL);
	temp = lst;
	while (temp)
	{
		if (!temp->next)
			return (temp);
		temp = temp->next;
	}
	return (lst);
}

void	ft_lstaddback_mapline(t_map **head, t_map *new)
{
	if (!head)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	ft_lstlast_mapline(*head)->next = new;
}

void ft_lstiter_mapline(t_map *head)
{
	while(head)
	{
		printf("%s\n", head->current_line);
		head = head->next;
	}
}
