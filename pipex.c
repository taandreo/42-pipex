/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 19:06:09 by tairribe          #+#    #+#             */
/*   Updated: 2023/01/09 00:23:05 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

typedef struct s_pipex {
	int 	fd[2];
	int		fd_infile;
	int 	fd_outfile;
	int 	pid0;
	int 	pid1;
	// char	**cmd_args0
	// char	**cmd_args1
	char	**paths;
	char	**envp;
} t_pipex;

char	**read_args(char *arg_str)
{
	char	**args;
	args = ft_split(arg_str, ' ');
	return (args);
}

// void free_pipex(t_pipex *pix)
// {
// 	if (pix->cmd_args0)
// 		free_mt(pix->cmd_args0)
// 	if (pix->cmd_args1)
// 		free_mt(pix->cmd_args1)
// 	if (pix->paths)
// 		free_mt(pix->paths)
// }

int	exec_cmd(t_pipex *pix, int in_fd, int out_fd, char *cmd)
{
	char	**cmd_args;
	char	*full_path;
	int		pid;

	pid = fork();
	if (pid < 0)
		error("Forking process");
	if (pid == 0)
	{
		cmd_args = read_args(cmd);
		if (cmd_args == NULL)
			error("Getting cmds.");
		full_path = get_bin_path(cmd_args[0], pix->paths);
		dup2(in_fd, STDIN_FILENO);
		dup2(out_fd, STDOUT_FILENO);
		close(pix->fd[0]);
		close(pix->fd[1]);
		if (execve(full_path, cmd_args, pix->envp) == -1)
		{
			perror(cmd_args[0]);
			free(full_path);
			free_mt((void **) full_path);
		}
	}
	return (pid);
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
	pix->fd_infile = 0;
	pix->fd_outfile = 0;
	pix->pid0 = 0;
	pix->pid1 = 0;
	pix->paths = NULL;
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	pix;
	char	*path_env;
	int		status;
	
	if (argc != 5)
		usage();
	
	// pix = ft_calloc(1, sizeof(t_pipex));
	init_pipex(&pix);
	
	pix.envp = envp;
	path_env = get_env(envp, "PATH");
	if (path_env == NULL)
	{	
		ft_dprintf(STDERR_FILENO, "Error: not found PATH environment variable");
		exit(EXIT_FAILURE);
	}
	pix.paths = ft_split(path_env, ':');
	pix.fd_infile = open_file(argv[1], O_RDONLY, 0);
	pix.fd_outfile = open_file(argv[argc - 1], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	pipe(pix.fd);
	exec_cmd(&pix, pix.fd_infile, pix.fd[1], argv[2]);
	exec_cmd(&pix, pix.fd[0], pix.fd_outfile, argv[3]);
	close_pipe(pix.fd);
	waitpid(pix.pid0, NULL, 0);
	waitpid(pix.pid1, &status, 0);
	free_mt((void **) pix.paths);
	if (WIFEXITED(status))
		return(status);
	else if (WIFSIGNALED(status))
	{
		ft_dprintf(1, "Child was terminated by signal: %i\n", WTERMSIG(status));
		return(128 + WTERMSIG(status));
	} else
		return(0);
}

