#ifndef SIMULATION_EPIDEMIE_UTILS_H
#define SIMULATION_EPIDEMIE_UTILS_H

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <mqueue.h>
#include <semaphore.h>
#include <mqueue.h>
#include <stdio.h>     /* printf() */
#include <stdlib.h>    /* exit() and execl()*/
#include <unistd.h>    /* fork(), close() */
#include <fcntl.h>     /* open() opening flags and file modes */
#include <sys/mman.h>  /* mmap() */
#include <sys/stat.h>  /* stat() */
#include <pthread.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include "city.h"
#define BUFFER_SIZE 128
#define ROUND_NUMBER 100
#define SHARED_MEM_CITY "/city_mem"
#define SEM_PATH "/city_sem"

/** Majority of methods from : @author Alain Lebret <alain.lebret@ensicaen.fr> */
/** Adapted for the context */

/**
 * Handles a fatal error. It displays a message, then exits.
 */
void handle_fatal_error(char[]);

/**
 * Create shared memory for city
 * @return int file descriptor
 */
int create_city_shared_memory();

/**
 * Creates a named pipe
 *
 * @param name the name of a pipe
 * @param mode the mode of the pipe
 * @return the int corresponding to the pipe
 */
int create_pipe(const char *, mode_t);

/** Opens a named pipe for the communication
 *
 * @param name
 * @return the int corresponding to the pipe
 */
int open_read_pipe(const char *);


/**
 * Opens a named pipe
 *
 * @param name the name of the pipe
 * @return the int corresponding to the pipe
 */
int open_write_pipe(const char *);


typedef sem_t semaphore_t;

/**
 * Creates a POSIX semaphore and returns it.
 * @param name The name of the semaphore on the Unix system.
 * @return A pointer on the created POSIX semaphore.
 */
semaphore_t *create_and_open_semaphore(char *);

/**
 * Opens an already created POSIX semaphore and returns it.
 * @param name The name of the semaphore on the Unix system.
 * @return A pointer on the POSIX semaphore.
 */
semaphore_t *open_semaphore(char *);

/**
 * Destroys the specifier POSIX semaphore.
 * @param sem The identifier of the semaphore to destroy
 */
void destroy_semaphore(semaphore_t *, char *);

/**
 * Performs a P() operation ("wait") on a semaphore.
 * @param sem Pointer on the semaphore.
 */
void P(semaphore_t *);

/**
 * Performs a V() operation ("signal") on a semaphore.
 * @param sem Pointer on the semaphore.
 */
void V(semaphore_t *);

/**
 * @param squares
 * @param i
 * @param j
 * @return the coordonates of all squares around the one of coordinates i,j
 */
unsigned int **get_tiles_around(int *, unsigned int, unsigned int);

/**
 * @return print error and exit process
 */
void manage_error_and_exit();

/**
 * exec gnuplot file
 * @param filename
 */
void exec_gnuplot(const char *);

#endif