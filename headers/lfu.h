#pragma once

typedef struct freq_node_t
{
    int value = 0;
    int length = 0;
    struct hashmap_node_t *head = nullptr;
    struct freq_node_t *prev = nullptr;
    struct freq_node_t *next = nullptr;
} freq_node;

typedef struct lfu_cache_t
{
    struct freq_node_t *freq_head = nullptr;
} lfu_cache;

int ACCESS(int key, hashmap Myhashmap, lfu_cache* Mylfu_cashe);
hashmap_node* NEW_LFU_ITEM(int data, freq_node* parent);
void DELETE_NODE(freq_node* freq);
freq_node*GET_NEW_NODE(int new_val, freq_node* freq, freq_node* next_freq);


