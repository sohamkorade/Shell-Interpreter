```
Soham Korade
2021101131
```

# How to run
```
$ make && ./main
```

# Files
```
autocomplete.c -> handles history autocomplete and path suggestions on TAB key
builtins.h -> all header files gathered
globals.c -> initializes global variables
history.c -> handles history
interpreter.c -> breaks up the input into args and calls runner, also implements pipes and redirection
main.c -> entry point
makefile -> contains commands to compile the source
prompt.c -> displays prompt and takes user input
README.md -> the file you're reading
runner.c -> handles background and foreground running of commands
terminal.c -> handles raw mode
utils.c -> misc functions

'builtins' folder contains individual source files for internal shell commands
```

# Assumptions
 
- `discover` doesn't recursively search in hidden folders
- assumed directory names would not contain any spaces
- `echo` doesn't handle multi-line strings and environmental variables 
- path suggestions on TAB key press are limited to the current directory
- maximum 1 input redirection and 1 output redirection supported
- maximum number of pipes supported is fixed (100)
- maximum number of jobs supported is fixed (100)
- maximum number of aliases supported is fixed (100)
