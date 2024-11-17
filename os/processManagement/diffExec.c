#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {

    pid_t pid;

    // Create a new process using fork
    pid = fork();

    if (pid < 0) {
        // Error creating a new process
        perror("fork");
        exit(1);
    } else if (pid == 0) {
        // Child process

        // Using execl() - specify the path and arguments directly
        printf("Using execl() to run /bin/ls:\n");
        if (execl("/bin/ls", "ls", "-l", (char *)NULL) == -1) {
            perror("execl failed");
        }

        // Using execle() - specify the path, arguments, and environment
        printf("Using execle() to run /bin/ls:\n");
        char *envp[] = { "PATH=/bin", NULL };
        if (execle("/bin/ls", "ls", "-l", (char *)NULL, envp) == -1) {
            perror("execle failed");
        }

        // Using execlp() - search for the program in PATH
        printf("Using execlp() to run ls:\n");
        if (execlp("ls", "ls", "-l", (char *)NULL) == -1) {
            perror("execlp failed");
        }

        // Using execv() - pass arguments as an array
        printf("Using execv() to run /bin/ls:\n");
        char *args1[] = { "ls", "-l", NULL };
        if (execv("/bin/ls", args1) == -1) {
            perror("execv failed");
        }

        // Using execvp() - search for the program in PATH
        printf("Using execvp() to run ls:\n");
        char *args2[] = { "ls", "-l", NULL };
        if (execvp("ls", args2) == -1) {
            perror("execvp failed");
        }

        // Using execvpe() - search for the program in PATH and provide environment variables
        //printf("Using execvpe() to run ls:\n");
        //char *args3[] = { "ls", "-l", NULL };
        //char *envp2[] = { "PATH=/bin", NULL };
        //if (execvpe("ls", args3, envp2) == -1) {
        //    perror("execvpe failed");
        //}

    } else {
        // Parent process
        wait(NULL);  // Wait for the child to finish
        printf("Parent process (PID: %d) finished\n", getpid());
    }

    return 0;
}

