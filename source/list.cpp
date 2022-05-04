#include "../headers/list.h"

list_t* list_ctor(){
    struct list_t *strc = (list_t*)calloc(1, sizeof(list_t));

    strc->arr = arr_ctor();
    FREE_ELEM = stackCtor();

    for (int num = size_of_list; num >= 1; num--)
        stackPush(FREE_ELEM, num);

    N_FREE_ELEM = size_of_list;
    HEAD = 1;
    TAIL = 0;
    strc->capacity = size_of_list;

    return strc;
}

void list_dtor(list_t*strc){
    assert(strc);

    free(strc->arr);
    stackDtor(FREE_ELEM);
    free(strc);
}

arr_t *arr_ctor() {
    struct arr_t *arr = (arr_t *)calloc(size_of_list + 1, sizeof(arr_t));
    assert(arr);

    for (int idx = 0; idx <= size_of_list; idx++) {
        arr[idx].data = 0;
        arr[idx].next = -1;
        arr[idx].prev = -1;
    }

    arr[0].next = 0;
    arr[0].prev = 0;

    return arr;
}

size_t idx_of_head(const list_t *strc){
    assert(strc);
    return HEAD;
}

size_t idx_of_tail(const list_t *strc){
    assert(strc);
    return TAIL;
}

size_t idx_of_next(const list_t *strc, size_t idx){
    assert(strc);
    assert(isfinite(idx));

    return NEXT(idx);
}

size_t idx_of_prev(const list_t *strc, size_t idx){
    assert(strc);
    assert(isfinite(idx));

    return PREV(idx);
}

void ins_at_the_beg(list_t *strc, double val){
    assert(strc);
    assert(isfinite(val));

    if (N_FREE_ELEM == 0) 
        realloc_list(strc); 

    int idx = get_free_elem_idx(strc);

    DATA(idx) = val;
    NEXT(idx) = HEAD;
    PREV(idx) = 0;

    PREV(HEAD) = idx;

    NEXT(0) = idx;

    HEAD = idx;
}

void ins_at_the_end(list_t *strc, double val){
    assert(strc);
    assert(isfinite(val));

    if (N_FREE_ELEM == 0) 
        realloc_list(strc); 

    int idx = get_free_elem_idx(strc);

    DATA(idx) = val;
    NEXT(idx) = 0;
    PREV(idx) = TAIL;

    NEXT(TAIL) = idx;

    PREV(0) = idx;

    TAIL = idx;
}

void ins_before(list_t *strc, size_t elem, double val){
    assert(strc);
    assert(isfinite(val));
    assert(isfinite(elem));

    if (N_FREE_ELEM == 0) 
        realloc_list(strc); 

    int idx = get_free_elem_idx(strc);

    size_t prev_of_elem = PREV(elem);

    DATA(idx) = val;
    NEXT(idx) = elem;
    PREV(idx) = prev_of_elem;

    PREV(elem) = idx;

    NEXT(prev_of_elem) = idx;
}

void ins_after(list_t *strc, size_t elem, double val){
    assert(strc);
    assert(isfinite(val));
    assert(isfinite(elem));

    if (N_FREE_ELEM == 0) 
        realloc_list(strc); 

    int idx = get_free_elem_idx(strc);

    size_t next_of_elem = NEXT(elem);

    DATA(idx) = val;
    NEXT(idx) = next_of_elem;
    PREV(idx) = elem;

    NEXT(elem) = idx;

    PREV(next_of_elem) = idx;
}

void del_elem(list_t *strc, size_t elem){
    assert(strc);
    assert(isfinite(elem));

    if (elem == HEAD)
        HEAD = NEXT(elem);

    if (elem == TAIL)
        TAIL = PREV(elem);

    size_t next_of_elem = NEXT(elem);
    size_t prev_of_elem = PREV(elem);

    PREV(next_of_elem) = prev_of_elem;
    NEXT(prev_of_elem) = next_of_elem;

    DATA(elem) = 0;
    NEXT(elem) = -1;
    PREV(elem) = -1;

    stackPush(FREE_ELEM, elem);
    N_FREE_ELEM++;
}

void del_ALL_elem(list_t *strc){
    assert(strc);

    for (unsigned idx = 0; idx <= strc->capacity; idx++) {
        DATA(idx) = 0;
        NEXT(idx) = -1;
        PREV(idx) = -1;         
    }
}

void realloc_list(list_t *strc){
    assert(strc);

    size_t old_capacity = strc->capacity;
    strc->capacity *= resize_coeff;

    arr_t *tmp = (arr_t *)realloc(strc->arr, (strc->capacity + 1) * sizeof(arr_t));
    if (!tmp) 
        assert(0);

    strc->arr = tmp;

    for (unsigned idx = strc->capacity; idx >= old_capacity + 1; idx--) {
        stackPush(FREE_ELEM, idx);
        N_FREE_ELEM++;

        DATA(idx) = 0;
        NEXT(idx) = -1;
        PREV(idx) = -1;  
    }
}


int get_free_elem_idx(list_t *strc) {
    assert(strc);

    int idx = FREE_ELEM->data[FREE_ELEM->Size - 1]; 
    stackPop(FREE_ELEM);                               
    N_FREE_ELEM--;

    return idx;
}




void make_graph_dump(const list_t *strc) {
    #ifdef GRAPH_DUMP
    
    assert(strc);

    FILE* output = fopen("GRAPH.dot", "wb");
    assert(output);

    fputs("digraph line{\n", output);
    fputs("\trankdir = LR;\n", output);
    
    for (unsigned num = 0;  num <= strc->capacity; num++) {
        if (strc->arr[num].next != -1) {
            fprintf(output, "\tstruct%d [shape = \"record\", style = \"filled\", fillcolor = \"beige\", label = \"<d>%d| %.2lf | <n>%d | %d\"];\n",
                            num, num, strc->arr[num].data, strc->arr[num].next, strc->arr[num].prev);
        }
    }

    fputs("subgraph cluster0 {\n", output);
    for (unsigned num = 0;  num <= strc->capacity; num++) {
        if (strc->arr[num].next == -1) 
            fprintf(output, "\tstruct%d [shape = \"octagon\", style = \"filled\", fillcolor = \"darkseagreen1\", label = \"%d\"];\n", num, num);
    }

    fputs("\tlabel = \"Free elements\";\n", output);
    fputs("\tcolor = green;\n", output);
    fprintf(output, "}\n");

    fprintf(output, "\tstructa [shape = \"house\", style = \"filled\", fillcolor = \"pink\", label = \"%ld\"];\n", strc->tail);
    fprintf(output, "\tstructb [shape = \"invhouse\", style = \"filled\", fillcolor = \"plum1\", label = \"%ld\"];\n", strc->head);

    for (unsigned num = 0; num <= strc->capacity; num++) {
        if (strc->arr[num].next !=-1) {
            fprintf(output, "\tstruct%d:<n> -> struct%d:<d>;\n", num, strc->arr[num].next);
        }
    }

    fprintf(output, "}");
    fclose(output);

    #endif
}