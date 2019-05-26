#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pwd.h>

#define MAX_LEN_LINE    15
#define LEN_HOSTNAME	30
int main(void)
{

	char hostname[LEN_HOSTNAME + 1];
	memset(hostname, 0x00, sizeof(hostname));
	printf("username: %s\n", getpwuid(getuid())->pw_name);
	gethostname(hostname, LEN_HOSTNAME);
	printf("hostname: %s\n", hostname);
	char compare[MAX_LEN_LINE] = "exit";
	char command[MAX_LEN_LINE];
	char *args[] = { command, NULL };
	int ret, status;
	pid_t pid, cpid;

	while (true) {
		char *s;
		int len;

		printf("%s@%s$", getpwuid(getuid())->pw_name, hostname);
		s = fgets(command, MAX_LEN_LINE, stdin);
		if (s == NULL) {
			fprintf(stderr, "fgets failed\n");
			exit(1);
		}




		len = strlen(command);
		printf("%d\n", len);
		if (command[len - 1] == '\n') {
			command[len - 1] = '\0';
		}

		printf("[%s]\n", command);


		if (strcmp(command, compare) == 0)
		{
			printf("exit_command");
			break;
		}



		char *ptr = strtok(command, ";");
		char *args[] = { ptr, NULL };
		while (ptr != NULL)
		{
			printf("%s\n", ptr);
			pid = fork();
			if (pid < 0)
			{
				fprintf(stderr, "fork failed\n");
				exit(1);
			}
			if (pid != 0)
			{  /* parent */
				cpid = waitpid(pid, &status, 0);
				if (cpid != pid)
				{
					fprintf(stderr, "waitpid failed\n");
				}
				printf("Child process terminated\n");
				if (WIFEXITED(status))
				{
					printf("Exit status is %d\n", WEXITSTATUS(status));
				}
			}
			else
			{  /* child */
				ret = execve(args[0], args, NULL);
				if (ret < 0)
				{
					fprintf(stderr, "execve failed\n");
					return 1;
				}
			}
			for (int i = 0; i < MAX_LEN_LINE; i++)
			{
				ptr[i] = ptr[i + 1];
			}
			ptr = strtok(NULL, ";");
		}
	}
	return 0;
}