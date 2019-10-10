#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <math.h>
#include <semaphore.h>

#define TAM 100000
int vetor_random[TAM];
int counter = TAM;

sem_t semaforo;

void move_vetor(int index){
    for (int j = index; j < TAM-1; j++)
    {
        int temp = vetor_random[j];
        vetor_random[j] = vetor_random[j+1];
        vetor_random[j+1] = temp;
    }
}

void* remove_par(void *arg){
    for (int i = TAM-1; i > -1; i--)
    {
        sem_wait(&semaforo);
        if(vetor_random[i]%2 == 0 && i < counter){
            //vetor_random[i] = -1;
            counter--; 
            move_vetor(i);          
        }
        sem_post(&semaforo); 
    }
    return NULL;
}

void* remove_multiplo_cinco(void *arg){
    for (int i = TAM-1; i > -1; i--)
    {
        sem_wait(&semaforo);
        if(vetor_random[i]%5 == 0 && i < counter){
            //vetor_random[i] = -1;
            counter--;
            move_vetor(i);
        }
        sem_post(&semaforo);       
    }
    return NULL;
}

int qtd_par_multiplocinco(int counter){
    int qtd = 0;
    for (int i = 0; i < counter; i++)
    {
        if(vetor_random[i]%5 == 0 || vetor_random[i]%2 == 0){
            qtd++;
        }
    }
    return qtd;
}
/*
int qtd_par_multiplocinco(){
    int qtd = 0;
    for (int i = 0; i < TAM; i++)
    {
        if(vetor_random[i]%5 == 0 || vetor_random[i]%2 == 0){
            qtd++;
        }
    }
    return qtd;
}
*/
int qtd_removido(){
    int qtd = 0;
    for (int i = 0; i < TAM; i++)
    {
        if(vetor_random[i] == -1){
            qtd++;
        }
    }
    return qtd;
}

int testa_ordem_vetor(int vetor_de_teste[TAM]){
    int j = 0;
    int qtd_errado = 0;
    for (int i = 0; i < TAM; i++)
    {
        if(vetor_de_teste[i]%5 != 0 && vetor_de_teste[i]%2 != 0){
            if(vetor_de_teste[i] != vetor_random[j]){
                qtd_errado++;        
            }
            j++;
        }
    }
    return qtd_errado;
}

int main(int argc, char const *argv[])
{
    int N = 2;
    pthread_t minhas_threads[N];
    int vetor_de_teste[TAM];
    sem_init(&semaforo, 0, 1);

    for (int i = 0; i < TAM; i++)
    {
        vetor_random[i] = (rand()%100) +1;
        vetor_de_teste[i] = vetor_random[i];
        //printf("%d\n", vetor_random[i]);
    }
    printf("Qtd de números que precisam ser removidos: %d\n\n", qtd_par_multiplocinco(TAM));
    
    pthread_create(&minhas_threads[0], NULL, remove_par, NULL);  
    pthread_create(&minhas_threads[1], NULL, remove_multiplo_cinco, NULL);     

    pthread_join(minhas_threads[0], NULL);
    pthread_join(minhas_threads[1], NULL);
    
    printf("Qtd de números que foram removidos: %d\n", TAM-counter);
    printf("Qtd de par e multiplos de cinco que sobrou: %d\n", qtd_par_multiplocinco(counter));
    printf("Qtd de números na posição errada: %d\n", testa_ordem_vetor(vetor_de_teste));

    return 0;
}
