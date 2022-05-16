#include "lfu.h"


int delete_node(struct freq_node_t* freq)
{
    assert(freq);

    ListDtor(freq->node_list);
    if (freq->prev == NULL || freq->next == NULL) {
        printf("error in delete_node\n");
        return -1;
    } 
    freq->prev->next = freq->next;
    freq->next->prev = freq->prev;

    free(freq);
    return 0;
}

struct node_t* get_lfu_item(struct lfu_cache_t* LfuCache)
{   
    assert(LfuCache);

    if (LfuCache->hash_map->number_of_elements <= 0) {
        printf("error in get_lfu_item\n");
        return NULL;
    }

    struct node_t* tmp_node = hashmap_get_data(LfuCache->hash_map, hash_count(LfuCache->freq_head->next->node_list->fictive->next->data), LfuCache->freq_head->next->node_list->fictive->next->data);
        
    return tmp_node;
}

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
    assert(cache);

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
    tmp->next->prev = tmp->prev;
    tmp->prev->next = tmp->next;
    freq->length--;
//---------------------------------------

//place tmp into freq+1 ------------------
    tmp->parent = next_freq;
    tmp->next = next_freq->node_list->fictive->next;
    tmp->prev = next_freq->node_list->fictive;
    next_freq->node_list->fictive->next->prev = tmp;
    next_freq->node_list->fictive->next = tmp;
    next_freq->length++;
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
    
    new_node->next = parent->node_list->fictive->next;
    new_node->prev = parent->node_list->fictive;
    parent->node_list->fictive->next->prev = new_node;
    parent->node_list->fictive->next = new_node;
    parent->length++;

    return new_node;
}


int insert(int key, lfu_cache_t *cache) 
{
    assert(cache);

    if (hashmap_get_data(cache->hash_map, hash_count(key), key)) 
    {
        //printf("Key is already exist");
        return 1;
    }

    cache->size++;
    if (cache->size > cache->capacity) 
    {
        struct node_t *leastElem = get_lfu_item(cache);
        
        leastElem->prev->next = leastElem->next;
        leastElem->next->prev = leastElem->prev;

        hashmap_delete_node(cache->hash_map, hash_count(leastElem->data), leastElem);

        free(leastElem);

        cache->freq_head->next->length--;
        if (cache->freq_head->next->length == 0)
            delete_node(cache->freq_head->next);
    }

    struct freq_node_t *freq = cache->freq_head->next;
    if (freq->value != 1) 
        freq = get_new_node(1, cache->freq_head, freq);

    struct node_t* lstNode = ListTailAdd(freq->node_list, key);
    lstNode->parent = freq;
    freq->length++;

    hashmap_node *tblNode = hashmap_create_node(lstNode);
    hashmap_add_node(cache->hash_map, tblNode, hash_count(key));

    return 0;
}


struct freq_node_t * new_freq_node() 
{
    struct freq_node_t *node = (freq_node_t*)calloc(1, sizeof(freq_node_t));
    assert(node);
    
    node->value = 0;
    node->length = 0;
    node->node_list = ListCtor();
    node->node_list->fictive->parent = node;
    node->prev = NULL;
    node->next = NULL;

    return node;
}

struct lfu_cache_t * new_lfu_cache(int size) 
{
    struct lfu_cache_t *cache = (struct lfu_cache_t*)calloc(1, sizeof(struct lfu_cache_t));
    assert(cache);

    cache->size = 0;
    cache->capacity = size;
    cache->freq_head = new_freq_node();
    cache->freq_head->next = cache->freq_head;
    cache->freq_head->prev = cache->freq_head;
    cache->hash_map = hashmap_create();

    return cache;
}

struct freq_node_t * get_new_node(int val, struct freq_node_t *freq_prev, struct freq_node_t *freq_next) 
{
    assert(freq_prev);
    assert(freq_next);

    struct freq_node_t *node = new_freq_node();
    node->value = val;
    node->prev = freq_prev;
    node->next = freq_next;
    


    freq_prev->next = node;
    freq_next->prev = node;

    return node;
}

void cacheDtor(lfu_cache_t *cache) 
{
    assert(cache);

    while(cache->freq_head->next->value != 0)
        delete_node(cache->freq_head->next);

    delete_node(cache->freq_head);

    hashmap_delete(cache->hash_map);

    free(cache);
    return;
}