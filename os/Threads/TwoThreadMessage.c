#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* message_function(void* arg) {
    printf("%s\n", (char*)arg);
    return NULL;
}

int main() {
    pthread_t thread1, thread2;
    const char *message1 = "Operating";
    const char *message2 = "Systems";

    if (pthread_create(&thread1, NULL, message_function, (void*)message1) != 0) {
        perror("pthread_create for thread1");
        exit(1);
    }

    if (pthread_create(&thread2, NULL, message_function, (void*)message2) != 0) {
        perror("pthread_create for thread2");
        exit(1);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}

