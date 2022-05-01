#pragma once

typedef struct list_node_t
{
    struct list_node_t *prev;
    struct list_node_t *next;
    int data;
} list_node;

list_node *create_list(int n);
void list_remove_elem(list_node *node);
list_node *list_get_last(list_node *top);
list_node *list_create_node(int data);
list_node *list_put_to_top(list_node *top, list_node *node);
list_node *list_move_to_top(list_node *top, list_node *node);
void list_delete(list_node * top);
