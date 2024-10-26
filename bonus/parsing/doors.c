/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   doors.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aboukdid <aboukdid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/17 19:44:13 by mboukour          #+#    #+#             */
/*   Updated: 2024/10/23 21:36:38 by aboukdid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"

t_door	*ft_lstnew_door(int door_x, int door_y)
{
	t_door	*new_node;

	new_node = malloc(sizeof(t_door));
	if (!new_node)
		return (NULL);
	new_node->door_x = door_x;
	new_node->door_y = door_y;
	new_node->is_open = false;
	new_node->is_in_fov = true;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

void	ft_lstclear_door(t_door *door)
{
	t_door	*tmp;

	tmp = NULL;
	while (door)
	{
		tmp = door;
		door = door->next;
		free(tmp);
	}
}

t_door	*ft_lstlast_door(t_door *lst)
{
	t_door	*temp;

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

void	ft_lstaddback_door(t_door **head, t_door *new)
{
	t_door	*last;

	if (!head)
		return ;
	if (!*head)
	{
		*head = new;
		return ;
	}
	last = ft_lstlast_door(*head);
	last->next = new;
	new->prev = last;
}

int	ft_lstsize_door(t_door *door)
{
	int	i;

	i = 0;
	while (door)
	{
		i++;
		door = door->next;
	}
	return (i);
}
