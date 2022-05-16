#include "hashmap.h"

//TODO: запихнуть сюда двусвязный список из либы BO$$a
//TODO: hashmap_dumpдля либы BO$$a
//TODO: комментарии к функциям

const int hash_pow = 10000; // powerfullness of hash

int hash_count(int key)
{
#ifdef HASHMAP_DEBUG
    int n;
    n = key;
#endif

    key += ~(key << 16);
    key ^= (key >> 5);
    key += (key << 3);
    key ^= (key >> 13);
    key += ~(key << 9);
    key ^= (key >> 17);

#ifdef HASHMAP_DEBUG
    printf("Hash for number: %d calculated: %d\n", n, key % hash_pow);
#endif

    return (key % hash_pow);
}

hashmap *hashmap_create()
{
    hashmap *temp;

    temp = (hashmap *)calloc(1, sizeof(hashmap));
    assert(temp);

    temp->number_of_elements = hash_pow;
    temp->node_arr = (hashmap_arr *)calloc(hash_pow, sizeof(hashmap_arr));
    assert(temp->node_arr);
#ifdef HASHMAP_DEBUG
    printf("Hashmap created, hash_pow = %d\n", hash_pow);
#endif
    return temp;
}

void hashmap_delete(hashmap *H)
{
    int i;

    hashmap_node * top, * curr;
    top = H -> list_start;

    curr = top -> next;
    free(top);

    while(curr != top) {
        hashmap_node * temp;
        temp = curr;
        curr = curr -> next;
        free(temp);
    }

    free(H->node_arr);
    free(H);
#ifdef HASHMAP_DEBUG
    printf("Hashmap deleted\n");
#endif
}

hashmap_node *hashmap_create_node(void *data)
{
    hashmap_node *temp;

    assert(data);

    temp = (hashmap_node *)calloc(1, sizeof(hashmap_node));
    temp->data = data;
#ifdef HASHMAP_DEBUG
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

#ifdef HASHMAP_DEBUG
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
#ifdef HASHMAP_DEBUG
            printf("Hashmap: got node with data %p, hash = %d\n", data, hash);
#endif
            return temp;
        }

        temp = temp->next;
    }

#ifdef HASHMAP_DEBUG
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
#ifdef HASHMAP_DEBUG
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

#ifdef HASHMAP_DEBUG
    printf("Hashmap: node(%p) deleted, hash = %d, data: %p\n", temp, hash, data);
#endif
}

#ifdef HASHMAP_DEBUG
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