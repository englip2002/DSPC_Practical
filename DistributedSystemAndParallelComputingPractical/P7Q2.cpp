// P7Q2.cpp : Defines the entry point for the console application.
//

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <iostream>
using namespace std;

#ifndef N
#define N 5
#endif
#ifndef FS
#define FS 38
#endif
#define MAX 10

struct node {
    int data;
    int fibdata;
    struct node* next;
};

int fib(int n) {
    int x, y;
    if (n < 2) {
        return (n);
    }
    else {
        x = fib(n - 1);
        y = fib(n - 2);
        return (x + y);
    }
}

void processwork(struct node* p)
{
    int n;
    n = p->data;
    p->fibdata = fib(n);
}

struct node* init_list(struct node* p) {
    int i;
    struct node* head = NULL;
    struct node* temp = NULL;

    head = (struct node*)malloc(sizeof(struct node));
    p = head;
    p->data = FS;
    p->fibdata = 0;
    for (i = 0; i < N; i++) {
        temp = (struct node*)malloc(sizeof(struct node));
        p->next = temp;
        p = temp;
        p->data = FS + i + 1;
        p->fibdata = i + 1;
    }
    p->next = NULL;
    return head;
}

void serial()
{
    double start, end;
    struct node* p = NULL;
    struct node* temp = NULL;
    struct node* head = NULL;

    printf("Process linked list\n");
    printf("  Each linked list node will be processed by function 'processwork()'\n");
    printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n", N, FS);

    p = init_list(p);
    head = p;
    start = omp_get_wtime();
    {
        while (p != NULL) {
            processwork(p);
            p = p->next;
        }
    }


    end = omp_get_wtime();
    p = head;
    while (p != NULL) {
        printf("%d : %d\n", p->data, p->fibdata);
        temp = p->next;
        free(p);
        p = temp;
    }
    free(p);

    printf("Serial: Compute Time: %f seconds\n", end - start);
}

void parallel() {
    double start, end;
    struct node* p = NULL;
    struct node* temp = NULL;
    struct node* head = NULL;
    struct node* parr[MAX];
    int i, count = 0;

    printf("Process linked list\n");
    printf("  Each linked list node will be processed by function 'processwork()'\n");
    printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n", N, FS);

    p = init_list(p);
    head = p;

    start = omp_get_wtime();
    omp_set_num_threads(5);

#pragma omp parallel
    {

#pragma omp for
        for (count = 0; count < N; count++) {
            struct node* parr = head;
            for (int i = 0; i <= count; i++) {
                parr = parr->next;
            }
            processwork(parr);
        }
    }

    end = omp_get_wtime();
    p = head;
    while (p != NULL) {
        printf("%d : %d\n", p->data, p->fibdata);
        temp = p->next;
        free(p);
        p = temp;
    }
    free(p);

    printf("Parallel: Compute Time: %f seconds\n", end - start);
}

void parallelAnswer() {
    double start, end;
    struct node* p = NULL;
    struct node* temp = NULL;
    struct node* head = NULL;
    struct node* parr[MAX];
    int i, count = 0;

    printf("Process linked list\n");
    printf("  Each linked list node will be processed by function 'processwork()'\n");
    printf("  Each ll node will compute %d fibonacci numbers beginning with %d\n", N, FS);

    p = init_list(p);
    head = p;

    p = head;

    start = omp_get_wtime();
    {
        while (p != NULL) {
            p = p->next;
            count++;
        }

        p = head;
        for (i = 0; i < count; i++) {
            parr[i] = p;
            p = p->next;
        }

        omp_set_num_threads(5);
#pragma omp parallel
        {
#pragma omp for schedule(dynamic,5)
            for (i = 0; i < count; i++)
                processwork(parr[i]);
        }
    }
    end = omp_get_wtime();
    p = head;

    while (p != NULL) {
        printf("%d : %d\n", p->data, p->fibdata);
        temp = p->next;
        free(p);
        p = temp;
    }
    free(p);

    printf("Parellel Compute Time: %f seconds\n", end - start);

}

int main(int argc, char* argv[]) {
    serial();
    cout << "\n";
    parallel();
    cout << "\n";
    cout << "\n";
    parallelAnswer();

    return 0;
}