/*
 * Function: dll_remove
 * Address:  0x8000C050
 * Category: dll
 * Status:   WIP
 *
 * Remove node from singly-linked list.
 * Searches list for target node and unlinks it.
 *
 * Compiler flags: -g0 -O2 -mips2 -G 0 -non_shared
 */

#include "types.h"

typedef struct DLLNode {
    struct DLLNode *next;
} DLLNode;

void dll_remove(DLLNode **head, DLLNode *target) {
    DLLNode *prev = (DLLNode *)head;
    DLLNode *curr = *head;

    while (curr != NULL) {
        if (curr == target) {
            /* Unlink: prev->next = target->next */
            prev->next = target->next;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}
