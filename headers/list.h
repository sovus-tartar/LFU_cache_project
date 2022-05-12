
typedef struct elem_t {

    struct elem_t *parent;
    struct elem_t *next;
    struct elem_t *prev;
    int data;
    
} elem;

typedef struct list {


    elem *fictive;
    int size;

} List;



const int BASE_LIST_SIZE = 10;


#define INSERT_AFT(num,val)    ListInsertAft(lst, num, val)
#define INSERT_BEF(num,val)    ListInsertBef(lst, num, val)
/*#define DELETE(num)             ListDelete(&lst, num)
#define TAILADD(num)            ListTailAdd(&lst, num)
#define HEADADD(num)            ListHeadAdd(&lst, num)
#define PRINTLIST               PrintList(&lst)
#define GRAPHPRINT              graph_print(lst)
*/

List *ListCtor      ();
int ListDtor        (List* lst);
int ListDelete      (List* lst, const int number);
elem *search_elem   (List *lst, const int number);
int ListInsertAft   (List* lst, const int number, const int value);
;int PrintList       (List *lst);
;int graph_print     (List  *lst);
;int ListHeadAdd     (List *lst, const int value);
;int ListTailAdd     (List *lst, const int value);
int ListInsertBef   (List *lst, const int number, const int value);
;int Sort            (List *lst);
;int ListInsertAftLogic (List *lst, const int number, const int value);
;int ListRealloc     (List *lst, const int plus_capacity);