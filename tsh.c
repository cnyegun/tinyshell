#define _POSIX_C_SOURCE 200809L
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_ARGS 256

int tsh_cd(char **args);
int tsh_help(char **args);
int tsh_exit(char **args);

char *builtin_str[] =
{
	"cd", 
	"help",
	"exit"
};

int (*builtin_func[]) (char **) =
{
	&tsh_cd,
	&tsh_help,
	&tsh_exit
};

int tsh_num_builtins()
{
	return sizeof(builtin_str) / sizeof(char *);
}

int tsh_cd(char **args)
{
	(void)args;
	if (args[1] == NULL) {
		fprintf(stderr, "tsh: expected argument to \"cd\".\n");
	} else {
		if (chdir(args[1]) != 0) {
			perror("tsh");
		}
	}

	return 1;
}

int tsh_help(char **args)
{
	(void)args;
	int i;
	printf("Chi Nguyen's Tinyshell\n");
	printf("Type program names and arguments, and hit enter.\n");
	printf("The following are built in:\n");

	for (i = 0; i < tsh_num_builtins(); ++i) {
		printf("	%s\n", builtin_str[i]);
	}
	printf("Use the man command for information on other programs.\n");
	return 1;
}

int tsh_exit(char **args)
{
	(void)args;
	return 0;
}

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

	// Must be terminated with a NULL pointer
	argv[*argc] = NULL;
	return argv;
}

int tsh_launch(char **argv)
{
	pid_t pid, wpid;
	int status;

	pid = fork();
	if (pid == 0)
	{
		// Child process
		if (execvp(argv[0], argv) == -1)
		{
			perror(argv[0]);
		}
		exit(EXIT_FAILURE);
	}
	else if (pid < 0) 
	{
		perror("tsh");
	}
	else 
	{
		// Parent process
		do 
		{
			wpid = waitpid(pid, &status, WUNTRACED);
			if (wpid == -1)
			{
				perror("tsh");
				break;
			}
		} while (!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

int tsh_execute(size_t argc, char **argv)
{
	int i;
	// Empty command;
	if (argc == 0) return 1;

	for (i = 0; i < tsh_num_builtins(); ++i) {
		if (strcmp(argv[0], builtin_str[i]) == 0) {
			return (*builtin_func[i])(argv);
		}
	}
	return tsh_launch(argv);
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
