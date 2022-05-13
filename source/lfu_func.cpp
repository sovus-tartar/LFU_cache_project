#include "../headers/hashmap.h"
#include "../headers/lfu.h"
#include "../headers/list.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>

struct node_t *hashmap_get_data(hashmap *H, int hash, int key)
{
    int n, i;
    hashmap_node *temp;
    assert(H);
    temp = (H->node_arr + hash)->node_ptr;
    n = (H->node_arr + hash)->collisions;

    for (i = 0; i < n; i++)
    {
        if (((node_t*)(temp->data))->data == key)
        {
            return (node_t*)(temp->data);
        }
        temp = temp->next;
    }

    return NULL;
}

int access(int key, struct lfu_cache_t* cache)
{
    struct node_t* tmp = hashmap_get_data(cache->hash_map, hash_count(key), key);
    if (tmp == nullptr) {
        //INSERT();
        //TODO: use here "INSERT"
        printf("No such key\n");
        return -1;
    }
    struct freq_node_t* freq = tmp->parent;
    struct freq_node_t* next_freq = freq->next;

    if (next_freq == cache->freq_head ||
    next_freq->value != freq->value + 1) {
        next_freq = get_new_node(freq->value + 1, freq, next_freq);
    }

//remove our tmp from freq ---------------
    if (freq->head == tmp) {
        freq->head = tmp->next;    
    }
    tmp->next->prev = tmp->prev;
    tmp->prev->next = tmp->next;
//---------------------------------------


//place tmp into freq+1 ------------------
    tmp->parent = next_freq;
    if (next_freq->head == nullptr) {
        next_freq->head = tmp;
        next_freq->head->prev = tmp; 
        next_freq->head->next = tmp;
    }
    else {
        next_freq->head->prev->next = tmp;
        next_freq->head->prev = tmp; 
        next_freq->head = tmp;
    }
//---------------------------------------

    if (freq->length == 0)
        delete_node(freq);
    
    return tmp->data;
}

struct node_t* new_lfu_item(int data, struct freq_node_t* parent)
{
    assert(parent != nullptr);

    struct node_t* new_node = (node_t*) calloc(1, sizeof(node_t));
    new_node->data = data;
    new_node->parent = parent;

    if (parent->head == nullptr) {
        parent->head = new_node;
        parent->head->prev = new_node; 
        parent->head->next = new_node;
    }
    else {
        parent->head->prev->next = new_node;
        parent->head->prev = new_node; 
        parent->head = new_node;
    }
    return new_node;
}


int insert(int key, lfu_cache_t *cache) {
    assert(cache);

    if (hashmap_get_data(cache->hash_map, hash_count(key), key)) {
        //printf("Key is already exist");
        return 0;
    }

    struct freq_node_t *freq = cache->freq_head->next;
    if (freq->value != 1) 
        freq = get_new_node(1, cache->freq_head, freq);

    struct node_t* lstNode = ListTailAdd(freq->node_list, key);
    lstNode->parent = freq; 

    hashmap_node *tblNode = hashmap_create_node(lstNode);
    hashmap_add_node(cache->hash_map, tblNode, hash_count(key));

    return 0;
}


struct freq_node_t * new_freq_node() {
    struct freq_node_t *node = (freq_node_t*)calloc(1, sizeof(freq_node_t));
    node->value = 0;
    node->length = 0;
    node->node_list = ListCtor();
    node->prev = NULL;
    node->next = NULL;

    return node;
}

struct lfu_cache_t * new_lfu_cache(int size) {
    struct lfu_cache_t *cache = (struct lfu_cache_t*)calloc(1, sizeof(struct lfu_cache_t));
    cache->size = size;
    cache->freq_head = new_freq_node();
    cache->hash_map = hashmap_create();

    return cache;
}