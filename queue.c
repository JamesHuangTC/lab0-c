#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));

    if (q) {
        q->head = NULL;
        q->tail = NULL;
        q->size = 0;
    }
    return q;
}

void q_free(queue_t *q)
{
    if (!q)
        return;

    list_ele_t *temp = q->head;

    while (q->head) {
        q->head = q->head->next;
        free(temp->value);
        free(temp);
        temp = q->head;
    }
    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newh;
    newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (!newh)
        return false;

    newh->value = (char *) malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value) {
        free(newh);
        return false;
    }
    strncpy(newh->value, s, strlen(s) + 1);
    newh->next = q->head;
    q->head = newh;
    if (q->size == 0)
        q->tail = newh;
    q->size++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        return false;

    list_ele_t *newt;
    newt = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (!newt)
        return false;
    newt->value = (char *) malloc(sizeof(char) * strlen(s) + 1);
    if (!newt->value) {
        free(newt);
        return false;
    }

    strncpy(newt->value, s, strlen(s) + 1);
    newt->next = NULL;

    if (q->size == 0) {
        q->head = newt;

    } else
        q->tail->next = newt;
    q->tail = newt;
    q->size++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || q->size == 0)
        return false;

    list_ele_t *indirect = q->head;

    if (sp) {
        int copy_len = bufsize - 1 > strlen(q->head->value)
                           ? strlen(q->head->value)
                           : bufsize - 1;
        sp[copy_len] = '\0';
        strncpy(sp, q->head->value, copy_len);
    }
    q->head = indirect->next;
    free(indirect->value);
    free(indirect);
    indirect = NULL;
    q->size--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q)
        return 0;
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || q->size <= 1)
        return;

    q->tail = q->head;

    list_ele_t *middle, *trail;
    middle = NULL;

    while (q->head) {
        trail = middle;
        middle = q->head;
        q->head = q->head->next;
        middle->next = trail;
    }
    q->head = middle;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */

list_ele_t *merge(list_ele_t *l1, list_ele_t *l2)
{
    list_ele_t *head = NULL;
    list_ele_t **tmp = &head;

    while (l1 && l2) {
        if (strcmp(l1->value, l2->value) < 0) {
            *tmp = l1;
            l1 = l1->next;
        } else {
            *tmp = l2;
            l2 = l2->next;
        }
        tmp = &((*tmp)->next);
    }

    if (l1)
        *tmp = l1;
    if (l2)
        *tmp = l2;

    return head;
}

list_ele_t *merge_sort(list_ele_t *head)
{
    if (!head || !head->next)
        return head;

    list_ele_t *fast = head->next;
    list_ele_t *slow = head;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }
    fast = slow->next;
    slow->next = NULL;

    list_ele_t *l1 = merge_sort(head);
    list_ele_t *l2 = merge_sort(fast);

    return merge(l1, l2);
}

void q_sort(queue_t *q)
{
    if (!q || q->size <= 1) {
        return;
    }

    q->head = merge_sort(q->head);

    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}