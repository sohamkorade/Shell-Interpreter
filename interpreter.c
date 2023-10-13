#include "interpreter.h"

void sanitize_input(char *input) {
  char *temp0 = new_string();
  char *temp1 = new_string();
  char *temp2 = new_string();
  join_str3(temp0, " ", input, " ");
  join_str3(temp1, " ", launch_dir, " ");
  replace_str(temp0, " ~ ", temp1);
  replace_str(temp0, "&", " & ");
  replace_str(temp0, "|", " | ");
  replace_str(temp0, "<", " < ");
  replace_str(temp0, ">>", "$$$append$$$");
  replace_str(temp0, ">", " > ");
  replace_str(temp0, "$$$append$$$", " >> ");
  join_str3(temp1, " ", launch_dir, "/");
  replace_str(temp0, " ~/", temp1);
  if (strncmp(temp0, " dalias ", 8))
    for (int i = 0; i < alias_n; i++) {
      join_str3(temp1, " ", alias[i][0], " ");
      join_str3(temp2, " ", alias[i][1], " ");
      if (!strncmp(temp0, temp1, strlen(temp1))) {
        join_str2(temp_str, temp2, temp0 + strlen(temp1));
        strcpy(temp0, temp_str);
      }
    }
  strcpy(input, temp0);
  free(temp0);
  free(temp1);
  free(temp2);
}

void interpret_cmd(char *cmd, int stdin2, int stdout2) {
  char *context, *token, *str;  // words
  int filemode = -1;

  int reader = -1, writer = -1;
  for (str = cmd;; str = NULL) {  // loop through words
    if (!(token = strtok_r(str, " \t", &context))) break;
    // printf("  > {%s}\n", token);
    if (filemode > -1) {
      if (filemode == O_RDONLY) {
        reader = open(token, filemode);
        if (reader == -1 || dup2(reader, STDIN_FILENO) < 0) {
          color_perror(red, "cannot redirect from file");
          close(reader);
          return;
        }
      } else {
        writer = open(token, filemode, 0644);
        if (writer == -1 || dup2(writer, STDOUT_FILENO) < 0) {
          color_perror(red, "cannot redirect to file");
          close(writer);
          return;
        }
      }
      filemode = -2;
    } else if (!strcmp(token, "<")) {  // input redirection
      filemode = O_RDONLY;
    } else if (!strcmp(token, ">")) {  // output redirection
      filemode = O_WRONLY | O_CREAT | O_TRUNC;
    } else if (!strcmp(token, ">>")) {  // output redirection with append
      filemode = O_WRONLY | O_CREAT | O_APPEND;
    } else if (!strcmp(token, "&")) {  // run command in background
      run_bg();
      args_n = 0;
      filemode = -1;
    } else {
      if (token[0] == '-')  // populate switches
        for (int i = 1; i < strlen(token); i++)
          if (token[i] >= 0 && token[i] < 128) switches[token[i]]++;
      args[args_n++] = token;
      filemode = -1;
    }
  }
  run(0);
  args_n = 0;
  close(reader), close(writer);

  // // reset i/o state
  // dup2(stdin2, STDIN_FILENO);
  // dup2(stdout2, STDOUT_FILENO);
}

void pipe_cmd(char *cmd, int stdin2, int stdout2, int pipe_in, int pipe_out,
              int is_first, int is_last) {
  // attach pipe to i/o
  if (!is_first) dup2(pipe_in, STDIN_FILENO);
  if (!is_last) dup2(pipe_out, STDOUT_FILENO);

  interpret_cmd(cmd, stdin2, stdout2);

  // close pipe
  if (!is_first) close(pipe_in);
  if (!is_last) close(pipe_out);

  // reset i/o state
  dup2(stdin2, STDIN_FILENO);
  dup2(stdout2, STDOUT_FILENO);
}

void interpret(char *input) {
  char *context1, *token1, *str1;  // statements
  char *context3, *token3, *str3;  // pipes

  sanitize_input(input);

  memset(switches, 0, sizeof(switches));  // reset switches

  // save i/o state
  int stdin2 = dup(STDIN_FILENO);
  int stdout2 = dup(STDOUT_FILENO);

  for (str1 = input;; str1 = NULL) {  // loop through statements
    if (!(token1 = strtok_r(str1, ";", &context1))) break;
    // printf("cmd > {%s}\n", token1);
    // count pipe commands
    int pipes[2][2], pipe_cmds_i = 0;
    int pipe_cmds_n = 1;
    for (int i = 1; i < strlen(token1); i++)
      if (token1[i] == '|') pipe_cmds_n++;

    for (str3 = token1; pipe_cmds_i < pipe_cmds_n;
         str3 = NULL, pipe_cmds_i++) {  // loop through pipe commands
      args_n = 0;
      if (!(token3 = strtok_r(str3, "|", &context3))) break;
      // printf("pipe > {%s}\n", token3);
      // create pipe
      if (pipe(pipes[pipe_cmds_i % 2]) < 0) {
        color_perror(red, "pipe error");
        break;
      }
      pipe_cmd(token3, stdin2, stdout2, pipes[1 - pipe_cmds_i % 2][0],
               pipes[pipe_cmds_i % 2][1], pipe_cmds_i == 0,
               pipe_cmds_i == pipe_cmds_n - 1);
    }
  }

  close(stdin2);
  close(stdout2);
}