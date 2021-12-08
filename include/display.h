#include <ncurses.h>
#include <sys/ioctl.h>
#include "utils.h"

#ifndef SIMULATION_EPIDEMIE_DISPLAY_H
#define SIMULATION_EPIDEMIE_DISPLAY_H

/**
 * init the pair colors
 */
void init_colors();

/**
 * refresh the display
 */
void round_display(city_t *, int);

/**
 * init the window display
 */
void init_display(city_t *);

/**
 * init the display of the map with contamination color and building types
 */
void init_city_map(city_t *);

/**
 * refresh the colors of the city for the contamination rate
 */
void refresh_city_map(city_t *);

/**
 * initialize display for the number of people on each tile
 */
void init_citizen_map(city_t *);

/**
 * refresh the number of citizen on each tile
 */
void refresh_citizen_map(city_t *);


/**
 * initialize the display for the numbers about the epidemic
 */
void init_info(city_t *);

/**
 * refresh the numbers about the epidemic
 */
void refresh_info(city_t *);

/**
 * init the death notice
 */
void init_death_notice();

/**
 * refresh the dead notice display
 */
void refresh_death_notice(city_t *);

/**
 * init the display for the press agency
 */
void init_agency(int, int, float, int);

/**
 * refresh the agency display
 */
void refresh_agency(int, int, float, int);

#endif