#pragma once

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include "stack.h"

//#define GRAPH_DUMP

#define PREV(x) strc->arr[x].prev
#define NEXT(x) strc->arr[x].next
#define DATA(x) strc->arr[x].data

#define HEAD strc->head
#define TAIL strc->tail

#define N_FREE_ELEM strc->num_of_free_elem
#define FREE_ELEM   strc->free_elem

struct arr_t {
    double data;
    int next;
    int prev;
};

struct list_t {
    struct arr_t *arr;
    struct stack_t *free_elem;
    size_t num_of_free_elem;
    size_t head;
    size_t tail;
    size_t capacity;
};

const int size_of_list = 1;
const int resize_coeff = 2;

list_t* list_ctor(); 
void    list_dtor(list_t *strc);
arr_t*  arr_ctor();

size_t idx_of_head(const list_t *strc);
size_t idx_of_tail(const list_t *strc);
size_t idx_of_next(const list_t *strc, size_t idx);
size_t idx_of_prev(const list_t *strc, size_t idx);

void ins_at_the_beg(list_t *strc, double val);
void ins_at_the_end(list_t *strc, double val);

void ins_before(list_t *strc, size_t elem, double val);
void ins_after (list_t *strc, size_t elem, double val);

void del_elem(list_t *strc, size_t elem);
void del_ALL_elem(list_t *strc);

void realloc_list(list_t *strc);

int  get_free_elem_idx(list_t *strc);

void make_graph_dump(const list_t *strc);