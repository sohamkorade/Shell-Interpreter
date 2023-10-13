#include "_quit.h"

int _quit() {
  printf("Bye ");
  getlogin_r(temp_str, MAX_LEN);
  color_printf(bold_white, "%c%s", toupper(temp_str[0]), temp_str + 1);
  puts("!");
  exit(EXIT_SUCCESS);
  return 0;
}