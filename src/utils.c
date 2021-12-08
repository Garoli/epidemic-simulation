#include "../include/utils.h"

int create_city_shared_memory() {
    int mem;

    mem = shm_open(SHARED_MEM_CITY, O_CREAT | O_RDWR, S_IRWXU);
    if (mem < 0) {
        handle_fatal_error("Error when calling shm_open()\n");
        exit(EXIT_FAILURE);
    }
    if (ftruncate(mem, sizeof(city_t)) < 0) {
        handle_fatal_error("Error when calling ftruncate()\n");
        exit(EXIT_FAILURE);
    }
    return mem;
}

int create_pipe(const char *name, mode_t mode) {
    int pipe;
    pipe = mkfifo(name, mode);
    return pipe;
}

int open_read_pipe(const char *name) {
    int pd;
    pd = open(name, O_RDONLY);
    if (pd == -1) {
        handle_fatal_error("Error when trying to open the named pipe\n");
    }
    return pd;
}

int open_write_pipe(const char *name) {
    int pd;
    pd = open(name, O_WRONLY);

    if (pd == -1) {
        handle_fatal_error("Error when trying to open the named pipe \n");
    }
    return pd;
}

void handle_fatal_error(char message[]) {
    fprintf(stderr, "%s\n", message);
    exit(EXIT_FAILURE);
}

semaphore_t *create_and_open_semaphore(char *name) {
    semaphore_t *sem = NULL;

    sem = sem_open(name, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR, 1);
    if (sem == SEM_FAILED) {
        handle_fatal_error("Error trying to create semaphore\n");
    }
    return sem;
}

semaphore_t *open_semaphore(char *name) {
    semaphore_t *sem = NULL;

    sem = sem_open(name, O_RDWR, S_IRUSR | S_IWUSR, 0);
    if (sem < SEM_FAILED) {
        sem_unlink(name);
        handle_fatal_error("Error trying to open semaphore\n");
    }
    return sem;
}

void destroy_semaphore(semaphore_t *sem, char *name) {
    int r;

    r = sem_close(sem);
    if (r < 0) {
        handle_fatal_error("Error trying to destroy semaphore\n");
    }
    r = sem_unlink(name);
    if (r < 0) {
        perror("Error trying to unlink semaphore\n");
    }
}

void P(semaphore_t *sem) {
    int r;

    r = sem_wait(sem);
    if (r < 0) {
        handle_fatal_error("Error with P() operation\n");
    }
}

void V(semaphore_t *sem) {
    int r = 0;

    sem_post(sem);
    if (r < 0) {
        handle_fatal_error("Error with V() operation\n");
    }
}

unsigned int **get_tiles_around(int *nb_of_squares_around, unsigned int i, unsigned int j) {
    unsigned int **prox;
    int k;
    int x;
    int y;
    int is, js;
    is = (int) i;
    js = (int) j;
    k = 0;
    prox = (unsigned int **) malloc(sizeof(int *));
    for (x = is - 1; x < is + 2; x++) {
        for (y = js - 1; y < js + 2; y++) {
            if (y >= 0 && x >= 0 && y <= 6 && x <= 6 && !(x == is && y == js)) {
                k++;
                prox = realloc(prox, sizeof(int *) * k);
                prox[k - 1] = (unsigned int *) malloc(sizeof(int) * 2);
                prox[k - 1][0] = x;
                prox[k - 1][1] = y;
            }
        }
    }
    *nb_of_squares_around = k;
    return prox;
}

void exec_gnuplot(const char *filename) {
    sleep(1);/* Don't be so hurry */
    if (execlp("gnuplot", "gnuplot", "-persist", filename, (void *) 0) < 0) {
        fprintf(stderr, "Error or gnuplot not installed\n");
        exit(EXIT_FAILURE);
    }
}

void manage_error_and_exit() {
    exit(EXIT_FAILURE);
}
