#include "stack.h"

stack_t* stackCtor(){
    struct stack_t *st = (stack_t*)calloc(1, sizeof(stack_t));

    st->capacity = START_STACK_SIZE; 
    st->Size = 0;

    st->data = (double*)calloc(st->capacity + 2, sizeof (double));
    assert(st->data);

    st->data++;

    return st;
}


//-----------------------------------------------------------------------------


void stackPush(stack_t* st, double value){
    assert(st);
    assert(isfinite(value));

    if (st->Size >= st->capacity)
        reallocate (st, st->capacity * RESIZE_COEFFICIENT);

    *(st->data + st->Size) = value;
    
    st->Size++;
}


//-----------------------------------------------------------------------------


void stackPop(stack_t* st){
    assert(st);

    if ((st->Size <= st->capacity/RESIZE_COEFFICIENT) && (st->capacity != START_STACK_SIZE))
        reallocate (st, st->capacity/RESIZE_COEFFICIENT);
    
    --st->Size;
}


//-----------------------------------------------------------------------------


void stackDtor(stack_t* st){
    assert(st);

    free (--st->data);
    free(st);
}


//-----------------------------------------------------------------------------


void reallocate(stack_t* st, const size_t newSize){
    assert(st);

    st->capacity = newSize;
    st->data--;

    double *tmp = (double*) realloc (st->data, (st->capacity + 2) * sizeof(double));
    if (tmp != NULL){   
        st->data = tmp;
        st->data++;
    }
    else
        assert(0);
}
