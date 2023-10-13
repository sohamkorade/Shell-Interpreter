#include "history.h"

void load_history() {
  join_str2(temp_str, launch_dir, "/.history");
  FILE* f = fopen(temp_str, "r");
  if (f) {
    int eof = 0;
    // read file into array
    for (int i = 0; i < MAX_STORE_HISTORY; i++) {
      history[i][0] = '\0';
      if (!eof) {
        eof = !fgets(history[i], MAX_LEN, f);
        history[i][strlen(history[i]) - 1] = '\0';
      }
    }
    fclose(f);
  }
}

void save_history() {
  join_str2(temp_str, launch_dir, "/.history");
  FILE* f = fopen(temp_str, "w");
  if (f) {
    for (int i = 0; i < MAX_STORE_HISTORY; i++) {
      if (history[i][0] == '\0') break;
      fprintf(f, "%s\n", history[i]);
    }
    fclose(f);
  }
}

void _history() {
  load_history();
  for (int i = 0; i < MAX_DISPLAY_HISTORY; i++) {
    if (history[i][0] == '\0') break;
    puts(history[i]);
  }
}

void update_history(char* input) {
  if (input[0] == '\0' || input[0] == ' ') return;

  load_history();
  int slide_till = MAX_STORE_HISTORY - 1;
  for (int i = 0; i < MAX_STORE_HISTORY; i++)
    if (!strcmp(history[i], input)) slide_till = i;

  // slide down history
  for (int i = slide_till; i > 0; i--) {
    strcpy(history[i], history[i - 1]);
  }
  strcpy(history[0], input);

  save_history();
}

void alloc_history() {
  for (int i = 0; i < MAX_STORE_HISTORY; i++) history[i] = new_string();
}

void free_history() {
  for (int i = 0; i < MAX_STORE_HISTORY; i++) free(history[i]);
}