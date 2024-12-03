/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzutter <mzutter@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/01 11:13:50 by mzutter           #+#    #+#             */
/*   Updated: 2024/12/03 11:19:39 by mzutter          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

char	*check_storage(char *storage)
{
	if (!storage)
		storage = ft_strdup("");
	return (storage);
}

static char	*free_storage(char *storage_fd)
{
	free(storage_fd);
	storage_fd = NULL;
	return (NULL);
}

static char	*set_storage(int fd, char *storage, char *buffer)
{
	char	*str;
	ssize_t	read_value;

	read_value = 1;
	while (read_value > 0)
	{
		read_value = read(fd, buffer, BUFFER_SIZE);
		if (read_value <= 0)
		{
			if (read_value == 0)
				break ;
			return (NULL);
		}
		buffer[read_value] = 0;
		storage = check_storage(storage);
		if (!storage)
			return (NULL);
		str = storage;
		storage = ft_strjoin(str, buffer);
		free (str);
		str = NULL;
		if (ft_strchr(buffer, '\n'))
			break ;
	}
	return (storage);
}

static char	*extract_line(char *next_line)
{
	char	*storage;
	size_t	i;

	i = 0;
	while (next_line[i] != '\n' && next_line[i] != 0)
		i++;
	if (next_line[i] == 0 || next_line[i + 1] == 0)
		return (NULL);
	storage = ft_substr(next_line, i + 1, ft_strlen(next_line) - i);
	if (!storage)
	{
		free(storage);
		storage = NULL;
	}
	next_line[i + 1] = 0;
	return (storage);
}

char	*get_next_line(int fd)
{
	static char	*storage[MAX_FD];
	char		*buffer;
	char		*next_line;

	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	if (fd < 0 || BUFFER_SIZE <= 0 || fd > MAX_FD)
	{
		free(buffer);
		buffer = NULL;
		if (fd > 0 && storage[fd] != NULL)
			free_storage(storage[fd]);
		return (NULL);
	}
	next_line = set_storage(fd, storage[fd], buffer);
	free(buffer);
	if (!next_line)
	{
		free(storage[fd]);
		storage[fd] = NULL;
		return (NULL);
	}
	storage[fd] = extract_line(next_line);
	return (next_line);
}
