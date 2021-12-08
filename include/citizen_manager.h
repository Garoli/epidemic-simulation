#include "../include/utils.h"

#ifndef CITIZEN_MANAGER_H
#define CITIZEN_MANAGER_H

typedef struct citizen_param {
    city_t *_city;
    sem_t *_sem;
    int _id;
} c_param_t;

/**
 * Describes a round of all citizens in a city
 */
void round(city_t *);

/**
 * Describes a round of one citizen
 * @param args for threading
 */
void *citizen_round(void *args);

#endif
