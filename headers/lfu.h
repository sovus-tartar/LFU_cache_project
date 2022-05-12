#pragma once
#include "list.h"

struct freq_node_t
{
    int value = 0;
    int length = 0;
    struct node_t *head = nullptr;
    struct freq_node_t *prev = nullptr;
    struct freq_node_t *next = nullptr;
};

struct lfu_cache
{
    int size = 0;
    struct freq_node_t *freq_head = nullptr;
    hashmap* hash_map = nullptr;
};

int access(int key, struct lfu_cache* cashe);
struct node_t* new_lfu_item(int data, struct freq_node_t* parent);
void delete_node(struct freq_node_t* freq);
struct freq_node_t *get_new_node(int new_val, struct freq_node_t* freq, struct freq_node_t* next_freq);
struct node_t *hashmap_get_data(hashmap *H, int hash, int key);


