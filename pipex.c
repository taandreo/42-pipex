/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 19:06:09 by tairribe          #+#    #+#             */
/*   Updated: 2023/03/17 03:01:55 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	fork0(t_pipex *pix, char *infile, char *cmd)
{
	int		infile_fd;

	close(pix->fd[0]);
	infile_fd = open_file(pix, infile, O_RDONLY, 0);
	pix->cmd_args = read_args(cmd);
	if (pix->cmd_args == NULL)
		error("Getting cmds.");
	pix->cmd_path = get_bin_path(pix);
	dup2(infile_fd, STDIN_FILENO);
	dup2(pix->fd[1], STDOUT_FILENO);
	close(infile_fd);
	if (execve(pix->cmd_path, pix->cmd_args, pix->envp) == -1)
	{
		perror(pix->cmd_args[0]);
		free_pipex(pix);
		exit(1);
	}
}

void	fork1(t_pipex *pix, char *outfile, char *cmd)
{
	int		outfile_fd;

	close(pix->fd[1]);
	outfile_fd = open_file(pix, outfile,
			O_WRONLY | O_CREAT | O_TRUNC, 0666);
	pix->cmd_args = read_args(cmd);
	if (pix->cmd_args == NULL)
		error("Getting cmds.");
	pix->cmd_path = get_bin_path(pix);
	dup2(pix->fd[0], STDIN_FILENO);
	dup2(outfile_fd, STDOUT_FILENO);
	close(outfile_fd);
	if (execve(pix->cmd_path, pix->cmd_args, pix->envp) != 1)
	{
		perror(pix->cmd_args[0]);
		free_pipex(pix);
		exit(1);
	}
}

void	wait_forks(t_pipex *pix)
{
	int	status;

	waitpid(pix->pid0, NULL, 0);
	waitpid(pix->pid1, &status, 0);
	if (WIFEXITED(status))
		exit(WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		ft_dprintf(1, "Child was terminated by signal: %i\n", WTERMSIG(status));
		exit(128 + WTERMSIG(status));
	}
}

void	make_forks(t_pipex *pix, char *argv[])
{
	int	pid;

	pid = fork();
	if (pid < 0)
		error("Forking process");
	if (pid == 0)
		fork0(pix, argv[1], argv[2]);
	pix->pid0 = pid;
	pid = fork();
	if (pid < 0)
		error("Forking process");
	if (pid == 0)
		fork1(pix, argv[4], argv[3]);
	pix->pid1 = pid;
	close_pipe(pix->fd);
	free_pipex(pix);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	pix;
	char	*path_env;

	if (argc != 5)
		usage();
	init_pipex(&pix);
	pix.envp = envp;
	path_env = get_env(envp, "PATH");
	if (path_env == NULL)
	{	
		ft_dprintf(STDERR_FILENO, "Error: not found PATH environment variable");
		exit(EXIT_FAILURE);
	}
	pix.paths = ft_split(path_env, ':');
	pipe(pix.fd);
	make_forks(&pix, argv);
	wait_forks(&pix);
	return (0);
}
