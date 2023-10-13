#include "prompt.h"

int show_prompt(char* input) {
  char username[MAX_LEN], hostname[MAX_LEN];
  getlogin_r(username, MAX_LEN);
  gethostname(hostname, MAX_LEN);

  // clear line
  printf("\e[2K\r");

  //<{username}@{hostname}:
  printf(cyan "<" purple "%s" cyan "@" purple "%s" cyan ":", username,
         hostname);

  //<{username}@{hostname}:{path}
  getcwd(temp_str, MAX_LEN);
  get_relative_path(temp_str, launch_dir);
  printf(green "%s", temp_str);

  //<{username}@{hostname}:{path}{elapsed time}
  if (seconds_elapsed >= 1) {
    printf(yellow " took %lds", seconds_elapsed);
  }

  //<{username}@{hostname}:{path}{elapsed time}>
  color_printf(cyan, "> ");

  int temp = printf("%s", input);
  fflush(stdout);
  return temp;
}

int get_history(int history_pos, int up) {
  if (up && history_pos < MAX_STORE_HISTORY - 1)
    history_pos++;
  else if (!up && history_pos >= 0)
    history_pos--;
  // color_printf(blue, "[%d/%d]", history_pos + 1, MAX_STORE_HISTORY);
  memset(input, '\0', MAX_LEN);
  strcpy(input, history_pos == -1 ? "" : history[history_pos]);
  return history_pos;
}

void handle_input() {
  char* autocomplete = new_string();
  memset(input, '\0', MAX_LEN);
  load_history();
  int history_pos = -1;
  char c;
  int pos = show_prompt(input);
  while (c = getchar()) {
    if (iscntrl(c)) {
      if (c == '\n') {
        pos = show_prompt(input);
        break;
      } else if (c == '\e') {
        if (getchar() == '[' && (c = getchar())) {
          if (c == 'A' || c == 'B')
            history_pos = get_history(history_pos, c == 'A');
          else if (c == 'C')
            fill_autocomplete(input, autocomplete);
          // else if (c == 'C' || c == 'D') {
          //   printf("\e[1%c", c);
          // }
        }
      } else {
        switch (c) {
          case 127:  // backspace
            if (pos > 0) {
              if (input[pos - 1] == '\t') printf("\b\b\b");
              input[--pos] = '\0';
            }
            break;
          case '\t':  // tab
            pos = show_prompt(input);
            suggest(input);
            break;
          case 4:  // end of transmission
            exit(EXIT_SUCCESS);
            break;
          case 14:  // ctrl-n
            fill_autocomplete(input, autocomplete);
            break;
          default:
            printf("<%d>\n", c);
        }
      }
      pos = show_prompt(input);
    } else {
      input[pos++] = c;
      pos = show_prompt(input);
      get_autocomplete(input, autocomplete, pos);
    }
  }
  puts("");
  free(autocomplete);

  // size_t line_length = MAX_LEN;
  // getline(&input, &line_length, stdin);
  // if (input[0] == '\0') break;
  // input[strlen(input) - 1] = '\0';  // clear terminating '\n'
}
