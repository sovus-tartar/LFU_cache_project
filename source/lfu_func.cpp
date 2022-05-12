#include "../headers/hashmap.h"
#include "../headers/lfu.h"
#include "../headers/list.h"

#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>


elem_t *hashmap_get_data(hashmap *H, int hash, int key)
{
    int n, i;
    hashmap_node *temp;
    assert(H);
    temp = (H->node_arr + hash)->node_ptr;
    n = (H->node_arr + hash)->collisions;

    for (i = 0; i < n; i++)
    {
        if (((elem_t*)(temp->data))->data == key)
        {
            return (elem_t*)(temp->data);
        }
        temp = temp->next;
    }

    return NULL;
}

int ACCESS(int key, lfu_cache* Mylfu_cache)
{
    elem_t* tmp = hashmap_get_data(Mylfu_cache->Myhashmap, hash_count(key), key);;
    if (tmp == nullptr) {
        //INSERT();
        //TODO: use here "INSERT"
        printf("No such key\n");
        return -1;
    }
    freq_node* freq = tmp->parent;
    freq_node* next_freq = freq->next;

    if (next_freq == Mylfu_cache->freq_head ||
    next_freq->value != freq->value + 1) {
        next_freq = GET_NEW_NODE(freq->value + 1, freq, next_freq);
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
        DELETE_NODE(freq);
    
    return tmp->data;
}

elem_t* NEW_LFU_ITEM(int data, freq_node* Myparent)
{
    assert(Myparent != nullptr);

    elem_t* new_node = (elem_t*) calloc(1, sizeof(elem_t));
    new_node->data = data;
    new_node->parent = Myparent;

    if (Myparent->head == nullptr) {
        Myparent->head = new_node;
        Myparent->head->prev = new_node; 
        Myparent->head->next = new_node;
    }
    else {
        Myparent->head->prev->next = new_node;
        Myparent->head->prev = new_node; 
        Myparent->head = new_node;
    }
    return new_node;
}


