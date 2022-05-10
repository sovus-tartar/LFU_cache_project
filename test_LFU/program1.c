#include <stdlib.h>
#include <stdio.h>

// this function reads the parameters of the tests and
// calls the corresponding functions
int test_generation(FILE *fp);

// all number is random
int all_random(FILE *fp, int size, int k, int l);

// wandering numbers near the changing mean
int wandering(FILE *fp, int size, int k, int l);

// sawtooth change of numbers
int sawtooth(FILE *fp, int size, int k, int l);

// cycles of number
int cyclic(FILE *fp, int size, int k, int l);

int main()
{
    printf("Enter name of test file\n");
    char name_of_tf[100];
    scanf("%s", name_of_tf);

    FILE *fp;

    fp = fopen(name_of_tf, "a");

    int N; // total number of tests
    N = test_generation(fp);

    fclose(fp);
}

int test_generation(FILE *fp)
{
    int N = 0;
    int flag = 0;
    int size, k, l, v;

    while (1 == 1)
    {
        printf("Enter -1 to stop program or other to make tests\n");
        scanf("%d", &flag);

        if (flag == -1)
        {
            break;
        }

        printf("Enter the size of cash\n");
        while (scanf("%d", &size) != 1 || size <= 0)
        {
            printf("Error\n Please,Enter the size of cash\n");
        }

        printf("Enter the number of tests\n");
        while (scanf("%d", &k) != 1 || k <= 0)
        {
            printf("Error\n Please,Enter the number of tests\n");
        }

        printf("Enter the length of tests\n");
        while (scanf("%d", &l) != 1 || l <= 0)
        {
            printf("Error\n Please,Enter the length of tests\n");
        }

        printf("Enter the type of tests\n1 - all random\n");
        printf("2 - wandering around a changing average\n3 - sawtooth changes\n");
        printf("4 - cyclic data\n");
        while (scanf("%d", &v) != 1 || v <= 0 || v > 4)
        {
            printf("Error\nEnter the type of tests\n1 - all random\n");
            printf("2 - wandering around a changing average\n 3 - sawtooth changes\n");
            printf("4 - cyclic data\n");
        }

        switch (v)
        {
        case 1:
            all_random(fp, size, k, l);
            break;
        case 2:
            wandering(fp, size, k, l);
            break;
        case 3:
            sawtooth(fp, size, k, l);
        case 4:
            cyclic(fp, size, k, l);
        default:;
            // abort();
        }
        N += k;
    }

    return N;
}

int all_random(FILE *fp, int size, int k, int l)
{
    int ran;
    printf("Enter range of numbers\n");
    while (scanf("%d", &ran) != 1 || ran <= 0)
    {
        printf("Error\n Please, Enter range of numbers\n");
    }

    for (int i = 0; i < k; i++)
    {
        fprintf(fp, "%d ", size);
        fprintf(fp, "%d ", l);
        for (int j = 0; j < l; j++)
        {
            fprintf(fp, "%d ", rand() % ran);
        }

        fprintf(fp, "\n\n");
    }

    return ran;
}

int wandering(FILE *fp, int size, int k, int l)
{
    int ran, ran_d, ran_w;
    printf("Enter range of numbers\n");
    while (scanf("%d", &ran) != 1 || ran <= 0)
    {
        printf("Error\n Please, Enter range of numbers\n");
    }

    printf("Enter range of deviation\n");
    while (scanf("%d", &ran_d) != 1 || ran_d <= 0)
    {
        printf("Error\n Please, Enter range of deviation\n");
    }

    printf("Enter range of the number of w_n\n");
    while (scanf("%d", &ran_w) != 1 || ran_w <= 0)
    {
        printf("Error\n Please, Enter range the number of w_n\n");
    }
    for (int i = 0; i < k; i++)
    {
        fprintf(fp, "%d ", size);
        fprintf(fp, "%d ", l);
        int count = 0;
        while (count < l)
        {
            int count2 = 0;
            int n_w_n = rand() % ran_w;
            int dev = (rand() % ran_d);
            int mean = rand() % ran + dev;
            while (count < l && count2 < n_w_n)
            {
                int x = mean - dev + (rand() % (2 * dev + 1));
                fprintf(fp, "%d ", x);
                count++;
                count2++;
            }
        }

        fprintf(fp, "\n\n");
    }

    return ran;
}

int sawtooth(FILE *fp, int size, int k, int l)
{
    int ran;
    printf("Enter range of numbers\n");
    while (scanf("%d", &ran) != 1 || ran <= 0)
    {
        printf("Error\n Please, Enter range of numbers\n");
    }

    for (int i = 0; i < k; i++)
    {
        fprintf(fp, "%d ", size);
        fprintf(fp, "%d ", l);
        int count = 0;
        int point = rand() % ran, purpose;
        fprintf(fp, "%d ", point);
        count++;
        while (count < l)
        {
            purpose = rand() % ran;
            if (purpose <= point)
            {
                point = purpose;
                fprintf(fp, "%d ", point);
                count++;
            }
            else
            {
                while (point < purpose && count < l)
                {
                    point++;
                    fprintf(fp, "%d ", point);
                    count++;
                }
            }
        }

        fprintf(fp, "\n\n");
    }

    return ran;
}

int cyclic(FILE *fp, int size, int k, int l)
{
    int ran_c, ran;
    printf("Enter range of numbers\n");
    while (scanf("%d", &ran) != 1 || ran <= 0)
    {
        printf("Error\n Please, Enter range of numbers\n");
    }
    printf("Enter range of cycle\n");
    while (scanf("%d", &ran_c) != 1 || ran_c <= 0)
    {
        printf("Error\n Please, Enter range of cycle\n");
    }
    for (int i = 0; i < k; i++)
    {
        fprintf(fp, "%d ", size);
        fprintf(fp, "%d ", l);
        int count = 0;
        while (count < l)
        {
            int count2 = 0;
            int l_of_cycle = rand() % ran_c;
            int *arr = calloc(l_of_cycle, sizeof(int));
            int n_of_cycle = rand() % 15;
            for (int j = 0; j < l_of_cycle; j++)
            {
                arr[j] = rand() % ran;
            }
            while (count < l && l_of_cycle * n_of_cycle > count2)
            {
                fprintf(fp, "%d ", arr[count2 % l_of_cycle]);
                count++;
                count2++;
            }
            free(arr);
        }

        fprintf(fp, "\n\n");
    }

    return ran;
}
