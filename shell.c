#include <stdio.h>
#include <stdlib.h>

#define EXIT_SUCESS 0
#define EXIT_FAILED 1

char *tsh_read_line(void)
{
	return NULL;
}

char **tsh_parse_args(char *line)
{
	return NULL;
}

int tsh_execute(char **args)
{
	return 0;
}

void tsh_loop(void)
{
	char *line;
	char **args;
	int status;

	do {
		printf("> ");
		line = tsh_read_line();
		args = tsh_parse_args(line);
		status = tsh_execute(args);

		free(line);
		free(args);
	} while (status);
}

int main(int argc, char **argv)
{
	// Load config files.
	
	// Run command loop.
	
	tsh_loop();

	return EXIT_SUCESS;
}
