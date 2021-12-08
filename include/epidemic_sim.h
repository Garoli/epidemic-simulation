#ifndef EPIDEMIC_SIM_H
#define EPIDEMIC_SIM_H

#include "display.h"

/**
 * send message to end press_agency
 */
void end_press_agency();

/**
 * distroy the shared memory for the city
 */
void destroy_shared_memory(int, city_t *);

/**
 * write new numbers in
 */
void write_evolution(FILE *, int[4]);

/**
 * @return numbers to write in evolution.txt
 */
int *get_numbers(city_t *);

#endif
