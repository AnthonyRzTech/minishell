#include "../../include/minishell.h"

void setup_redirection(int oldfd, int newfd, int *saved) {
  *saved = dup(oldfd);
  if (dup2(newfd, oldfd) == -1) {
    perror("dup2");
    exit(EXIT_FAILURE);
  }
  close(newfd);
}

#define TMP_FILENAME "/tmp/my_temp_file"

FILE *setup_append_redirection(char *delimiter) {
  char *buffer;
  int tempfile_fd;

  // Création du fichier temporaire avec open
  tempfile_fd =
      open(TMP_FILENAME, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if (tempfile_fd == -1) {
    perror("open");
    return NULL;
  }

  while ((buffer = readline("")) != NULL) {
    if (strncmp(buffer, delimiter, strlen(delimiter)) == 0 &&
        buffer[strlen(delimiter)] == '\n') {
      free(buffer);
      break;
    }
    // Écriture dans le fichier temporaire
    write(tempfile_fd, buffer, strlen(buffer));
    write(tempfile_fd, "\n",
          1); // Ajouter le caractère de nouvelle ligne manuellement
    free(buffer);
  }

  // Utilisation de lseek pour rembobiner le fichier
  lseek(tempfile_fd, 0, SEEK_SET);

  // Astuce pour obtenir un FILE * depuis le descripteur de fichier
  dup2(tempfile_fd,
       STDIN_FILENO); // Duplique le descripteur de fichier vers stdin
  close(tempfile_fd); // Ferme le descripteur de fichier original

  return stdin; // Retourne le flux stdin, qui est maintenant associé au fichier
                // temporaire
}

void cleanup_append_redirection(int saved_stdin, FILE *tempfile) {
  restore_fd(STDIN_FILENO, saved_stdin);
  fclose(tempfile);
}

void execute(t_node *root) {
  if (!root)
    return;

  if (root->type == NODE_COMMAND) {
    execute_command(root);
  } else if (root->type == NODE_PIPE) {
    execute_pipeline(root);
  } else if (root->type == NODE_REDIRECT_IN) {
    execute_redirect_in(root);
  } else if (root->type == NODE_REDIRECT_OUT) {
    execute_redirect_out(root);
  } else if (root->type == NODE_REDIRECT_IN_APPEND) {
    execute_redirect_in_append(root);
  } else if (root->type == NODE_REDIRECT_OUT_APPEND) {
    execute_redirect_out_append(root);
  } else {
    write(2, "Error: Unknown node type\n", 23);
  }
}
