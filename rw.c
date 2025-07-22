#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

sem_t wrt;                    // Semaphore for writer
pthread_mutex_t mutex;       // Mutex for reader count
int cnt = 1;                 // Shared data
int numreader = 0;           // Count of active readers

void *writer(void *wno) {
    sem_wait(&wrt); // Writer needs exclusive access

    cnt = cnt * 2;
    printf("Writer %d modified cnt to %d\n", *((int *)wno), cnt);

    sem_post(&wrt); // Writer releases access
    return NULL;
}

void *reader(void *rno) {
    // Reader lock to update numreader
    pthread_mutex_lock(&mutex);
    numreader++;
    if (numreader == 1) {
        // First reader blocks writers
        sem_wait(&wrt);
    }
    pthread_mutex_unlock(&mutex);

    // Reading section
    printf("Reader %d: read cnt as %d\n", *((int *)rno), cnt);

    // Reader lock to update numreader
    pthread_mutex_lock(&mutex);
    numreader--;
    if (numreader == 0) {
        // Last reader unblocks writers
        sem_post(&wrt);
    }
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main() {
    pthread_t read[3], write[3];
    int a[3] = {1, 2, 3}; // Thread identifiers

    pthread_mutex_init(&mutex, NULL); // Initialize mutex
    sem_init(&wrt, 0, 1);             // Initialize semaphore

    // Create writer threads first
    for (int i = 0; i < 3; i++) {
        pthread_create(&write[i], NULL, writer, &a[i]);
    }

    // Join writer threads first (wait till all finish)
    for (int i = 0; i < 3; i++) {
        pthread_join(write[i], NULL);
    }

    // Then create reader threads
    for (int i = 0; i < 3; i++) {
        pthread_create(&read[i], NULL, reader, &a[i]);
    }

    // Join reader threads
    for (int i = 0; i < 3; i++) {
        pthread_join(read[i], NULL);
    }

    // Cleanup
    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);

    return 0;
}
