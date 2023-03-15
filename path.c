/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tairribe <tairribe@student.42sp.org.br>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/08 12:43:18 by tairribe          #+#    #+#             */
/*   Updated: 2023/01/08 12:54:57 by tairribe         ###   ########.fr       */
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

char	*get_bin_path(char *bin_file, char **paths)
{
	char	*full_path;

	full_path = find_path(bin_file, paths);
	if (full_path == NULL)
	{
		// ft_dprintf(STDERR_FILENO, "Command not found: %s\n", bin_file);
		// exit(1);
		return (ft_strdup(bin_file));
	}
	return (full_path);
}
