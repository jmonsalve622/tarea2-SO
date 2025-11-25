#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

int N = 5;
int E = 4;

void* work(void *arg) {
	int id = (int)(size_t)arg;
	for (int i = 0; i < E; i++) {
		usleep(100000);
		printf("tid %d esperando en etapa %d\n", id, i);
	}
}

int main(int argc, char const *argv[]) {
	pthread_t threads[N];

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

	return 0;
}