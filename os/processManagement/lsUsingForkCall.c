#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>  // Required for wait()

int main() {

    DIR *dir = opendir(".");  // "." -> current directory

    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    // Forks a new process to list files in the directory
    pid_t pid = fork();

    if (pid < 0) {
        // Error in forking
        perror("fork");
        closedir(dir);
        return 1;
    }

    if (pid == 0) {
        // Child process
        printf("Child process (PID: %d) listing files in the current directory:\n", getpid());
        
        // Open the directory again in the child process
        DIR *child_dir = opendir(".");
        if (child_dir == NULL) {
            perror("opendir in child");
            return 1;
        }

        struct dirent *entry;
        while ((entry = readdir(child_dir)) != NULL) {
            printf("%s\n", entry->d_name);  // Correct format string for printing file names
        }

        closedir(child_dir);  // Close the directory in the child process
    } else {
        // Parent process
        wait(NULL);  // Wait for the child process to finish
        printf("Parent process (PID: %d) finished\n", getpid());
    }

    closedir(dir);  // Close the directory in the parent process
    return 0;
}

