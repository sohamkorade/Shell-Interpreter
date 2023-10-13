#include "_echo.h"

int _echo() {
  // print all args
  for (int i = 1; i < args_n; i++) printf("%s ", args[i]);
  puts("");
  return 0;
}