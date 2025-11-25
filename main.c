#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

typedef struct {
	int count;
	int N;
	int etapa;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} barrier_t;

void barrier_init(barrier_t *b, int N) {
	b->count = 0;
	b->N = N;
	b->etapa = 1;
	pthread_mutex_init(&b->mutex, NULL);
	pthread_cond_init(&b->cond, NULL);
}

void barrier_wait(barrier_t *b) {
	pthread_mutex_lock(&b->mutex);
	b->count++;

	if (b->count == b->N) {
		b->count = 0;
		b->etapa++;
		pthread_cond_broadcast(&b->cond);
	}
	else {
		pthread_cond_wait(&b->cond, &b->mutex);
	}

	pthread_mutex_unlock(&b->mutex);
}

void barrier_destroy(barrier_t *b) {
	pthread_mutex_destroy(&b->mutex);
	pthread_cond_destroy(&b->cond);
}

int N = 5;
int E = 4;
barrier_t barrier;

void* work(void *arg) {
	int id = (int)(size_t)arg;
	for (int i = 0; i < E; i++) {
		usleep(100000);
		printf("tid %d esperando en etapa %d\n", id, barrier.etapa);
		barrier_wait(&barrier);
		printf("tid %d paso barrera en etapa %d\n", id, barrier.etapa - 1);
	}
}

int main(int argc, char const *argv[]) {
	pthread_t threads[N];
	barrier_init(&barrier, N);

	for (int i = 0; i < N; i++) {
		if (pthread_create(&threads[i], NULL, work, (void*)(size_t)i) != 0) {
			perror("Fallo en creación de hebra\n");
		}
	}

	for (int i = 0; i < N; i++) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("Fallo en unión de hebra\n");
		}
	}

	barrier_destroy(&barrier);

	return 0;
}