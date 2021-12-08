
#ifndef SIMULATION_EPIDEMIE_CITY_H
#define SIMULATION_EPIDEMIE_CITY_H
#define CITY_WIDTH 7
#define CITY_HEIGHT 7
#define NB_TOTAL 37

#include <stdlib.h>
#include <stdio.h>
#include "citizen.h"
#include "utils.h"

typedef enum building_type {
    WASTELAND, HOUSE, HOSPITAL, FIRESTATION
} building_type_e;

/**
 * Represents a tile on the city.
 * @param x The x coordinate in the city.
 * @param y The y coordinate in the city.
 * @param capacity The max number of citizens on this tile.
 * @param nb_citizens The current number of citizens on the tile.
 * @param type The type of the building on the tile.
 * @param contamination The contamination level on the tile.
 */
typedef struct tile {
    int _capacity;
    int _nb_citizens;
    building_type_e _type; /* Un enum WASTELAND, HOUSE, HOSPITAL, FIRESTATION */
    double _contamination;
} tile_t;

/**
 * Represents the city in which the epidemic occurs.
 * @param map A 2D array of tiles representing the city.
 * @param citizens An array of status representing the citizens.
 */
typedef struct city {
    tile_t _map[CITY_HEIGHT][CITY_WIDTH];
    status_t _citizens[NB_TOTAL];
} city_t;

/**
 * Create and initialise tile
 * @param building type, coordinates x and y
 * @return the created tile
 */
tile_t create_tile(building_type_e);

/**
 * Initialize the city with the right maping and contamination rate on each tile
 */
void init_city(city_t *);

/**
 * Add citizen in the map and list of citizen of city
 * @param city
 * @param num_c
 */
void add_citizen(city_t *, int, citizen_type_e);

/**
 * create the population of the city with the right number of each citizen type
 */
void create_population(city_t *);

/**
 * Display the city in stdout
 */
void display_city(city_t *);

/**
 * Add contamination on the tile
 */
void add_tile_contamination(tile_t *, double);

/**
 * Return -1 if none sick on the tile
 * @param city
 * @param x
 * @param y
 * @return
 */
int sickest_of_tile(city_t *, unsigned int, unsigned int);

/**
 * @return the average contamniation of all the city
 */
double average_contamination(city_t *);

/**
 * Contamination spread through wastelands in the beginning of rounds
 */
void wasteland_contamination_spread(city_t *);

/**
 * contaminate the people on the tile with tile contamination
 */
void contaminate_citizens_on_tile(city_t *, unsigned int, unsigned int);

/**
 * Contaminate tile and tiles around with the tile contamination rate
 */
void contaminate_citizens_on_tiles_around(city_t *, unsigned int, unsigned int);

/**
 * Set attributes of healed citizens on hospital to free space
 */
void eject_healed(city_t *);

/**
 * @return 1 if a doctor is present on the tile [x,y] 0 if not
 */
int doctor_present(city_t *, unsigned int, unsigned int);

/**
 * @return 1 if a firefighter is present on the tile [x,y] 0 if not
 */
int hasFireFighter(city_t *, unsigned int, unsigned int);

/**
 * @return the number of citizen of the city
 */
int get_number_citizen(city_t *);

/**
 * @return the number of dead people of the city
 */
int get_number_of_dead(city_t *);

/**
 * @return the number of sick people of the city
 */
int get_number_of_sick(city_t *);

/**
 * @return the number of healthy people of the city
 */
int get_number_of_healthy(city_t *);

/**
 * @return the number of burned people of the city
 */
int get_number_of_burned(city_t *);

/**
 * @return the number of citizen on the tile
 */
int get_number_citizen_on_tile(int, int, city_t *);

/**
 * @return the ids of the dead people
 */
int *get_dead_id(city_t *);

#endif
