#include "../headers/lfu.h"

int main() {
    //printf("Enter size of cache, number of elements and all elements:\n");

    int cacheSize, n, i;
    printf("Enter name of test file\n");
    char namefr[100], namefw[100];
    scanf("%s", namefr);
    FILE *fr = fopen(namefr, "r");
    printf("Enter name of res file\n");
    scanf("%s", namefw);
    FILE *fw = fopen(namefw, "w");

    while (fscanf(fr, "%d", &cacheSize) == 1)
    {

        assert(fscanf(fr, "%d", &n));
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

            fprintf(fw, "%0.2f\n", ((float) success*100)/tries);
            cacheDtor(cache);
        }
    }
}
