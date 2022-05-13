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


int access(int key, struct lfu_cache_t* cashe);
struct node_t* new_lfu_item(int data, struct freq_node_t* parent);
struct freq_node_t * get_new_node(int val, struct freq_node_t *freq_prev, struct freq_node_t *freq_next);
struct node_t *hashmap_get_data(hashmap *H, int hash, int key);

struct node_t* get_lfu_item(struct lfu_cache_t* LfuCache);

int insert(int key, lfu_cache_t *cache);

int delete_node(struct freq_node_t* freq);
struct freq_node_t * new_freq_node();
struct lfu_cache_t * new_lfu_cache(int size);
