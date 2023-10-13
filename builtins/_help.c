#include "_help.h"

int _help() {
  color_printf(cyan, "Soham Shell");
  puts(", version 2.1\n");
  puts("List of commands that are defined internally:");
  puts("alias\t\t\t run long commands with shorter names");
  puts("bg\t\t\t send jobs to background");
  puts("cd\t\t\t change the working directory");
  puts("dalias\t\t\t delete already defined alias");
  puts("discover\t\t emulate the basics of the find command");
  puts("echo\t\t\t display a line of text");
  puts("fg\t\t\t bring job to foreground");
  puts("help\t\t\t show help");
  puts("history\t\t\t list previously entered commands");
  puts("jobs\t\t\t list jobs started by this shell");
  puts("ls\t\t\t list files and directories");
  puts("pinfo\t\t\t print the process-related info");
  puts("pwd\t\t\t print current working directory");
  puts("sig\t\t\t send a signal to a process");
  puts("bye|exit|quit\t\t close this program");
  return 0;
}