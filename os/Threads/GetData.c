#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>  // Add this header for strcspn()

typedef struct {
    char name[100];
    char surname[100];
    int age;
} PersonalInfo;

PersonalInfo info;

void* get_name(void* arg) {
    printf("Enter your name: ");
    fgets(info.name, sizeof(info.name), stdin);
    info.name[strcspn(info.name, "\n")] = '\0';  // Remove newline character
    return NULL;
}

void* get_surname(void* arg) {
    printf("Enter your surname: ");
    fgets(info.surname, sizeof(info.surname), stdin);
    info.surname[strcspn(info.surname, "\n")] = '\0';  // Remove newline character
    return NULL;
}

void* get_age(void* arg) {
    printf("Enter your age: ");
    scanf("%d", &info.age);
    while (getchar() != '\n');  // Consume the newline character left by scanf
    return NULL;
}

int main() {
    pthread_t thread1, thread2, thread3;

    if (pthread_create(&thread1, NULL, get_name, NULL) != 0) {
        perror("pthread_create for thread1");
        exit(1);
    }

    if (pthread_create(&thread2, NULL, get_surname, NULL) != 0) {
        perror("pthread_create for thread2");
        exit(1);
    }

    if (pthread_create(&thread3, NULL, get_age, NULL) != 0) {
        perror("pthread_create for thread3");
        exit(1);
    }

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);

    printf("\nPersonal Information:\n");
    printf("Name: %s\n", info.name);
    printf("Surname: %s\n", info.surname);
    printf("Age: %d\n", info.age);

    return 0;
}

