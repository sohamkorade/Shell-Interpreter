#include "_discover.h"

int discover_dir(char* dir_name, char* filename, int optFiles, int optFolders,
                 int depth) {
  struct dirent* entry;
  DIR* dir = opendir(dir_name);
  if (!dir) {
    if (depth == 0) color_perror(red, "discover: cannot open dir");
    return -1;
  }

  if (optFolders && filename[0] == '\0' && depth == 0) {
    puts(".");
  }

  while (entry = readdir(dir)) {
    // if (!strcmp(entry->d_name, "..") || !strcmp(entry->d_name, "."))
    // continue;
    if (entry->d_name[0] == '.') continue;
    char* curr_path = new_string();
    strcpy(curr_path, dir_name);
    if (curr_path[strlen(curr_path) - 1] != '/') strcat(curr_path, "/");
    strcat(curr_path, entry->d_name);
    struct stat status;
    if (stat(curr_path, &status) < 0) {
      // perror("discover: cannot get status for file/dir");
      continue;
    }
    if (filename[0] == '\0' || !strcmp(entry->d_name, filename)) {
      strcpy(temp_str, curr_path);
      get_relative_path(temp_str, launch_dir);
      if (S_ISDIR(status.st_mode) && optFolders ||
          S_ISREG(status.st_mode) && optFiles) {
        color_printf(get_color(status.st_mode), "%s\n", temp_str);
      }
    }
    if (S_ISDIR(status.st_mode) && !S_ISLNK(status.st_mode))
      discover_dir(curr_path, filename, optFiles, optFolders, depth + 1);
    free(curr_path);
  }
  closedir(dir);
  return 0;
}

int _discover() {
  char path[MAX_LEN] = "", filename[MAX_LEN] = "";

  int optFiles = switches['f'], optFolders = switches['d'];
  for (int i = 1; i < args_n; i++) {
    if (args[i][0] == '-') continue;
    if (args[i][0] == '"')
      remove_quotes_and_copy(filename, args[i]);
    else
      strcpy(path, args[i]);
  }
  if (path[0] == '\0') strcpy(path, ".");

  if (!optFiles && !optFolders) optFiles = optFolders = 1;

  // printf("path: {%s}\n", path);
  // printf("filename: {%s}\n", filename);
  // printf("options: {%c%c}\n", optFiles ? 'f' : ' ', optFolders ? 'd' : ' ');

  return discover_dir(path, filename, optFiles, optFolders, 0);
}