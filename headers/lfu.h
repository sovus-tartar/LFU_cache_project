#pragma once
#include "list.h"

struct freq_node_t
{
    int value = 0;
    int length = 0;
    List *node_list = nullptr;
    struct freq_node_t *prev = nullptr;
    struct freq_node_t *next = nullptr;
};

struct lfu_cache_t
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

struct lfu_cache* new_lfu_cache(int size);
struct node_t* get_lfu_item(struct lfu_cache* LfuCache);

int insert(int key, lfu_cache_t *cache);

struct freq_node_t * new_freq_node();
struct lfu_cache_t * new_lfu_cache(int size);
