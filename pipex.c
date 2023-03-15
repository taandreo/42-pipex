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

// int	exec_cmd(t_pipex *pix, int in_fd, int out_fd, char *cmd)
// {
// 	char	**cmd_args;
// 	char	*full_path;
// 	int		pid;

// 	pid = fork();
// 	if (pid < 0)
// 		error("Forking process");
// 	if (pid == 0)
// 	{
// 		cmd_args = read_args(cmd);
// 		if (cmd_args == NULL)
// 			error("Getting cmds.");
// 		full_path = get_bin_path(cmd_args[0], pix->paths);
// 		dup2(in_fd, STDIN_FILENO);
// 		dup2(out_fd, STDOUT_FILENO);
// 		close_pipe()
// 		close(in_fd);
// 		close(out_fd);
// 		if (execve(full_path, cmd_args, pix->envp) == -1)
// 		{
// 			perror(cmd_args[0]);
// 			free(full_path);
// 			free_mt((void **) cmd_args);
// 		}
// 	}
// 	return (pid);
// }

int	fork0(t_pipex *pix, char *infile, char *cmd)
{
	char	**cmd_args;
	char	*full_path;
	int		infile_fd;
	int		pid;

	pid = fork();
	if (pid < 0)
		error("Forking process");
	if (pid == 0)
	{
		close(pix->fd[0]);
		infile_fd = open_file(pix, infile, O_RDONLY, 0);
		cmd_args = read_args(cmd);
		if (cmd_args == NULL)
			error("Getting cmds.");
		full_path = get_bin_path(cmd_args[0], pix->paths);
		dup2(infile_fd, STDIN_FILENO);
		dup2(pix->fd[1], STDOUT_FILENO);
		close(infile_fd);
		if (execve(full_path, cmd_args, pix->envp) == -1)
		{
			perror(cmd_args[0]);
			free(full_path);
			free_mt((void **) cmd_args);
			free_pipex(pix);
		}
	}
	return (pid);
}

int	fork1(t_pipex *pix, char *outfile, char *cmd)
{
	char	**cmd_args;
	char	*full_path;
	int		outfile_fd;
	int		pid;

	pid = fork();
	if (pid < 0)
		error("Forking process");
	if (pid == 0)
	{
		close(pix->fd[1]);
		outfile_fd = open_file(pix, outfile, O_WRONLY | O_CREAT | O_TRUNC, 0666);
		cmd_args = read_args(cmd);
		if (cmd_args == NULL)
			error("Getting cmds.");
		full_path = get_bin_path(cmd_args[0], pix->paths);
		dup2(pix->fd[0], STDIN_FILENO);
		dup2(outfile_fd, STDOUT_FILENO);
		close(outfile_fd);
		if (execve(full_path, cmd_args, pix->envp) != 1)
		{
			perror(cmd_args[0]);
			free(full_path);
			free_mt((void **) cmd_args);
			free_pipex(pix);
			exit(1);
		}
	}
	return (pid);
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
	pix.pid0 = fork0(&pix, argv[1], argv[2]);
	pix.pid1 = fork1(&pix, argv[4], argv[3]);
	close_pipe(pix.fd);
	waitpid(pix.pid0, NULL, 0);
	waitpid(pix.pid1, &status, 0);
	free_pipex(&pix);
	if (WIFEXITED(status))
		return(WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
	{
		ft_dprintf(1, "Child was terminated by signal: %i\n", WTERMSIG(status));
		return(128 + WTERMSIG(status));
	} else
		return(0);
}

