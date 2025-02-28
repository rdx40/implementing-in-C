/*reference:
 * https://how.dev/answers/how-to-use-the-pipe-system-call-for-inter-process-communication
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>


int count_vowels(const char* str, char *vowel_list){
	int count = 0;
	const char *vowels = "AEIOUaeiou";
	int pos =0;

	while(*str){
		if(strchr(vowels,*str)){
			count++;
			vowel_list[pos++] = *str;
		}
		str++;
	}
	vowel_list[pos] = '\0';
	return count;
}
int is_valid_name(const char* name){
    if (name == NULL || *name == '\0') {
        return 0;
    }

    for(int i = 0; name[i]; i++){
        if(!(isalpha(name[i]) || isspace(name[i]))) { //only accepting strings nad whitespaces
            return 0;
        }
    }

    return 1;
}

int main(){
	int pipefd[2];
	char name[100];
	pid_t pid;

	if(pipe(pipefd) == -1){
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	pid = fork();

	if(pid<0){
		perror("fork");
		exit(EXIT_FAILURE);
	}
	else if(pid>0){
		close(pipefd[0]);
		printf("Parent: Enter your name: ");
		if(scanf(" %99[^\n]", name) != 1 || !is_valid_name(name)){
			printf("Error invalid entry.\n");
			close(pipefd[1]);
			exit(EXIT_FAILURE);
		}
		write(pipefd[1], name, strlen(name)+1);
		close(pipefd[1]);
	}
	else{
		close(pipefd[1]);
		char recieved_name[100];
		char vowels_found[100];
		read(pipefd[0], recieved_name, sizeof(recieved_name));
		close(pipefd[0]);

		if(strlen(recieved_name) == 0 || !is_valid_name(recieved_name)){
			printf("Invalid entry");
			exit(EXIT_FAILURE);
		}

		int vowel_count = count_vowels(recieved_name, vowels_found);
		if(vowel_count>0){
			printf("Child: The name '%s' contains %d vowels: %s\n", recieved_name, vowel_count, vowels_found);
		}
		else{
			printf("The name '%s' has no vowels\n", recieved_name);
		}
	}
	return 0;
}
