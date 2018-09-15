#include <stdio.h>
#include <stdlib.h>

static int index=0;

typedef struct node {
    void *ptr;
    struct node * next;
    int i; // for debugging
} node_t;

node_t *list;


void initialMemAlloc()
{
    list = (node_t *)malloc(sizeof(node_t));
    if (list==NULL) {
        fprintf(stderr, "failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    list->ptr = NULL;
    list->next = NULL;
    list->i=0;
}
void collectgarbage()
{
   node_t *current;

   while (list)
    {
       current = list;
       list = list->next;
       free(current->ptr);
       free(current);
    }

}
void push(void *p)
{
    node_t *current = list;
    while (current->next) {
        current = current->next;
    }

    current->next = malloc(sizeof(node_t));
    if (current->next==NULL) {
        fprintf(stderr, "failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    current->next->ptr = p;
    current->next->next = NULL;
    current->next->i = ++index;
}
void *b_malloc(size_t size)
{
    void *p = malloc(size);
    if (p==NULL) {
        fprintf(stderr, "failed to allocate memory.\n");
        exit(EXIT_FAILURE);
    }
    push(p);
    return p;
}
void b_free(void *p)
{
    node_t *current = list;
    node_t *pre = list;
    while(current){
        if (current->ptr==p){
            printf("free index= %d\n",current->i);
            pre->next=current->next;
            free(current);
            free(p);
            return ;
        }
        pre=current;
        current=current->next;
    }
}
void print_list()
{
    node_t *current=list;
    while (current) {
        printf("index= %d\n",current->i);
        current=current->next;
    }
}

int main()
{
    /*initialize linked list*/
    initialMemAlloc();

    int *ptr1=(int *)b_malloc(sizeof(int));
    *ptr1=1;
    b_free(ptr1);
    
    int *ptr2=(int *)b_malloc(sizeof(int));
    *ptr2=2;
    b_free(ptr2);
    
    int *ptr3=(int *)b_malloc(sizeof(int));
    *ptr3=3;

    print_list();

    collectgarbage();

    return 0;
}
/*
 * valgrind --leak-check=full -v ./program
 */
