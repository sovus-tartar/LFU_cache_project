#pragma once

#include <stdlib.h>
#include <assert.h>
#include <math.h>

struct stack_t{
    double *data;
    size_t capacity;
    size_t Size;
};

const int START_STACK_SIZE   = 4;
const int RESIZE_COEFFICIENT = 2;

stack_t* stackCtor();
void stackPush  (stack_t* st, double value);
void stackPop   (stack_t* st); 
void stackDtor  (stack_t* st);
void reallocate (stack_t* st, size_t newSize);