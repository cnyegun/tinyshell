#define _POSIX_C_SOURCE 200809L
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ARGS 256

char *tsh_read_line(void)
{
	char *line = NULL;
	size_t line_size = 0;
	ssize_t line_len = getline(&line, &line_size, stdin);

	if (line_len == -1) {
		free(line);
		return NULL;
	}

	if (line_len > 0 && line[line_len - 1] == '\n')
		line[--line_len] = '\0';

	return line;
}

char **tsh_parse_args(size_t *argc, char *line)
{
	char **argv = malloc(sizeof *argv * (MAX_ARGS + 1));
	if (!argv) 
	{
		fprintf(stderr, "Error: allocation error\n");
		return NULL;
	}

	char *saveptr = NULL;
	*argc = 0;
	const char *delimiter = " \t";
	char *token = strtok_r(line, delimiter, &saveptr);

	while (token != NULL) 
	{
		if (*argc >= MAX_ARGS) 
		{
			fprintf(stderr, "Error: too many arguments\n");
			free(argv);
			return NULL; 
		}

		argv[(*argc)++] = token;
		token = strtok_r(NULL, delimiter, &saveptr);
	}

	argv[*argc] = NULL;
	return argv;
}

int tsh_execute(size_t argc, char **argv)
{
	(void)argc;
	(void)argv;
	return 1;
}

void tsh_loop(void)
{
	char *line;
	char **argv;
	size_t argc;
	int status = 1;

	do {
		printf("> ");
		fflush(stdout);

		line = tsh_read_line();
		if (line == NULL) {
			printf("\n");
			return;
		}

		argv = tsh_parse_args(&argc, line);
		if (argv == NULL) {
			fprintf(stderr, "Error: failed to parse\n");
			free(line);
			continue;
		}

		// Ignores empty line
		if (argc == 0) {
			free(line);
			free(argv);
			continue;
		}

		for (size_t i = 0; i < argc; i++) {
			printf("argv[%zu]: %s\n", i, argv[i]);
		}

		status = tsh_execute(argc, argv);

		// Clean up
		free(argv);
		free(line);

	} while (status);
}

int main(int argc, char **argv)
{
	// Load config files.
	(void)argc;
	(void)argv;
	// Run command loop.
	
	tsh_loop();

	return EXIT_SUCCESS;
}
