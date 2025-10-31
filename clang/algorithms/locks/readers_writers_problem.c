#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define READERS 5
#define WRITERS 2

#define RANDOM_SLEEP_MIN 2
#define RANDOM_SLEEP_MAX 7

sem_t mutex;
sem_t wrt;
int readcount;

void *reader_func() {
    pthread_t curr_thread_id = pthread_self();

    // Se não houverem escritores, permite leitura
    // N leitores podem ler ao mesmo tempo
    while(true) {
        sem_wait(&mutex);
        // Se não há leitores, pode haver algum escritor, que irá usar o mesmo semáforo
        if(readcount == 0) {
            printf("Thread %ld starts waiting to read\n", curr_thread_id);
            // Semáforo global de acesso ao recurso
            sem_wait(&wrt);
        }
        // É seguro continuar com a leitura pois há ao menos 1 leitor, então o semáforo já está liberado
        readcount++;
        sem_post(&mutex);

        int sleep_secs = rand() % (RANDOM_SLEEP_MAX + 1 - RANDOM_SLEEP_MIN) + RANDOM_SLEEP_MIN;
        printf("Thread %ld reading the resource for %d seconds\n", curr_thread_id, sleep_secs);
        sleep(sleep_secs);
        printf("Thread %ld finish reading\n", curr_thread_id);

        sem_wait(&mutex);
        readcount--;
        // Último leitor libera o semáforo
        if(readcount == 0) {
            sem_post(&wrt);
        }
        sem_post(&mutex);

        sleep(10);
    }
}

void *writer_func() {
    pthread_t curr_thread_id = pthread_self();

    // Somente 1 escritor por vez.
    // Deve aguardar todos os leitores terminarem(se houver) antes de começar a escrever.
    while(true) {
        // Aguarda liberação dos leitores ou outro escritor
        printf("Thread %ld is waiting to write\n", curr_thread_id);
        sem_wait(&wrt);

        printf("Thread %ld is writing in the resource\n", curr_thread_id);
        // Apenas um número
        sleep(RANDOM_SLEEP_MAX);
        printf("Thread %ld finish writing\n", curr_thread_id);

        // Libera o recurso
        sem_post(&wrt);

        sleep(6);
    }
}

int main(void) {
    sem_init(&mutex, 0, 1);
    sem_init(&wrt, 0, 1);
    readcount = 0;

    pthread_t *r_threads = malloc(sizeof(pthread_t)*READERS);
    pthread_t *w_threads = malloc(sizeof(pthread_t)*WRITERS);

    for(int i = 0; i < READERS; i++) {
        pthread_create(&r_threads[i], NULL, &reader_func, NULL);
    }
    for(int i = 0; i < WRITERS; i++) {
        pthread_create(&w_threads[i], NULL, &writer_func, NULL);
    }

    for(int i = 0; i < READERS; i++) {
        pthread_join(r_threads[i], NULL);
    }
    // Todos são while(true) então readers serão infinitors, mas join colocado para ficar completo
    for(int i = 0; i < WRITERS; i++) {
        pthread_join(w_threads[i], NULL);
    }
    return 0;
}
