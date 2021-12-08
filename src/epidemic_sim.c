#include "../include/epidemic_sim.h"

int main(void) {
    int citizen_number;
    int city_mem;
    int timer_pd, round_pd;
    int running;
    city_t *c;
    char buffer[BUFFER_SIZE];
    ssize_t message_length;
    FILE *fptr;
    int pid;
    int round_nb;

    running = 1;
    round_nb = 1;

    initscr();

    srand(time(NULL));

    /* opening and initializing evolution file */
    fptr = fopen("evolution.txt", "w+");
    if (fptr == NULL) {
        printf("Graph file error while opening");
    }

    /*Creating shared memory and mapping */
    city_mem = create_city_shared_memory();

    c = (city_t *) mmap(NULL, sizeof(city_t), PROT_READ | PROT_WRITE, MAP_SHARED, city_mem, 0);

    init_city(c);

    /* Initialize and link pipe to synchronize timer, epidemic and citizen_manager*/
    fflush(stdout);
    create_pipe("./roundPipe", S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
    strcpy(buffer, "");
    fflush(stdout);
    do {
        citizen_number = get_number_citizen(c);
    } while (citizen_number == 0);

    init_display(c);
    fflush(stdout);
    timer_pd = open_read_pipe("./timerPipe");

    /* Start of simulation*/
    while (running) {
        message_length = read(timer_pd, buffer, BUFFER_SIZE);
        if (message_length > 0) {
            round_nb++;
            /* signal from timer to end, then sent to citizen manager*/
            if (strcmp("end", buffer) == 0) {
                running = 0;
            }
            round_pd = open_write_pipe("./roundPipe");
            write(round_pd, buffer, BUFFER_SIZE);
            close(round_pd);
            strcpy(buffer, "");
            write_evolution(fptr, get_numbers(c));
            round_display(c, round_nb);
        }
    }
    end_press_agency();
    destroy_shared_memory(city_mem, c);
    fclose(fptr);
    pid = fork();
    if (pid < 0) {
        manage_error_and_exit();
    }
    if (pid == 0) {
        exec_gnuplot("commands.gp");
    }
    endwin();
    exit(EXIT_SUCCESS);
}


void destroy_shared_memory(int mem, city_t *c) {
    if (munmap(c, sizeof(city_t)) < 0) {
        perror("Error when calling munmap()\n");
    }
    if (close(mem) < 0) {
        perror("Error when calling close(city_mem)\n");
    }
    if (shm_unlink(SHARED_MEM_CITY) < 0) {
        perror("Error when calling shm_unlink()\n");
    }
}

void end_press_agency() {
    mqd_t mq;
    char buffer[BUFFER_SIZE];
    strcpy(buffer, "end");
    mq = mq_open("/news", O_WRONLY | O_CREAT, 0644, NULL);
    mq_send(mq, buffer, sizeof(buffer), 100);
}

void write_evolution(FILE *fptr, int evo[4]) {
    fprintf(fptr, "%d ", evo[0]);
    fprintf(fptr, "%d ", evo[1]);
    fprintf(fptr, "%d ", evo[2]);
    fprintf(fptr, "%d \n", evo[3]);
}

int *get_numbers(city_t *city) {
    static int tmp[4];

    tmp[0] = get_number_of_healthy(city);
    tmp[1] = get_number_of_sick(city);
    tmp[2] = get_number_of_dead(city);
    tmp[3] = get_number_of_burned(city);
    return tmp;
}

