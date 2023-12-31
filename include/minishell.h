/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anthrodr <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 12:06:05 by anthrodr          #+#    #+#             */
/*   Updated: 2023/10/09 12:08:57 by anthrodr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>
# define TMP_FILENAME "/tmp/my_temp_file"

extern int			g_sigint_called;
typedef struct s_env
{
	char			*value;
	struct s_env	*next;
}					t_env;
t_env				*copy_environ_to_list(char **environorig);
// -- -- -- -- -- -- -- -- - LEXER -- -- -- -- -- -- -- -- -- -- -

typedef enum e_token_type
{
	TOKEN_COMMAND,
	TOKEN_ARGUMENT,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_IN_APPEND,
	TOKEN_REDIRECT_OUT_APPEND,
	TOKEN_ERROR,
	TOKEN_EOF
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
}					t_token;

typedef struct s_lexer
{
	char			*current;
	t_token			*tokens;
	int				tokencount;
	char			*buffer;
	int				bufidx;
	int				error;
}					t_lexer;

t_token				*tokenize_with_quotes(char *input);
void				free_tokens(t_token *tokens);
void				handle_escape_char(t_lexer *lexer);
void				handle_quoted_string(t_lexer *lexer, char quote_char);
void				handle_space_char(t_lexer *lexer);
void				handle_pipe(t_lexer *lexer);
void				handle_redirect_in(t_lexer *lexer);
void				handle_redirect_out(t_lexer *lexer);
void				handle_token_end(t_lexer *lexer);
void				handle_special_char(t_lexer *lexer, t_token_type type);

//-- -- -- -- -- -- -- -- -- -- PARSER -- -- -- -- -- -- -- -- -- -- -

typedef enum e_nodetype
{
	NODE_COMMAND,
	NODE_ARGUMENT,
	NODE_PIPE,
	NODE_REDIRECT_IN,
	NODE_REDIRECT_OUT,
	NODE_REDIRECT_IN_APPEND,
	NODE_REDIRECT_OUT_APPEND,
}					t_nodetype;

typedef struct s_node
{
	t_nodetype		type;
	char			*value;
	struct s_node	*left;
	struct s_node	*right;
}					t_node;
t_node				*create_node(t_nodetype type, char *value);
void				free_tree(t_node *root);
t_node				*parse_command(t_token *tokens, int *index);
t_node				*parse_redirect(t_token *tokens, int *index,
						t_nodetype redirect_type);
t_node				*parse_pipeline(t_token *tokens, int *index);
t_node				*parse(t_token *tokens);
void				free_tree(t_node *root);
//-- -- -- -- -- -- -- -- -- -- EXPANDER -- -- -- -- -- -- -- -- -- --
char				*get_value_from_env_list(char *var_name, t_env *env_list);
typedef struct s_expander
{
	const char		*current;
	int				in_single_quotes;
	int				in_double_quotes;
	char			*write_pos;
	size_t			buffer_size;
	char			*expanded_str;
}					t_expander;
void				init_expander(t_expander *exp, char *input);
void				handle_single_quote(t_expander *exp);
void				handle_double_quote(t_expander *exp);
void				get_env_var_name(t_expander *exp, char *var_name);
void				expand_env_var(t_expander *exp, t_env *env_list);
char				*expand_env_variables(char *input, t_env *env_list);
//------------------------------EXECUTOR-------------------------------
typedef struct s_pipeline_data
{
	int				*pipe_fd;
	t_node			*root;
	int				*saved_stdin;
	t_env			*env_list;
}					t_pipeline_data;
void				execute(t_node *root, t_env *env_list);
void				cleanup_append_redirection(int saved_stdin, FILE *tempfile);
FILE				*setup_append_redirection(char *delimiter);
void				setup_redirection(int oldfd, int newfd, int *saved);
int					open_file_append(const char *path);
int					open_file_write(const char *path);
int					open_file_read(const char *path);
void				execute_redirect_out_append(t_node *root, t_env *env_list);
void				execute_redirect_out(t_node *root, t_env *env_list);
void				execute_redirect_in_append(t_node *root, t_env *env_list);
void				execute_redirect_in(t_node *root, t_env *env_list);
void				parent_pipeline(pid_t pid, t_pipeline_data *data);
void				child_pipeline(int *pipe_fd, t_node *root, t_env *env_list);
void				execute_pipeline(t_node *root, t_env *env_list);
int					wait_for_child(pid_t pid);
void				execute_command(t_node *node, t_env *env_list);
void				setup_pipe(int *pipe_fd);
void				restore_fd(int oldfd, int saved);
void				close_pipes(int *pipe_fd);
void				dup_to(int oldfd, int newfd, int *saved);
char				**create_args_from_ast(t_node *node);
int					calculate_ast_depth(t_node *node);
char				**populate_args_from_ast(char **args, t_node *node,
						int depth);
int					handle_builtin_commands(t_node *node, t_env *env_list);
void				execute_external_command(t_node *node, t_env *env_list);
//------------------------------BUILTINS-------------------------------

void				cd_command(t_node *commandnode);
void				echo_command(t_node *commandnode);
void				pwd_command(void);

void				export_command(t_node *commandNode, t_env *env_list);
void				print_env_vars(void);
char				*create_new_entry(char *key, char *value);
void				replace_existing_var(t_env *env_list, char *key,
						char *value);
t_env				*is_key_present(t_env *env_list, char *key, int len);
void				add_new_env_var(t_env **env_list, char *key, char *value);
void				handle_var_set(t_env **env_list, char *key, char *value);
void				set_env_var(t_node *commandnode, t_env **env_list);

void				unset_command(t_node *commandnode, t_env **env_list);
void				env_command(t_env *env_list);
void				exit_command(void);

//-----------------------------UTILS------------------------------------
int					ft_strcmp(const char *s1, const char *s2);
int					ft_execvp(const char *file, char *const argv[],
						char **copyenv);
char				*ft_strncpy(char *dest, const char *src, unsigned int n);
char				*ft_strcpy(char *dest, char *src);
char				*ft_strcat(char *dest, const char *src);
void				handle_sigint(int sig);
void				handle_sigquit(int sig);
void				initialize_signal_handlers(void);
int					handle_pwd_command(t_node *node);
int					handle_unset_command(t_node *node, t_env *env_list);
int					handle_export_command(t_node *node, t_env *env_list);
#endif
