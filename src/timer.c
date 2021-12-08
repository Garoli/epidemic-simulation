#include "../include/timer.h"

int roundNb;
int running;

void startRound() {
    roundNb++;
    if (roundNb <= ROUND_NUMBER) {
        struct sigaction action;
        action.sa_handler = (__sighandler_t) &endOfRound;
        sigaction(SIGALRM, &action, NULL);
        fflush(stdout);
        alarm(DURATION);
    }
}

void endOfRound() {
    int pd;
    char buffer[BUFFER_SIZE];

    pd = open_write_pipe("./timerPipe");
    strcpy(buffer, "Message de fin de tour\n");
    if (roundNb == ROUND_NUMBER) {
        strcpy(buffer, "end");
        running = 0;
    }
    write(pd, buffer, BUFFER_SIZE);

    close(pd);
    startRound();
}

int main() {
    int pipe;

    roundNb = 0;
    running = 1;
    pipe = create_pipe("./timerPipe", S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);

    if (pipe == -1) {
        printf("Couldn't create pipe (maybe already created)\n");
    }
    startRound();
    while (running);
    return (0);
}



