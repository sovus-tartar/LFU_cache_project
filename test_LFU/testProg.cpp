#include "../headers/lfu.h"

int lfu(FILE *tests);

int main() {
    printf("Enter name of file with tests:\n");
    char file[25] = {};
    int chk = scanf("%s", file);
    assert(chk == 1);

    FILE *tests = fopen(file, "r");
    assert(tests);

    printf("Enter number of tests in file:\n");
    int testNum = 0;
    chk = scanf("%d", &testNum);
    assert(chk == 1);

    for (int i = 0; i < testNum; i++) 
        lfu(tests);

    fclose(tests);
}

int lfu(FILE *tests) {
    int cacheSize = 0, n = 0;
    int chk = fscanf(tests, "%d %d", &cacheSize, &n);
    assert(chk == 2);

    struct lfu_cache_t *cache = new_lfu_cache(cacheSize);

    int cnt = 0;

    for (int i = 0; i < n; i++) {
        int req = 0;
        int chk = fscanf(tests, "%d", &req);
        assert(chk == 1);

        int ver = insert(req, cache);

        if (ver) {
            access(req, cache);
            cnt++;
        }
    }
    
    printf("%d\n", cnt);

    cacheDtor(cache);

    return 0;
}