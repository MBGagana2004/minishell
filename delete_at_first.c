#include "main.h"

void delete_at_first(P_list **head, P_list **tail)
{
    if(*head == NULL || *tail == NULL)  // List empty
    {
        return;
    }

    P_list *temp = *head;

    if(*head == *tail)  // If only one node
    {
        *head = NULL;
        *tail = NULL;
    }
    else
    {
        *head = temp->next;
        (*head)->prev = NULL;
    }

    free(temp);
    return;
}
