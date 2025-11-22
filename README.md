# Lifetime

A Shell does three main things
1. Initialize: read and exec config files eg .bashrc
2. Interpret: read command from stdin and exec them
3. Terminate: executes shutdown commands, frees up memory 

# Basic loop

1. Read: from stdin.
2. Parse: separate the command string to program names and arguments.
3. Execute: run the parsed commands. 

# How to launch a program

1. Create a fork of the current program.
2. Run the arguments with execvp on the child process.
3. Parent process wait for the child process to exits/signals.


