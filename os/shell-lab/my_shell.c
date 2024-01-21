#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h> // EXIT_SUCCESS, EXIT_FAILURE
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
#define MAX_BG_PROCS 64

/* Splits the string by space and returns the array of tokens
 *
 */

struct bg_proc
{
	char name[100];
	int pid;
	int is_running;
};

char **tokenize(char *line, int *bgFlag)
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
				if (strcmp(token, "&") == 0)
					*bgFlag = 1;
				else
				{
					tokens[tokenNo] = (char *)malloc(MAX_TOKEN_SIZE * sizeof(char));
					strcpy(tokens[tokenNo++], token);
				}
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

void show_jobs(struct bg_proc *bg_procs)
{
	int is_job = -1;
	for (int i = 0; i < MAX_BG_PROCS; i++)
	{
		char status[100];
		strcpy(status, "running");
		if (bg_procs[i].is_running < 1)
		{
			strcpy(status, "zombie");
		}
		if (bg_procs[i].pid > 0)
		{
			if (is_job < 0)
			{
				printf(BOLD "\nPROC_NAME     PROC_PID    PROC_STATUS\n");
				is_job = 1;
			}
			if (bg_procs[i].is_running)
				printf(ANSI_COLOR_YELLOW " %s         %d       %s\n", bg_procs[i].name, bg_procs[i].pid, status);
			else
				printf(ANSI_COLOR_RED " %s         %d       %s\n", bg_procs[i].name, bg_procs[i].pid, status);
		}
	}
	if (is_job < 0)
	{
		printf(ANSI_COLOR_YELLOW "No Background Process. . .\n");
	}
}

void add_proc(struct bg_proc *bg_procs, int pid, int is_running, char *name)
{
	for (int i = 0; i < MAX_BG_PROCS; i++)
	{
		if (bg_procs[i].pid < 0)
		{
			bg_procs[i].pid = pid;
			bg_procs[i].is_running = is_running;
			strcpy(bg_procs[i].name, name);
			return;
		}
	}
	printf(ANSI_COLOR_RED BOLD "P-TABLE FULL!!");
}

void init_jobs(struct bg_proc *bg_procs)
{
	for (int i = 0; i < MAX_BG_PROCS; i++)
	{
		strcpy(bg_procs[i].name, "tomb");
		bg_procs[i].is_running = 0;
		bg_procs[i].pid = -1;
	}
}

void grim_reaper(struct bg_proc *bg_procs)
{
	for (int i = 0; i < MAX_BG_PROCS; i++)
	{
		if (bg_procs[i].pid > 0)
		{
			if (bg_procs[i].is_running > 0)
			{
				int status;
				int stat = waitpid(bg_procs[i].pid, &status, WNOHANG);
				if (stat > 0)
				{
					// process has been reaped, clear table entry, show message
					printf(ANSI_COLOR_YELLOW BOLD "\ngrim_reaper " ANSI_COLOR_RESET);
					printf(ANSI_COLOR_YELLOW ": %s[%d] has been executed. . .\n" ANSI_COLOR_RESET, bg_procs[i].name, bg_procs[i].pid);
					bg_procs[i].pid = -1;
				}
				else
				{
					// wait
				}
			}
			else
			{
				// remove table entry, show message
				printf(ANSI_COLOR_YELLOW BOLD "\ngrim_reaper " ANSI_COLOR_RESET);
				printf(ANSI_COLOR_YELLOW ": %s[%d] has been executed. . .\n" ANSI_COLOR_RESET, bg_procs[i].name, bg_procs[i].pid);
				bg_procs[i].pid = -1;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	// system("clear");
	struct bg_proc bg_procs[MAX_BG_PROCS];
	init_jobs(bg_procs);
	// show_jobs(bg_procs);
	char line[MAX_INPUT_SIZE];
	char exitF[1000];
	char **tokens;
	int i;
	char username[1000];
	getlogin_r(username, (size_t)1000);
	printf(ANSI_COLOR_YELLOW BOLD "---------------------------------------------------------🗲G R E E T I N G S-☸\n" ANSI_COLOR_RESET);
	printf(ANSI_COLOR_YELLOW BOLD "-----------------------🗲Hi %s, nice to see you!!\n" ANSI_COLOR_RESET, username);
	const char *home = getenv("HOME");
	while (1)
	{
		grim_reaper(bg_procs);
		printf("\n");
		int bgFlag = 0;
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
		printf(ANSI_COLOR_RESET ANSI_COLOR_YELLOW "┌──[" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_RED BOLD "flash" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_YELLOW "]" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_YELLOW "-" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_YELLOW BOLD "☸" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_YELLOW "-" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_YELLOW "{" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_YELLOW BOLD "%s" ANSI_COLOR_RESET, add);
		printf(ANSI_COLOR_YELLOW "} \n" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_YELLOW "└─" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_YELLOW "🗲" ANSI_COLOR_RESET);
		printf(ANSI_COLOR_YELLOW BOLD "🗲 " ANSI_COLOR_RESET);
		// style input
		printf(BOLD "");

		// input
		scanf("%[^\n]", line);
		getchar();
		if (strlen(line) == 0)
			continue;
		line[strlen(line)] = '\n'; // terminate with new line
		tokens = tokenize(line, &bgFlag);
		if (strcmp(tokens[0], "exit") == 0)
		{
			for (int i = 0; i < MAX_BG_PROCS; i++)
			{
				if (bg_procs[i].pid > 0 && bg_procs[i].is_running > 0)
				{
					int kill_stat = kill(bg_procs[i].pid, SIGKILL);
					if (kill_stat > -1)
					{
						printf(ANSI_COLOR_RED BOLD "\ngrim_reaper " ANSI_COLOR_RESET);
						printf(ANSI_COLOR_YELLOW BOLD ": %s[%d] has been killed. . ." ANSI_COLOR_RESET, bg_procs[i].name, bg_procs[i].pid);
					}
					else
					{
						printf(ANSI_COLOR_RED BOLD "\ngrim_reaper " ANSI_COLOR_RESET);
						printf(ANSI_COLOR_RED BOLD ": error killing %s[%d]. . ." ANSI_COLOR_RESET, bg_procs[i].name, bg_procs[i].pid);
					}
				}
			}
			for (i = 0; tokens[i] != NULL; i++)
			{
				free(tokens[i]);
			}
			free(tokens);
			printf(ANSI_COLOR_YELLOW BOLD "\n---------------------------------------------------------------------🗲B Y E-☸\n" ANSI_COLOR_RESET);
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
						if (tokens[1][0] != '/')
						{
							char homer[1000];
							strcpy(homer, "./");
							strcpy(homer + 2, tokens[1]);
							ret = chdir(homer);
						}
						else
						{
							ret = chdir(tokens[1]);
						}
					}
				}
				else
				{
					ret = chdir(home);
				}
				if (ret)
				{
					sprintf(exitF, ANSI_COLOR_RED BOLD "[%d] eww, error!!", EXIT_FAILURE);
					perror(("%s", exitF));
				}
			}
			else if (strcmp("jobs", tokens[0]) == 0)
			{
				show_jobs(bg_procs);
			}
			else
			{
				if (bgFlag > 0)
				{
					int fork_u = fork();
					if (fork_u == 0)
					{
						execvp(tokens[0], tokens);
						sprintf(exitF, ANSI_COLOR_RED BOLD "[%d] eww, error!!", EXIT_FAILURE);
						perror(("%s", exitF));
						exit(-1);
					}
					else
					{
						int status;
						sleep(1);
						int stat = waitpid(fork_u, &status, WNOHANG);
						printf(ANSI_COLOR_YELLOW BOLD "spawner" ANSI_COLOR_RESET);
						printf(ANSI_COLOR_YELLOW ": %s[%d] spawned. . .\n", tokens[0], fork_u);
						if (stat > 0)
						{
							add_proc(bg_procs, fork_u, 0, tokens[0]);
						}
						else
						{
							add_proc(bg_procs, fork_u, 1, tokens[0]);
						}
					}
				}
				else
				{
					int fork_u = fork();
					if (fork_u == 0)
					{
						execvp(tokens[0], tokens);
						sprintf(exitF, ANSI_COLOR_RED BOLD "[%d] eww, error!!", EXIT_FAILURE);
						perror(("%s", exitF));
						exit(-1);
					}
					else
					{
						int status;
						int stat = waitpid(fork_u, &status, 0);
					}
				}
			}
		}
		for (i = 0; tokens[i] != NULL; i++)
		{
			free(tokens[i]);
		}
		free(tokens);
	}
	printf(ANSI_COLOR_YELLOW BOLD "\n🗲---------------------------------------------------------------------B Y E-☸\n" ANSI_COLOR_RESET);
	return 0;
}
