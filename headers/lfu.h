#pragma once
#include "list.h"

typedef struct freq_node_t
{
    int value = 0;
    int length = 0;
    struct elem_t *head = nullptr;
    struct freq_node_t *prev = nullptr;
    struct freq_node_t *next = nullptr;
} freq_node;

typedef struct lfu_cache_t
{
    int size = 0;
    struct freq_node_t *freq_head = nullptr;
    hashmap* Myhashmap = nullptr;
} lfu_cache;

int ACCESS(int key, lfu_cache* Mylfu_cashe);
elem_t* NEW_LFU_ITEM(int data, freq_node* parent);
void DELETE_NODE(freq_node* freq);
freq_node *GET_NEW_NODE(int new_val, freq_node* freq, freq_node* next_freq);

elem_t *hashmap_get_data(hashmap *H, int hash, int key);


