#include "main.h"

void insert_at_first(P_list **head, P_list **tail, int pid, char *input_string)
{
    // Create new node
    P_list *new = (P_list *)malloc(sizeof(P_list));  

    // validate new node
    if(new == NULL)
    {
        return;
    }

    // Update with values
    new->pid = pid;
    strcpy(new->command, input_string);
    
    new->prev = NULL;
    new->next = *head;

    if (*head == NULL)
    {
        *head = new;
        *tail = new;
    }
    else
    {
        (*head)->prev = new;
        *head = new;
    }

    return;
}