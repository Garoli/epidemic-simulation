#include "../include/display.h"

int w_width;
int w_height;

void init_display(city_t *city) {

    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    system("clear");
    if (has_colors() == FALSE) {
        endwin();
        handle_fatal_error("Your terminal does not support color \n");
    }

    init_colors();

    w_width = w.ws_col;
    w_height = w.ws_row;

    mvprintw(1, (w_width / 2) - 8, "Epidemic Simulation");
    mvprintw(1, w_width - 9, "Round %d", 1);

    init_city_map(city);
    init_citizen_map(city);
    init_info(city);
    init_death_notice();
    refresh();
}

void round_display(city_t *city, int round_nb) {
    refresh_city_map(city);
    refresh_citizen_map(city);
    refresh_info(city);
    refresh_death_notice(city);
    mvprintw(1, w_width - 3, "%d", round_nb);
    refresh();
}

void init_colors() {
    start_color();
    init_pair(1, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_YELLOW);
    init_pair(3, COLOR_WHITE, COLOR_RED);
    init_pair(4, COLOR_WHITE, COLOR_GREEN);
    init_pair(6, COLOR_CYAN, COLOR_BLACK);
    init_pair(7, COLOR_BLACK, COLOR_WHITE);
    init_pair(8, COLOR_WHITE, COLOR_BLACK);
    init_pair(9, COLOR_BLACK, COLOR_RED);
}

int get_color(float contamination_rate) {
    if (contamination_rate < 0.1) {
        return 4;
    }
    if (contamination_rate < 0.3) {
        return 2;
    }
    return 3;
}

char *get_type(building_type_e type) {
    switch (type) {
        case HOSPITAL:
            return " H ";
        case FIRESTATION:
            return " F ";
        case HOUSE:
            return " ^ ";
        default:
            return "   ";
    }

}

void init_city_map(city_t *city) {

    attron(COLOR_PAIR(1));
    mvprintw(3, 7, "Disney town");
    attroff(COLOR_PAIR(1));
    refresh_city_map(city);
}


void refresh_city_map(city_t *city) {
    int i, j, start_i, start_j;
    int color;
    start_i = 5;
    start_j = 2;

    for (i = 0; i < CITY_WIDTH; i++) {
        for (j = 0; j < CITY_HEIGHT; j++) {
            color = get_color(city->_map[i][j]._contamination);
            attron(COLOR_PAIR(color));
            mvprintw(start_i + i, start_j + (j * 3), get_type(city->_map[i][j]._type));
            attroff(COLOR_PAIR(color));
        }
    }
}

void init_citizen_map(city_t *city) {

    int start_j = (w_width / 2) + 2;

    attron(COLOR_PAIR(6));
    mvprintw(3, start_j + 4, "Citizens movements");
    attroff(COLOR_PAIR(6));

    refresh_citizen_map(city);
}

void refresh_citizen_map(city_t *city) {
    int i, j, start_i, start_j;
    start_i = 5;
    start_j = (w_width / 2) + 2;

    for (i = 0; i < CITY_WIDTH; i++) {
        for (j = 0; j < CITY_HEIGHT; j++) {
            attron(COLOR_PAIR(7));
            mvprintw(start_i + i, start_j + (j * 3), " %d ", get_number_citizen_on_tile(i, j, city));
            attroff(COLOR_PAIR(7));
        }
    }
}

void init_info(city_t *city) {
    attron(COLOR_PAIR(8));
    mvprintw((w_height / 2) + 1, 2, "Disney epidemic in numbers");
    attroff(COLOR_PAIR(8));

    mvprintw((w_height / 2) + 3, 2, "Average contamination: %f", average_contamination(city));
    mvprintw((w_height / 2) + 4, 2, "Healthy people:        %d", get_number_of_healthy(city));
    mvprintw((w_height / 2) + 5, 2, "Sick people:           %d", get_number_of_sick(city));
    mvprintw((w_height / 2) + 6, 2, "Dead people:           %d", get_number_of_dead(city));
    mvprintw((w_height / 2) + 7, 2, "Burned people:         %d", get_number_of_burned(city));

}

void refresh_info(city_t *city) {
    mvprintw((w_height / 2) + 3, 25, "%f   ", average_contamination(city));
    mvprintw((w_height / 2) + 4, 25, "%d   ", get_number_of_healthy(city));
    mvprintw((w_height / 2) + 5, 25, "%d   ", get_number_of_sick(city));
    mvprintw((w_height / 2) + 6, 25, "%d   ", get_number_of_dead(city));
    mvprintw((w_height / 2) + 7, 25, "%d   ", get_number_of_burned(city));
}


void init_death_notice() {
    attron(COLOR_PAIR(7));
    mvprintw((w_height / 2) + 1, (w_width / 2) + 8, "Death Notice");
    attroff(COLOR_PAIR(7));
    mvprintw((w_height / 2) + 3, (w_width / 2) + 5, "No corpse can be found ... yet. ");
}

void refresh_death_notice(city_t *city) {
    int nb = get_number_of_dead(city);
    int *deads = get_dead_id(city);
    int i;

    for (i = 0; i < NB_TOTAL; i++) {
        mvprintw((w_height / 2) + 3 + i, (w_width / 2) + 4, "                                 ");
    }

    if (nb != 0) {
        attron(COLOR_PAIR(9));
        mvprintw((w_height / 2) + 1, (w_width / 2) + 8, "Death Notice");
        attroff(COLOR_PAIR(9));
        for (i = 0; i < nb; i++) {
            mvprintw((w_height / 2) + 3 + i, (w_width / 2) + 4, "%s is dead        ", city->_citizens[deads[i]].name);
        }
    } else {
        init_death_notice();
    }
}

void init_agency(int deads, int sicks, float conta, int journalist) {
    attron(COLOR_PAIR(9));
    mvprintw((w_height / 2) + 9, 2, "!! BREAKING NEWS !!");
    attroff(COLOR_PAIR(9));
    attron(COLOR_PAIR(8));
    mvprintw((w_height / 2) + 10, 2, "!! NEW DISEASE IN TOWN !!");
    attroff(COLOR_PAIR(8));
    refresh_agency(deads, sicks, conta, journalist);
}

void refresh_agency(int deads, int sicks, float conta, int journalist) {
    mvprintw((w_height / 2) + 11, 2, "%d people already DIED", deads);
    mvprintw((w_height / 2) + 12, 2, "%d are already SICK", sicks);
    mvprintw((w_height / 2) + 13, 2, "%f average tile contamination", conta);
    if (journalist) {
        mvprintw((w_height / 2) + 14, 2, "even our journalist reporting those news is sick");
    } else {
        mvprintw((w_height / 2) + 14, 2, "                                                 ");
    }
}
