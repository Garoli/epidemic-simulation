#include "../include/citizen_manager.h"

int main(void) {
    int pd_round;
    int shmd_city;
    char buffer[BUFFER_SIZE];
    city_t *city;
    ssize_t messageLength;
    int running;

    running = 1;

    /*Open the shared memory*/
    shmd_city = shm_open(SHARED_MEM_CITY, O_RDWR, S_IRUSR | S_IWUSR);

    if (shmd_city == -1) {
        perror("Error when trying to open shared memory\n");
        exit(EXIT_FAILURE);
    }

    ftruncate(shmd_city, sizeof(city_t));

    /*Create a memory mapping of the shared memory into the virtual memory of the process*/
    city = (city_t *) mmap(NULL, sizeof(city_t), PROT_READ | PROT_WRITE, MAP_SHARED, shmd_city, 0);

    /*Create the population associated with the city */
    create_population(city);

    /* Start of simulation*/
    while (running) {
        pd_round = open_read_pipe("./roundPipe");
        messageLength = read(pd_round, buffer, BUFFER_SIZE);
        close(pd_round);
        /* message from epidemic sim to end*/
        if (strcmp("end", buffer) == 0) {
            running = 0;
        }
        if (messageLength > 0) {
            strcpy(buffer, "");
            round(city);
        }
        sleep(1);
    }

    exit(EXIT_SUCCESS);
}


void citizen_job(status_t *citizen, city_t *city) {
    int ppl;
    int has_healed = 0;
    char buffer[16];
    mqd_t mq;
    status_t *current;
    double max_tool_usage;
    double needed, used;
    if (citizen->type == FIREFIGHTER) {
        max_tool_usage = citizen->tool > 100 ? 100 : citizen->tool;
        used = 0;
        for (ppl = 0; ppl < NB_TOTAL; ppl++) {
            current = &city->_citizens[ppl];
            if (is_at_position(city->_citizens[ppl], citizen->x, citizen->y)) {
                if (current->contamination > 0 && current->type != BURNED && current->type != DEAD) {
                    needed = current->contamination * 20;
                    if (needed < max_tool_usage - used) {
                        add_citizen_contamination(current, needed);
                        used += needed;
                    }
                }
                if (current->type == DEAD) {
                    current->type = BURNED;
                }
            }
        }
        needed = city->_map[citizen->x][citizen->y]._contamination * 20;
        if (needed < max_tool_usage - used) {
            add_tile_contamination(&city->_map[citizen->x][citizen->y],
                                   -0.2 * city->_map[citizen->x][citizen->y]._contamination);
            used += needed;
        }
        citizen->tool -= (int) used;
    }

    /* Doctor heal the sickest person on the tile*/
    if (citizen->type == DOCTOR) {
        ppl = sickest_of_tile(city, citizen->x, citizen->y);
        if (ppl != -1 && citizen->tool > 0) {
            heal(&city->_citizens[ppl]);
            has_healed++;
            if ((city->_map[citizen->x][citizen->y]._type != HOSPITAL)) {
                citizen->tool--;
            }
        }
    }

    /* healers heal themselves */
    if (citizen->type == DOCTOR && citizen->tool > 0 && citizen->sickness_duration < 10 && !has_healed) {
        heal(citizen);
    }


    /*journalist job with message queue*/
    if (citizen->type == REPORTER) {

        /* send number of sick ppl, p2 */
        mq = mq_open("/news", O_WRONLY | O_CREAT, 0644, NULL);
        sprintf(buffer, "%d", get_number_of_sick(city));
        mq_send(mq, buffer, sizeof(buffer), 2);

        /* send number average contamination of city, p5 */
        sprintf(buffer, "%f", average_contamination(city));
        mq_send(mq, buffer, sizeof(buffer), 5);

        /* send number of dead people, p10 */
        sprintf(buffer, "%d", get_number_of_dead(city) + get_number_of_burned(city));
        mq_send(mq, buffer, sizeof(buffer), 10);

        /* send number journalist contamination of city, p1 */
        sprintf(buffer, "%f", citizen->contamination);
        mq_send(mq, buffer, sizeof(buffer), 1);

    }
}


void personal_contamination(int moved, tile_t tile, status_t *citizen) {
    double new_contamination;
    new_contamination = citizen->contamination;
    if (moved) {
        if (tile._type == HOSPITAL) {
            new_contamination *=
                    0.9 * citizen->contamination < tile._contamination ? 0 : -0.1;
        }
        if (tile._type == FIRESTATION) {
            new_contamination *= -0.2;
        } else {
            new_contamination = 0.02 * tile._contamination;
        }
    } else {
        new_contamination = 0.05 * tile._contamination;
    }
    add_citizen_contamination(citizen, new_contamination);
}

void field_contamination(int moved, tile_t *tile, double contamination) {
    if (moved) {
        add_tile_contamination(tile, contamination * 0.01);
    }
}

int can_move_to(unsigned int x, unsigned int y, status_t *citizen, city_t *city) {
    building_type_e building = city->_map[x][y]._type;

    if (city->_map[x][y]._nb_citizens + 1 > city->_map[x][y]._capacity) {
        return 0;
    }

    switch (building) {
        case HOSPITAL:
            if (citizen->is_sick && city->_map[x][y]._nb_citizens == city->_map[x][y]._capacity) {
                eject_healed(city);
                return 0;
            }
            if (citizen->is_sick || (citizen->type == DOCTOR && citizen->days_out_of_hospital >= 2)) {
                return 1;
            }
            break;
        case FIRESTATION:
            if ((citizen->type != FIREFIGHTER && hasFireFighter(city, x, y)) || citizen->type == FIREFIGHTER) {
                return 1;
            }
            break;
        default :
            return 1;
    }
    return 0;
}

int move_citizen(status_t *citizen, unsigned int **tiles_around, int nbSquares, city_t *city) {
    int direction;
    int i;
    int free_spaces = 0;
    for (i = 0; i < nbSquares; i++) {
        if (can_move_to(tiles_around[i][0], tiles_around[i][1], citizen, city)) {
            free_spaces++;
        }
    }
    if (free_spaces == 0) {
        return 0;
    }
    /* should find a client ;) */
    do {
        direction = rand() % nbSquares;
    } while (!can_move_to(tiles_around[direction][0], tiles_around[direction][1], citizen, city));
    city->_map[citizen->x][citizen->y]._nb_citizens--;
    citizen->x = tiles_around[direction][0];
    citizen->y = tiles_around[direction][1];
    city->_map[citizen->x][citizen->y]._nb_citizens++;
    return 1;
}


void *citizen_round(void *args) {
    int chance_to_move;
    unsigned int **tiles_around;
    int *nb_of_squares_around;
    int has_moved;
    double chance_to_die;
    status_t citizen;
    tile_t * current_tile;
    c_param_t *params;

    has_moved = 0;
    params = (c_param_t *) args;
    citizen = params->_city->_citizens[params->_id];

    P(params->_sem);

    /*corpses or sick people spread their contamination */
    if (citizen.type == DEAD || citizen.is_sick) {
        contaminate_citizens_on_tiles_around(params->_city, citizen.x, citizen.y);
    }

    /* do not work for the BURNED or DEAD*/
    if (citizen.type == DEAD || citizen.type == BURNED) {
        V(params->_sem);
        pthread_exit(NULL);
    }

    /* compute the chance to move*/
    chance_to_move = rand() % 100;
    nb_of_squares_around = (int *) malloc(sizeof(int));

    /*case citizen is healed and for 2 days in hospital : force the movement */
    if (!citizen.is_sick && citizen.days_spent_in_hospital_healthy == 2) {
        chance_to_move = 2;
    }

    /* citizen can move */
    if (chance_to_move < 40) {
        tiles_around = get_tiles_around(nb_of_squares_around, citizen.x, citizen.y);
        has_moved = move_citizen(&citizen, tiles_around, *nb_of_squares_around, params->_city);
    }
    current_tile = &params->_city->_map[citizen.x][citizen.y];
    /* healers refill their tools */
    if (citizen.type == FIREFIGHTER || citizen.type == DOCTOR) {
        refill_tools(&citizen, current_tile->_type == HOSPITAL, current_tile->_type == FIRESTATION);
    }

    /* compute if days out or in hospital */
    if (current_tile->_type != HOSPITAL) {
        if (citizen.type == DOCTOR) {
            citizen.days_out_of_hospital++;
        }
    } else {
        if (!citizen.is_sick) {
            citizen.days_spent_in_hospital_healthy++;
        }
    }

    /*contaminate himself and people around */
    personal_contamination(has_moved, *current_tile, &citizen);
    field_contamination(has_moved, current_tile, citizen.contamination);

    /* healers and journalists do their jobs*/
    citizen_job(&citizen, params->_city);

    /* try do get sick or to die */
    if (!citizen.is_sick) {
        get_sick(&citizen);
    }
    if (citizen.is_sick) {
        if (citizen.sickness_duration >= 5) {
            chance_to_die = 0.05;
            if (current_tile->_type == HOSPITAL) {
                chance_to_die = chance_to_die / 4;
            }
            if (doctor_present(params->_city, citizen.x, citizen.y)) {
                chance_to_die = chance_to_die / 2;
            }
            die(&citizen, chance_to_die);
            if (citizen.type == DEAD) {
                current_tile->_nb_citizens--;
                if (current_tile->_nb_citizens < 0) {
                    current_tile->_nb_citizens = 0;
                }
            }
        }

        if (citizen.sickness_duration < 5 && citizen.type != DEAD) {
            citizen.sickness_duration++;
        }
    }

    /*update and close*/
    params->_city->_citizens[params->_id] = citizen;
    V(params->_sem);
    pthread_exit(NULL);
}


void round(city_t *city) {
    sem_t *sem;
    int ppl;
    pthread_t pt[NB_TOTAL];
    c_param_t c_params[NB_TOTAL];
    srand(time(NULL));

    sem = create_and_open_semaphore(SEM_PATH);

    wasteland_contamination_spread(city);

    for (ppl = 0; ppl < NB_TOTAL; ppl++) {
        c_params[ppl]._city = city;
        c_params[ppl]._sem = sem;
        c_params[ppl]._id = ppl;
        pthread_create(&pt[ppl], NULL, citizen_round, &c_params[ppl]);
    }

    for (ppl = 0; ppl < NB_TOTAL; ppl++) {
        pthread_join(pt[ppl], NULL);
    }

    destroy_semaphore(sem, SEM_PATH);
}
