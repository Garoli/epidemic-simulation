#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../include/citizen.h"

status_t create_citizen(citizen_type_e type, unsigned int x, unsigned int y, int name_id) {
    const char *const names[] = {"Duchesse", "Mégara", "Judy Hopps", "Mulan", "Tiana", "Jasmine", "Ariel", "Hercule",
                                 "Buzz", "Baymax", "Rémy", "Robin", "Couette", "Mérida", "Mickey", "Pluto", "Tic",
                                 "Tac", "Scar", "Gantu", "Stich", "Tamatoa", "Hans", "Anna", "Elsa", "Edna Mode",
                                 "Zazu", "Simba", "Nala", "Dumbo", "Lilo", "Eve", "Alice", "Quasimodo", "Timide",
                                 "Olaf", "Atchoum"};
    status_t cit;
    cit.x = x;
    cit.y = y;
    cit.contamination = 0.0;
    cit.is_sick = 0;
    strcpy(cit.name, names[name_id]);
    cit.sickness_duration = 0;
    cit.type = type;
    cit.days_spent_in_hospital_healthy = 0;
    cit.days_out_of_hospital = 0;
    cit.tool = 0;
    if (type == DOCTOR) {
        cit.tool = 5;
    }
    if (type == FIREFIGHTER) {
        cit.tool = 500;
    }
    return cit;
}

void display_citizen(status_t cit) {
    printf("[%d,%d]", cit.x, cit.y);
    printf("contamination %.2f ", cit.contamination);
    printf("sick %d ", cit.is_sick);
    printf("name %s ", cit.name);
    printf("sickness duration %d ", cit.sickness_duration);
    display_status(cit.type);
    printf("days_spent_in_hospital_healthy %d ", cit.days_spent_in_hospital_healthy);
    printf("days_out_of_hospital %d ", cit.days_out_of_hospital);
    if (cit.type == DOCTOR) {
        printf("aid kits : %d", cit.tool);
    }
    if (cit.type == FIREFIGHTER) {
        printf("sprayer capacity : %d ", cit.tool);
    }
    printf("\n");
}

void display_status(citizen_type_e status) {
    switch (status) {
        case NORMAL:
            printf("NORMAL ");
            break;
        case FIREFIGHTER:
            printf("FIREFIGHTER ");
            break;
        case DOCTOR:
            printf("DOCTOR ");
            break;
        case REPORTER:
            printf("REPORTER ");
            break;
        case DEAD:
            printf("DEAD ");
            break;
        case BURNED:
            printf("BURNED ");
            break;
        default:
            printf("BUG ");
            break;
    }
}

void get_sick(status_t *citizen) {
    double roll = (rand() / (RAND_MAX + 1.0));
    if (roll < citizen->contamination) {
        citizen->is_sick = 1;
    }
}

void add_citizen_contamination(status_t *citizen, double cont) {
    if (citizen->type == FIREFIGHTER) {
        cont = cont / 10;
    }
    citizen->contamination += cont;
    if (citizen->contamination < 0) {
        citizen->contamination = 0;
    }
    if (citizen->contamination > 1) {
        citizen->contamination = 1;
    }
}

int is_at_position(status_t citizen, unsigned int x, unsigned int y) {
    return (citizen.x == x && citizen.y == y) ? 1 : 0;
}


void die(status_t *citizen, double chance) {
    int roll = (rand() % 100);
    if (roll < chance * 100) {
        citizen->type = DEAD;
    }
}

void heal(status_t *citizen) {
    citizen->is_sick = 0;
    citizen->contamination = 0;
    citizen->days_spent_in_hospital_healthy = 0;
}

void refill_tools(status_t *citizen, int in_hospital, int in_firestation) {
    if (citizen->type == DOCTOR && in_hospital) {
        citizen->tool = 10;
    }
    if (citizen->type == FIREFIGHTER && in_firestation) {
        citizen->tool = 1000;
    }
}
