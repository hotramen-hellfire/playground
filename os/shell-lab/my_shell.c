#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"
#define RESET "\x1B[0m"
#define BOLD "\x1B[1m" // Bold Text Formula...
#define MAX_INPUT_SIZE 1024
#define MAX_TOKEN_SIZE 64
#define MAX_NUM_TOKENS 64

/* Splits the string by space and returns the array of tokens
 *
 */
char **tokenize(char *line)
{
	char **tokens = (char **)malloc(MAX_NUM_TOKENS * sizeof(char *));
	char *token = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
	int i, tokenIndex = 0, tokenNo = 0;

	for (i = 0; i < strlen(line); i++)
	{

		char readChar = line[i];

		if (readChar == ' ' || readChar == '\n' || readChar == '\t')
		{
			token[tokenIndex] = '\0';
			if (tokenIndex != 0)
			{
				tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
				strcpy(tokens[tokenNo++], token);
				tokenIndex = 0;
			}
		}
		else
		{
			token[tokenIndex++] = readChar;
		}
	}

	free(token);
	tokens[tokenNo] = NULL;
	return tokens;
}

int main(int argc, char *argv[])
{
	system("clear");
	char line[MAX_INPUT_SIZE];
	char **tokens;
	int i;
	char username[1000];
	getlogin_r(username, (size_t)1000);
	printf(ANSI_COLOR_GREEN BOLD "---------------------------------------------------------G R E E T I N G S-㉿\n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_GREEN BOLD "-----------------------Hi %s, nice to see you!!\n" ANSI_COLOR_RESET, username);
	const char *home = getenv("HOME");
	// if (s)
	// 	printf("%s\n", s);
	// else
	// 	printf("envvar notfound\n");
	while (1)
	{
		char cwd[1000];
		char add[1000];
		if (getcwd(cwd, sizeof(cwd)) == NULL)
		{
			perror("getcwd() error");
			return 1;
		}
		/* BEGIN: TAKING INPUT */
		int i = 0;
		int is = 1;
		if (strlen(cwd) > 7 && ('/' == cwd[0]) && ('h' == cwd[1]) && ('o' == cwd[2]) && ('m' == cwd[3]) && ('e' == cwd[4]) && ('/' == cwd[5]))
		{
			int i = 6;
			int j = 0;
			while ((cwd[i] != '\0' && username[j] != '\0'))
			{
				if (cwd[i] == username[j])
				{
					i++;
					j++;
				}
				else
				{
					is = 0;
					break;
				}
			}
		}
		else
		{
			is = 0;
		}
		if (is)
		{
			add[0] = '~';
			strcpy(add + 1, cwd + strlen(username) + 6);
		}
		else
		{
			strcpy(add, cwd);
		}
		bzero(line, sizeof(line));
		printf(ANSI_COLOR_GREEN "┌──[" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_RED BOLD "rogue" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_GREEN "]" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_GREEN "-" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_GREEN BOLD "☸" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_GREEN "-" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_GREEN "{" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_YELLOW BOLD "%s" ANSI_COLOR_RESET, add);
		printf(ANSI_COLOR_GREEN "} \n" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_GREEN "└─🗲" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_GREEN BOLD "🗲 " ANSI_COLOR_RESET);
		scanf("%[^\n]", line);
		getchar();
		// printf("Command entered: %s (remove this debug output later)\n", line);
		/* END: TAKING INPUT */
		if (strlen(line) == 0)
			continue;
		line[strlen(line)] = '\n'; // terminate with new line
		tokens = tokenize(line);
		if (strcmp(tokens[0], "exit") == 0)
		{
			for (i = 0; tokens[i] != NULL; i++)
			{
				free(tokens[i]);
			}
			free(tokens);
			printf(ANSI_COLOR_GREEN BOLD "---------------------------------------------------------------------B Y E-㉿\n" ANSI_COLOR_RESET);
			return 0;
		}
		else
		{
			if (strcmp("cd", tokens[0]) == 0)
			{
				int ret = 1;
				if (tokens[1] != NULL)
				{
					if (tokens[1][0] == '~')
					{
						if (strlen(tokens[1]) > 1)
						{
							char homer[1000];
							strcpy(homer, home);
							strcpy(homer + strlen(home), tokens[1] + 1);
							ret = chdir(homer);
						}
						else
							ret = chdir(home);
					}
					else
					{
						ret = chdir(tokens[1]);
					}
				}
				else
				{
					ret = chdir(home);
				}
				if (ret)
					printf(ANSI_COLOR_RED "eww, err: bad address!!" ANSI_COLOR_RESET "\n");
			}
			else
			{
				int fork_u = fork();
				if (fork_u == 0)
				{
					// printf("--service started\n");
					execvp(tokens[0], tokens);
					printf(ANSI_COLOR_RED "eww, err!!" ANSI_COLOR_RESET "\n");
					exit(0);
				}
				else
				{
					wait(NULL);
				}
			}
		}
		for (i = 0; tokens[i] != NULL; i++)
		{
			free(tokens[i]);
		}
		free(tokens);
	}
	printf(ANSI_COLOR_GREEN BOLD "---------------------------------------------------------------------B Y E-㉿\n" ANSI_COLOR_RESET);
	return 0;
}
