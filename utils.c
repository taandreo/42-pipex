/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 20:09:21 by tairribe          #+#    #+#             */
/*   Updated: 2023/01/08 12:02:31 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error(char *msg)
{
	ft_dprintf(2, "Error: %s", msg);
	exit(EXIT_FAILURE);
}

void	usage(void)
{
	ft_dprintf(2, "usage: ./pipex file1 cmd1 cmd2 file2\n");
	exit(EXIT_FAILURE);
}

int	open_file(char *filename, int flag, int mode)
{
	int	fd;

	if (mode != 0)
		fd = open(filename, flag, mode);
	else
		fd = open(filename, flag);
	if (fd < 0)
	{
		perror(filename);
		exit (1);
	}
	return (fd);
}