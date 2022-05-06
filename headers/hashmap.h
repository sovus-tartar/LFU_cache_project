#pragma once

typedef struct hashmap_node_t
{
    void *data; //!!!!!!!!
    struct hashmap_node_t *prev;
    struct hashmap_node_t *next;
    struct freq_node_t *parent;
} hashmap_node;

typedef struct hashmap_arr_t 
{
    hashmap_node * node_ptr; //ptr to the start of part of list of collisions
    int collisions;
} hashmap_arr;

typedef struct hashmap_t
{
    int number_of_elements;          // powerfullness of hash
    hashmap_arr * node_arr; // node_arr[hash]
    hashmap_node * list_start;
} hashmap;

int hash_count(int key);
hashmap *hashmap_create();
void hashmap_delete(hashmap *H);
hashmap_node *hashmap_create_node(void *data);
void hashmap_add_node(hashmap *H, hashmap_node *node, int hash);
hashmap_node *hashmap_get_node(hashmap *H, int hash, void *data);
void hashmap_delete_node(hashmap *H, int hash, void *data);
void hashmap_dump(hashmap *H);
