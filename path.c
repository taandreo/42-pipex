/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 12:43:18 by tairribe          #+#    #+#             */
/*   Updated: 2023/03/17 03:03:16 by tairribe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

char	*find_path(char *bin_name, char **paths)
{
	int		i;
	char	*s1;
	char	*full_path;

	i = -1;
	while (paths[++i])
	{
		s1 = ft_strjoin("/", bin_name);
		full_path = ft_strjoin(paths[i], s1);
		if (access(full_path, X_OK) == 0)
		{
			free(s1);
			return (full_path);
		}
		free(s1);
		free(full_path);
	}
	return (NULL);
}

char	*get_env(char **envp, char *env_var)
{
	int	i;
	int	len;

	i = -1;
	len = ft_strlen(env_var);
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], env_var, len) == 0)
			return (envp[i] + len + 1);
	}
	return (NULL);
}

void	get_local_path(t_pipex *pix)
{
	if (access(pix->cmd_args[0], X_OK) == 0)
		return ;
	if (access(pix->cmd_args[0], F_OK) == 0)
	{
		ft_dprintf(STDERR_FILENO, "bash: %s: ", pix->cmd_args[0]);
		ft_dprintf(STDERR_FILENO, "Permission denied\n");
		free_pipex(pix);
		exit(NOTEXEC);
	}
	ft_dprintf(STDERR_FILENO, "bash: %s: ", pix->cmd_args[0]);
	ft_dprintf(STDERR_FILENO, " No such file or directory\n");
	free_pipex(pix);
	exit(CMDNOTFOUND);
}

char	*get_bin_path(t_pipex *pix)
{
	char	*full_path;

	if (pix->cmd_args[0] == NULL)
	{
		ft_dprintf(STDERR_FILENO, "bash: '': command not found\n");
		free_pipex(pix);
		exit(CMDNOTFOUND);
	}
	if (ft_strchr(pix->cmd_args[0], '/'))
	{
		get_local_path(pix);
		return (ft_strdup(pix->cmd_args[0]));
	}
	full_path = find_path(pix->cmd_args[0], pix->paths);
	if (full_path == NULL)
	{
		ft_dprintf(STDERR_FILENO, "%s: ", pix->cmd_args[0]);
		ft_dprintf(STDERR_FILENO, "command not found\n");
		free_pipex(pix);
		exit(CMDNOTFOUND);
	}
	return (full_path);
}
