#include "../headers/lfu.h"

int main() {
    //printf("Enter size of cache, number of elements and all elements:\n");

    int cacheSize = 0, n = 0;
    int chk = scanf("%d %d", &cacheSize, &n);
    assert(chk == 2);

    struct lfu_cache_t *cache = new_lfu_cache(cacheSize);

    int cnt = 0;

    for (int i = 0; i < n; i++) {
        int req = 0;
        int chk = scanf("%d", &req);
        assert(chk == 1);

        int ver = insert(req, cache);

        if (ver) {
            access(req, cache);
            cnt++;
        }
    }
    
    printf("%d\n", cnt);

    cacheDtor(cache);
}