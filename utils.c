#include "utils.h"

int expect_args(int n) {
  printf("expected %d %s arguments\n", abs(args_n - n - 1),
         args_n - 1 > n ? "less" : "more");
  return -1;
}

char *new_string() { return malloc(MAX_LEN * sizeof(char)); }

int get_relative_path(char *path, char *relative_to) {
  if (!strncmp(path, relative_to, strlen(relative_to)) &&
      (strlen(path) == strlen(relative_to) ||
       path[strlen(relative_to)] == '/')) {
    strcpy(path, "~");
    strcat(path, path + strlen(relative_to));
    return 1;
  }
  return 0;
}

void replace_str(char *s, char *find, char *replace) {
  strcpy(temp_str, "");
  char *insert_point = &temp_str[0];
  char *pos = s;
  int find_n = strlen(find);
  int replace_n = strlen(replace);

  while (1) {
    char *p = strstr(pos, find);

    if (!p) {
      strcpy(insert_point, pos);
      break;
    }

    strncpy(insert_point, pos, p - pos);
    insert_point += p - pos;

    strncpy(insert_point, replace, replace_n);
    insert_point += replace_n;

    pos = p + find_n;
  }

  strcpy(s, temp_str);
}

void join_str2(char *s, char *s1, char *s2) {
  strcpy(s, s1);
  strcat(s, s2);
}
void join_str3(char *s, char *s1, char *s2, char *s3) {
  strcpy(s, s1);
  strcat(s, s2);
  strcat(s, s3);
}

char *get_color(mode_t mode) {
  if (S_ISDIR(mode))
    return bold_blue;
  else if (mode & S_IXUSR)
    return bold_cyan;
  return S_ISLNK(mode) ? bold_cyan : white;
}

void remove_quotes_and_copy(char *new_s, char *old_s) {
  strcpy(new_s, old_s + 1);
  new_s[strlen(new_s) - 1] = '\0';
}

char *print_greeting() {
  time_t present = time(NULL);
  int hour = localtime(&present)->tm_hour;

  if (hour >= 4 && hour <= 12)
    printf("Good morning, ");
  else if (hour > 12 && hour <= 18)
    printf("Good afternoon, ");
  else
    printf("Good evening, ");

  getlogin_r(temp_str, MAX_LEN);
  color_printf(bold_white, "%c%s", toupper(temp_str[0]), temp_str + 1);
  puts("!");
}

void color_printf(char *color, char *fmt, ...) {
  if (isatty(STDOUT_FILENO)) printf("%s", color);
  va_list args;
  va_start(args, fmt);
  vprintf(fmt, args);
  va_end(args);
  if (isatty(STDOUT_FILENO)) printf("%s", white);
}

void color_perror(char *color, char *s) {
  if (isatty(STDOUT_FILENO)) printf("%s", color);
  perror(s);
  if (isatty(STDOUT_FILENO)) printf("%s", white);
}