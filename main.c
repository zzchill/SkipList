#include <stdio.h>
#include "skip_list.h"

#define TEST_NUM    1000

int main()
{
    
    skip_list_t *list = creat_skip_list();
    skip_list_node_t *cur = NULL;
    skip_list_node_t *index = NULL;
    printf("list:%x, list->level_index:%x\n", list, list->level_index);
    for (int i = 0; i < TEST_NUM; i++)
    {
        add_node(i, list);
    }
    printf("add succese\n");
    printf("list->max_level:%d\n", list->max_level);

    index = list->level_index;
    while (index)
    {
        cur = index;
        while (cur)
        {
            printf("%d->", cur->key);
            cur = cur->next;
        }
        index = index->down;
        printf("\n");
    }

    for (int i = 0; i < TEST_NUM; i++)
    {
        cur = find_node(i, list);
        // printf("find result:0X%x, cur->key:%d\n", cur, cur->next->key);
    }

    for (int i = 0; i < TEST_NUM; i++)
    {
        delete_node(i, list);
        // printf("i:%d\t",i);
        // printf("delet node result:%d\n", delete_node(i, list));
    }

    printf("\n");
    index = list->level_index;
    while (index)
    {
        cur = index;
        while (cur)
        {
            printf("%d->", cur->key);
            cur = cur->next;
        }
        index = index->down;
        printf("\n");
    }

    printf("list->max_level:%d\n", list->max_level);

    
    return 0;
}