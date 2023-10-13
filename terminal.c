#include "terminal.h"

struct termios orig_termios;

void disable_raw_mode() {
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    perror("tcsetattr"), exit(EXIT_FAILURE);
}

void enable_raw_mode() {
  if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    perror("tcgetattr"), exit(EXIT_FAILURE);
  atexit(disable_raw_mode);
  struct termios raw = orig_termios;
  raw.c_lflag &= ~(ICANON | ECHO);
  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    perror("tcsetattr"), exit(EXIT_FAILURE);
  setbuf(stdout, NULL);
}