#include "_ls.h"

void print_permissions(struct stat* status) {
  if (S_ISDIR(status->st_mode))
    putchar('d');
  else if (S_ISLNK(status->st_mode))
    putchar('l');
  else
    putchar('-');
  putchar((status->st_mode & S_IRUSR) ? 'r' : '-');
  putchar((status->st_mode & S_IWUSR) ? 'w' : '-');
  putchar((status->st_mode & S_IXUSR) ? 'x' : '-');
  putchar((status->st_mode & S_IRGRP) ? 'r' : '-');
  putchar((status->st_mode & S_IWGRP) ? 'w' : '-');
  putchar((status->st_mode & S_IXGRP) ? 'x' : '-');
  putchar((status->st_mode & S_IROTH) ? 'r' : '-');
  putchar((status->st_mode & S_IWOTH) ? 'w' : '-');
  putchar((status->st_mode & S_IXOTH) ? 'x' : '-');
}

void print_file_time(struct stat* status) {
  time_t present = time(NULL);
  int file_year = localtime(&status->st_mtime)->tm_year;
  if (file_year == localtime(&present)->tm_year)
    printf("%.12s ", ctime(&status->st_mtime) + 4);
  else  // show year instead of time if year not current year
    printf("%.7s %d ", ctime(&status->st_mtime) + 4, 1900 + file_year);
}

void print_resolved_link_path(char* absolute_path) {
  char resolved_path[MAX_LEN] = "";
  int resolved_path_n = readlink(absolute_path, resolved_path, MAX_LEN);
  printf("-> ");
  if (resolved_path_n > 0) {
    resolved_path[resolved_path_n] = '\0';
    strcpy(temp_str, "/");
    strcat(temp_str, resolved_path);
    struct stat status_resolved;
    if (stat(temp_str, &status_resolved) < 0) {
      color_perror(red, "ls: cannot get status for link");
      return;
    }
    color_printf(get_color(status_resolved.st_mode), "%s", resolved_path);
  } else {
    color_printf(red, "(broken link)");
  }
}

void list_file(char* absolute_path, char* filename, struct stat* status,
               int optAll, int optLong, int link_count_width,
               int byte_count_width) {
  if (optLong) {
    print_permissions(status);
    printf(" %*ld ", link_count_width, status->st_nlink);
    printf("%s ", getpwuid(status->st_uid)->pw_name);
    printf("%s ", getgrgid(status->st_gid)->gr_name);
    printf("%*ld ", byte_count_width, status->st_size);
    print_file_time(status);
  }

  color_printf(get_color(status->st_mode), "%s", filename);

  if (optLong) {
    printf(" ");
    if (S_ISLNK(status->st_mode)) print_resolved_link_path(absolute_path);
  }
  puts("");
}

int alphasort_caseless(const struct dirent** a, const struct dirent** b) {
  return strcasecmp((*a)->d_name, (*b)->d_name);
}

void list_dir(char* dir_name, int optAll, int optLong) {
  struct dirent** namelist;
  int n = scandir(dir_name, &namelist, NULL, alphasort_caseless);
  if (n == -1) {
    color_perror(red, "ls: unable to scan directory");
    return;
  }

  char absolute_path[MAX_LEN];
  struct stat* status = malloc(n * sizeof(struct stat));
  int total = 0;
  int link_count_width = 0;
  int byte_count_width = 0;
  int filename_width = 0;
  for (int i = 0; i < n; i++) {
    if (namelist[i]->d_name[0] != '.' || optAll) {
      join_str3(absolute_path, dir_name, "/", namelist[i]->d_name);
      if (lstat(absolute_path, &status[i]) < 0) {
        color_perror(red, "ls: cannot get status");
        return;
      }
      total += status[i].st_blocks;
      int temp;
      temp = sprintf(temp_str, "%ld", status[i].st_nlink);
      if (link_count_width < temp) link_count_width = temp;
      temp = sprintf(temp_str, "%ld", status[i].st_size);
      if (byte_count_width < temp) byte_count_width = temp;
      temp = strlen(namelist[i]->d_name);
      if (filename_width < temp) filename_width = temp;
    }
  }
  if (optLong) printf("total %d\n", total / 2);

  int columns_printed = 0;
  for (int i = 0; i < n; i++) {
    if (namelist[i]->d_name[0] != '.' || optAll) {
      // char* color = get_color(status[i].st_mode);
      join_str3(absolute_path, dir_name, "/", namelist[i]->d_name);
      if (optLong || !isatty(STDOUT_FILENO)) {
        list_file(absolute_path, namelist[i]->d_name, &status[i], optAll,
                  optLong, link_count_width, byte_count_width);
      } else {
        color_printf(get_color(status[i].st_mode), "%-*s  ", filename_width,
                     namelist[i]->d_name);
        columns_printed += filename_width + 2;
        if (columns_printed >= 80) {
          puts("");
          columns_printed = 0;
        }
      }
    }
    free(namelist[i]);
  }
  if (columns_printed != 0) puts("");

  free(namelist);
  free(status);
}

int _ls() {
  int paths = 0;
  for (int i = 1; i < args_n; i++)
    if (args[i][0] != '-') paths++;

  int optAll = switches['a'], optLong = switches['l'];

  if (paths == 0) {
    list_dir(".", optAll, optLong);
    return 0;
  }

  int listed_file = -1;
  for (int i = 1; i < args_n; i++) {
    if (args[i][0] == '-') continue;
    char* absolute_path = realpath(args[i], NULL);
    if (!absolute_path) {
      color_perror(red, "ls: invalid path");
      break;
    }
    struct stat status;
    if (stat(absolute_path, &status) < 0) {
      color_perror(red, "ls: cannot get status!");
      free(absolute_path);
      break;
    }
    if (S_ISREG(status.st_mode)) {  // handle files
      if (listed_file == 0) puts("");
      if (optLong)
        list_file(absolute_path, args[i], &status, optAll, optLong, 0, 0);
      else
        puts(args[i]);
      listed_file = 1;
    } else if (S_ISDIR(status.st_mode)) {  // handle folders
      if (listed_file != -1) puts("");
      if (paths > 2) color_printf(bold_blue, "%s:\n", args[i]);
      list_dir(absolute_path, optAll, optLong);
      listed_file = 0;
    }
    free(absolute_path);
  }
  return 0;
}