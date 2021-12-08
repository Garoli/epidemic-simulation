#include "../include/press_agency.h"


void minimize_by_x(char *buff, double perc) {
    double tmp = atof(buff);
    tmp = tmp - (perc * tmp);
    sprintf(buff, "%f", tmp);
}

int main() {
    int n;
    unsigned int priority;
    mqd_t mq;
    struct mq_attr attr;
    char *buffer = NULL;
    int running;
    int journalist_contaminated, dead_people, sick_people;
    double average_contamination;
    int news = 0;

    mq = mq_open("/news", O_RDONLY);
    running = 1;
    journalist_contaminated = 0;
    dead_people = 0;
    sick_people = 0;
    average_contamination = 0;

    while (running) {
        mq_getattr(mq, &attr);
        buffer = (char *) malloc(attr.mq_msgsize);
        n = mq_receive(mq, buffer, attr.mq_msgsize, &priority);
        if (n == -1) {
            printf("Error in press agency reading message \n");
            exit(EXIT_FAILURE);
        }
        if (buffer != NULL) {
            switch (priority) {
                case 1:
                    if (atof(buffer) > 0.8) {
                        journalist_contaminated = 1;
                    }
                    break;
                case 10:
                    minimize_by_x(buffer, 0.35);
                    dead_people = atoi(buffer);
                    break;
                case 5:
                    minimize_by_x(buffer, 0.10);
                    average_contamination = atof(buffer);
                    break;
                case 2:
                    minimize_by_x(buffer, 0.10);
                    sick_people = atoi(buffer);
                    break;
                case 100:
                    running = 0;
                    break;
                default:
                    fprintf(stdout, "unknown priority : %s\n", buffer);
                    exit(EXIT_FAILURE);
            }
        }

        /* Only notify people if the news are interesting, no one care about a little cold isn't it .. */
        if (dead_people > 0 || sick_people >= 2) {
            news++;
            if (news == 1) {
                init_agency(dead_people, sick_people, average_contamination, journalist_contaminated);
            } else {
                refresh_agency(dead_people, sick_people, average_contamination, journalist_contaminated);
            }

        }
    }
    return 0;
}
