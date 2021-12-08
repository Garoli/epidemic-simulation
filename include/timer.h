#ifndef SIMULATION_EPIDEMIE_TIMER_H
#define SIMULATION_EPIDEMIE_TIMER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <signal.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include "utils.h"
#define DURATION 2

/**
 * start the round
 */
void startRound();

/**
 * end the round
 */
void endOfRound();

#endif