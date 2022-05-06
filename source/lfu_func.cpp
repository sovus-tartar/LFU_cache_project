#include "../headers/hashmap.h"
#include "../headers/lfu.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <math.h>


int ACCESS(int key, hashmap* Myhashmap, lfu_cache* Mylfu_cashe)
{
    hashmap_node* tmp = hashmap_get_node(Myhashmap, hash_count(key), &key);
    if (tmp == nullptr) {
        //INSERT();
        //TODO: use here "INSERT"
        //printf("No such key\n");
        return -1;
    }
    freq_node* freq = tmp->parent;
    freq_node* next_freq = freq->next;

    if (next_freq == Mylfu_cashe->freq_head ||
    next_freq->value != freq->value + 1) {
        next_freq = GET_NEW_NODE(freq->value + 1, freq, next_freq);
    }

//remove from freq our tmp ---------------
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
    
    return *((int*)tmp->data);
}

hashmap_node* NEW_LFU_ITEM(int data, freq_node* Myparent)
{
    hashmap_node* new_node = (hashmap_node*) calloc(1, sizeof(hashmap_node));
    *((int*)new_node->data) = data;
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


