/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/17 02:56:58 by tairribe          #+#    #+#             */
/*   Updated: 2023/03/17 03:02:29 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	**read_args(char *arg_str)
{
	char	**args;

	args = ft_split(arg_str, ' ');
	return (args);
}

void	close_pipe(int fd[2])
{
	close(fd[0]);
	close(fd[1]);
}

void	init_pipex(t_pipex *pix)
{
	pix->fd[0] = 0;
	pix->fd[1] = 0;
	pix->cmd_path = NULL;
	pix->cmd_args = NULL;
	pix->pid0 = 0;
	pix->pid1 = 0;
	pix->paths = NULL;
}
