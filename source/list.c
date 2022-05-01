#include "../headers/list.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
list_node *create_list(int n)
{
    int i;
    list_node *top, *curr;

    top = calloc(1, sizeof(list_node));
    assert(top);
    curr = top;

    for (i = 1; i < n; i++)
    {
        list_node *temp;

        temp = calloc(1, sizeof(list_node));
        assert(temp);
        temp->prev = curr;
        curr->next = temp;
        curr = temp;
    }

    curr->next = top;
    top->prev = curr;
#ifdef DEBUG
    printf("List created\n");
#endif
    return top;
}

void list_remove_elem(list_node *node)
{
    list_node *prev_el, *next_el;

    prev_el = node->prev;
    next_el = node->next;

    prev_el->next = next_el;
    next_el->prev = prev_el;
#ifdef DEBUG
    printf("List: node (%p) with val: %d removed\n", node, node->data);
#endif
    free(node);
}

list_node *list_get_last(list_node *top)
{
#ifdef DEBUG
    printf("List: got last element (%p)\n", top->prev);
#endif
    return top->prev;
}

list_node *list_create_node(int data)
{
    list_node *temp;
    temp = calloc(1, sizeof(list_node));
    assert(temp);
    temp->data = data;
#ifdef DEBUG
    printf("List: node (%p) with data: %d created\n", temp, data);
#endif
    return temp;
}

list_node *list_put_to_top(list_node *top, list_node *node)
{
    assert(top);
    assert(node);

    node->next = top;
    node->prev = top->prev;
    top->prev->next = node;
    top->prev = node;

#ifdef DEBUG
    printf("List: node (%p) with data: %d put to top\n", node, node->data);
#endif


    return node;
}

list_node *list_move_to_top(list_node *top, list_node *node)
{
    list_node *prev_el, *next_el;

    assert(top);
    assert(node);

    if (top == node) {
#ifdef DEBUG
        printf("List: node (%p) is already on the top\n", node);
#endif
        return node;
    }
    prev_el = node->prev;
    next_el = node->next;

    prev_el->next = next_el;
    next_el->prev = prev_el;

#ifdef DEBUG
    printf("List: node (%p) moved to top\n", node);
#endif

    return list_put_to_top(top, node);
}

void list_delete(list_node * top) {
    list_node * curr;

    curr = top -> next;
    free(top);

    while(curr != top) {
        list_node * temp;
        temp = curr;
        curr = curr -> next;
        free(temp);
    }
}