/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mboukour <mboukour@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/03 11:29:09 by mboukour          #+#    #+#             */
/*   Updated: 2024/07/23 19:49:22 by mboukour         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3D.h"


size_t	ft_strlen(const char *s)
{
	int		i;
	size_t	length;

	i = 0;
	length = 0;
	while (s[i])
	{
		length++;
		i++;
	}
	return (length);
}

char	*ft_strchr(char *s, int c)
{
	char	x;
	size_t	i;

	if (!s)
		return (NULL);
	x = (char)c;
	i = 0;
	while (s[i])
	{
		if (s[i] == x)
			return (&s[i]);
		i++;
	}
	if (x == '\0')
		return (&s[i]);
	return (NULL);
}

char	*ft_strdup(char *s1)
{
	char	*copy;
	size_t	i;
	size_t	len;

	i = 0;
	len = ft_strlen(s1);
	copy = (char *)malloc(sizeof(char) * (len + 1));
	if (!copy)
		return (NULL);
	while (i < len)
	{
		copy[i] = s1[i];
		i++;
	}
	copy[i] = '\0';
	return (copy);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*substr;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= ft_strlen(s))
		return (ft_strdup(""));
	if (len > ft_strlen(s + start))
		len = ft_strlen(s + start);
	substr = malloc((len + 1) * sizeof(char));
	if (!substr)
		return (NULL);
	while (i < len)
	{
		substr[i] = s[start + i];
		i++;
	}
	substr[i] = '\0';
	return (substr);
}

char	*ft_strjoin(char *s1, char *s2)
{
	char	*finalstr;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1)
		return (ft_strdup(s2));
	else if (!s2)
		return (ft_strdup(s1));
	i = 0;
	j = 0;
	finalstr = malloc(ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!finalstr)
	{
		return (NULL);
	}
	while (s1[i])
		finalstr[j++] = s1[i++];
	i = 0;
	while (s2[i])
		finalstr[j++] = s2[i++];
	finalstr[j] = '\0';
	return (finalstr);
}
