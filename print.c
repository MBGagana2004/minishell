#include "main.h"

extern int job_count;

void print_list(P_list *tail)
{
    if(tail == NULL)  // If there is no current jobs
    {
        printf("jobs : No current jobs\n");
        return;
    }

    int i = 1;

    while(tail != NULL)
    {
        if(i == job_count)
        {
            printf("[%d]+   Stopped\t\t%s\n", i++, tail->command);
        }
        else if(i == job_count - 1)
        {
            printf("[%d]-   Stopped\t\t%s\n", i++, tail->command);
        }
        else
        {
            printf("[%d]   Stopped\t\t%s\n", i++, tail->command);
        }

        tail = tail->prev;
    }

    return;
}