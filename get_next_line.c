/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yben-abd <yben-abd@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/11/14 10:28:44 by yben-abd          #+#    #+#             */
/*   Updated: 2026/11/16 13:12:00 by yben-abd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static char *fill_stash(int fd, char *stash, char *buffer)
{
	int		byte_read;
	char	*tmp;

	byte_read = 1;
	while (byte_read > 0)
	{
		byte_read = read(fd, buffer, BUFFER_SIZE);
		if (byte_read == -1)
			return (NULL);
		if (byte_read == 0)
			break ;
		buffer[byte_read] = '\0';
		tmp = stash;
		stash = ft_strjoin(tmp, buffer);
		free(tmp);
		if (!stash)
			return (NULL);
		if (ft_strchr(stash))
			break ;
	}
	return (stash);
}

static char *extract_line(char *data)
{
	char	*line;
	int		i;
	
	if (!data)
		return (NULL);
	i = 0;
	while (data[i] && data[i] != '\n')
		i++;
	if(data[i] == '\n')
		i++;
	line = ft_substr(data, 0, i);
	return (line);
}

static char *stash_trim(char *data)
{
	int		i;
	char	*stash;
	
	if (!data)
		return (NULL);
	i = 0;
	while (data[i] && data[i] != '\n')
		i++;
	if(data[i] == '\n')
		i++;
	stash = ft_substr(data, i, ft_strlen(data) - i);
	return (stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*buffer;
	char 		*data;
	char 		*line;
	
	if (fd < 0 || BUFFER_SIZE <= 0)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	buffer = malloc(BUFFER_SIZE + 1);
	if (!buffer)
	{
		free(stash);
		stash = NULL;
		return (NULL);
	}
	data = fill_stash(fd, stash, buffer);
	free(buffer);
	line = extract_line(data);
	stash = stash_trim(data);
	free(data);
	return (line);
}
