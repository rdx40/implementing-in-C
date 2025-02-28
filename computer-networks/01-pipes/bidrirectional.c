#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>

int main(){
	int p2c[2]; //parent to child
	int c2p[2]; //child to parent
	//pipes in Unix/Linux systems are unidirectional so a bidirectional one is just basically two unidrectional ones
	pid_t pid;

	/*
	 * creating the pipes
	 */
	 if(pipe(p2c)){
		 perror("failed in creating parent to child pipe");
		 exit(EXIT_FAILURE);
	 }
	if(pipe(c2p)){
		 perror("failed in creating child to parent pipe");
		 exit(EXIT_FAILURE);
	 }

	pid = fork();
	if(pid < 0){
		perror("failed");
		exit(EXIT_FAILURE);
	}
	

	//parent
	else if(pid>0){
		char input_name[] = "omar";
		int recieved_count;


		//close parent-read and child-write

		close(p2c[0]);
		close(c2p[1]);

		//send name to child
		printf("Parent: Sending name: %s\n", input_name);
		write(p2c[1], input_name, strlen(input_name)+1);
		close(p2c[1]);

		//recieced vowel cnt
		read(c2p[0], &recieved_count, sizeof(recieved_count));

		printf("[Parent] Recieved vowel count: %d\n", recieved_count);
		close(c2p[0]);
		wait(NULL); // wait for child to exit
	}
	else{
		char received_name[100];
	        int vowel_count = 0;
       	
                close(p2c[1]);  // close write p2c
                close(c2p[0]);  // close read c2p
               
                read(p2c[0], received_name, sizeof(received_name));
                close(p2c[0]);  
               
                for (int i = 0; received_name[i]; i++) {
                    char c = tolower(received_name[i]);
                    if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
                        vowel_count++;
                    }
                }
               
                printf("[Child] Calculated vowels: %d\n", vowel_count);
                write(c2p[1], &vowel_count, sizeof(vowel_count));
	        close(c2p[1]);  

        	exit(EXIT_SUCCESS);
	}
	return EXIT_SUCCESS;
}



	 
