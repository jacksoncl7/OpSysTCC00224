#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>

void* my_function(void *arg){
    char *nome = (char*) arg; 
    printf("Eu sou a %s e meu ID é %ld.\n", nome, pthread_self());
    printf("Eu sou a %s e meu ID é %ld.\n", nome, syscall(SYS_gettid));
    return NULL;
}


int main(int argc, char const *argv[])
{
    int N;
    scanf("%d", &N);
    pthread_t minhas_threads[N];
    char nome_das_threads[N][21];

    for (int i = 0; i < N; i++)
    {
        char str[12];
        sprintf(str, "%d", i);
        strcpy(nome_das_threads[i], "Thread_");
        strcat(nome_das_threads[i], str);

        pthread_create(&minhas_threads[i], NULL, my_function, nome_das_threads[i]);
        
    }

    for (int i = 0; i < N; i++)
    {
        pthread_join(minhas_threads[i], NULL);
    }

    return 0;
}
