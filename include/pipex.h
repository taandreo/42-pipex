/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 19:49:13 by tairribe          #+#    #+#             */
/*   Updated: 2023/01/08 21:07:20 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include "ft_printf.h"
#include <stdio.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>

typedef struct s_pipex {
	int 	fd[2];
	int		fd_infile;
	int 	fd_outfile;
	int 	pid0;
	int 	pid1;
	char	**paths;
	char	**envp;
} t_pipex;

// utils.c
void	usage(void);
int	    open_file(t_pipex *pix, char *filename, int flag, int mode);
void	error(char *msg);
int     free_mt(void **mt);
void    free_pipex(t_pipex *pix);
// path.c
char	*find_path(char *bin_name, char **paths);
char	*get_env(char **envp, char *env_var);
char	*get_bin_path(char *bin_file, char **paths);

#endif