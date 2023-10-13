#include "_pwd.h"

int _pwd() {
  if (args_n > 1) {
    color_printf(red, "pwd: expected no args\n");
    return -1;
  }
  getcwd(temp_str, MAX_LEN);
  puts(temp_str);
  return 0;
}