#include "_alias.h"

int _alias() {
  if (args_n == 3) {
    for (int i = 0; i < alias_n; i++) {
      if (!strcmp(alias[i][0], args[1])) {
        color_printf(red, "alias: you cannot modify an existing alias\n");
        return -1;
      }
    }
    alias[alias_n][0] = new_string();
    alias[alias_n][1] = new_string();
    strcpy(alias[alias_n][0], args[1]);
    remove_quotes_and_copy(alias[alias_n][1], args[2]);

    alias_n++;
  } else if (args_n == 1) {
    // print aliases
    for (int i = 0; i < alias_n; i++)
      printf("%s -> %s\n", alias[i][0], alias[i][1]);
  } else {
    return -1;
  }
  return 0;
}

int _dalias() {
  if (args_n == 2) {
    int pos = -1;
    for (int i = 0; i < alias_n; i++) {
      if (!strcmp(alias[i][0], args[1])) {
        pos = i;
        break;
      }
    }
    if (pos == -1) {
      color_printf(red, "alias '%s' doesn't exist\n", args[1]);
      return -1;
    }
    for (int i = pos; i < alias_n - 1; i++) {
      strcpy(alias[i][0], alias[i + 1][0]);
      strcpy(alias[i][1], alias[i + 1][1]);
    }
    free(alias[alias_n - 1][0]);
    free(alias[alias_n - 1][1]);
    alias_n--;
  } else {
    return -1;
  }
  return 0;
}

void free_aliases() {
  for (int i = 0; i < alias_n; i++) free(alias[i][0]), free(alias[i][1]);
}