/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthrodr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 11:25:16 by anthrodr          #+#    #+#             */
/*   Updated: 2023/10/09 12:01:05 by anthrodr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../include/minishell.h"
int g_sigint_called = 0;





void handle_sigint(int sig)
{
    (void)sig;
    rl_on_new_line(); // Move to a new line for readline
    rl_replace_line("", 0);  // Clear the current line

    write(STDOUT_FILENO, "\nminishell> ", 12);
    rl_on_new_line_with_prompt();  // Move the cursor back to prompt
    rl_redisplay();

    g_sigint_called = 1;  // Set this so the main loop can be aware.
}






void	handle_sigquit(int sig)
{
	(void)sig;
}

void	initialize_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);
	sa_quit.sa_handler = handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	process_input_line(char *input)
{
	t_token	*tokens;
	t_node	*root;

	if (!input)
	{
		write(STDOUT_FILENO, "\n", 1);
		exit(0);
	}
	add_history(input);
	input = expand_env_variables(input);
	tokens = tokenize_with_quotes(input);
//  print_lexer(tokens);
	if (tokens != NULL)
	{
		root = parse(tokens);
  //  print_parser(root);
		execute(root);
		free_tree(root);
	}
	free(input);
}


int main(void)
{
    char *input;

    initialize_signal_handlers();

    while (1)
    {
        if (!g_sigint_called)
        {
            input = readline("minishell> ");
        }
        else
        {
            input = readline("");  // No prompt since handle_sigint already displayed it.
            g_sigint_called = 0;  // Reset flag
        }

        if (!input)  // If the user presses CTRL+D
        {
            write(STDOUT_FILENO, "\n", 1);
            exit(0);
        }

        process_input_line(input);
        free(input);  // Free the memory allocated by readline
    }

    return (0);
}

