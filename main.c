#include "main.h"

int main() {
  alloc_history();
  getcwd(last_dir, MAX_LEN);
  getcwd(launch_dir, MAX_LEN);
  register_signal_handlers();

  print_greeting();

  while (1) {
    enable_raw_mode();
    handle_input();
    disable_raw_mode();

    update_history(input);
    interpret(input);
  }

  free_history();
  free_aliases();
  free_jobs();
}
