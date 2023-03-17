/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 19:49:13 by tairribe          #+#    #+#             */
/*   Updated: 2023/03/17 03:05:47 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include "ft_printf.h"
# include <stdio.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <errno.h>

# define NOTEXEC 126
# define CMDNOTFOUND 127

typedef struct s_pipex {
	int		fd[2];
	char	*cmd_path;
	char	**cmd_args;
	int		pid0;
	int		pid1;
	char	**paths;
	char	**envp;
}			t_pipex;

// utils.c
void	usage(void);
int		open_file(t_pipex *pix, char *filename, int flag, int mode);
void	error(char *msg);
int		free_mt(void **mt);
void	free_pipex(t_pipex *pix);
// path.c
char	*find_path(char *bin_name, char **paths);
char	*get_env(char **envp, char *env_var);
char	*get_bin_path(t_pipex *pix);
// pipex2.c
char	**read_args(char *arg_str);
void	close_pipe(int fd[2]);
void	init_pipex(t_pipex *pix);

#endif