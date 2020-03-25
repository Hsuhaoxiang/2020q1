#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct __list list;
struct __list {
    int data;
    struct __list *addr;
};
void bubble_sort(list *, int);
#define XOR(a, b) ((list *) ((uintptr_t)(a) ^ (uintptr_t)(b)))

void insert_node(list **l, int d)
{
    list *tmp = malloc(sizeof(list));
    tmp->data = d;

    if (!(*l)) {
        tmp->addr = NULL;
    } else {
        (*l)->addr = XOR(tmp, (*l)->addr);
        tmp->addr = *l;
    }
    *l = tmp;
}

void delete_list(list *l)
{
    while (l) {
        list *next = l->addr;
        if (next)
            next->addr = XOR(next->addr, l);
        free(l);
        l = next;
    }
}


void swap(list *node1, list *node2)
{
    int tmp = node1->data;
    node1->data = node2->data;
    node2->data = tmp;
}
list *sort(list *start, int size, int S)
{
    if (!start || !start->addr)
        return start;
    if (size < S) {
        bubble_sort(start, size);
        return start;
    }
    int m2 = size / 2;

    list *left = start, *right = start->addr, *left_end = start;

    for (int i = 0; i < m2 - 1; i++) {
        list *tmp;
        tmp = right;
        right = XOR(right->addr, left_end);
        left_end = tmp;
    }


    left_end->addr = XOR(right, left_end->addr);
    right->addr = XOR(right->addr, left_end);
    left = sort(left, m2, S);
    right = sort(right, size - m2, S);

    for (list *merge = NULL; left || right;) {
        if (!right || (left && left->data < right->data)) {
            list *next = left->addr;
            if (next)
                next->addr = XOR(left, next->addr);

            if (!merge) {
                start = merge = left;
                merge->addr = NULL;
            } else {
                // LL1
                merge->addr = XOR(merge->addr, left);
                // LL2
                left->addr = merge;
                merge = left;
            }
            left = next;
        } else {
            list *next = right->addr;
            if (next)
                next->addr = XOR(right, next->addr);

            if (!merge) {
                start = merge = right;
                merge->addr = NULL;
            } else {
                // RR1
                merge->addr = XOR(merge->addr, right);
                // RR2
                right->addr = merge;
                merge = right;
            }
            right = next;
        }
    }

    return start;
}



list *sort_slow(list *start)
{
    if (!start || !start->addr)
        return start;

    list *left = start, *right = start->addr;
    left->addr = NULL;
    right->addr = XOR(right->addr, left);

    left = sort_slow(left);
    right = sort_slow(right);


    for (list *merge = NULL; left || right;) {
        if (!right || (left && left->data < right->data)) {
            list *next = left->addr;
            if (next)
                next->addr = XOR(left, next->addr);

            if (!merge) {
                start = merge = left;
                merge->addr = NULL;
            } else {
                // LL1
                merge->addr = XOR(merge->addr, left);
                // LL2
                left->addr = merge;
                merge = left;
            }
            left = next;
        } else {
            list *next = right->addr;
            if (next)
                next->addr = XOR(right, next->addr);

            if (!merge) {
                start = merge = right;
                merge->addr = NULL;
            } else {
                // RR1
                merge->addr = XOR(merge->addr, right);
                // RR2
                right->addr = merge;
                merge = right;
            }
            right = next;
        }
    }

    return start;
}

void bubble_sort(list *head, int count)
{
    int i, j, swapped;
    for (i = 0; i < count - 1; i++) {
        list *h = head;
        list *pre = NULL;
        swapped = 0;
        list *p1 = h, *p2 = XOR(p1->addr, pre), *temp;
        for (j = 0; j < count - i - 1; j++) {
            if (p1->data > p2->data) {
                /* update the link after swapping */
                swap(p1, p2);
                swapped = 1;
            }
            temp = p1;
            p1 = p2;
            p2 = XOR(temp, p1->addr);
        }

        /* break if the loop ended without any swap */
        if (swapped == 0)
            break;
    }
}

void best_S(list *mylist, int S)
{
    // list *head = mylist;
    // list *pre = NULL;
    // for (int i=0;i<1000;i++){
    //     printf("%d ",head->data);
    //     list *tmp;
    //     tmp = head;
    //     head = XOR(head->addr,pre);
    //     pre = tmp;
    // }

    // printf("\n\n\n");
    mylist = sort(mylist, 100000, S);
    // head = mylist;
    // pre = NULL;
    // for (int i=0;i<1000;i++){
    //     printf("%d ",head->data);
    //     list *tmp;
    //     tmp = head;
    //     head = XOR(head->addr,pre);
    //     pre = tmp;
    // }
}
long diff_ns(struct timespec t1, struct timespec t2)
{
    return ((long) 1.0e+9 * t2.tv_sec + t2.tv_nsec) -
           ((long) 1.0e+9 * t1.tv_sec + t1.tv_nsec);
}
int main()
{
    FILE *diff = fopen("diff_ns.txt", "w");
    struct timespec t1, t2;
    time_t t;
    srand((unsigned) time(&t));

    for (int i = 1; i <= 10000; i++) {
        list *mylist = NULL;
        for (int j = 0; j < i; j++)
            insert_node(&(mylist), (rand() % 10000));
        clock_gettime(CLOCK_REALTIME, &t1);
        sort(mylist, i,0);
        clock_gettime(CLOCK_REALTIME, &t2);
        fprintf(diff, "%d ", i);
        fprintf(diff, "%ld ", diff_ns(t1, t2));

        mylist = NULL;
        for (int j = 0; j < i; j++)
            insert_node(&(mylist), (rand() % 10000));
        clock_gettime(CLOCK_REALTIME, &t1);
        sort_slow(mylist);
        clock_gettime(CLOCK_REALTIME, &t2);
        fprintf(diff, "%ld \n", diff_ns(t1, t2));
    }
    return 0;
}