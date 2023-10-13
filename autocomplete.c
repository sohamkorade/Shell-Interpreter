#include "autocomplete.h"

void suggest(char* input) {
  // temp points to the beginning of the last word in input
  char* temp = strrchr(input, ' ');
  if (!temp)
    temp = input;
  else
    ++temp;

  // printf("<%s>\n", temp);

  struct dirent* entry;
  DIR* dir = opendir(".");
  int match_count = 0;
  while (entry = readdir(dir))
    if (!strncasecmp(entry->d_name, temp, strlen(temp))) match_count++;
  if (match_count > 1) puts("");
  dir = opendir(".");
  char common_text[MAX_LEN] = "";
  int first = 1;
  while (entry = readdir(dir)) {
    if (!strncasecmp(entry->d_name, temp, strlen(temp))) {
      if (temp[0] == '\0' && entry->d_name[0] == '.') continue;
      struct stat status;
      getcwd(temp_str, MAX_LEN);
      strcat(temp_str, "/");
      strcat(temp_str, entry->d_name);
      if (stat(temp_str, &status) < 0) {
        // perror("cannot get status for file/dir");
        continue;
      }
      if (match_count == 1) {
        join_str2(temp, entry->d_name, S_ISDIR(status.st_mode) ? "/ " : " ");
      } else {
        color_printf(get_color(status.st_mode), entry->d_name);
        puts(S_ISDIR(status.st_mode) ? "/" : "");
        if (first) strcpy(common_text, entry->d_name);
        first = 0;
        char *p1 = common_text, *p2 = entry->d_name;
        int common_length = 0;
        while (*p1 == *p2 && *p1 != '\0' && *p2 != '\0')
          ++p1, ++p2, common_length++;
        common_text[common_length] = '\0';
        // printf("{%s}\n", common_text);
      }
    }
  }
  // puts("");
  if (common_text[0] != '\0') strcpy(temp, common_text);
}

void get_autocomplete(char* input, char* autocomplete, int pos) {
  autocomplete[0] = '\0';
  for (int i = 0; i < MAX_STORE_HISTORY; i++) {
    if (pos < strlen(history[i]) && !strncasecmp(history[i], input, pos)) {
      strcpy(autocomplete, history[i]);
      break;
    }
  }
  if (autocomplete[0] != '\0') {
    color_printf(gray, "%s", autocomplete + pos);
    // move cursor back to the end of input
    printf("\e[%ldD", strlen(autocomplete) - pos);
  }
}

void fill_autocomplete(char* input, char* autocomplete) {
  if (autocomplete[0] != '\0') strcpy(input, autocomplete);
}