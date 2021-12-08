#ifndef SIMULATION_EPIDEMIE_CITIZEN_H
#define SIMULATION_EPIDEMIE_CITIZEN_H
#define CITIZEN_NAME_MAX_LENGTH 32

typedef enum citizen_type_e {
    NORMAL, FIREFIGHTER, DOCTOR, REPORTER, DEAD, BURNED
} citizen_type_e;

/**
 * Defines the status of a citizen.
 * @param x The x coordinate in the city
 * @param y The y coordinate in the city
 * @param contamination The level of contamination (between 0 and 1)
 * @param is_sick A boolean to say if the citizen is sick.
 * @param name The name of the citizen (Ah ah ah)
 * @param sickness_duration The number of days the citizen have been sick.
 * @param type The type of the citizen.
 * @param days_spend_in_hospital_healthy the number of days spent in the hospital while not being sick
 */
typedef struct status {
    unsigned int x;
    unsigned int y;
    double contamination;
    int is_sick;
    char name[CITIZEN_NAME_MAX_LENGTH];
    int sickness_duration;
    citizen_type_e type; /* Un enum : NORMAL, FIREFIGHTER, DOCTOR, REPORTER, DEAD, BURNED */
    int days_spent_in_hospital_healthy;
    int days_out_of_hospital;
    int tool;
} status_t;

/**
 * Create and initiaze right values for type citizen
 * @return
 */
status_t create_citizen(citizen_type_e, unsigned int, unsigned int, int);

/**
 * display citizen
 */
void display_citizen(status_t);

/**
 * display status of citizen
 */
void display_status(citizen_type_e);

/**
 * try to make the citizen sick
 */
void get_sick(status_t *);

/**
 * add to the contamination rate of the citizen
 */
void add_citizen_contamination(status_t *, double);

/**
 *
 * @return  1 if citizen at the position 0 if not
 */
int is_at_position(status_t, unsigned int, unsigned int);

/**
 * try to kill the citizen
 */
void die(status_t *, double);

/**
 * heal the citizen
 */
void heal(status_t *);

/**
 * refill the tools
 * (only for types DOCTOR and FIREFIGHTER)
 */
void refill_tools(status_t *, int, int);

#endif
