#include <stddef.h>
#include "ll_cycle.h"

#include <stdbool.h>

int ll_has_cycle(node *head) {
    if (head == NULL)
        return 0;

    const node *hare = head;
    const node *tortoise = head;

    while (true) {
        if (hare->next == NULL || hare->next->next == NULL)
            break;

        hare = hare->next->next;
        tortoise = tortoise->next;

        if (hare == tortoise)
            return 1;
    }

    return 0;
}
