/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthrodr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 11:38:41 by anthrodr          #+#    #+#             */
/*   Updated: 2023/10/09 11:38:43 by anthrodr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../gc/gc.h"
#include "../../include/minishell.h"

int	get_environ_length(char **environorig)
{
	int	i;

	i = 0;
	while (environorig[i])
		i++;
	return (i);
}

char	**alloc_environ_copy(int length)
{
	char	**copie;

	copie = (char **)gc_malloc((length + 1) * sizeof(char *));
	if (!copie)
	{
		perror("Erreur d'allocation mémoire");
		exit(1);
	}
	return (copie);
}

char	**copieenviron(char **environorig)
{
	int		i;
	int		j;
	char	**copie;

	i = get_environ_length(environorig);
	copie = alloc_environ_copy(i);
	j = 0;
	while (j < i)
	{
		copie[j] = ft_strdup(environorig[j]);
		if (!copie[j])
		{
			perror("Erreur d'allocation mémoire");
			exit(1);
		}
		j++;
	}
	copie[i] = NULL;
	return (copie);
}
