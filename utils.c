/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 20:09:21 by tairribe          #+#    #+#             */
/*   Updated: 2023/03/17 03:03:45 by tairribe         ###   ########.fr       */
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

int	free_mt(void **mt)
{
	int	i;

	i = 0;
	while (mt[i])
		free(mt[i++]);
	free(mt);
	mt = NULL;
	return (i);
}

void	free_pipex(t_pipex *pix)
{
	if (pix->paths)
		free_mt((void **) pix->paths);
	if (pix->cmd_args)
		free_mt((void **) pix->cmd_args);
	if (pix->cmd_path)
		free(pix->cmd_path);
}

int	open_file(t_pipex *pix, char *filename, int flag, int mode)
{
	int	fd;

	if (mode != 0)
		fd = open(filename, flag, mode);
	else
		fd = open(filename, flag);
	if (fd < 0)
	{
		free_pipex(pix);
		perror(filename);
		exit(errno);
	}
	return (fd);
}
