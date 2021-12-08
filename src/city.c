#include "../include/city.h"


tile_t create_tile(building_type_e type) {
    tile_t tile;
    tile._nb_citizens = 0;
    tile._type = type;
    tile._contamination = 0.0;
    switch (type) {
        case WASTELAND:
            tile._capacity = 16;
            break;
        case HOUSE:
            tile._capacity = 8;
            break;
        case HOSPITAL:
            tile._capacity = 12;
            break;
        case FIRESTATION:
            tile._capacity = 8;
            break;
        default:
            break;
    }
    return tile;
}

void init_city(city_t *city) {
    int i, j, k;
    /* mapping of hospitals and firestations */

    city->_map[3][3] = create_tile(HOSPITAL);
    city->_map[0][6] = create_tile(FIRESTATION);
    city->_map[6][0] = create_tile(FIRESTATION);

    /* mapping of randomly disposed houses */

    i = rand() % CITY_WIDTH;
    j = rand() % CITY_HEIGHT;

    for (k = 0; k < 12; k++) {
        while (city->_map[i][j]._capacity != 0) {
            i = rand() % CITY_WIDTH;
            j = rand() % CITY_HEIGHT;
        }
        city->_map[i][j] = create_tile(HOUSE);
    }

    /* mapping of wasteland with their initial contamination rate*/
    for (i = 0; i < CITY_WIDTH; i++) {
        for (j = 0; j < CITY_HEIGHT; ++j) {
            if (city->_map[i][j]._type == 0) {
                city->_map[i][j] = create_tile(WASTELAND);
                if (rand() % 100 < 10) {
                    add_tile_contamination(&city->_map[i][j], 0.20 + (1.0 * rand() / RAND_MAX) * 0.20);
                }
            }
        }
    }
}

void add_citizen(city_t *city, int num_c, citizen_type_e type) {
    unsigned int i, j;
    i = (unsigned int) (rand() % CITY_WIDTH);
    j = (unsigned int) (rand() % CITY_HEIGHT);
    while (city->_map[i][j]._nb_citizens == city->_map[i][j]._capacity) {
        i = (unsigned int) (rand() % CITY_WIDTH);
        j = (unsigned int) (rand() % CITY_HEIGHT);
    }
    city->_citizens[num_c] = create_citizen(type, i, j, num_c);
    city->_map[i][j]._nb_citizens++;
}

void create_population(city_t *city) {
    int ppl;
    /* FIREFIGHTERS : 6, 1 on each FIRESTATION */
    city->_citizens[0] = create_citizen(FIREFIGHTER, 0, 6, 0);
    city->_map[0][6]._nb_citizens++;
    city->_citizens[1] = create_citizen(FIREFIGHTER, 6, 0, 1);
    city->_map[6][0]._nb_citizens++;

    ppl = 2;
    while (ppl < 6) {
        add_citizen(city, ppl, FIREFIGHTER);
        ppl++;
    }

    /*  DOCTORs: 4, 1 on hospital */
    city->_citizens[ppl] = create_citizen(DOCTOR, 3, 3, ppl);
    city->_map[3][3]._nb_citizens++;
    ppl++;
    while (ppl < 10) {
        add_citizen(city, ppl, DOCTOR);
        ppl++;
    }

    /* REPORTERS: 2 */
    while (ppl < 12) {
        add_citizen(city, ppl, REPORTER);
        ppl++;
    }

    /* NORMALS :  25 */
    while (ppl < NB_TOTAL) {
        add_citizen(city, ppl, NORMAL);
        ppl++;
    }
}

void add_tile_contamination(tile_t *tile, double cont) {
    if (tile->_type == FIRESTATION) {
        return;
    }
    if (tile->_type == HOSPITAL) {
        cont *= 0.25;
    }
    tile->_contamination += cont;
    if (tile->_contamination < 0) {
        tile->_contamination = 0;
    }
    if (tile->_contamination > 1) {
        tile->_contamination = 1;
    }
}


int sickest_of_tile(city_t *city, unsigned int x, unsigned int y) {
    int ppl;
    int sickest_id = -1;
    int sickest = 0;
    for (ppl = 0; ppl < NB_TOTAL; ppl++) {
        if (is_at_position(city->_citizens[ppl], x, y) && city->_citizens[ppl].sickness_duration > sickest) {
            sickest = city->_citizens[ppl].sickness_duration;
            sickest_id = ppl;
        }
    }
    return sickest_id;
}

double average_contamination(city_t *city) {
    double sum = 0;
    int i, j;
    for (i = 0; i < CITY_HEIGHT; i++) {
        for (j = 0; j < CITY_WIDTH; j++) {
            sum += city->_map[i][j]._contamination;
        }
    }
    return sum / (CITY_HEIGHT * CITY_WIDTH);
}

void contaminate_citizens_on_tile(city_t *city, unsigned int x, unsigned int y) {
    int ppl;
    for (ppl = 0; ppl < NB_TOTAL; ppl++) {
        if (is_at_position(city->_citizens[ppl], x, y)) {
            add_citizen_contamination(&(city->_citizens[ppl]), city->_citizens[ppl].contamination * 0.01);
        }
    }
}

void contaminate_citizens_on_tiles_around(city_t *city, unsigned int x, unsigned int y) {
    unsigned int **tiles_around;
    int nbOfSquares;
    int i;
    if (rand() % 100 < 10) {
        contaminate_citizens_on_tile(city, x, y);
    }
    if (city->_map[x][y]._type == WASTELAND) {
        if (rand() % 100 < 1) {
            tiles_around = get_tiles_around(&nbOfSquares, x, y);
            for (i = 0; i < nbOfSquares; i++) {
                contaminate_citizens_on_tile(city, tiles_around[i][0], tiles_around[i][1]);
            }
        }
    }
}

void eject_healed(city_t *city) {
    int ppl;
    status_t *person;
    for (ppl = 0; ppl < NB_TOTAL; ppl++) {
        person = &city->_citizens[ppl];
        if (!person->is_sick && city->_map[person->x][person->y]._type == HOSPITAL) {
            person->days_spent_in_hospital_healthy = 2;
        }
    }
}

int doctor_present(city_t *city, unsigned int x, unsigned int y) {
    int ppl;
    for (ppl = 0; ppl < NB_TOTAL; ppl++) {
        if (city->_citizens[ppl].type == DOCTOR && is_at_position(city->_citizens[ppl], x, y)) {
            return 1;
        }
    }
    return 0;
}

int hasFireFighter(city_t *city, unsigned int x, unsigned int y) {
    int ppl;
    for (ppl = 0; ppl < NB_TOTAL; ppl++) {
        if (city->_citizens[ppl].type == FIREFIGHTER && city->_citizens[ppl].x == x && city->_citizens[ppl].y == y) {
            return 1;
        }
    }
    return 0;
}

int get_number_of_dead(city_t *city) {
    int i;
    int deadPeople = 0;
    for (i = 0; i < NB_TOTAL; i++) {
        if (city->_citizens[i].type == DEAD) {
            deadPeople++;
        }
    }
    return deadPeople;
}

int get_number_of_sick(city_t *city) {
    int i;
    int sickPeople = 0;
    for (i = 0; i < NB_TOTAL; i++) {
        if (city->_citizens[i].is_sick && city->_citizens[i].type != DEAD && city->_citizens[i].type != BURNED) {
            sickPeople++;
        }
    }
    return sickPeople;
}


int get_number_of_healthy(city_t *city) {
    int i;
    int healthyPeople = 0;
    for (i = 0; i < NB_TOTAL; i++) {
        if (!city->_citizens[i].is_sick && city->_citizens[i].type != DEAD && city->_citizens[i].type != BURNED) {
            healthyPeople++;
        }
    }
    return healthyPeople;
}

void display_city(city_t *city) {
    int i, j;
    int deadPeople;

    printf("\n------------------------------------------ \n");
    for (i = 0; i < CITY_WIDTH; i++) {
        for (j = 0; j < CITY_HEIGHT; j++) {
            printf("%d  %d %f |", city->_map[i][j]._nb_citizens, city->_map[i][j]._type,
                   city->_map[i][j]._contamination);
        }
        printf("\n------------------------------------------ \n");
    }
    deadPeople = get_number_of_dead(city) + get_number_of_burned(city);
    printf("There are %d dead and burned people\n", deadPeople);
    printf("There are %d sick people\n", get_number_of_sick(city));
    printf("There are %d healthy people\n", get_number_of_healthy(city));

}


int get_number_citizen(city_t *city) {
    int nb = 0;
    int i, j;
    for (i = 0; i < CITY_WIDTH; i++) {
        for (j = 0; j < CITY_HEIGHT; j++) {
            nb += city->_map[i][j]._nb_citizens;
        }
    }
    return nb;
}


int get_number_of_burned(city_t *city) {
    int i;
    int burned = 0;
    for (i = 0; i < NB_TOTAL; i++) {
        if (city->_citizens[i].type == BURNED) {
            burned++;
        }
    }
    return burned;
}

int get_number_citizen_on_tile(int x, int y, city_t *city) {
    int ppl;
    int on_tile = 0;
    for (ppl = 0; ppl < NB_TOTAL; ppl++) {
        if (is_at_position(city->_citizens[ppl], x, y)) {
            on_tile++;
        }
    }
    return on_tile;
}

int *get_dead_id(city_t *city) {
    int ppl;
    int i = 0;
    static int dead[NB_TOTAL];
    for (ppl = 0; ppl < NB_TOTAL; ppl++) {
        if (city->_citizens[ppl].type == DEAD) {
            dead[i] = ppl;
            i++;
        }
    }
    return dead;
}

void wasteland_contamination_spread(city_t *city) {
    unsigned int x, y;
    int i;
    int nb_around;
    unsigned int **tiles_around;
    double contamination_diff;
    for (x = 0; x < CITY_WIDTH; x++) {
        for (y = 0; y < CITY_HEIGHT; y++) {
            if (city->_map[x][y]._type == WASTELAND) {
                tiles_around = get_tiles_around(&nb_around, x, y);
                for (i = 0; i < nb_around; i++) {
                    tile_t *checked_tile = &city->_map[tiles_around[i][0]][tiles_around[i][1]];
                    contamination_diff =  city->_map[x][y]._contamination  - checked_tile->_contamination;
                    if ( contamination_diff > 0 && checked_tile->_type == WASTELAND) {
                        if (rand() % 100 < 15) {
                            add_tile_contamination(checked_tile, contamination_diff * ((rand() % 20) + 1.0) / 100);
                        }
                    }
                }
            }
        }
    }
}
