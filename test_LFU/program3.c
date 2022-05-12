typedef struct hashmap_node_t
{
    void *data;
    struct hashmap_node_t *prev;
    struct hashmap_node_t *next;
} hashmap_node;

typedef struct hashmap_arr_t
{
    hashmap_node *node_ptr; // ptr to the start of part of list of collisions
    int collisions;
} hashmap_arr;

typedef struct hashmap_t
{
    int number_of_elements; // powerfullness of hash
    hashmap_arr *node_arr;  // node_arr[hash]
    hashmap_node *list_start;
} hashmap;

typedef struct list_node_t
{
    struct list_node_t *prev;
    struct list_node_t *next;
    int data;
} list_node;

list_node *create_list(int n);
void list_remove_elem(list_node *node);
list_node *list_get_last(list_node *top);
list_node *list_create_node(int data);
list_node *list_put_to_top(list_node *top, list_node *node);
list_node *list_move_to_top(list_node *top, list_node *node);

typedef struct cache_t
{
    int cache_size;
    int filled_nodes;
    struct list_node_t *list_ptr;
    struct hashmap_t *hash_table;
} cache;

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

const int hash_pow = 1000000; // powerfullness of hash

int hash_count(int key)
{
#ifdef DEBUG
    int n;
    n = key;
#endif

    key += ~(key << 16);
    key ^= (key >> 5);
    key += (key << 3);
    key ^= (key >> 13);
    key += ~(key << 9);
    key ^= (key >> 17);

#ifdef DEBUG
    printf("Hash for number: %d calculated: %d\n", n, key % hash_pow);
#endif

    return (key % hash_pow);
}

hashmap *hashmap_create()
{
    hashmap *temp;

    temp = calloc(1, sizeof(hashmap));
    assert(temp);

    temp->number_of_elements = hash_pow;
    temp->node_arr = calloc(hash_pow, sizeof(hashmap_arr));
    assert(temp->node_arr);
#ifdef DEBUG
    printf("Hashmap created, hash_pow = %d\n", hash_pow);
#endif
    return temp;
}

void hashmap_delete(hashmap *H)
{
    int i;

    for (i = 0; i < H->number_of_elements; i++)
    {
        hashmap_node *temp;

        temp = (H->node_arr + i)->node_ptr;

        while (temp)
        {
            hashmap_node *next_nd;
            next_nd = temp->next;
            free(temp);
            temp = next_nd;
        }
    }

    free(H->node_arr);
    free(H);
#ifdef DEBUG
    printf("Hashmap deleted\n");
#endif
}

hashmap_node *hashmap_create_node(void *data)
{
    hashmap_node *temp;

    assert(data);

    temp = calloc(1, sizeof(hashmap_node));
    temp->data = data;
#ifdef DEBUG
    printf("Hashmap node(%p) created with data(%p)\n", temp, data);
#endif
    return temp;
}

void hashmap_add_node(hashmap *H, hashmap_node *node, int hash)
{
    hashmap_arr *temp;

    assert(H);
    assert(node);

    temp = H->node_arr + hash;

    if (temp->node_ptr)
    {
        hashmap_node *curr;
        int n, i;

        n = temp->collisions;
        curr = temp->node_ptr;

        for (i = 0; i < n - 1; i++)
        {
            curr = curr->next;
        }
        node->prev = curr;
        node->next = curr->next;
        curr->next = node;
        node->next->prev = node;
        temp->collisions += 1;
    }
    else
    {
        if (H->list_start == NULL)
        {
            H->list_start = node;
            temp->node_ptr = node;
            node->next = node;
            node->prev = node;
            temp->collisions += 1;
        }
        else
        {
            temp->node_ptr = node;
            node->prev = H->list_start->prev;
            H->list_start->prev->next = node;
            H->list_start->prev = node;
            node->next = H->list_start;
            temp->collisions += 1;
            H->list_start = node;
        }
    }

#ifdef DEBUG
    printf("Hashmap: node(%p) added, hash = %d\n", node, hash);
#endif
}

hashmap_node *hashmap_get_node(hashmap *H, int hash, void *data)
{
    int n, i;
    hashmap_node *temp;
    assert(H);
    assert(data);
    temp = (H->node_arr + hash)->node_ptr;
    n = (H->node_arr + hash)->collisions;

    for (i = 0; i < n; i++)
    {
        if (temp->data == data)
        {
#ifdef DEBUG
            printf("Hashmap: got node with data %p, hash = %d\n", data, hash);
#endif
            return temp;
        }

        temp = temp->next;
    }

#ifdef DEBUG
    printf("Hashmap: can't get node with data %p, hash = %d\n", data, hash);
#endif

    return NULL;
}

void hashmap_delete_node(hashmap *H, int hash, void *data)
{
    hashmap_node *temp, *prev_nd, *next_nd;

    assert(H);
    assert(data);

    temp = hashmap_get_node(H, hash, data);

    if (temp == NULL)
    {
        printf("No data (%p) to delete with hash %d in hashmap (%p)\n", data, hash, H);
        abort();
    }

    if (temp == (H->node_arr + hash)->node_ptr)
    {
        if ((H->node_arr + hash)->collisions == 1)
        {
            (H->node_arr + hash)->node_ptr = NULL;
        }
        else
        {
            (H->node_arr + hash)->node_ptr = ((H->node_arr + hash)->node_ptr)->next;
        }

        if (H->list_start == temp)
            H->list_start = H->list_start->next;
    }

    prev_nd = temp->prev;
    next_nd = temp->next;
#ifdef DEBUG
    printf("prev_nd = %p next_nd = %p for node %p\n", prev_nd, next_nd, temp);
#endif
    (H->node_arr + hash)->collisions -= 1;

    if ((prev_nd == temp) || (next_nd == temp))
    {
        free(temp);
        H->list_start = NULL;
    }

    prev_nd->next = next_nd;
    next_nd->prev = prev_nd;
    free(temp);

#ifdef DEBUG
    printf("Hashmap: node(%p) deleted, hash = %d, data: %p\n", temp, hash, data);
#endif
}

list_node *create_list(int n)
{
    int i;
    list_node *top, *curr;

    top = calloc(1, sizeof(list_node));
    assert(top);
    curr = top;

    for (i = 1; i < n; i++)
    {
        list_node *temp;

        temp = calloc(1, sizeof(list_node));
        assert(temp);
        temp->prev = curr;
        curr->next = temp;
        curr = temp;
    }

    curr->next = top;
    top->prev = curr;
#ifdef DEBUG
    printf("List created\n");
#endif
    return top;
}

void list_remove_elem(list_node *node)
{
    list_node *prev_el, *next_el;

    prev_el = node->prev;
    next_el = node->next;

    prev_el->next = next_el;
    next_el->prev = prev_el;
#ifdef DEBUG
    printf("List: node (%p) with val: %d removed\n", node, node->data);
#endif
    free(node);
}

list_node *list_get_last(list_node *top)
{
#ifdef DEBUG
    printf("List: got last element (%p)\n", top->prev);
#endif
    return top->prev;
}

list_node *list_create_node(int data)
{
    list_node *temp;
    temp = calloc(1, sizeof(list_node *));
    assert(temp);
    temp->data = data;
#ifdef DEBUG
    printf("List: node (%p) with data: %d created\n", temp, data);
#endif
    return temp;
}

list_node *list_put_to_top(list_node *top, list_node *node)
{
    assert(top);
    assert(node);

    node->next = top;
    node->prev = top->prev;
    top->prev->next = node;
    top->prev = node;

#ifdef DEBUG
    printf("List: node (%p) with data: %d put to top\n", node, node->data);
#endif

    return node;
}

list_node *list_move_to_top(list_node *top, list_node *node)
{
    list_node *prev_el, *next_el;

    assert(top);
    assert(node);

    if (top == node)
    {
#ifdef DEBUG
        printf("List: node (%p) is already on the top\n", node);
#endif
        return node;
    }
    prev_el = node->prev;
    next_el = node->next;

    prev_el->next = next_el;
    next_el->prev = prev_el;

#ifdef DEBUG
    printf("List: node (%p) moved to top\n", node);
#endif

    return list_put_to_top(top, node);
}

#ifdef DEBUG
void hashmap_dump(hashmap *H)
{
    int i, num;
    hashmap_node *curr;
    num = H->number_of_elements;

    printf("START OF HASHMAP DUMP~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    for (i = 0; i < num; i++)
    {
        hashmap_node *curr, *temp;
        int j;
        curr = (H->node_arr + i)->node_ptr;
        printf("HASH = %d:\n", i);

        for (j = 0; j < (H->node_arr + i)->collisions; j++)
        {
            printf("%d--", ((list_node *)(curr->data))->data);
            curr = curr->next;
        }
        printf("\n");
    }

    printf("END OF HASHMAP DUMP~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    printf("START OF HASHMAP LIST DUMP\n");

    curr = H->list_start;
    printf("%d-->", ((list_node *)(curr->data))->data);
    curr = curr->next;
    while (curr != H->list_start)
    {
        printf("%d-->", ((list_node *)(curr->data))->data);
        curr = curr->next;
    }
    printf("\n");

    printf("%d-->", ((list_node *)(curr->data))->data);
    curr = curr->prev;
    while (curr != H->list_start)
    {
        printf("%d-->", ((list_node *)(curr->data))->data);
        curr = curr->prev;
    }
    printf("\nEND OF HASHMAP LIST DUMP\n");
}

#endif

list_node *hashmap_get_data(hashmap *H, int hash, int data)
{
    hashmap_node *curr;
    list_node *temp;
    int n, i;
    curr = (H->node_arr + hash)->node_ptr;
    n = (H->node_arr + hash)->collisions;

    for (i = 0; i < n; i++)
    {

        temp = curr->data;

        if (temp->data == data)
        {

#ifdef DEBUG
            printf("Hashmap: found data %p (%d) with hash %d\n", temp, data, hash);
#endif
            return temp;
        }
        curr = curr->next;
    }
#ifdef DEBUG
    printf("Hashmap: can't get data %p (%d) with hash %d\n", temp, data, hash);
#endif

    return NULL;
}

cache *init_cache(int n)
{
    cache *temp;
    temp = calloc(1, sizeof(cache));
    temp->cache_size = n;
    temp->list_ptr = create_list(n);
    temp->filled_nodes = 0;
    temp->hash_table = hashmap_create();
#ifdef DEBUG
    printf("Cache initialized, hashmap: %p, list: %p, size: %d\n", temp->hash_table, temp->list_ptr, n);
#endif
    return temp;
}

void cache_add_elem(cache *C, int data, int hash_data)
{
    hashmap_node *temp;
    list_node *node;
    assert(C);
    assert(C->filled_nodes < C->cache_size);
    C->filled_nodes += 1;

    node = list_create_node(data);

    C->list_ptr = list_put_to_top(C->list_ptr, node);

    temp = hashmap_create_node(node);
    hashmap_add_node(C->hash_table, temp, hash_data);

#ifdef DEBUG
    printf("Cache: element with data: %d (hash = %d) added\n", data, hash_data);
#endif
}

void cache_remove_last(cache *C)
{
    int data, hash;
    hashmap_node *temp;
    list_node *last;

    assert(C);

    last = list_get_last(C->list_ptr);
    data = last->data;
    hash = hash_count(data);

#ifdef DEBUG
    printf("Cache: removed last element (%p) with data: %d (hash = %d)\n", last, data, hash);
#endif
    if (C->filled_nodes == C->cache_size)
    {

        C->filled_nodes -= 1;
        hashmap_delete_node(C->hash_table, hash, last);
    }

    list_remove_elem(last);
}

list_node *cache_try_get(cache *C, int data)
{
    int hash_data;
    list_node *temp;
    hash_data = hash_count(data);
    assert(C);
#ifdef DEBUG
    printf("Cache: requested element with data: %d (hash = %d)\n", data, hash_data);
#endif
    temp = hashmap_get_data(C->hash_table, hash_data, data);

    if (temp)
    {
#ifdef DEBUG
        printf("Cache: request element with data: %d (hash = %d) SUCCESS\n", data, hash_data);
#endif
        C->list_ptr = list_move_to_top(C->list_ptr, temp);
    }
    else
    {
        list_node *last;
#ifdef DEBUG
        printf("Cache: request element with data: %d (hash = %d) FAILED\n", data, hash_data);
#endif
        last = list_get_last(C->list_ptr);
        cache_remove_last(C);
        cache_add_elem(C, data, hash_data);
    }
#ifdef DEBUG
    hashmap_dump(C->hash_table);
#endif
    return temp;
}

void cache_show(cache *C)
{
    int n, i;
    list_node *curr;

    n = C->cache_size;
    curr = C->list_ptr;

    for (i = 0; i < n; i++)
    {
        printf("%d ", curr->data);
        curr = curr->next;
    }

    printf("\n");
}

int main()
{
    int m, tries, i, success;
    cache *C;
    printf("Enter name of test file\n");
    char namefr[100], namefw[100];
    scanf("%s", namefr);
    FILE *fr = fopen(namefr, "r");
    printf("Enter name of res file\n");
    scanf("%s", namefw);
    FILE *fw = fopen(namefw, "w");

    while (fscanf(fr, "%d", &m) == 1)
    {
        C = init_cache(m);

        assert(fscanf(fr, "%d", &tries));
        success = 0;
        for (i = 0; i < tries; i++)
        {
            int num;
            assert(fscanf(fr, "%d", &num));
            if (cache_try_get(C, num))
                success += 1;
        }
        fprintf(fw, "%0.2f\n", ((float) success*100)/tries);
    }
    
}
