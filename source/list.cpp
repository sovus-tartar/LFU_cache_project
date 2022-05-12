#include "../headers/hashmap.h"
#include "../headers/lfu.h"
#include "../headers/list.h"
#include <stdio.h>
#include <stdlib.h>

List *ListCtor () 
{
    List* lst = (list*) calloc (1, sizeof(list));
    
    lst->fictive = (node_t*) calloc (1, sizeof(node_t));
    lst->fictive->prev = lst->fictive;
    lst->fictive->next = lst->fictive;

    lst->size = 0;

    return lst;
}

int ListDtor (List* lst) 
{
    if (lst == NULL)
    {
        printf("Err in dtor\n");
        return -1;
    }
    
    struct node_t *tmp_elem = lst->fictive->next;

    while (tmp_elem != lst->fictive) {
        struct node_t *next_tmp_elem  = tmp_elem->next; 
        free(tmp_elem);
        tmp_elem = next_tmp_elem;
    }

    lst->size = 0;

    free(lst->fictive);
    free(lst);
    
    return 0;
}


int ListDelete (List* lst, const int number) 
{
    
    if (lst->size == 0) {
        printf("ERROR in delite\n");
        return -1;
    }

    struct node_t *searching = search_elem(lst, number);

    searching->prev->next = searching->next;
    searching->next->prev = searching->prev;

    free(searching);

    lst->size--;
   

    return 0;
}

struct node_t *search_elem (List *lst, const int number) 
{
    
    if (lst->size < number) {
        printf("No searching elem\n");
        return NULL;
    }

    struct node_t *tmp = lst->fictive;

    for (int i = 0; i < number; i++) 
        tmp = tmp->next;

    return tmp;
}

int ListInsertAft (List *lst, const int number, const int value) 
{
    struct node_t *cur_ptr = search_elem (lst, number);

    struct node_t *new_elem = (node_t*) calloc (1, sizeof(node_t));

    new_elem->data = value;
    new_elem->next = cur_ptr->next;
    new_elem->prev = cur_ptr;

    new_elem->next->prev = new_elem;

    cur_ptr->next = new_elem;

    lst->size++;

    return 0;
}

int ListInsertBef (List *lst, const int number, const int value) 
{
    struct node_t *cur_ptr = search_elem (lst, number);
    struct node_t *new_elem = (node_t*) calloc (1, sizeof(node_t));

    new_elem->data = value;
    new_elem->prev = cur_ptr->next;
    new_elem->next = cur_ptr;

    new_elem->prev->next = new_elem;

    cur_ptr->prev = new_elem;

    lst->size++;

    return 0;
}


int ListHeadAdd (List *lst, const int value) {

    struct node_t *new_elem = (node_t*) calloc (1, sizeof(node_t));

    new_elem->data = value;
    new_elem->next = lst->fictive->next;
    new_elem->prev = lst->fictive;

    new_elem->next->prev = new_elem;

    lst->fictive->next = new_elem;
    
    lst->size++;

    return 0;
}

int ListTailAdd (List *lst, const int value) {

    struct node_t *new_elem = (node_t*) calloc (1, sizeof(node_t));

    new_elem->data = value;
    new_elem->next = lst->fictive;
    new_elem->prev = lst->fictive->prev;

    new_elem->prev->next = new_elem;

    lst->fictive->prev = new_elem;
    
    lst->size++;

    return 0;
}


int graph_print (List *lst) {

    FILE *graph = fopen("graph++.txt", "w");

    struct node_t *cur_pos = lst->fictive->next;
    int counter = 0;
    int print_counter = 0;

    if (cur_pos == cur_pos->next) {
        printf("error in dump");
        return -1;
    }

    fprintf(graph, "digraph G{\n"
                   "\trankdir=LR;\n");
    
    while (counter < lst->size) {
        fprintf(graph, "\tstruct%d [shape=record,label=\"  mass\\n%d | <f0> next\\n %p | previous\\n %p\" ];\n", counter, cur_pos->data, cur_pos->next, cur_pos->prev);
        counter++;
        cur_pos = cur_pos->next;
    }

    fprintf(graph, "\t");
    
    while (counter > 0) {
        fprintf(graph, "struct%d:<f0>", print_counter);
        if (counter != 1)
            fprintf(graph, " -> ");

        print_counter++;
        counter--;
    }

    fprintf(graph, ";\n");

    fprintf(graph, "}");

    fclose(graph);

    return 0;
}
