/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/28 19:06:09 by tairribe          #+#    #+#             */
/*   Updated: 2023/01/08 12:39:12 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

typedef struct s_pipex {
	int 	fd[2];
	int		fd_infile;
	int 	fd_outfile;
	int 	pid0;
	int 	pid1;
	char	**paths;
} t_pipex;

char	**read_args(char *arg_str)
{
	char	**args;

	args = ft_split(arg_str, ' ');
	return (args);
}

char	*find_path(char *bin_name, char **paths)
{
	int		i;
	char	*s1;
	char	*full_path;

	i = -1;
	while(paths[++i])
	{
		s1 = ft_strjoin("/", bin_name);
		full_path = ft_strjoin(paths[i], s1);
		if (access(full_path, X_OK) == 0)
		{
			free(s1);
			return (full_path);
		}
		free(full_path);
	}
	return (NULL);
}

char	*get_env(char **envp, char *env_var)
{
	int i;
	int len;

	i = -1;
	len = ft_strlen(env_var);
	
	while(envp[++i])
	{
		if (ft_strncmp(envp[i], env_var, len) == 0)
			return (envp[i] + len + 1);
	}
	return (NULL);
}

char	*get_bin_path(char *bin_file, char **paths)
{
	char *full_path;

	full_path =  find_path(bin_file, paths);
	ft_freemt((void **) paths);
	if (full_path == NULL)
	{
		ft_dprintf(STDERR_FILENO, "Command not found: %s\n", bin_file);
		exit(1);
	}
	return (full_path);
}

void	exec_cmd(t_pipex *pix, int in_fd, int out_fd, char *cmd)
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
		full_path = get_bin_path(cmd_args[0], pix->paths);
		dup2(in_fd, STDIN_FILENO);
		dup2(out_fd, STDOUT_FILENO);
		close(pix->fd[0]);
		close(pix->fd[1]);
		if (execve(full_path, cmd_args, NULL) == -1)
		{
			perror(cmd_args[0]);
			exit(EXIT_FAILURE);	
		}	
	}
}

int	main(int argc, char *argv[], char *envp[])
{
	t_pipex	*pix;
	char	*path_env;
	
	if (argc != 5)
		usage();

	pix = ft_calloc(1, sizeof(t_pipex));
	path_env = get_env(envp, "PATH");
	if (path_env == NULL)
		ft_dprintf(STDERR_FILENO, "Error: not found PATH environment variable");
	pix->paths = ft_split(path_env, ':');

	pix->fd_infile = open_file(argv[1], O_RDONLY, 0);
	pix->fd_outfile = open_file(argv[4], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	pipe(pix->fd);
	exec_cmd(pix, pix->fd_infile, pix->fd[1], argv[2]);
	exec_cmd(pix, pix->fd[0], pix->fd_outfile, argv[3]);
	wait(NULL);
	ft_printf("Done!\n");
	return (0);
}

