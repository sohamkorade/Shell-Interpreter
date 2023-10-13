#include "_cd.h"

int _cd() {
  if (args_n > 2) return expect_args(1);
  char *path;
  if (args_n == 1) {
    path = launch_dir;
  } else if (args[1][0] == '-') {
    path = last_dir;
  } else {
    path = args[1];
  }
  getcwd(temp_str, MAX_LEN);
  int e = chdir(path);
  // swap last_dir and current_dir
  if (e >= 0) strcpy(last_dir, temp_str);

  return e;
}