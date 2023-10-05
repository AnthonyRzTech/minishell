// pwd.c
#include "../../include/minishell.h"

void pwd_command() {
  char cwd[1024];
  if (getcwd(cwd, sizeof(cwd)) != NULL) {
    printf("%s\n", cwd);
  } else {
    perror("pwd");
  }
}
