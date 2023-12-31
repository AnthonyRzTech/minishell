/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executor7.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthrodr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 11:35:31 by anthrodr          #+#    #+#             */
/*   Updated: 2023/10/09 11:35:34 by anthrodr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../../gc/gc.h"
#include "../../include/minishell.h"

int	handle_pwd_command(t_node *node)
{
	if (ft_strcmp(node->value, "pwd") == 0)
	{
		pwd_command();
		return (1);
	}
	return (0);
}

int	handle_unset_command(t_node *node, t_env *env_list)
{
	if (ft_strcmp(node->value, "unset") == 0)
	{
		unset_command(node, &env_list);
		return (1);
	}
	return (0);
}

int	handle_export_command(t_node *node, t_env *env_list)
{
	if (ft_strcmp(node->value, "export") == 0)
	{
		export_command(node, env_list);
		return (1);
	}
	return (0);
}

void	execute_command(t_node *node, t_env *env_list)
{
	if (!handle_builtin_commands(node, env_list))
		execute_external_command(node, env_list);
}
