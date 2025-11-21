#include <stdio.h>
#include <stdlib.h>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define N 12

extern char **environ;

char *allowed[N] = {"cp","touch","mkdir","ls","pwd","cat","grep","chmod","diff","cd","exit","help"};

int isAllowed(const char*cmd) {
    if (cmd == NULL) {
        return 0;
    }

    for (int i = 0; i < N; i++) {
        if (strcmp(cmd, allowed[i]) == 0) {
            return 1;
        }
    }

	return 0;
}

int main() {

    char line[256];
    char* argv[21];
    int argc;
    char* token;
    pid_t pid;
    int status;

    char line[256];

    while (1) {

	    fprintf(stderr,"rsh>");

	    if (fgets(line,256,stdin)==NULL) continue;

	    if (strcmp(line,"\n")==0) continue;

	    line[strlen(line)-1]='\0';

        argc = 0;
        token = strtok(line, " \t");
        while (token != NULL && argc < 20) {
            argv[argc++] = token;
            token = strtok(NULL, " \t");
        }
        argv[argc] = NULL;

        if (argc == 0) {
            continue;
        }

        if (!isAllowed(argv[0])) {
            printf("NOT ALLOWED!\n");
            continue;
        }

        //exit
        if (strcmp(argv[0], "exit") == 0) {
            return 0;
        }

        //help
        if (strcmp(argv[0], "help") == 0) {
            printf("The allowed commands are:\n");
            printf("1: cp\n");
            printf("2: touch\n");
            printf("3: mkdir\n");
            printf("4: ls\n");
            printf("5: pwd\n");
            printf("6: cat\n");
            printf("7: grep\n");
            printf("8: chmod\n");
            printf("9: diff\n");
            printf("10: cd\n");
            printf("11: exit\n");
            printf("12: help\n");
            continue;
        }

        //cd
        if (strcmp(argv[0], "cd") == 0) {
            if (argc > 2) {
                printf("-rsh: cd: too many arguments\n");
            }
            else if (argc == 2) {
                (void)chdir(argv[1]);
            }
            else {
                char* home = getenv("HOME");
                if (home != NULL) {
                    (void)chdir(home);
                }
            }
            continue;
        }

        if (posix_spawnp(&pid, argv[0], NULL, NULL, argv, environ) != 0) {
            perror("spawn failed");
        }
        else {
            (void)waitpid(pid, &status, 0);
        }
    }
    return 0;
}
