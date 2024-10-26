/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ll_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/24 22:14:31 by mboukour          #+#    #+#             */
/*   Updated: 2024/10/23 21:32:20 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

t_map	*ft_lstnew_mapline(char *line)
{
	t_map	*new_node;

	new_node = malloc(sizeof(t_map));
	if (!new_node)
		return (NULL);
	new_node->current_line = line;
	new_node->line_len = ft_strlen(line);
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

void	ft_lstclear_mapline(t_map *map)
{
	t_map	*tmp;

	while (map)
	{
		free(map->current_line);
		tmp = map;
		map = map->next;
		free(tmp);
	}
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
	t_map	*last;

	if (!head)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	last = ft_lstlast_mapline(*head);
	last->next = new;
	new->prev = last;
}

int	ft_lstsize_mapline(t_map *map)
{
	int	i;

	i = 0;
	while (map)
	{
		i++;
		map = map->next;
	}
	return (i);
}
